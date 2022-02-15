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
//    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) { }
      : TargetLowering(TM), Subtarget(STI) { }

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
  return DAG.getNode(SmallBirdISD::Ret, DL, MVT::Other, Chain,
                     DAG.getRegister(SmallBird::LR, MVT::i32));
}
