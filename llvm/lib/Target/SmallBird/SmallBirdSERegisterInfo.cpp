
#include "SmallBirdSERegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "SmallBird-reg-info"

SmallBirdSERegisterInfo::SmallBirdSERegisterInfo(const SmallBirdSubtarget &ST)
  : SmallBirdRegisterInfo(ST) {}

const TargetRegisterClass *
SmallBirdSERegisterInfo::intRegClass(unsigned Size) const {
  return &SmallBird::CPURegsRegClass;
}
