#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDREGISTERINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDREGISTERINFO_H

#include "SmallBird.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "SmallBirdGenRegisterInfo.inc"

namespace llvm {
class SmallBirdSubtarget;
class TargetInstrInfo;
class Type;

class SmallBirdRegisterInfo : public SmallBirdGenRegisterInfo {
protected:
  const SmallBirdSubtarget &Subtarget;

public:
  SmallBirdRegisterInfo(const SmallBirdSubtarget &Subtarget);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override;

  /// Stack Frame Processing Methods
  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// Debug information queries.
  llvm::Register getFrameRegister(const MachineFunction &MF) const override;

  /// \brief Return GPR register class.
  virtual const TargetRegisterClass *intRegClass(unsigned Size) const = 0;
};

} // end namespace llvm

#endif
