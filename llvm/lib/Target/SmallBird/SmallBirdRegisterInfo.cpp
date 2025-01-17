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
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  SmallBirdMachineFunctionInfo *SmallBirdFI = MF.getInfo<SmallBirdMachineFunctionInfo>();

  unsigned i = 0;
  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() &&
           "Instr doesn't have FrameIndex operand!");
  }

  LLVM_DEBUG(errs() << "\nFunction : " << MF.getFunction().getName() << "\n";
             errs() << "<---------->\n" << MI);

  int FrameIndex = MI.getOperand(i).getIndex();
  uint64_t stackSize = MF.getFrameInfo().getStackSize();
  int64_t spOffset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  LLVM_DEBUG(errs() << "FrameIndex : " << FrameIndex << "\n"
                    << "spOffset   : " << spOffset   << "\n"
                    << "stackSize  : " << stackSize  << "\n");

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  int MinCSFI = 0;
  int MaxCSFI = -1;

  if (CSI.size()) {
    MinCSFI = CSI[0].getFrameIdx();
    MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
  }

  // The following stack frame objects are always referenced relative to $sp:
  //  1. Outgoing arguments.
  //  2. Pointer to dynamically allocated stack space.
  //  3. Locations for callee-saved registers.
  // Everything else is referenced relative to whatever register
  // getFrameRegister() returns.
  unsigned FrameReg;

  FrameReg = SmallBird::SP;

  // Calculate final offset.
  // There is no need to change the offset if the frame object is one of the
  // following: an outgoing argument, pointer to a dynamically allocated
  // stack space or a $gp restore location,
  // If the frame object is any of the following, its offset must be adjust
  // by adding the size of the stack: incomming argument, callee-saved register
  // location or local variable.
  int64_t Offset;
  Offset = spOffset + (int64_t)stackSize;

  Offset += MI.getOperand(i+1).getImm();

  LLVM_DEBUG(errs() << "Offset : " << Offset << "\n"
                    << "<---------->\n");

  // If MI is not a debug value, make sure Offset fits in the 16-bit immediate
  // field.
  if (!MI.isDebugValue() && !isInt<16>(Offset)) {
    assert("(!MI.isDebugValue() && !isInt<16>(Offset))");
  }

  MI.getOperand(i).ChangeToRegister(FrameReg, false);
  MI.getOperand(i+1).ChangeToImmediate(Offset);
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
