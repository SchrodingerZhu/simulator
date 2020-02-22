//
// Created by schrodinger on 2/20/20.
//

#include "instruction_impl.h"
#include "mainwindow.ipp"
#define OP_AMONG_REGS(NAME, A, B, op, C)\
SimImplDef(NAME, {\
    mainW->updateRegValue(instr.INST_R.A, mainW->REGS[instr.INST_R.B] op mainW->REGS[instr.INST_R.C]);\
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

MainWindow *InstructionImpl::mainW = nullptr;

InstructionImpl::InstructionImpl(Instruction instr) : instr(instr) {}

SimImplDef(J, {
    mainW->updateProgramCounter(instr.INST_J.A);
})

ComImplDef(JAL, J, {
    mainW->updateRegValue(31, mainW->PC + 8);
    JImpl::exec();
})

SimImplDef(ADDI, {
    mainW->updateRegValue(instr.INST_I.t,
                          (int8_t) mainW->REGS[instr.INST_I.s] + (int16_t) instr.INST_I.C);
})

SimImplDef(ADDIU, {
    mainW->updateRegValue(instr.INST_I.t,
                          (unsigned) mainW->REGS[instr.INST_I.s] + (unsigned) instr.INST_I.C);
})

SimImplDef(ANDI, {
    mainW->updateRegValue(instr.INST_I.t,
                          mainW->REGS[instr.INST_I.s] & instr.INST_I.C);
})

SimImplDef(BEQ, {
    if (mainW->REGS[instr.INST_I.s] == mainW->REGS[instr.INST_I.t]) {
        mainW->updateProgramCounter(mainW->PC + (int16_t)instr.INST_I.C);
    }
})

SimImplDef(BGEZ, {
    if (mainW->REGS[instr.INST_I.s] >= 0) {
        mainW->updateProgramCounter(mainW->PC + (int16_t)instr.INST_I.C);
    }
})

SimImplDef(BGTZ, {
    if (mainW->REGS[instr.INST_I.s] > 0) {
        mainW->updateProgramCounter(mainW->PC + (int16_t)instr.INST_I.C);
    }
})

SimImplDef(BLEZ, {
    if (mainW->REGS[instr.INST_I.s] <= 0) {
        mainW->updateProgramCounter(mainW->PC + (int16_t)instr.INST_I.C);
    }
})

SimImplDef(BLTZ, {
    if (mainW->REGS[instr.INST_I.s] < 0) {
        mainW->updateProgramCounter(mainW->PC + (int16_t)instr.INST_I.C);
    }
})

SimImplDef(BNE, {
    if (instr.INST_I.s != instr.INST_I.t) {
        mainW->updateProgramCounter(mainW->PC + (int16_t)instr.INST_I.C);
    }
})

SimImplDef(LB, {
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int8_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int8_t>(off));
    }
})

SimImplDef(LBU, {
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<uint8_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<uint8_t>(off));
    }
})

SimImplDef(LH, {
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int16_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int16_t>(off));
    }
})

SimImplDef(LHU, {
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<uint16_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<uint16_t>(off));
    }
})

SimImplDef(LUI, {
        mainW->updateRegValue(instr.INST_I.t, instr.INST_I.C << 16u);
})

SimImplDef(LW, {
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
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
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
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
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->editHeap<uint16_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint16_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(SW, {
    int32_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->editHeap<uint32_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint32_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(XORI, {
    mainW->updateRegValue(instr.INST_I.t,
                          mainW->REGS[instr.INST_I.s] ^ instr.INST_I.C);
})

OP_AMONG_REGS(ADD, d, s, +, t)
OP_AMONG_REGS(ADDU, d, s, +, t)
OP_AMONG_REGS(SUB, d, s, -, t)
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
    mainW->updateRegValue(instr.INST_R.d, ~ (mainW->REGS[instr.INST_R.s] | mainW->REGS[instr.INST_R.t]));
})

SHIFT_IMM(SLL, d, t, <<, s, uint32_t)
SHIFT_REAL(SLLV, d, t, <<, s, uint32_t)
SHIFT_IMM(SRL, d, t, >>, s, uint32_t)
SHIFT_REAL(SRLV, d, t, >>, s, uint32_t)
SHIFT_IMM(SRA, d, t, >>, s, int32_t)
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

SimImplDef(SYSCALL, {/* TODO: FIXME */})
