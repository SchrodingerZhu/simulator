#ifndef INSTRCUTION_H
#define INSTRCUTION_H

#include "global.h"

union Instruction {
    uint32_t __content;
    struct {
        uint32_t f: 6, S: 5, d: 5, t:5, s: 5, op: 6;
    } INST_R;
    struct {
        uint32_t C: 16, t: 5, s: 5, op: 6;
    } INST_I;
    struct {
        uint32_t A: 26, op: 6;
    } INST_J;
};

enum TYPE : uint8_t {
    R, I, J
};

#define OPJ_J       0b000010
#define OPJ_JAL     0b000011

#define OPI_ADDI    0b001000
#define OPI_ADDIU   0b001001
#define OPI_ANDI    0b001100
#define OPI_BEQ     0b000100
#define OPI_BGEZ    0b000001
#define OPI_BGTZ    0b000111
#define OPI_BLEZ    0b000110
#define OPI_BLTZ    0b000001
#define OPI_BNE     0b000101
#define OPI_LB      0b100000
#define OPI_LBU     0b100100
#define OPI_LH      0b100001
#define OPI_LHU     0b100101
#define OPI_LUI     0b001111
#define OPI_LW      0b100011
#define OPI_ORI     0b001101
#define OPI_SB      0b101000
#define OPI_SLTI    0b001010
#define OPI_SLTIU   0b001011
#define OPI_SH      0b101001
#define OPI_SW      0b101011
#define OPI_SWCL    0b111001
#define OPI_XORI    0b001110

#define FCR_ADD     0b100000
#define FCR_ADDU    0b100001
#define FCR_AND     0b100100
#define FCR_BREAK   0b001101
#define FCR_DIV     0b011010
#define FCR_DIVU    0b011011
#define FCR_JALR    0b001001
#define FCR_JR      0b001000
#define FCR_MFHI    0b010000
#define FCR_MFLO    0b010010
#define FCR_MTHI    0b010001
#define FCR_MTLO    0b010011
#define FCR_MULT    0b011000
#define FCR_MULTU   0b011001
#define FCR_NOR     0b100111
#define FCR_OR      0b100101
#define FCR_SLL     0b000000
#define FCR_SLLV    0b000100
#define FCR_SLT     0b101010
#define FCR_SLTU    0b101011
#define FCR_SRA     0b000011
#define FCR_SRAV    0b000111
#define FCR_SRL     0b000010
#define FCR_SRLV    0b000110
#define FCR_SUB     0b100010
#define FCR_SUBU    0b100011
#define FCR_SYSCALL 0b001100
#define FCR_XOR     0b100110

TYPE resolv_type(Instruction inst);

#endif // INSTRCUTION_H
