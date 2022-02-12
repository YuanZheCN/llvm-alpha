//FILE 0 : Provide by LLVM, we do not need to write it
include "llvm/Target/Target.td"
//FILE 1 : Register Infomation, Write it
include "SmallBirdRegisterInfo.td"

//FILE 2: Calling Conversion, write it
include "SmallBirdSchedule.td"

//FILE 3: Instruction Description, write it
include "SmallBirdInstrInfo.td"


//Declare Target and inst info
def SmallBirdInstrInfo : InstrInfo;

def SmallBird : Target {
  let InstructionSet = SmallBirdInstrInfo;
}