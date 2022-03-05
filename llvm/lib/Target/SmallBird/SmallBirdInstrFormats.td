//===-- SmallBirdInstrFormats.td - SmallBird Instruction Formats -----*- tablegen -*-===//
//
//                    The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.

//
//===----------------------------------------------------------------------===//
//
// Describe SmallBird instructions format
//
// CPU INSTRUCTION FORMATS
//
// opcode - operation code.
// ra     - dst reg, only used on 3 regs instr.
// rb     - src reg.
// rc     - src reg (on a 3 reg instr).
// cx     - immediate
//
//===----------------------------------------------------------------------===//

// Format specifies the encoding used by the instruction. This is part of the
// ad-hoc solution used to emit machine instruction encodings by our machine
// code emitter.
class Format<bits<4> val>
{
  bits<4> Value = val;
}

def Pseudo      : Format<0>;
def FrmA        : Format<1>;
def FrmL        : Format<2>;
def FrmJ        : Format<3>;
def FrmOther    : Format<4>; // Instruction w/ a custom format

// Generic SmallBird Format
class SmallBirdInst<dag outs, dag ins, string asmStr, list<dag> pattern,
               InstrItinClass itin, Format f> : Instruction
{
  // Inst and Size: for tablegen(... -gen-emitter) and
  // tablegen(... -gen-disassembler) in CMakeLists.txt
  field bits<32> Inst;
  Format Form = f;

  let Namespace = "SmallBird";

  let Size = 4;

  bits<8> Opcode = 0;

  // Top 8 bits are the 'opcode' field
  let Inst{31-24} = Opcode;

  let OutOperandList = outs;
  let InOperandList  = ins;

  let AsmString = asmStr;
  let Pattern   = pattern;
  let Itinerary = itin;

  // Attributes specific to SmallBird instructions
  bits<4> FormBits = Form.Value;

  let TSFlags{3-0} = FormBits;

  let DecoderNamespace = "SmallBird";

  field bits<32> SoftFail = 0;
}

//===----------------------------------------------------------------------===//
// Format A instruction class in SmallBird : <|opcode|ra|rb|rc|cx|>
//===----------------------------------------------------------------------===//

class FA<bits<8> op, dag outs, dag ins, string asmStr,
         list<dag> pattern, InstrItinClass itin>
  : SmallBirdInst<outs, ins, asmStr, pattern, itin, FrmA>
{
  bits<4>  ra;
  bits<4>  rb;
  bits<4>  rc;
  bits<12> shamt;

  let Opcode = op;

  let Inst{23-20} = ra;
  let Inst{19-16} = rb;
  let Inst{15-12} = rc;
  let Inst{11-0}  = shamt;
}

//===----------------------------------------------------------------------===//
// Format L instruction class in SmallBird : <|opcode|ra|rb|cx|>
//===----------------------------------------------------------------------===//

class FL<bits<8> op, dag outs, dag ins, string asmStr,
         list<dag> pattern, InstrItinClass itin>
  : SmallBirdInst<outs, ins, asmStr, pattern, itin, FrmL>
{
  bits<4>  ra;
  bits<4>  rb;
  bits<16> imm16;

  let Opcode = op;

  let Inst{23-20} = ra;
  let Inst{19-16} = rb;
  let Inst{15-0}  = imm16;
}

//===----------------------------------------------------------------------===//
// Format J instruction class in SmallBird : <|opcode|address|>
//===----------------------------------------------------------------------===//

class FJ<bits<8> op, dag outs, dag ins, string asmStr,
         list<dag> pattern, InstrItinClass itin>
  : SmallBirdInst<outs, ins, asmStr, pattern, itin, FrmJ>
{
  bits<24> addr;

  let Opcode = op;

  let Inst{23-0} = addr;
}

//===----------------------------------------------------------------------===//
// Pseudo Instructions Format
//===----------------------------------------------------------------------===//

// SmallBird Pseudo Instructions Format
class SmallBirdPseudo<dag outs, dag ins, string asmstr, list<dag> pattern,
                 InstrItinClass itin = IIPseudo>
    : SmallBirdInst<outs, ins, asmstr, pattern, itin, Pseudo> {
  let isCodeGenOnly = 1;
  let isPseudo = 1;
}

// SmallBird32 Pseudo Instruction Format
class PseudoSE<dag outs, dag ins, list<dag> pattern,
               InstrItinClass itin = IIPseudo>
    : SmallBirdPseudo<outs, ins, "", pattern> {}

// Pseudo-instructios for alternate assembly syntax (never used by codegen)
class SmallBirdAsmPseudoInst<dag outs, dag ins, string asmstr>
    : SmallBirdInst<outs, ins, asmstr, [], IIPseudo, Pseudo> {
  let isPseudo = 1;
  let Pattern = [];
}