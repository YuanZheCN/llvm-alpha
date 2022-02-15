#include "SmallBird.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheSmallBirdTarget() {
  static Target TheSmallBirdTarget;
  return TheSmallBirdTarget;
}

extern "C" void LLVMInitializeSmallBirdTargetInfo() {
  RegisterTarget<Triple::SmallBird, /*HasJIT=*/true> X(getTheSmallBirdTarget(),
                                                  "SmallBird", "SmallBird", "SmallBird");
}