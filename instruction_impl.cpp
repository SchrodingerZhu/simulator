//
// Created by schrodinger on 2/20/20.
//

#include "instruction_impl.h"
#include "mainwindow.ipp"

MainWindow *InstructionImpl::mainW = nullptr;

InstructionImpl::InstructionImpl(Instruction instr) : instr(instr) {}

SimImplDef(J, {
    mainW->updateProgramCounter(instr.INST_J.A);
})

ComImplDef(JALI, J, {
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
    int8_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int8_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int8_t>(off));
    }
})

SimImplDef(LBU, {
    int8_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<uint8_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<uint8_t>(off));
    }
})

SimImplDef(LH, {
    int8_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchHeap<int16_t>(off));
    } else {
        mainW->updateRegValue(instr.INST_I.t, mainW->fetchStack<int16_t>(off));
    }
})

SimImplDef(LHU, {
    int8_t data = mainW->REGS[instr.INST_I.s];
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
    int8_t data = mainW->REGS[instr.INST_I.s];
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
    int8_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->editHeap<uint8_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint8_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(SH, {
    int8_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->editHeap<uint16_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint16_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(SW, {
    int8_t data = mainW->REGS[instr.INST_I.s];
    int16_t off = instr.INST_I.C + data;
    if (mainW->inHeap(off)) {
        mainW->editHeap<uint32_t>(off, mainW->REGS[instr.INST_I.t]);
    } else {
        mainW->editStack<uint32_t>(off, mainW->REGS[instr.INST_I.t]);
    }
})

SimImplDef(SLTI, {
    int8_t rs = mainW->REGS[instr.INST_I.s];
    int16_t data = instr.INST_I.C;
    mainW->updateRegValue(instr.INST_I.t, rs < data ? 1 : 0);
})

SimImplDef(SLTIU, {
    uint8_t rs = mainW->REGS[instr.INST_I.s];
    uint16_t data = instr.INST_I.C;
    mainW->updateRegValue(instr.INST_I.t, rs < data ? 1 : 0);
})
