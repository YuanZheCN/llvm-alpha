#ifndef LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDISELLOWERING_H
#define LLVM_LIB_TARGET_SMALLBIRD_SMALLBIRDISELLOWERING_H

//#include "MCTargetDesc/SmallBirdABIInfo.h"
#include "SmallBird.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/Function.h"
#include <deque>

namespace llvm {
namespace SmallBirdISD {
enum NodeType {
  // Start the numbering from where ISD NodeType finishes.
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Jump and link (call)
  JmpLink,

  // Tail call
  TailCall,

  // Get the Higher 16 bits from a 32-bit immediate
  // No relation with SmallBird Hi register
  Hi,

  // Get the Lower 16 bits from a 32-bit immediate
  // No relation with SmallBird Lo register
  Lo,

  // Handle gp_rel (small data/bss sections) relocation.
  GPRel,

  // Thread Pointer
  ThreadPointer,

  // Return
  Ret,

  EH_RETURN,

  // DivRem(u)
  DivRem,
  DivRemU,

  Wrapper,
  DynAlloc,

  Sync
};
} // End SmallBirdISD namespace

//===----------------------------------------------------------------------===//
// TargetLowering Implementation
//===----------------------------------------------------------------------===//
class SmallBirdFunctionInfo;
class SmallBirdSubtarget;

class SmallBirdTargetLowering : public TargetLowering {
public:
  explicit SmallBirdTargetLowering(const SmallBirdTargetMachine &TM,
                              const SmallBirdSubtarget &STI);

  static const SmallBirdTargetLowering *create(const SmallBirdTargetMachine &TM,
                                          const SmallBirdSubtarget &STI);

  // This method returns the name of a target specific DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

protected:
  // Byval argument information.
  struct ByValArgInfo {
    unsigned FirstIdx;  // Index of the first register used.
    unsigned NumRegs;   // Number of registers used for this argument.
    unsigned Address;   // Offset of the stack area used to pass this argument.

    ByValArgInfo() : FirstIdx(0), NumRegs(0), Address(0) { }
  };

  // Subtarget Info
  const SmallBirdSubtarget &Subtarget;
  // Cache the ABI from the TargetMachine, we use it everywhere.
  // const SmallBirdABIInfo &ABI;
  // This class provides methods used to analyze formal and call arguments and
  // inquire about calling convention information.
  class SmallBirdCC {
  public:
    enum SpecialCallingConvType {
      NoSpecialCallingConv
    };

    SmallBirdCC(CallingConv::ID CallConv, bool IsO32, CCState &Info,
          SpecialCallingConvType SpecialCallingConv = NoSpecialCallingConv);

    void analyzeCallResult(const SmallVectorImpl<ISD::InputArg> &Ins,
                          bool IsSoftFloat, const SDNode *CallNode,
                          const Type *RetTy) const;

    void analyzeReturn(const SmallVectorImpl<ISD::OutputArg> &Outs,
                      bool IsSoftFloat, const Type *RetTy) const;

    const CCState &getCCInfo() const { return CCInfo; }

    // Returns true if function has byval arguments.
    bool hasByValArg() const { return !ByValArgs.empty(); }

    // The size of the area the caller reserves for register arguments.
    // This is 16-byte if ABI is O32.
    unsigned reservedArgArea() const;

    typedef SmallVectorImpl<ByValArgInfo>::const_iterator byval_iterator;
    byval_iterator byval_begin() const { return ByValArgs.begin(); }
    byval_iterator byval_end() const { return ByValArgs.end(); }

  private:
    // Return the type of the register which is used to pass an argument or
    // return a value. This function returns f64 if the argument is an i64
    // value whihc has been generated as a result of softening an f128 value.
    // otherwise, it just returns VT.
    MVT getRegVT(MVT VT, const Type *OrigTy, const SDNode *CallNode,
                bool IsSoftFloat) const;

    template<typename Ty>
    void analyzeReturn(const SmallVectorImpl<Ty> &RetVals, bool IsSoftFloat,
                      const SDNode *CallNode, const Type *RetTy) const;

    CCState &CCInfo;
    CallingConv::ID CallConv;
    bool IsO32;
    SmallVector<ByValArgInfo, 2> ByValArgs;
  };
  
private:
  // Lower Operand specifics
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerFormalArguments(SDValue Chain,
                               CallingConv::ID CallConv, bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain,
                      CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals,
                      const SDLoc &dl, SelectionDAG &DAG) const override;
};

const SmallBirdTargetLowering *
createSmallBirdSETargetLowering(const SmallBirdTargetMachine &TM,
                           const SmallBirdSubtarget &STI);

} // End llvm namespace


#endif
