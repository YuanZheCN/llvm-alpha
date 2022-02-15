#include "SmallBirdSEFrameLowering.h"

#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBirdSEInstrInfo.h"
#include "SmallBirdSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegionInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

SmallBirdSEFrameLowering::SmallBirdSEFrameLowering(const SmallBirdSubtarget &STI)
  : SmallBirdFrameLowering(STI, STI.stackAlignment()) { }

void SmallBirdSEFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  /* Leave empty temporary */
}

void SmallBirdSEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  /* Leave empty temporary */
}

const SmallBirdFrameLowering *llvm::createSmallBirdSEFrameLowering(const SmallBirdSubtarget
                                                                   &ST) {
  return new SmallBirdSEFrameLowering(ST);
}