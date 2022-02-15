#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include <map>

namespace llvm {
  // This class is derived from MachienFunction private SmallBird target-specific
  // information for each MachineFunction.
  class SmallBirdMachineFunctionInfo : public MachineFunctionInfo {
  public:
    SmallBirdMachineFunctionInfo(MachineFunction &MF)
        : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0) { }

    ~SmallBirdMachineFunctionInfo();

    int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
    void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  private:
    virtual void anchor();

    MachineFunction &MF;

    // Frame index for start of varargs area
    int VarArgsFrameIndex;

    unsigned MaxCallFrameSize;
  };
} // End llvm namespace
#endif
