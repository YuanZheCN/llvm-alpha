//===-- SmallBirdSEISelDAGToDAG.cpp - A DAG to DAG Inst Selector for SmallBirdSE -*-C++ -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of SmallBirdDAGToDAGISel specialized for SmallBird32.
//
//===----------------------------------------------------------------------===//

#include "SmallBirdSEISelDAGToDAG.h"

#include "MCTargetDesc/SmallBirdBaseInfo.h"
#include "SmallBird.h"
#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBirdRegisterInfo.h"

#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "SmallBird-isel"

bool SmallBirdSEDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &static_cast<const SmallBirdSubtarget &>(MF.getSubtarget());
  return SmallBirdDAGToDAGISel::runOnMachineFunction(MF);
}

void SmallBirdSEDAGToDAGISel::processFunctionAfterISel(MachineFunction &MF) {
}

bool SmallBirdSEDAGToDAGISel::trySelect(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();
  SDLoc DL(Node);

  // Instruction Selection not handled by the auto-generated
  // tablegen selection should be handled here.

  EVT NodeTy = Node->getValueType(0);
  unsigned MultOpc;

  switch(Opcode) {
  default: break;
  case ISD::MULHS:
  case ISD::MULHU: {
    MultOpc = (Opcode == ISD::MULHU ? SmallBird::MULTu : SmallBird::MULT);
    auto LoHi = selectMULT(Node, MultOpc, DL, NodeTy, false, true);
    ReplaceNode(Node, LoHi.second);
    return true;
  }
  case ISD::Constant: {
    const ConstantSDNode *CN = dyn_cast<ConstantSDNode>(Node);
    unsigned Size = CN->getValueSizeInBits(0);

    if (Size == 32)
      break;

    return true;
  }
  }

  return false;
}

FunctionPass *llvm::createSmallBirdSEISelDAG(SmallBirdTargetMachine &TM,
                                        CodeGenOpt::Level OptLevel) {
  return new SmallBirdSEDAGToDAGISel(TM, OptLevel);
}

// Select multiply instructions.
std::pair<SDNode *, SDNode *>
SmallBirdSEDAGToDAGISel::selectMULT(SDNode *N, unsigned Opc, const SDLoc &DL, EVT Ty,
                               bool HasLo, bool HasHi) {
  SDNode *Lo = 0, *Hi = 0;
  SDNode *Mul = CurDAG->getMachineNode(Opc, DL, MVT::Glue, N->getOperand(0),
                                       N->getOperand(1));
  SDValue InFlag = SDValue(Mul, 0);

  if (HasLo) {
    Lo = CurDAG->getMachineNode(SmallBird::MFLO, DL, Ty, MVT::Glue, InFlag);
    InFlag = SDValue(Lo, 1);
  }
  if (HasHi) {
    Hi = CurDAG->getMachineNode(SmallBird::MFHI, DL, Ty, InFlag);
  }

  return std::make_pair(Lo, Hi);
}