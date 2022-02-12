#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRD_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRD_H

#include "MCTargetDesc/SmallBirdMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class SmallBirdTargetMachine;
  class FunctionPass;
}

#define ENABLE_GPRESTORE  // The $gp register caller saved register enable

#endif