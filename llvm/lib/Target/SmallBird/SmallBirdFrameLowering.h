#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDFRAMELOWERING_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDFRAMELOWERING_H

#include "SmallBird.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class SmallBirdSubtarget;

class SmallBirdFrameLowering : public TargetFrameLowering {
protected:
  const SmallBirdSubtarget &STI;

public:
  //explicit SmallBirdFrameLowering(const SmallBirdSubtarget &sti, unsigned Alignment)
  explicit SmallBirdFrameLowering(const SmallBirdSubtarget &sti, llvm::Align Alignment)
    : TargetFrameLowering(StackGrowsDown, Alignment, 0, Alignment),
      STI(sti) { }

  static const SmallBirdFrameLowering *create(const SmallBirdSubtarget &ST);

  bool hasFP(const MachineFunction &MF) const override;

};

// Create SmallBirdFrameLowering objects.
const SmallBirdFrameLowering *createSmallBirdSEFrameLowering(const SmallBirdSubtarget &ST);

} // End llvm namespace

#endif
