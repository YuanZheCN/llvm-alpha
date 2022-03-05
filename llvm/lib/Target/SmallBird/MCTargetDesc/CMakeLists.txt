
add_llvm_component_library(LLVMSmallBirdDesc
  SmallBirdMCAsmInfo.cpp
  SmallBirdMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  SmallBirdInfo
  SmallBirdAsmPrinter
  Support
  )