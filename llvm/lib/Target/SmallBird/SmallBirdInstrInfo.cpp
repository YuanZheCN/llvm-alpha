#include "SmallBirdInstrInfo.h"

#include "SmallBirdTargetMachine.h"
#include "SmallBirdMachineFunctionInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "SmallBirdGenInstrInfo.inc"

// Pin the vtable to this file
void SmallBirdInstrInfo::anchor() { }

SmallBirdInstrInfo::SmallBirdInstrInfo(const SmallBirdSubtarget &STI)
  : Subtarget(STI) { }

const SmallBirdInstrInfo *SmallBirdInstrInfo::create(SmallBirdSubtarget &STI) {
  return llvm::createSmallBirdSEInstrInfo(STI);
}

unsigned SmallBirdInstrInfo::GetInstSizeInBytes(const MachineInstr &MI) const {
  switch (MI.getOpcode()) {
    default:
      return MI.getDesc().getSize();
  }
}

MachineMemOperand *
SmallBirdInstrInfo::GetMemOperand(MachineBasicBlock &MBB, int FI,
                             MachineMemOperand::Flags Flags) const {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  Align Align_a = MFI.getObjectAlign(FI);

  return MF.getMachineMemOperand(MachinePointerInfo::getFixedStack(MF, FI),
                                 Flags, MFI.getObjectSize(FI), Align_a);
}