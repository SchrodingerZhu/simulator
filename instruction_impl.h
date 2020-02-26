//
// Created by schrodinger on 2/20/20.
//

#ifndef SIMULATOR_INSTRUCTION_IMPL_H
#define SIMULATOR_INSTRUCTION_IMPL_H

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define DEFAULT_INIT(NAME, FATHER) NAME##Impl::NAME##Impl(Instruction instr) : FATHER##Impl(instr) {}

#define ComDef(CLASS, FATHER) \
    struct CLASS##Impl : public FATHER##Impl {\
        explicit CLASS##Impl(Instruction instr);\
        void exec() override;\
    };

#define SimDef(CLASS) ComDef(CLASS, Instruction)

#define ComImplDef(CLASS, FATHER, BLOCK)\
    void CLASS##Impl::exec() BLOCK\
    DEFAULT_INIT(CLASS, FATHER)

#define SimImplDef(CLASS, BLOCK) ComImplDef(CLASS, Instruction, BLOCK)

struct InstructionImpl {
    static MainWindow *mainW;
    Instruction instr;

    virtual void exec() = 0;

    explicit InstructionImpl(Instruction instr);
};

SimDef(NOP)

// J-TYPES
SimDef(J)

ComDef(JAL, J)

// I-TYPES
SimDef(ADDI)

SimDef(ADDIU)

SimDef(ANDI)

SimDef(BEQ)

SimDef(BGEZ)

SimDef(BGTZ)

SimDef(BLEZ)

SimDef(BLTZ)

SimDef(BNE)

SimDef(LB)

SimDef(LBU)

SimDef(LH)

SimDef(LHU)

SimDef(LUI)

SimDef(LW)

SimDef(ORI)

SimDef(SB)

SimDef(SLTI)

SimDef(SLTIU)

SimDef(SH)

SimDef(SW)

SimDef(SWCL)

SimDef(XORI)

// R-TYPES
SimDef(ADD)

SimDef(ADDU)

SimDef(AND)

SimDef(BREAK)

SimDef(DIV)

SimDef(DIVU)

SimDef(JR)

ComDef(JALR, JR)

SimDef(MFHI)

SimDef(MFLO)

SimDef(MTHI)

SimDef(MTLO)

SimDef(MULT)

SimDef(MULTU)

SimDef(NOR)

SimDef(OR)

SimDef(SLL)

SimDef(SLLV)

SimDef(SLT)

SimDef(SLTU)

SimDef(SRA)

SimDef(SRAV)

SimDef(SRL)

SimDef(SRLV)

SimDef(SUB)

SimDef(SUBU)

SimDef(SYSCALL)

SimDef(XOR)

SimDef(BLTZL)

SimDef(BGEZL)

SimDef(TGEI)

SimDef(TGEIU)

SimDef(TLTI)

SimDef(TLTIU)

SimDef(TEQI)

SimDef(TNEI)

SimDef(BLTZAL)

SimDef(BGEZAL)

SimDef(BLTZALL)

SimDef(BGEZALL)

SimDef(CLO)

SimDef(CLZ)

SimDef(MUL)

SimDef(MADD)

SimDef(MADDU)

SimDef(MSUB)

SimDef(MSUBU)

SimDef(TEQ)

SimDef(TNE)

SimDef(TGE)

SimDef(TGEU)

SimDef(TLT)

SimDef(TLTU)

SimDef(SWL)

SimDef(SWR)

SimDef(LWL)

SimDef(LWR)

#endif //SIMULATOR_INSTRUCTION_IMPL_H
