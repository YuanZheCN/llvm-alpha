include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Target-dependent interfaces
//===----------------------------------------------------------------------===//

// Register Infomation
include "SmallBirdRegisterInfo.td"

// Calling Conversion
include "SmallBirdSchedule.td"

// Instruction Description
include "SmallBirdInstrInfo.td"

// Calling Convention
include "SmallBirdCallingConv.td"

//===----------------------------------------------------------------------===//
// SmallBird SUbtarget features
//===----------------------------------------------------------------------===//

def FeatureCmp            : SubtargetFeature<"cmp", "HasCmp", "true",
                                             "Enable 'cmp' instructions.">;
def FeatureSlt            : SubtargetFeature<"slt", "HasSlt", "true",
                                             "Enable 'slt' instructions.">;
def FeatureSmallBird32I        : SubtargetFeature<"SmallBird32I", "SmallBirdArchVersion",
                                             "SmallBird32I", "SmallBird32I ISA Support",
                                             [FeatureCmp]>;
def FeatureSmallBird32II       : SubtargetFeature<"SmallBird32II", "SmallBirdArchVersion",
                                             "SmallBird32II", "SmallBird32II ISA Support",
                                             [FeatureCmp, FeatureSlt]>;

//===----------------------------------------------------------------------===//
// SmallBird processors supported
//===----------------------------------------------------------------------===//
class Proc<string Name, list<SubtargetFeature> Features>
  : Processor<Name, SmallBirdGenericItineraries, Features>;

def : Proc<"SmallBird32I", [FeatureSmallBird32I]>;
def : Proc<"SmallBird32II", [FeatureSmallBird32II]>;
// Above make SmallBirdGenSubtargetInfo.inc set feature bit as the following order:
// enum {
//   FeatureCmp =  1ULL << 0,
//   FeatureSmallBird32I =  1ULL << 1,
//   FeatureSmallBird32II =  1ULL << 2,
//   FeatureSlt =  1ULL << 3
// };

def SmallBirdInstrInfo : InstrInfo;

// Whill generate SmallBirdGenAsmWrite.inc included by SmallBirdInstPrinter.cpp, contents
// as follows,
// void SmallBirdInstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char* SmallBirdInstPrinter::getRegisterName(unsigned RegNo) {...}
def SmallBird : Target {
  // def SmallBirdInstrInfo : InstrInfo as before.
  let InstructionSet = SmallBirdInstrInfo;
}
