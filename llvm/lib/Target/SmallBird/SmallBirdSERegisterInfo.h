#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEREGISTERINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEREGISTERINFO_H

#include "SmallBirdRegisterInfo.h"

namespace llvm {
class SmallBirdSEInstrInfo;

class SmallBirdSERegisterInfo : public SmallBirdRegisterInfo {
public:
  SmallBirdSERegisterInfo(const SmallBirdSubtarget &Subtarget);

  const TargetRegisterClass *intRegClass(unsigned Size) const override;
};

} // end namespace llvm

#endif