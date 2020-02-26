//
// Created by schrodinger on 2/20/20.
//

#include "instruction_impl.h"
#include "mainwindow.ipp"
#include "syscall.h"

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

MainWindow *InstructionImpl::mainW = nullptr;

InstructionImpl::InstructionImpl(Instruction instr) : instr(instr) {}

SimImplDef(NOP, {
    // DO NOTHING;
})

SimImplDef(J, {
    mainW->advanceCounter = false;
    mainW->updateProgramCounter(instr.INST_J.A << 2u); /// @todo: CHECK WHETHER THIS IS REQUIRED
})

ComImplDef(JAL, J, {
    mainW->updateRegValue(31, mainW->PC + 4);
    JImpl::exec();
})

SimImplDef(ADDI, {
    int32_t a = mainW->REGS[instr.INST_I.s];
    int16_t b = instr.INST_I.C;
    int32_t c = 0;
    if (__builtin_add_overflow(a, b, &c)) throw std::runtime_error("overflow");
    mainW->updateRegValue(instr.INST_I.t, c);
})

SimImplDef(ADDIU, {
    mainW->updateRegValue(instr.INST_I.t,
                          (unsigned) mainW->REGS[instr.INST_I.s] + (unsigned) instr.INST_I.C);
})

SimImplDef(ANDI, {
    mainW->updateRegValue(instr.INST_I.t,
                          mainW->REGS[instr.INST_I.s] & instr.INST_I.C);
})

BRANCH_IF(BEQ, mainW->REGS[instr.INST_I.s] == mainW->REGS[instr.INST_I.t])

BRANCH_IF(BGEZ, mainW->REGS[instr.INST_I.s] >= 0)

BRANCH_IF(BGTZ, mainW->REGS[instr.INST_I.s] > 0)

BRANCH_IF(BLEZ, mainW->REGS[instr.INST_I.s] <= 0)

BRANCH_IF(BLTZ, mainW->REGS[instr.INST_I.s] < 0)

BRANCH_IF(BNE, instr.INST_I.s != instr.INST_I.t)

BRANCH_IF_SAVE(BGEZAL, mainW->REGS[instr.INST_I.s] >= 0)

BRANCH_IF_SAVE(BLTZAL, mainW->REGS[instr.INST_I.s] < 0)



SimImplDef(LB, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int8_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int8_t>(off));
    }
})

SimImplDef(LBU, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<uint8_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<uint8_t>(off));
    }
})

SimImplDef(LH, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int16_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int16_t>(off));
    }
})

SimImplDef(LHU, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<uint16_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<uint16_t>(off));
    }
})

SimImplDef(LUI, {
    mainW->updateRegValue(instr.INST_I.t, instr.INST_I.C << 16u);
})

SimImplDef(LW, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int32_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int32_t>(off));
    }
})

SimImplDef(ORI, {
    mainW->updateRegValue(instr.INST_I.t,
                          mainW->REGS[instr.INST_I.s] | instr.INST_I.C);
})

