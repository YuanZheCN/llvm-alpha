#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEISELLOWERING_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEISELLOWERING_H

#include "SmallBirdISelLowering.h"
#include "SmallBirdRegisterInfo.h"

namespace llvm {
class SmallBirdSETargetLowering : public SmallBirdTargetLowering {
public:
  explicit SmallBirdSETargetLowering(const SmallBirdTargetMachine &TM,
                                const SmallBirdSubtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
};

}// End llvm namespace

#endif