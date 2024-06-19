#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "cfcss"
int counter = 0;

static LLVMContext Context;

namespace {

struct CFCSS : public FunctionPass {
  static char ID;
  CFCSS() : FunctionPass(ID) {}

  GlobalVariable *G;
  GlobalVariable *D;
  std::map<BasicBlock *, int> blocksign;

  bool runOnFunction(Function &F) override {
    ++counter;
    Module *M = F.getParent();

    if (F.getName() == "__ctt_error") {
      return false;
    }

    G = createGlobalVar(M, "G");
    D = createGlobalVar(M, "D");

    // assign unique signatures to basic blocks
    int signcount = 1;
    for (BasicBlock &BB : F) {
      blocksign[&BB] = signcount++;
    }

    // process each basic block
    for (BasicBlock &BB : F) {
      IRBuilder<> IR(M->getContext());

      // setup insertion point at the beginning of the basic block
      Instruction *firstinst = &BB.front();
      IR.SetInsertPoint(firstinst);

      // determine predecessor information
      BasicBlock *predecessor = BB.getUniquePredecessor(); //returns nullptr in case of multiple predecessors
      int predsign = 0;
      int predcount = 0;

      if (predecessor != nullptr) {
        // single predecessor case
        predsign = blocksign[predecessor];
        predcount = 1;
      } else {
        // multiple predecessors case
        for (auto begin = pred_begin(&BB), end = pred_end(&BB); begin != end; ++begin) {
          predecessor = *begin;
          IR.CreateStore(IR.getInt32(blocksign[predecessor]), D); //still an issue?
          predcount++;
        }
      }

      // calculate current block's signature difference
      int currsign = blocksign[&BB];
      int diffinsign = predsign ^ currsign;

      // generate XOR operations based on predecessor count
      if (predcount == 0) {
        IR.CreateStore(IR.getInt32(currsign), G);
      } else if (predcount == 1) {
        Value *newGValue = createXORdValue(M, &BB.front(), BB, IR.getInt32(diffinsign));
        IR.CreateStore(newGValue, G);
      } else if (predcount > 1) {
        BasicBlock *predBlock;
        Value *finalGValue = IR.getInt32(currsign);
        for (auto begin = pred_begin(&BB), end = pred_end(&BB); begin != end; ++begin) {
          predBlock = *begin;
          LoadInst *currentDValue =IR.CreateLoad(D->getValueType(), D); // Provide type and pointer
          Value *newDValue = IR.CreateXor(currentDValue, IR.getInt32(blocksign[predBlock]));
          Value *newGValue = createXORdValue(M, &BB.front(), BB, IR.getInt32(diffinsign));
          finalGValue = IR.CreateXor(newGValue, newDValue);
  
        }
        IR.CreateStore(finalGValue, G);
      }
    }

    return true;
  }

  Value *createXORdValue(Module *M, Instruction *Inst, BasicBlock &BB,Value *difference) {
    IRBuilder<> IR(Inst->getContext());
    IR.SetInsertPoint(Inst);
    LoadInst *currentGValue = IR.CreateLoad(G->getValueType(), G); 
    Value *newXORdValue = IR.CreateXor(currentGValue, difference);
    return newXORdValue;
  }

   llvm::GlobalVariable *createGlobalVar(Module *M, std::string name) {
    IRBuilder<> Builder(M->getContext());
    M->getOrInsertGlobal(name, Builder.getInt32Ty());
    llvm::GlobalVariable *globalVar = M->getNamedGlobal(name);
    globalVar->setLinkage(llvm::GlobalValue::CommonLinkage);
    globalVar->setAlignment(Align(4));
    globalVar->setInitializer(Builder.getInt32(0));
    return globalVar;
  }
};

} 

char CFCSS::ID = 0;
static RegisterPass<CFCSS> X("cfcss", "Control-Flow Checking by Signature (CFCSS)");
