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
        : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0), EmitNOAT(false) { }

    ~SmallBirdMachineFunctionInfo();

    unsigned getSRetReturnReg() const { return SRetReturnReg; }
    void setSRetReturnReg(unsigned Reg) { SRetReturnReg = Reg; }

    int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
    void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

    bool hasByvalArg() const { return HasByvalArg; }
    void setFormalArgInfo(unsigned Size, bool HasByval) {
      IncomingArgSize = Size;
      HasByvalArg = HasByval;
    }

    bool getEmitNOAT() const { return EmitNOAT; }
    void setEmitNOAT() { EmitNOAT = true; }

    unsigned getIncomingArgSize() const { return IncomingArgSize; }

    bool callsEhReturn() const { return CallsEhReturn; }
    void setCallsEhReturn() { CallsEhReturn = true; }

    bool callsEhDwarf() const { return CallsEhDwarf; }
    void setCallsEhDwarf() { CallsEhDwarf = true; }

    void createEhDataRegsFI();
    int getEhDataRegFI(unsigned Reg) const { return EhDataRegFI[Reg]; }

    unsigned getMaxCallFrameSize() const { return MaxCallFrameSize; }
    void setMaxCallFrameSize(unsigned S) { MaxCallFrameSize = S; }

  private:
    virtual void anchor();

    MachineFunction &MF;

    // Some subtargets require that sret lowering includes returning the value
    // of the returned struct in a register. This field holds the virtual
    // register into which the sret argument is passed.
    unsigned SRetReturnReg;

    // Frame index for start of varargs area.
    int VarArgsFrameIndex;

    // True if function has a byval argument.
    bool HasByvalArg;

    // Size of incoming argument area.
    unsigned IncomingArgSize;

    // Whether the function calls llvm.eh.return.
    bool CallsEhReturn;

    // Whether the function calls llvm.eh.dwarf.
    bool CallsEhDwarf;

    // Frame objects for spilling eh data registers.
    bool EhDataRegFI[2];

    unsigned MaxCallFrameSize;

    bool EmitNOAT;
  };
} // End llvm namespace
#endif

