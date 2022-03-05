//===-- SmallBirdMCInstLower.h - Lower MachineInstr to MCInst --------*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDMCINSTLOWER_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDMCINSTLOWER_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineFunction;
class SmallBirdAsmPrinter;

class LLVM_LIBRARY_VISIBILITY SmallBirdMCInstLower {
  typedef MachineOperand::MachineOperandType MachineOperandType;
  MCContext *Ctx;
  SmallBirdAsmPrinter &AsmPrinter;
public:
  SmallBirdMCInstLower(SmallBirdAsmPrinter &asmprinter);
  void Initialize(MCContext *C);
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
  MCOperand LowerOperand(const MachineOperand &MO, unsigned offset = 0) const;
};

} // end namespace llvm

#endif
