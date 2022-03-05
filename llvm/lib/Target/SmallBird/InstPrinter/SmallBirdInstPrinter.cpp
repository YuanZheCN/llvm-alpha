//===-- SmallBirdInstPrinter.cpp - Convert MCInst to assembly syntax -*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an SmallBird MCInst to an assembly file.
//
//===----------------------------------------------------------------------===//

#include "SmallBirdInstPrinter.h"

#include "SmallBirdInstrInfo.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "SmallBirdGenAsmWriter.inc"

void SmallBirdInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  // getRegisterName(RegNo) defiend in SmallBirdGenAsmWriter.inc which indicate in SmallBird.td
  OS << '$' << StringRef(getRegisterName(RegNo)).lower();
}


 //void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
 //                const MCSubtargetInfo &STI, raw_ostream &O) override;

 // void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
 //                const MCSubtargetInfo &STI, raw_ostream &O) override;

void SmallBirdInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,raw_ostream &OS) {
  // Try to print any aliases first
  if (!printAliasInstr(MI, Address, OS))
    printInstruction(MI, Address, OS);
  printAnnotation(OS, Annot);
}

void SmallBirdInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Op.getExpr()->print(O, &MAI, true);
}

void SmallBirdInstPrinter::printUnsignedImm(const MCInst *MI, int OpNum,
                                       raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(OpNum);
  if (MO.isImm())
    O << (unsigned short int)MO.getImm();
  else
    printOperand(MI, OpNum, O);
}

void SmallBirdInstPrinter::printMemOperand(const MCInst *MI, int OpNum,
                                      raw_ostream &O) {
  // Load/Store memory operands => imm($reg)
  // If PIC target, the target is loaded as the pattern => ld $t9, %call16(%gp)
  printOperand(MI, OpNum+1, O);
  O << "(";
  printOperand(MI, OpNum, O);
  O << ")";
}
