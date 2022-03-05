//===-- SmallBirdISelLowering.cpp - SmallBird DAG Lowering Implementation -*- C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that SmallBird uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "SmallBirdISelLowering.h"

#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBirdTargetMachine.h"
#include "SmallBirdTargetObjectFile.h"
#include "SmallBirdSubtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegionInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "SmallBird-lower"

const char *SmallBirdTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
    case SmallBirdISD::JmpLink        :  return "SmallBirdISD::JmpLink";
    case SmallBirdISD::TailCall       :  return "SmallBirdISD::TailCall";
    case SmallBirdISD::Hi             :  return "SmallBirdISD::Hi";
    case SmallBirdISD::Lo             :  return "SmallBirdISD::Lo";
    case SmallBirdISD::GPRel          :  return "SmallBirdISD::GPRel";
    case SmallBirdISD::Ret            :  return "SmallBirdISD::Ret";
    case SmallBirdISD::EH_RETURN      :  return "SmallBirdISD::EH_RETURN";
    case SmallBirdISD::DivRem         :  return "SmallBirdISD::DivRem";
    case SmallBirdISD::DivRemU        :  return "SmallBirdISD::DivRemU";
    case SmallBirdISD::Wrapper        :  return "SmallBirdISD::Wrapper";
    default                      :  return NULL;
  }
}

SmallBirdTargetLowering::SmallBirdTargetLowering(const SmallBirdTargetMachine &TM,
                                       const SmallBirdSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI){
  // Set .align 2,
  // It will emit .align 2 later
  setMinFunctionAlignment(Align(2));
}

const SmallBirdTargetLowering *
SmallBirdTargetLowering::create(const SmallBirdTargetMachine &TM,
                           const SmallBirdSubtarget &STI) {
  return createSmallBirdSETargetLowering(TM, STI);
}
//===----------------------------------------------------------------------===//
// Lower Helper Functions
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Misc Lower Operation Implementation
//===----------------------------------------------------------------------===//

#include "SmallBirdGenCallingConv.inc"

//===----------------------------------------------------------------------===//
// Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

// Transform physical registers into virtual registers and generate load
// operations for arguments places on the stack.
SDValue
SmallBirdTargetLowering::LowerFormalArguments(SDValue Chain,
                                         CallingConv::ID CallConv, bool IsVarArg,
                                         const SmallVectorImpl<ISD::InputArg> &Ins,
                                         const SDLoc &DL, SelectionDAG &DAG,
                                         SmallVectorImpl<SDValue> &InVals)
                                         const {
  return Chain;  // Leave empty temporary
}

//===----------------------------------------------------------------------===//
// Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue
SmallBirdTargetLowering::LowerReturn(SDValue Chain,
                                CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutsVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {
  // CCValAssign represent the assignment of the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;
  MachineFunction &MF = DAG.getMachineFunction();

  // CCState represent the info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, *DAG.getContext());
  //SmallBirdCC SmallBirdCCInfo(CallConv, ABI.IsO32(), CCInfo);
  SmallBirdCC SmallBirdCCInfo(CallConv, 0, CCInfo);

  // Analysis return values.
  SmallBirdCCInfo.analyzeReturn(Outs, Subtarget.abiUsesSoftFloat(),
                           MF.getFunction().getReturnType());

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    SDValue Val = OutsVals[i];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    if (RVLocs[i].getValVT() != RVLocs[i].getLocVT())
      Val = DAG.getNode(ISD::BITCAST, DL, RVLocs[i].getLocVT(), Val);

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  // The SmallBird ABIs for returning structs by value requires that we copy the
  // sret argument into $v0 for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into $v0.
  if (MF.getFunction().hasStructRetAttr()) {
    SmallBirdMachineFunctionInfo *SmallBirdFI = MF.getInfo<SmallBirdMachineFunctionInfo>();
    unsigned Reg = SmallBirdFI->getSRetReturnReg();

    if (!Reg)
      llvm_unreachable("sret virtual register not created in the entry block");
    SDValue Val = DAG.getCopyFromReg(Chain, DL, Reg,
                                     getPointerTy(DAG.getDataLayout()));
    unsigned V0 = SmallBird::V0;

    Chain = DAG.getCopyToReg(Chain, DL, V0, Val, Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(V0, getPointerTy(DAG.getDataLayout())));
  }

  RetOps[0] = Chain;    // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return on SmallBird is always a "ret $lr"
  return DAG.getNode(SmallBirdISD::Ret, DL, MVT::Other, RetOps);
}

//===----------------------------------------------------------------------===//
// Methods implement for CallingConv
//===----------------------------------------------------------------------===//

template<typename Ty>
void SmallBirdTargetLowering::SmallBirdCC::
analyzeReturn(const SmallVectorImpl<Ty> &RetVals, bool IsSoftFloat,
              const SDNode *CallNode, const Type *RetTy) const {
  CCAssignFn *Fn;

  Fn = RetCC_SmallBird;

  for (unsigned I = 0, E = RetVals.size(); I < E; ++I) {
    MVT VT = RetVals[I].VT;
    ISD::ArgFlagsTy Flags = RetVals[I].Flags;
    MVT RegVT = this->getRegVT(VT, RetTy, CallNode, IsSoftFloat);

    if (Fn(I, VT, RegVT, CCValAssign::Full, Flags, this->CCInfo)) {
#ifndef NDEBUG
      dbgs() << "Call result #" << I << " has unhandled type "
             << EVT(VT).getEVTString() << '\n';
#endif
      llvm_unreachable(nullptr);
    }
  }
}

void SmallBirdTargetLowering::SmallBirdCC::
analyzeCallResult(const SmallVectorImpl<ISD::InputArg> &Ins, bool IsSoftFloat,
                  const SDNode *CallNode, const Type *RetTy) const {
  analyzeReturn(Ins, IsSoftFloat, CallNode, RetTy);
}

void SmallBirdTargetLowering::SmallBirdCC::
analyzeReturn(const SmallVectorImpl<ISD::OutputArg> &Outs, bool IsSoftFloat,
              const Type *RetTy) const {
  analyzeReturn(Outs, IsSoftFloat, nullptr, RetTy);
}

unsigned SmallBirdTargetLowering::SmallBirdCC::reservedArgArea() const {
  return (IsO32 && (CallConv != CallingConv::Fast)) ? 8 : 0;
}

MVT SmallBirdTargetLowering::SmallBirdCC::getRegVT(MVT VT, const Type *OrigTy,
                                         const SDNode *CallNode,
                                         bool IsSoftFloat) const {
  if (IsSoftFloat || IsO32)
    return VT;

  return VT;
}

SmallBirdTargetLowering::SmallBirdCC::SmallBirdCC(
  CallingConv::ID CC, bool IsO32_, CCState &Info,
  SmallBirdCC::SpecialCallingConvType SpecialCallingConv_)
  : CCInfo(Info), CallConv(CC), IsO32(IsO32_) {
  // Pre-allocate reserved argument area.
  CCInfo.AllocateStack(reservedArgArea(), Align(1));
}