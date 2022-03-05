class CCIfSubtarget<string F, CCAction A>
  : CCIf<!strconcat("State.getTarget().getSubtarget<SmallBirdSubtarget>().", F), A>;

def CSR_032 : CalleeSavedRegs<(add LR, FP, (sequence "S%u", 1, 0))>;

def RetCC_SmallBird : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[V0, V1, A0, A1]>>
]>;
