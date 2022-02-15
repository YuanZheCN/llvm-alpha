//===-- SmallBirdTargetMachine.cpp - Define TargetMachine for SmallBird ---*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about SmallBird target spec
//
//===----------------------------------------------------------------------===//

#include "SmallBirdTargetMachine.h"
#include "SmallBird.h"
#include "SmallBirdTargetObjectFile.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "SmallBird"

extern "C" void LLVMInitializeSmallBirdTarget() {
  // Register the target.
  // Big endian Target Machine
  //RegisterTargetMachine<SmallBirdebTargetMachine> X(getTheSmallBirdTarget());
  // Little endian Target Machine
  RegisterTargetMachine<SmallBirdelTargetMachine> Y(getTheSmallBirdTarget());
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options,
                                     bool isLittle) {
  std::string Ret = "";
  if (isLittle)
    Ret += "e";
  else
    Ret += "E";

  Ret += "-m:m";

  // Pointer size and alignment
  Ret += "-p:32:32";

  // 8 and 16 bits integers only need to have natural alignment, but try to
  // align them to 32 bits. 64 bits integers have natural alignment.
  Ret += "-i8:8:32-i16:16:32-i64:64";

  // 32 bits registers are always available and the stack is at least 64 bits aligned
  Ret += "-n32-s64";

  return Ret;
}

static Reloc::Model getEffectiveRelocModel(bool JIT,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue() || JIT)
    return Reloc::Static;
  return *RM;
}
// DataLayout --> Big-endian, 32-bit pointer/ABI/alignment
// The stack is always 8 byte aligned
// On function prologue, the stack is created by decrementing
// its pointer. Once decremented, all references are done with positive
// offset from the stack/frame pointer, using StackGrowsUp enables
// an easier handling.
SmallBirdTargetMachine::SmallBirdTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT,
                                     bool isLittle)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, isLittle), TT,
                        CPU, FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      isLittle(isLittle), TLOF(std::make_unique<SmallBirdTargetObjectFile>()),
      //ABI(SmallBirdABIInfo::computeTargetABI()),
      //ABI()),
      DefaultSubtarget(TT, CPU, FS, isLittle, *this) {
  initAsmInfo();
}

SmallBirdTargetMachine::~SmallBirdTargetMachine() { }

// void SmallBirdebTargetMachine::anchor() { }

// SmallBirdebTargetMachine::SmallBirdebTargetMachine(const Target &T, const Triple &TT,
//                                          StringRef CPU, StringRef FS,
//                                          const TargetOptions &Options,
//                                          Optional<Reloc::Model> RM,
//                                          Optional<CodeModel::Model> CM,
//                                          CodeGenOpt::Level OL, bool JIT)
//     : SmallBirdTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, false) { }

void SmallBirdelTargetMachine::anchor() { }

SmallBirdelTargetMachine::SmallBirdelTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         Optional<CodeModel::Model> CM,
                                         CodeGenOpt::Level OL, bool JIT)
    : SmallBirdTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, true) { }

const SmallBirdSubtarget *
SmallBirdTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
                        ? CPUAttr.getValueAsString().str()
                        : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attribute::None)
                       ? FSAttr.getValueAsString().str()
                       : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<SmallBirdSubtarget>(TargetTriple, CPU, FS, isLittle,
                                         *this);
  }
  return I.get();
}

namespace {
// SmallBird Code Generator Pass Configuration Options.
class SmallBirdPassConfig : public TargetPassConfig {
public:
  SmallBirdPassConfig(SmallBirdTargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) { }

  SmallBirdTargetMachine &getSmallBirdTargetMachine() const {
    return getTM<SmallBirdTargetMachine>();
  }

  const SmallBirdSubtarget &getSmallBirdSubtarget() const {
    return *getSmallBirdTargetMachine().getSubtargetImpl();
  }
};
} // end namespace

TargetPassConfig *SmallBirdTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new SmallBirdPassConfig(*this, PM);
}
