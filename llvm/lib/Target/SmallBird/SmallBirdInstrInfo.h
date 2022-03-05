#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDINSTRINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDINSTRINFO_H

#include "SmallBird.h"
#include "SmallBirdRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SmallBirdGenInstrInfo.inc"

namespace llvm {

class SmallBirdInstrInfo : public SmallBirdGenInstrInfo {
  virtual void anchor();
protected:
  const SmallBirdSubtarget &Subtarget;
public:
  explicit SmallBirdInstrInfo(const SmallBirdSubtarget &STI);

  static const SmallBirdInstrInfo *create(SmallBirdSubtarget &STI);

  // TargetInstrInfo is a superset of MRegister info. As such, whenever a client
  // has an instance of instruction info, it should always be able to get
  // register info as well (through this method).
  virtual const SmallBirdRegisterInfo &getRegisterInfo() const = 0;

  // Return the number of bytes of code the specified instruction maybe.
  unsigned GetInstSizeInBytes(const MachineInstr &MI) const;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI,
                           Register SrcReg, bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override {
    storeRegToStack(MBB, MBBI, SrcReg, isKill, FrameIndex, RC, TRI, 0);
  }

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override {
    loadRegFromStack(MBB, MBBI, DestReg, FrameIndex, RC, TRI, 0);
  }

  virtual void storeRegToStack(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator MI,
                               Register SrcReg, bool isKill, int FrameIndex,
                               const TargetRegisterClass *RC,
                               const TargetRegisterInfo *TRI,
                               int64_t Offset) const = 0;

  virtual void loadRegFromStack(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator MI,
                                Register DestReg, int FrameIndex,
                                const TargetRegisterClass *RC,
                                const TargetRegisterInfo *TRI,
                                int64_t Offset) const = 0;

  virtual void adjustStackPtr(unsigned SP, int64_t Amount,
                              MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator I) const = 0;

protected:
  MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB, int FI,
                                   MachineMemOperand::Flags Flags) const;
};


const SmallBirdInstrInfo *createSmallBirdSEInstrInfo(const SmallBirdSubtarget &STI);
}

#endif