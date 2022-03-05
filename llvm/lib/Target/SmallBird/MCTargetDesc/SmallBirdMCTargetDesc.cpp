//===-- SmallBirdMCTargetDesc.cpp - SmallBird Target Descriptions ---------*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides SmallBird specific target descirptions.
//
//===----------------------------------------------------------------------===//

#include "SmallBirdMCTargetDesc.h"
#include "InstPrinter/SmallBirdInstPrinter.h"
#include "SmallBirdMCAsmInfo.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "SmallBirdGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SmallBirdGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SmallBirdGenRegisterInfo.inc"

// Select the SmallBird Architecture Feature for the given triple and cpu name.
// The function will be called at command 'llvm-objdump -d' for SmallBird elf input.
static StringRef selectSmallBirdArchFeature(const Triple &TT, StringRef CPU) {
  StringRef SmallBirdArchFeature;
  if (CPU.empty() || CPU == "generic") {
    if (TT.getArch() == Triple::SmallBird ) {
      if (CPU.empty() || CPU == "SmallBird32II") {
        SmallBirdArchFeature = "+SmallBird32II";
      } else if (CPU == "SmallBird32I") {
        SmallBirdArchFeature = "+SmallBird32I";
      }
    }
  }
  return SmallBirdArchFeature;
}

static MCInstrInfo *createSmallBirdMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSmallBirdMCInstrInfo(X); // defined in SmallBirdGenInstrInfo.inc
  return X;
}

static MCRegisterInfo *createSmallBirdMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSmallBirdMCRegisterInfo(X, SmallBird::SW); // defined in SmallBirdGenRegisterInfo.inc
  return X;
}

static MCSubtargetInfo *createSmallBirdMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  StringRef ArchFS = selectSmallBirdArchFeature(TT, CPU);
  if (!FS.empty()) {
    if (!ArchFS.empty()) {
      ArchFS = StringRef(ArchFS.str() + "," + FS.str());
    } else {
      ArchFS = FS;
    }
  }
  return createSmallBirdMCSubtargetInfoImpl(TT, CPU, CPU ,ArchFS);
  // createSmallBirdMCSubtargetInfoImpl defined in SmallBirdGenSubtargetInfo.inc
}

static MCAsmInfo *createSmallBirdMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new SmallBirdMCAsmInfo(TT);

  unsigned SP = MRI.getDwarfRegNum(SmallBird::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);

  return MAI;
}

static MCInstPrinter *createSmallBirdMCInstPrinter(const Triple &TT,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new SmallBirdInstPrinter(MAI, MII, MRI);
}

namespace {
class SmallBirdMCInstrAnalysis : public MCInstrAnalysis {
public:
  SmallBirdMCInstrAnalysis(const MCInstrInfo *Info) : MCInstrAnalysis(Info) { }
};
}

static MCInstrAnalysis *createSmallBirdMCInstrAnalysis(const MCInstrInfo *Info) {
  return new SmallBirdMCInstrAnalysis(Info);
}

extern "C" void LLVMInitializeSmallBirdTargetMC() {
  Target &TheSmallBirdTarget = getTheSmallBirdTarget();
  //Target &TheSmallBirdelTarget = getTheSmallBirdelTarget();
  for (Target *T : {&TheSmallBirdTarget}) {
    // Register the MC asm info
    RegisterMCAsmInfoFn X(*T, createSmallBirdMCAsmInfo);

    // Register the MC instruction info
    TargetRegistry::RegisterMCInstrInfo(*T, createSmallBirdMCInstrInfo);

    // Register the MC register info
    TargetRegistry::RegisterMCRegInfo(*T, createSmallBirdMCRegisterInfo);

    // Register the MC Subtarget info
    TargetRegistry::RegisterMCSubtargetInfo(*T, createSmallBirdMCSubtargetInfo);

    // Register the MC instruction analyzer
    TargetRegistry::RegisterMCInstrAnalysis(*T, createSmallBirdMCInstrAnalysis);

    // Register the MC instruction printer
    TargetRegistry::RegisterMCInstPrinter(*T, createSmallBirdMCInstPrinter);
  }
}
