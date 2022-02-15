#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDINSTRINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDINSTRINFO_H

#include "SmallBird.h"
#include "SmallBirdRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SmallBirdGenInstrInfo.inc"

namespace llvm {

class SmallBirdInstrInfo : public SmallBirdGenInstrInfo {
  virtual void anchor();
protected:
  const SmallBirdSubtarget &Subtarget;
public:
  explicit SmallBirdInstrInfo(const SmallBirdSubtarget &STI);

  static const SmallBirdInstrInfo *create(SmallBirdSubtarget &STI);

  // TargetInstrInfo is a superset of MRegister info. As such, whenever a client
  // has an instance of instruction info, it should always be able to get
  // register info as well (through this method).
  virtual const SmallBirdRegisterInfo &getRegisterInfo() const = 0;

  // Return the number of bytes of code the specified instruction maybe.
  unsigned GetInstSizeInBytes(const MachineInstr &MI) const;

};

const SmallBirdInstrInfo *createSmallBirdSEInstrInfo(const SmallBirdSubtarget &STI);
}

#endif