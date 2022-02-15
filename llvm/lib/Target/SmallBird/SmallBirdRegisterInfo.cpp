#include "SmallBirdRegisterInfo.h"

#include "SmallBird.h"
#include "SmallBirdSubtarget.h"
#include "SmallBirdMachineFunctionInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#define GET_REGINFO_TARGET_DESC
#include "SmallBirdGenRegisterInfo.inc"

#define DEBUG_TYPE "SmallBird-reg-info"

using namespace llvm;

SmallBirdRegisterInfo::SmallBirdRegisterInfo(const SmallBirdSubtarget &ST)
  : SmallBirdGenRegisterInfo(SmallBird::LR), Subtarget(ST) {}

//===----------------------------------------------------------------------===//
// Callee Saved Registers methods
//===----------------------------------------------------------------------===//
/// SmallBird Callee Saved Registers
// In SmallBirdCallConv.td, defined CalleeSavedRegs
const MCPhysReg *
SmallBirdRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_032_SaveList;
}

const uint32_t *
SmallBirdRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const {
  return CSR_032_RegMask;
}

BitVector SmallBirdRegisterInfo::
getReservedRegs(const MachineFunction &MF) const {
  static const uint16_t ReservedCPURegs[] = {
    SmallBird::ZERO, SmallBird::AT, SmallBird::SP, SmallBird::LR, SmallBird::PC
  };
  BitVector Reserved(getNumRegs());

  for (unsigned I = 0; I < array_lengthof(ReservedCPURegs); ++I)
    Reserved.set(ReservedCPURegs[I]);

  return Reserved;
}

//- If no eliminateFrameIndex(), it will hang on run.
// FrameIndex represent objects inside a abstract stack.
// We must replace FrameIndex with an stack/frame pointer
// direct reference.
void SmallBirdRegisterInfo::
eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                    unsigned FIOperandNum, RegScavenger *RS) const {
}

bool
SmallBirdRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return true;
}

bool
SmallBirdRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  return true;
}

//unsigned SmallBirdRegisterInfo::
llvm::Register SmallBirdRegisterInfo::
getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  return TFI->hasFP(MF) ? (SmallBird::FP) :
                          (SmallBird::SP);
}
