
add_llvm_component_library(LLVMSmallBirdAsmPrinter
SmallBirdInstPrinter.cpp

LINK_COMPONENTS
Support

ADD_TO_COMPONENT
SmallBird
)

