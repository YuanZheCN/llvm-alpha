#ifndef LLVM_LIB_TARGET_SMALLBIRD_MCTARGETDESC_SMALLBIRDMCASMINFO_H
#define LLVM_LIB_TARGET_SMALLBIRD_MCTARGETDESC_SMALLBIRDMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
  class Triple;

  class SmallBirdMCAsmInfo : public MCAsmInfoELF {
    void anchor() override;
  public:
    explicit SmallBirdMCAsmInfo(const Triple &TheTriple);
  };

} // end namespace llvm

#endif