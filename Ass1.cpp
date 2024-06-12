#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h" // For control flow graph utilities
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct FunctionAnalysisPass : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    FunctionAnalysisPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      errs() << "Function: " << F.getName() << "\n";

      int numBasicBlocks = 0;
      int maxSuccessors = 0;
      int maxPredecessors = 0;

      // Iterate over each basic block in the function
      for (BasicBlock &BB : F) {
        numBasicBlocks++;
        int numInstructions = BB.size();
        int numSuccessors = succ_size(BasicBlock &&BB);
        int numPredecessors = pred_size(BasicBlock &&BB);


        // Get number of successors
        if (numSuccessors > maxSuccessors) {
          maxSuccessors = numSuccessors;
        }

        // Get number of predecessors
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

      return false; // This pass does not modify the function
    }
  };
}

char FunctionAnalysisPass::ID = 0;
static RegisterPass<FunctionAnalysisPass> X("func-analysis", "Function Analysis Pass", false, false);
