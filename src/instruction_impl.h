//
// Created by schrodinger on 2/20/20.
//

#ifndef SIMULATOR_INSTRUCTION_IMPL_H
#define SIMULATOR_INSTRUCTION_IMPL_H

#include "mainwindow.h"
#include "./ui_mainwindow.h"
/*!
 * A handy macro to create a default constructor based on `NAME` and `FATHER`.
 * @param NAME name of the instruction
 * @param FATHER direct base of the instruction
 */
#define DEFAULT_INIT(NAME, FATHER) NAME##Impl::NAME##Impl(Instruction instr) : FATHER##Impl(instr) {}
/*!
 * Complex declaration of an instruction implementation class.
 * @param NAME name of the instruction
 * @param FATHER direct base of the instruction
 */
#define ComDef(CLASS, FATHER) \
    struct CLASS##Impl : public FATHER##Impl {\
        /*! construct the target instruction implementation */ \
        explicit CLASS##Impl(/*! the instruction value */ Instruction instr);\
        /*! execute the instruction, depends on the real implementation in each struct */ \
        void exec() override;\
    };
/*!
 * Simple declaration for those instruction implementations with a direct base of `InstructionImpl`
 * @param CLASS the name of the instruction
 */
#define SimDef(CLASS) ComDef(CLASS, Instruction)
/*!
 * Complex real definition of instruction behaviors
 * @attention must be used together with declarations
 */
#define ComImplDef(CLASS, FATHER, BLOCK)\
    void CLASS##Impl::exec() BLOCK\
    DEFAULT_INIT(CLASS, FATHER)

#define SimImplDef(CLASS, BLOCK) ComImplDef(CLASS, Instruction, BLOCK)

#define OP_AMONG_REGS(NAME, A, B, op, C)\
SimImplDef(NAME, {\
    mainW->updateRegValue(instr.INST_R.A, mainW->REGS[instr.INST_R.B] op mainW->REGS[instr.INST_R.C]);\
})

#define OP_AMONG_REGS_OVERFLOW(NAME, A, B, op, C)\
SimImplDef(NAME, {\
    int32_t a = mainW->REGS[instr.INST_R.B];\
    int32_t b = mainW->REGS[instr.INST_R.C];\
    int32_t c = 0;\
    if(__builtin_##op##_overflow(a, b, &c)) throw std::runtime_error {"overflow"};\
    mainW->updateRegValue(instr.INST_R.A, c);\
})

#define SHIFT_REAL(NAME, A, B, op, C, TYPE)\
SimImplDef(NAME, {\
    mainW->updateRegValue(instr.INST_R.A,\
    static_cast<TYPE>(mainW->REGS[instr.INST_R.B]) op static_cast<TYPE>(0b11111 & mainW->REGS[instr.INST_R.C]));\
})

#define SHIFT_IMM(NAME, A, B, op, C, TYPE)\
SimImplDef(NAME, {\
    mainW->updateRegValue(instr.INST_R.A,\
    static_cast<TYPE>(mainW->REGS[instr.INST_R.B]) op static_cast<TYPE>(instr.INST_R.C));\
})

#define TRAP_R(NAME, op, TYPE)\
SimImplDef(NAME, {\
    TYPE a = mainW->REGS[instr.INST_R.s];\
    TYPE b = mainW->REGS[instr.INST_R.t];\
    if (a op b) throw std::runtime_error {"conditionally trapped!"};\
})

#define TRAP_RI(NAME, op, TYPE)\
SimImplDef(NAME, {\
    TYPE a = mainW->REGS[instr.INST_I.s];\
    TYPE b = instr.INST_I.C;\
    if (a op b) throw std::runtime_error {"conditionally trapped!"};\
})

#define BRANCH_IF(NAME, COND) \
SimImplDef(NAME, {\
    if (COND) {\
        mainW->updateProgramCounter(mainW->PC + (int16_t) instr.INST_I.C * int32_t(4));\
    }\
})

#define BRANCH_IF_SAVE(NAME, COND) \
SimImplDef(NAME, {\
    if (COND) {\
        mainW->updateRegValue(31, mainW->PC + 4);\
        mainW->updateProgramCounter(mainW->PC + (int16_t) instr.INST_I.C * int32_t(4));\
    }\
})

/*!
 * The base class of the instruction implementations.
 * All subclasses must implement the `exec()` function, and this function decides the behavior of each construction.
 */
struct InstructionImpl {
    /*! Static pointer back to the main windows.
     * This pointer allow us to interact with the ui components.
     * @attention This pointer should never be modified in any situation except for the setting
     * happens during the initialization MainWindow
     */
    static MainWindow *mainW;

    /// the instruction value.
    Instruction instr;

    /*!
     * The purely virtual function `exec()` is to be overwritten in the subclasses.
     */
    virtual void exec() = 0;

    /*!
     * Construct the base class: InstructionImpl.
     * @attention This function will set the instruction and should be invoked by all subclasses in some way.
     * @param instr the instruction value
     */
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

ComDef(SC, SW)

ComDef(LL, LW)

#endif //SIMULATOR_INSTRUCTION_IMPL_H
