//===-- SmallBirdMCInstLower.cpp - Convert SmallBird MachineInstr to MCInst-*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower SmallBird MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "SmallBirdMCInstLower.h"

#include "SmallBirdAsmPrinter.h"
#include "SmallBirdInstrInfo.h"
#include "MCTargetDesc/SmallBirdBaseInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

SmallBirdMCInstLower::SmallBirdMCInstLower(SmallBirdAsmPrinter &asmprinter)
  : AsmPrinter(asmprinter) { }

void SmallBirdMCInstLower::Initialize(MCContext *C) {
  Ctx = C;
}

static void CreateMCInst(MCInst &Inst, unsigned Opc, const MCOperand &Operand0,
                         const MCOperand &Operand1,
                         const MCOperand &Operand2 = MCOperand() ) {
  Inst.setOpcode(Opc);
  Inst.addOperand(Operand0);
  Inst.addOperand(Operand1);
  if (Operand2.isValid())
    Inst.addOperand(Operand2);
}

MCOperand SmallBirdMCInstLower::LowerOperand(const MachineOperand &MO,
                                        unsigned offset) const {
  MachineOperandType MOTy = MO.getType();

  switch (MOTy) {
  default: llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands
    if (MO.isImplicit()) break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm() + offset);
  case MachineOperand::MO_RegisterMask:
    break;
  }

  return MCOperand();
}

void SmallBirdMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMCInst) const {
  OutMCInst.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCOp = LowerOperand(MO);

    if (MCOp.isValid())
      OutMCInst.addOperand(MCOp);
  }
}
