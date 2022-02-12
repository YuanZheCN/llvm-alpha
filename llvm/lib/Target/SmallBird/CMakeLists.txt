
add_llvm_component_group(SmallBird)
set (LLVM_TARGET_DEFINITIONS SmallBird.td)

# Generate SmallBirdGenRegisterInfo.inc and SmallBirdGenInstrInfo.inc which included by
#  your hand code C++ files.
# SmallBirdGenRegisterInfo.inc came from SmallBirdRegisterInfo.td, SmallBirdGenInstrInfo.inc
#  came from SmallBirdInstrInfo.td.
tablegen(LLVM SmallBirdGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SmallBirdGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SmallBirdGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM SmallBirdGenMCPseudoLowering.inc -gen-pseudo-lowering)

# SmallBirdCommonTableGen must be defined
add_public_tablegen_target(SmallBirdCommonTableGen)

# SmallBirdCodeGen should match with LLVMBuild.txt SmallBirdCodeGen
add_llvm_target(SmallBirdCodeGen
  SmallBirdTargetMachine.cpp
  )

# Should match with "subdirectories = MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
