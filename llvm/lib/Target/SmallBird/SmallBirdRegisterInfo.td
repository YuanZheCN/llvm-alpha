// We have banks of 16 registers each.
class SmallBirdReg<bits<16> Enc, string n> : Register<n> {
  // For tablegen(... -gen-emitter) in CMakeLists.txt
  let HWEncoding = Enc;
  let Namespace = "SmallBird";
}

// Two type of registers
class SmallBirdGPRReg<bits<16> Enc, string n> : SmallBirdReg<Enc, n>;
class SmallBirdC0Reg<bits<16> Enc, string n> : SmallBirdReg<Enc, n>;

//define all regs
let Namespace = "SmallBird" in {
  //@ General Purpose Registers
  def ZERO : SmallBirdGPRReg<0,   "zero">, DwarfRegNum<[0]>;
  def AT   : SmallBirdGPRReg<1,   "1">,    DwarfRegNum<[1]>;
  def V0   : SmallBirdGPRReg<2,   "2">,    DwarfRegNum<[2]>;
  def V1   : SmallBirdGPRReg<3,   "3">,    DwarfRegNum<[3]>;
  def A0   : SmallBirdGPRReg<4,   "4">,    DwarfRegNum<[4]>;
  def A1   : SmallBirdGPRReg<5,   "5">,    DwarfRegNum<[5]>;
  def T9   : SmallBirdGPRReg<6,   "6">,    DwarfRegNum<[6]>;
  def T0   : SmallBirdGPRReg<7,   "7">,    DwarfRegNum<[7]>;
  def T1   : SmallBirdGPRReg<8,   "8">,    DwarfRegNum<[8]>;
  def S0   : SmallBirdGPRReg<9,   "9">,    DwarfRegNum<[9]>;
  def S1   : SmallBirdGPRReg<10,  "10">,   DwarfRegNum<[10]>;
  def GP   : SmallBirdGPRReg<11,  "11">,   DwarfRegNum<[11]>;
  def FP   : SmallBirdGPRReg<12,  "12">,   DwarfRegNum<[12]>;
  def SP   : SmallBirdGPRReg<13,  "13">,   DwarfRegNum<[13]>;
  def LR   : SmallBirdGPRReg<14,  "14">,   DwarfRegNum<[14]>;
  def SW   : SmallBirdGPRReg<15,  "15">,   DwarfRegNum<[15]>;

  def PC   : SmallBirdC0Reg<0, "pc">,    DwarfRegNum<[20]>;
  def EPC  : SmallBirdC0Reg<1, "epc">,   DwarfRegNum<[21]>;


  def HI   : SmallBirdReg<0, "ac0">, DwarfRegNum<[18]>;
  def LO   : SmallBirdReg<0, "ac0">, DwarfRegNum<[19]>;
}


//Def Register Groups
def CPURegs : RegisterClass<"SmallBird", [i32], 32, (add
  // Reserved
  ZERO, AT,
  // Return Values and Arguments
  V0, V1, A0, A1,
  // Not preserved across procedure calls
  T9, T0, T1,
  // Callee save
  S0, S1,
  // Reserved
  GP, FP,
  SP, LR, SW)>;

// @Status Registers class
def SR : RegisterClass<"SmallBird", [i32], 32, (add SW)>;

// @Co-processor 0 Registers class
def C0Regs : RegisterClass<"SmallBird", [i32], 32, (add PC, EPC)>;

def GPROut : RegisterClass<"SmallBird", [i32], 32, (add (sub CPURegs, SW))>;

def HILO   : RegisterClass<"SmallBird", [i32], 32, (add HI, LO)>;