//===-- SmallBirdTargetDesc.h - SmallBird Target Descriptions -------------*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides SmallBird specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SmallBird_MCTARGETDESC_SmallBirdMCTARGETDESC_H
#define LLVM_LIB_TARGET_SmallBird_MCTARGETDESC_SmallBirdMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
  class Target;
  class Triple;
  class MCAsmBackend;
  class MCCodeEmitter;
  class MCContext;
  class MCInstrInfo;
  class MCObjectWriter;
  class MCRegisterInfo;
  class MCSubtargetInfo;
  class StringRef;

  class raw_ostream;

  Target &getTheSmallBirdTarget();
  //Target &getTheSmallBirdelTarget();
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
