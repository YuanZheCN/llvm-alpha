#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDTARGETOBJECTFILE_H

#include "SmallBirdTargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class SmallBirdTargetMachine;
class SmallBirdTargetObjectFile : public TargetLoweringObjectFileELF {
  MCSection *SmallDataSection;
  MCSection *SmallBSSSection;
  const SmallBirdTargetMachine *TM;
public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

};
} // end namespace llvm

#endif