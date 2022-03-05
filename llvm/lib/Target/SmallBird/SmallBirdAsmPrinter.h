//===-- SmallBirdAsmPrinter.h - SmallBird Assembly Printer ----------------*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// SmallBird assembly printer class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDASMPRINTER_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDASMPRINTER_H

#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBirdMCInstLower.h"
#include "SmallBirdSubtarget.h"
#include "SmallBirdTargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MCStreamer;
class MachineInstr;
class MachineBasicBlock;
class Module;
class raw_ostream;

class LLVM_LIBRARY_VISIBILITY SmallBirdAsmPrinter : public AsmPrinter {

  void EmitInstrWithMacroNoAT(const MachineInstr *MI);

private:
  // lowerOperand : Convert a MachineOperand into the equivalent MCOperand
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp);

public:
  const SmallBirdSubtarget *Subtarget;
  const SmallBirdMachineFunctionInfo *SmallBirdMFI;
  SmallBirdMCInstLower MCInstLowering;

  explicit SmallBirdAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)),
      MCInstLowering(*this) {
    Subtarget = static_cast<SmallBirdTargetMachine &>(TM).getSubtargetImpl();
  }

  virtual StringRef getPassName() const override {
    return "SmallBird Assmebly Printer";
  }

  virtual bool runOnMachineFunction(MachineFunction &MF) override;

  // EmitInstruction() must exists or will have run time error
  void emitInstruction(const MachineInstr *MI) override;
  void printSavedRegsBitmask(raw_ostream &O);
  void printHex32(unsigned int Value, raw_ostream &O);
  void emitFrameDirective();
  const char *getCurrentABIString() const;
  void emitFunctionEntryLabel() override;
  void emitFunctionBodyStart() override;
  void emitFunctionBodyEnd() override;
  void emitStartOfAsmFile(Module &M) override;
  void PrinterDebugValueComment(const MachineInstr *MI, raw_ostream &OS);
};
} // end namespace llvm

#endif
