
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
tablegen(LLVM SmallBirdGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SmallBirdGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SmallBirdGenCodeEmitter.inc -gen-emitter)
tablegen(LLVM SmallBirdGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM SmallBirdGenAsmWriter.inc -gen-asm-writer)

# SmallBirdCommonTableGen must be defined
add_public_tablegen_target(SmallBirdCommonTableGen)

# SmallBirdCodeGen should match with LLVMBuild.txt SmallBirdCodeGen
add_llvm_target(SmallBirdCodeGen
SmallBirdTargetMachine.cpp
SmallBirdInstrInfo.cpp
SmallBirdISelLowering.cpp
SmallBirdMachineFunctionInfo.cpp
SmallBirdRegisterInfo.cpp
SmallBirdFrameLowering.cpp
SmallBirdSEFrameLowering.cpp
SmallBirdSEInstrInfo.cpp
SmallBirdSEISelLowering.cpp
SmallBirdSERegisterInfo.cpp
SmallBirdSubtarget.cpp
SmallBirdTargetObjectFile.cpp
SmallBirdAsmPrinter.cpp
SmallBirdMCInstLower.cpp
SmallBirdISelDAGToDAG.cpp
SmallBirdSEISelDAGToDAG.cpp
SmallBirdAnalyzeImmediate.cpp
  
  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  SmallBirdDesc
  SmallBirdInfo
  SmallBirdAsmPrinter
  SelectionDAG
  Support
  Target
  GlobalISel
  ADD_TO_COMPONENT
  SmallBird
  )
  

# Should match with "subdirectories = MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
add_subdirectory(InstPrinter)