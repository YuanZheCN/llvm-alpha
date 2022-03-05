#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEINSTRINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEINSTRINFO_H

#include "SmallBirdInstrInfo.h"
#include "SmallBirdSERegisterInfo.h"
#include "SmallBirdMachineFunctionInfo.h"

namespace llvm {

class SmallBirdSEInstrInfo : public SmallBirdInstrInfo {
  const SmallBirdSERegisterInfo RI;

public:
  explicit SmallBirdSEInstrInfo(const SmallBirdSubtarget &STI);

  const SmallBirdRegisterInfo &getRegisterInfo() const override;

  // Expand Pseudo instructions into real backend instructions
  bool expandPostRAPseudo(MachineInstr &MI) const override;

void storeRegToStack(MachineBasicBlock &MBB,
                       MachineBasicBlock::iterator MI,
                       Register SrcReg, bool isKill, int FrameIndex,
                       const TargetRegisterClass *RC,
                       const TargetRegisterInfo *TRI,
                       int64_t Offset) const override;

  void loadRegFromStack(MachineBasicBlock &MBB,
                        MachineBasicBlock::iterator MI,
                        Register DestReg, int FrameIndex,
                        const TargetRegisterClass *RC,
                        const TargetRegisterInfo *TRI,
                        int64_t Offset) const override;

  // Adjust SP by Amount bytes.
  void adjustStackPtr(unsigned SP, int64_t Amount, MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator I) const override;

  // Emit a series of instructions to load an immediate. If NewImm is a
  // non-NULL parameter, the last instruction is not emitted, but instead
  // its immediate operand is returned in NewImm.
  unsigned loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                         MachineBasicBlock::iterator II, const DebugLoc &DL,
                         unsigned *NewImm) const;
private:
  void expandRetLR(MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const;
  
};
} // End llvm namespace

#endif