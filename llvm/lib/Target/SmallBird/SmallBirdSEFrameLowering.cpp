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
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo &MFI    = MF.getFrameInfo();
  SmallBirdMachineFunctionInfo *SmallBirdFI = MF.getInfo<SmallBirdMachineFunctionInfo>();

  const SmallBirdSEInstrInfo &TII =
    *static_cast<const SmallBirdSEInstrInfo*>(STI.getInstrInfo());
  const SmallBirdRegisterInfo &RegInfo =
    *static_cast<const SmallBirdRegisterInfo*>(STI.getRegisterInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  //SmallBirdABIInfo ABI = STI.getABI();
  unsigned SP = SmallBird::SP;
  const TargetRegisterClass *RC = &SmallBird::GPROutRegClass;

  // First, compute final stack size.
  uint64_t StackSize = MFI.getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI.adjustsStack()) return;

  MachineModuleInfo &MMI = MF.getMMI();
  const MCRegisterInfo *MRI = MMI.getContext().getRegisterInfo();

  // Adjust stack.
  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);

  // emit ".cfi_def_cfa_offset StackSize"
  unsigned CFIIndex = MF.addFrameInst(
    MCCFIInstruction::cfiDefCfaOffset(nullptr, -StackSize));
  BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
      .addCFIIndex(CFIIndex);

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  if (CSI.size()) {
    // Find the instruction past the last instruction that saves a callee-saved
    // register to the stack.
    for (unsigned i = 0; i < CSI.size(); ++i)
      ++MBBI;

    // Iterate over list of callee-saved registers and emit .cfi_offset
    // directives.
    for (std::vector<CalleeSavedInfo>::const_iterator I = CSI.begin(),
           E = CSI.end(); I != E; ++I) {
      int64_t Offset = MFI.getObjectOffset(I->getFrameIdx());
      unsigned Reg = I->getReg();
      {
        // Reg is in CPURegs
        unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::createOffset(
            nullptr, MRI->getDwarfRegNum(Reg, 1), Offset));
        BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
            .addCFIIndex(CFIIndex);
      }
    }
  }
}

void SmallBirdSEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  SmallBirdMachineFunctionInfo *SmallBirdFI = MF.getInfo<SmallBirdMachineFunctionInfo>();

  const SmallBirdSEInstrInfo &TII =
    *static_cast<const SmallBirdSEInstrInfo*>(STI.getInstrInfo());
  const SmallBirdRegisterInfo &RegInfo =
    *static_cast<const SmallBirdSERegisterInfo*>(STI.getRegisterInfo());

  DebugLoc dl = MBBI->getDebugLoc();
  //SmallBirdABIInfo ABI = STI.getABI();
  unsigned SP = SmallBird::SP;

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  if (!StackSize)
    return;

  // Adjust stack.
  TII.adjustStackPtr(SP, StackSize, MBB, MBBI);
}

const SmallBirdFrameLowering *llvm::createSmallBirdSEFrameLowering(const SmallBirdSubtarget
                                                                   &ST) {
  return new SmallBirdSEFrameLowering(ST);
}
bool SmallBirdSEFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();

  // Reserve call frame if the size of the maximum call frame fits into 16-bit
  // immediate field and there are no variable sized objects on the stack.
  // Make sure the second register scavenger spill slot can be accessed with one
  // instruction.
  return isInt<16>(MFI.getMaxCallFrameSize() + getStackAlignment()) &&
    !MFI.hasVarSizedObjects();
}

// Mark Reg and all registers aliasing it in the bitset.
static void setAliasRegs(MachineFunction &MF, BitVector &SavedRegs, unsigned Reg) {
  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
  for (MCRegAliasIterator AI(Reg, TRI, true); AI.isValid(); ++AI)
    SavedRegs.set(*AI);
}

// This method is called immediately before PrologEpilogInserter scans the
// physical registers used to determine what callee saved registers should be
// spilled. This method is optional.
void SmallBirdSEFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                               BitVector &SavedRegs,
                                               RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  SmallBirdMachineFunctionInfo *SmallBirdFI = MF.getInfo<SmallBirdMachineFunctionInfo>();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  if (MF.getFrameInfo().hasCalls())
    setAliasRegs(MF, SavedRegs, SmallBird::LR);

  return;
}