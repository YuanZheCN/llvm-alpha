#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBirdISelLowering.h"
#include "SmallBirdSEISelLowering.h"

#include "SmallBirdRegisterInfo.h"
#include "SmallBirdTargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegionInfo.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

using namespace llvm;

#define DEBUG_TYPE "SmallBird-isel"

static cl::opt<bool>
EnableSmallBirdTailCalls("enable-SmallBird-tail-calls", cl::Hidden,
                    cl::desc("SmallBird: Enable tail calls."),
                    cl::init(false));

SmallBirdSETargetLowering::SmallBirdSETargetLowering(const SmallBirdTargetMachine &TM,
                                           const SmallBirdSubtarget &STI)
    : SmallBirdTargetLowering(TM, STI) {
  // Set up the register classes
  addRegisterClass(MVT::i32, &SmallBird::CPURegsRegClass);

  // Once all of the register classes are added, this allows us to compute
  // deirved properties we expose.
  computeRegisterProperties(Subtarget.getRegisterInfo());
}

SDValue SmallBirdSETargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  return SmallBirdTargetLowering::LowerOperation(Op, DAG);
}

const SmallBirdTargetLowering *
llvm::createSmallBirdSETargetLowering(const SmallBirdTargetMachine &TM,
                           const SmallBirdSubtarget &STI) {
  return new SmallBirdSETargetLowering(TM, STI);
}
