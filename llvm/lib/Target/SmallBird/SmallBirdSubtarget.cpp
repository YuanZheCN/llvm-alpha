#include "SmallBirdSubtarget.h"

#include "SmallBirdMachineFunctionInfo.h"
#include "SmallBird.h"
#include "SmallBirdRegisterInfo.h"

#include "SmallBirdTargetMachine.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "SmallBird-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SmallBirdGenSubtargetInfo.inc"

extern bool FixGlobalBaseReg;

static cl::opt<bool> EnableOverflowOpt
                 ("cpu0-enable-overflow", cl::Hidden, cl::init(false),
                 cl::desc("Use trigger overflow instructions add and sub \
                 instead of non-overflow instructions addu and subu"));


void SmallBirdSubtarget::anchor() { }

SmallBirdSubtarget::SmallBirdSubtarget(const Triple &TT, const StringRef CPU,
                             const StringRef FS, bool little,
                             const SmallBirdTargetMachine &_TM) :
  // SmallBirdGenSubtargetInfo will display features by llc -march=SmallBird -mcpu=help
  SmallBirdGenSubtargetInfo(TT, CPU, CPU,FS),
  IsLittle(little), TM(_TM), TargetTriple(TT), TSInfo(),
      InstrInfo(
          SmallBirdInstrInfo::create(initializeSubtargetDependencies(CPU, FS, TM))),
      FrameLowering(SmallBirdFrameLowering::create(*this)),
      TLInfo(SmallBirdTargetLowering::create(TM, *this)) {

}

bool SmallBirdSubtarget::isPositionIndependent() const {
  return TM.isPositionIndependent();
}

SmallBirdSubtarget &
SmallBirdSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                               const TargetMachine &TM) {
  if (TargetTriple.getArch() == Triple::SmallBird) {
    if (CPU.empty() || CPU == "generic") {
      CPU = "SmallBird32II";
    }
    else if (CPU == "help") {
      CPU = "";
      return *this;
    }
    else if (CPU != "SmallBird32I" && CPU != "SmallBird32II") {
      CPU = "SmallBird32II";
    }
  }
  else {
    errs() << "!!!Error, TargetTriple.getArch() = " << TargetTriple.getArch()
           <<  "CPU = " << CPU << "\n";
    exit(0);
  }

  if (CPU == "SmallBird32I")
    SmallBirdArchVersion = SmallBird32I;
  else if (CPU == "SmallBird32II")
    SmallBirdArchVersion = SmallBird32II;

  if (isSmallBird32I()) {
    HasCmp = true;
    HasSlt = false;
  }
  else if (isSmallBird32II()) {
    HasCmp = true;
    HasSlt = true;
  }
  else {
    errs() << "-mcpu must be empty(default:SmallBird32II), SmallBird32I or SmallBird32II" << "\n";
  }

  // Parse features string.
  ParseSubtargetFeatures(CPU,/*TUNE*/CPU ,FS);
  // Initialize scheduling itinerary for the specified CPU.
  InstrItins = getInstrItineraryForCPU(CPU);

  return *this;
}

bool SmallBirdSubtarget::abiUsesSoftFloat() const {
//  return TM->Options.UseSoftFloat;
  return true;
}

//const SmallBirdABIInfo &SmallBirdSubtarget::getABI() const { return TM.getABI(); }
