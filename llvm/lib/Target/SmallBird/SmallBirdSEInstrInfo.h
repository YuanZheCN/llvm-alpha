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

};
} // End llvm namespace

#endif