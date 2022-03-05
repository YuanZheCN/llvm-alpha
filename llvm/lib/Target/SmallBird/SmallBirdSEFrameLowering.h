#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEFRAMELOWERING_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEFRAMELOWERING_H

#include "SmallBirdFrameLowering.h"

namespace llvm {
class SmallBirdSEFrameLowering : public SmallBirdFrameLowering {
public:
  explicit SmallBirdSEFrameLowering(const SmallBirdSubtarget &STI);

  // These methods insert prolog and epilog code into the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

};

} // End llvm namespace

#endif
