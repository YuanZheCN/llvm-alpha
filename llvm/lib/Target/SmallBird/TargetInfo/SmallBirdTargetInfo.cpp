#include "SmallBird.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheSmallBirdTarget() {
  static Target TheSmallBirdTarget;
  return TheSmallBirdTarget;
}
Target &llvm::getTheSmallBirdelTarget() {
  static Target TheSmallBirdelTarget;
  return TheSmallBirdelTarget;
}

extern "C" void LLVMInitializeSmallBirdTargetInfo() {
  RegisterTarget<Triple::SmallBird, /*HasJIT=*/true> X(getTheSmallBirdTarget(),
                                                  "SmallBird", "SmallBird", "SmallBird");

  RegisterTarget<Triple::SmallBirdel, /*HasJIT=*/true> Y(getTheSmallBirdelTarget(),
                                                    "SmallBirdel", "SmallBirdel", "SmallBird");
}