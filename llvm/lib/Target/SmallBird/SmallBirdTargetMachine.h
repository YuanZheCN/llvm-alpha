#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDTARGETMACHINE_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDTARGETMACHINE_H

//#include "MCTargetDesc/SmallBirdABIInfo.h"
#include "SmallBirdSubtarget.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class formatted_raw_ostream;
class SmallBirdRegisterInfo;

class SmallBirdTargetMachine : public LLVMTargetMachine {
  bool isLittle;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  //SmallBirdABIInfo ABI;
  SmallBirdSubtarget DefaultSubtarget;

  mutable StringMap<std::unique_ptr<SmallBirdSubtarget>> SubtargetMap;
public:
  SmallBirdTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    CodeGenOpt::Level OL, bool JIT, bool isLittle);
  ~SmallBirdTargetMachine() override;

  const SmallBirdSubtarget *getSubtargetImpl() const {
    return &DefaultSubtarget;
  }

  // Can use this interface to fetch subtarget
  const SmallBirdSubtarget *getSubtargetImpl(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
  bool isLittleEndian() const { return isLittle; }
  //const SmallBirdABIInfo &getABI() const { return ABI; }
};

// This is big endian type SmallBird32 target machine.
// class SmallBirdebTargetMachine : public SmallBirdTargetMachine {
//   virtual void anchor();
// public:
//   SmallBirdebTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
//                       StringRef FS, const TargetOptions &Options,
//                       Optional<Reloc::Model> RM,
//                       Optional<CodeModel::Model> CM,
//                       CodeGenOpt::Level OL, bool JIT);
// };

//We only implement little end target, a bit end can also implement here
// This is default little endian SmallBird32 target machine.
class SmallBirdelTargetMachine : public SmallBirdTargetMachine {
  virtual void anchor();
public:
  SmallBirdelTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM,
                      Optional<CodeModel::Model> CM,
                      CodeGenOpt::Level OL, bool JIT);
};
} // End llvm namespace

#endif