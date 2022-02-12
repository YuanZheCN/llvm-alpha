#ifndef LLVM_LIB_TARGET_SMALLBIRD_MCTARGETDESC_SmallBirdMCTARGETDESC_H
#define LLVM_LIB_TARGET_SMALLBIRD_MCTARGETDESC_SmallBirdMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
  class Target;
  class Triple;

  Target &getTheSmallBirdTarget();
  Target &getTheSmallBirdelTarget();
}

// Defines symbolic names for SmallBird registers. This defines a mapping from
//  register name to register number.
#define GET_REGINFO_ENUM
#include "SmallBirdGenRegisterInfo.inc"

// Defines symbolic names for SmallBird instructions.
#define GET_INSTRINFO_ENUM
#include "SmallBirdGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "SmallBirdGenSubtargetInfo.inc"

#endif