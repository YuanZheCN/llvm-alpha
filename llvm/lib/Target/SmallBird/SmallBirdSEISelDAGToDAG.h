//===-- SmallBirdSEISelDAGToDAG.h - A DAG to DAG Inst Selector for SmallBirdSE -*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEISELDAGTODAG_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDSEISELDAGTODAG_H

#include "SmallBirdISelDAGToDAG.h"

namespace llvm {

class SmallBirdSEDAGToDAGISel : public SmallBirdDAGToDAGISel {
public:
  explicit SmallBirdSEDAGToDAGISel(SmallBirdTargetMachine &TM, CodeGenOpt::Level OL)
      : SmallBirdDAGToDAGISel(TM, OL) { }

private:

  bool runOnMachineFunction(MachineFunction &MF) override;

  bool trySelect(SDNode *Node) override;

  void processFunctionAfterISel(MachineFunction &MF) override;

  std::pair<SDNode *, SDNode *> selectMULT(SDNode *N, unsigned Opc,
                                           const SDLoc &DL, EVT Ty,
                                           bool HasLo, bool HasHi);
};

// Create new instr selector, called in SmallBirdTargetMachine.cpp for registrary pass
FunctionPass *createSmallBirdSEISelDAG(SmallBirdTargetMachine &TM,
                                  CodeGenOpt::Level OptLevel);

} // end of llvm namespace

#endif