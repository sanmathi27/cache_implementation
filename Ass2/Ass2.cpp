#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

namespace {
  struct FunctionAnalysisPass : public FunctionPass {
    static char ID; 
    FunctionAnalysisPass() : FunctionPass(ID) {}

    // This will hold the reference to the global variable
    GlobalVariable *gVar = nullptr;

    bool doInitialization(Module &M) override {
      // Create a global variable of type i32 and initialize it to zero
      LLVMContext &Context = M.getContext();
      gVar = new GlobalVariable(M, Type::getInt32Ty(Context), false, 
                                GlobalValue::CommonLinkage, 
                                ConstantInt::get(Type::getInt32Ty(Context), 0), 
                                "myGlobalVar");
      gVar->setAlignment(Align(4)); // Set alignment to 4 bytes using Align class
      return true;
    }

    bool runOnFunction(Function &F) override {
      LLVMContext &Context = F.getContext();
      IRBuilder<> Builder(Context);

      errs() << "Function: " << F.getName() << "\n";

      int numBasicBlocks = 0;
      int maxSuccessors = 0;
      int maxPredecessors = 0;

      // Insert store instruction in the entry block
      BasicBlock &EntryBB = F.getEntryBlock();
      Builder.SetInsertPoint(&EntryBB, EntryBB.getFirstInsertionPt());
      Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Context), 0), gVar);

      // Iterate each basic block in function
      for (BasicBlock &BB : F) {
        numBasicBlocks++;
        int numInstructions = BB.size();
        int numSuccessors = succ_size(&BB);
        int numPredecessors = pred_size(&BB);


        // Number of successors
        if (numSuccessors > maxSuccessors) {
          maxSuccessors = numSuccessors;
        }

        // Number of predecessors
        if (numPredecessors > maxPredecessors) {
          maxPredecessors = numPredecessors;
        }

        errs() << "  Basic Block: " << numBasicBlocks << "\n";
        errs() << "    Number of Instructions: " << numInstructions << "\n";
        errs() << "    Number of Successors: " << numSuccessors << "\n";
        errs() << "    Number of Predecessors: " << numPredecessors << "\n";
      }

      errs() << "Number of Basic Blocks: " << numBasicBlocks << "\n";
      errs() << "Maximum Successors: " << maxSuccessors << "\n";
      errs() << "Maximum Predecessors: " << maxPredecessors << "\n";

      return false; 
    }
  };
}

char FunctionAnalysisPass::ID = 0;
static RegisterPass<FunctionAnalysisPass> X("func-analysis", "Function Analysis Pass", false, false);