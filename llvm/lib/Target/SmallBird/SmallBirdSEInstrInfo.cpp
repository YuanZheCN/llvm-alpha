#include "SmallBirdSEInstrInfo.h"

#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBirdTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

SmallBirdSEInstrInfo::SmallBirdSEInstrInfo(const SmallBirdSubtarget &STI)
    : SmallBirdInstrInfo(STI),
      RI(STI) {}

const SmallBirdRegisterInfo &SmallBirdSEInstrInfo::getRegisterInfo() const {
  return RI;
}

const SmallBirdInstrInfo *llvm::createSmallBirdSEInstrInfo(const SmallBirdSubtarget &STI) {
  return new SmallBirdSEInstrInfo(STI);
}
