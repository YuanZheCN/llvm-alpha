
#include "SmallBirdMachineFunctionInfo.h"

#include "SmallBirdInstrInfo.h"
#include "SmallBirdSubtarget.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool FixGlobalBaseReg;

SmallBirdMachineFunctionInfo::~SmallBirdMachineFunctionInfo() { }

void SmallBirdMachineFunctionInfo::anchor() { }