SimImplDef(SB, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->editHeap<uint8_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint8_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(SLTI, {
    int32_t rs = mainW->REGS[instr.INST_I.s];
    int16_t data = instr.INST_I.C;
    mainW->updateRegValue(instr.INST_I.t, rs < data ? 1 : 0);
})

SimImplDef(SLTIU, {
    uint32_t rs = mainW->REGS[instr.INST_I.s];
    uint16_t data = instr.INST_I.C;
    mainW->updateRegValue(instr.INST_I.t, rs < data ? 1 : 0);
})

SimImplDef(SH, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->editHeap<uint16_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint16_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(SW, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    if (!mainW->inStack(off)) {
        mainW->editHeap<uint32_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint32_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(XORI, {
    mainW->updateRegValue(instr.INST_I.t,
                          mainW->REGS[instr.INST_I.s] ^ instr.INST_I.C);
})

OP_AMONG_REGS_OVERFLOW(ADD, d, s, add, t)

OP_AMONG_REGS(ADDU, d, s, +, t)

OP_AMONG_REGS_OVERFLOW(SUB, d, s, sub, t)

OP_AMONG_REGS(SUBU, d, s, -, t)

OP_AMONG_REGS(AND, d, s, &, t)

OP_AMONG_REGS(OR, d, s, |, t)

OP_AMONG_REGS(XOR, d, s, ^, t)

SimImplDef(BREAK, { /* TODO: FIXME */})

SimImplDef(DIV, {
    int32_t s = mainW->REGS[instr.INST_R.s];
    int32_t t = mainW->REGS[instr.INST_R.t];
    mainW->updateLow(s / t);
    mainW->updateHigh(s % t);
})

SimImplDef(DIVU, {
    uint32_t s = mainW->REGS[instr.INST_R.s];
    uint32_t t = mainW->REGS[instr.INST_R.t];
    mainW->updateLow(s / t);
    mainW->updateHigh(s % t);
})

ComImplDef(JALR, JR, {
    JRImpl::exec();
    mainW->updateRegValue(instr.INST_R.d, mainW->PC + 8);
})

SimImplDef(JR, {
    mainW->advanceCounter = false;
    mainW->updateProgramCounter(mainW->REGS[instr.INST_R.s]);
})


SimImplDef(MFHI, {
    mainW->updateRegValue(instr.INST_R.d, mainW->ACC.part.high);
})

SimImplDef(MFLO, {
    mainW->updateRegValue(instr.INST_R.d, mainW->ACC.part.low);
})

SimImplDef(MTHI, {
    mainW->updateHigh(mainW->REGS[instr.INST_R.s]);
})

SimImplDef(MTLO, {
    mainW->updateLow(mainW->REGS[instr.INST_R.s]);
})

SimImplDef(MULT, {
    int32_t s = mainW->REGS[instr.INST_R.s];
    int32_t t = mainW->REGS[instr.INST_R.t];
    mainW->updateAcc(static_cast<int64_t>(s) * static_cast<int64_t>(t));
})

SimImplDef(MULTU, {
    uint32_t s = mainW->REGS[instr.INST_R.s];
    uint32_t t = mainW->REGS[instr.INST_R.t];
    mainW->updateAcc(static_cast<uint64_t>(s) * static_cast<uint64_t>(t));
})

SimImplDef(NOR, {
    mainW->updateRegValue(instr.INST_R.d, ~(mainW->REGS[instr.INST_R.s] | mainW->REGS[instr.INST_R.t]));
})

SHIFT_IMM(SLL, d, t, <<, S, uint32_t)

SHIFT_REAL(SLLV, d, t, <<, s, uint32_t)

SHIFT_IMM(SRL, d, t, >>, S, uint32_t)

SHIFT_REAL(SRLV, d, t, >>, s, uint32_t)

SHIFT_IMM(SRA, d, t, >>, S, int32_t)

SHIFT_REAL(SRAV, d, t, >>, s, int32_t)

SimImplDef(SLT, {
    int32_t rs = mainW->REGS[instr.INST_R.s];
    int32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateRegValue(instr.INST_R.d, rs < rt ? 1 : 0);
})

SimImplDef(SLTU, {
    uint32_t rs = mainW->REGS[instr.INST_R.s];
    uint32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateRegValue(instr.INST_R.d, rs < rt ? 1 : 0);
})

SimImplDef(SYSCALL, {
    mainW->handleSyscall();
})

SimImplDef(CLO, {
    uint32_t rs = mainW->REGS[instr.INST_R.s];
    mainW->updateRegValue(instr.INST_R.d, __builtin_clz(~rs));
})

SimImplDef(CLZ, {
    uint32_t rs = mainW->REGS[instr.INST_R.s];
    mainW->updateRegValue(instr.INST_R.d, __builtin_clz(rs));
})

SimImplDef(MUL, {
    int32_t rs = mainW->REGS[instr.INST_R.s];
    int32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateRegValue(instr.INST_R.d, rs * rt);
})

SimImplDef(MADD, {
    int32_t rs = mainW->REGS[instr.INST_R.s];
    int32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateAcc(mainW->ACC.all + (rs * rt));
})

SimImplDef(MADDU, {
    uint32_t rs = mainW->REGS[instr.INST_R.s];
    uint32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateAcc(mainW->ACC.all + (rs * rt));
})

SimImplDef(MSUB, {
    int32_t rs = mainW->REGS[instr.INST_R.s];
    int32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateAcc(mainW->ACC.all - (rs * rt));
})

SimImplDef(MSUBU, {
    uint32_t rs = mainW->REGS[instr.INST_R.s];
    uint32_t rt = mainW->REGS[instr.INST_R.t];
    mainW->updateAcc(mainW->ACC.all - (rs * rt));
})

TRAP_R(TEQ, ==, uint32_t)

TRAP_R(TNE, !=, uint32_t)

TRAP_R(TGE, >=, int32_t)

TRAP_R(TGEU, >=, uint32_t)

TRAP_R(TLT, <, int32_t)

TRAP_R(TLTU, <, uint32_t)

TRAP_RI(TEQI, ==, uint32_t)

TRAP_RI(TNEI, !=, uint32_t)

TRAP_RI(TGEI, >=, int32_t)

TRAP_RI(TGEIU, >=, uint32_t)

TRAP_RI(TLTI, <, int32_t)

TRAP_RI(TLTIU, <, uint32_t)

SimImplDef(LWL, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    auto t = (1u << ((4u - ((uint32_t)off & 0b11u)) << 3u)) - 1u;
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int32_t>(off) & t);
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int32_t>(off) & t);
    }
})

SimImplDef(LWR, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    auto t = ~((1u << ((4u - ((uint32_t)off & 0b11u)) << 3u)) - 1u);
    if (!mainW->inStack(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int32_t>(off) & t);
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int32_t>(off) & t);
    }
})

SimImplDef(SWL, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    auto t = (1u << ((4u - ((uint32_t)off & 0b11u)) << 3u)) - 1u;
    if (!mainW->inStack(off)) {
        mainW->editHeap<uint32_t>(off, mainW->REGS[instr.INST_I.t] & t);
    } else {
        mainW->editStack<uint32_t>(off, mainW->REGS[instr.INST_I.t] & t);
    }
})

SimImplDef(SWR, {
    uint32_t data = mainW->REGS[instr.INST_I.s];
    int16_t imm = instr.INST_I.C;
    uint32_t off = imm + data;
    auto t = ~((1u << ((4u - ((uint32_t)off & 0b11u)) << 3u)) - 1u);
    if (!mainW->inStack(off)) {
        mainW->editHeap<uint32_t>(off, mainW->REGS[instr.INST_I.t] & t);
    } else {
        mainW->editStack<uint32_t>(off, mainW->REGS[instr.INST_I.t] & t);
    }
})
