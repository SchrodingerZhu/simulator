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
    R, I, J, RI, RLIKE
};

#define OPC_J       0b000010
#define OPC_JAL     0b000011

#define OPC_ADDI    0b001000
#define OPC_ADDIU   0b001001
#define OPC_ANDI    0b001100
#define OPC_BEQ     0b000100
#define OPC_BGTZ    0b000111
#define OPC_BLEZ    0b000110
#define OPC_BNE     0b000101
#define OPC_LB      0b100000
#define OPC_LBU     0b100100
#define OPC_LH      0b100001
#define OPC_LHU     0b100101
#define OPC_LUI     0b001111
#define OPC_LW      0b100011
#define OPC_ORI     0b001101
#define OPC_SB      0b101000
#define OPC_SLTI    0b001010
#define OPC_SLTIU   0b001011
#define OPC_SH      0b101001
#define OPC_SW      0b101011
#define OPC_SWCL    0b111001
#define OPC_XORI    0b001110
#define OPC_LWL     0b100010
#define OPC_LWR     0b100110
#define OPC_SWL     0b101010
#define OPC_SWR     0b101110
#define OPC_SC      0b111000
#define OPC_LL      0b110000

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
#define FCR_TEQ     0b110100
#define FCR_TNE     0b110110
#define FCR_TGE     0b110000
#define FCR_TGEU    0b110001
#define FCR_TLT     0b110010
#define FCR_TLTU    0b110011

#define RI_BLTZ    0b00000
#define RI_BGEZ    0b00001
#define RI_TGEI    0b01000
#define RI_TGEIU    0b01001
#define RI_TLTI    0b01010
#define RI_TLTIU   0b01011
#define RI_TEQI    0b01100
#define RI_TNEI    0b01110
#define RI_BLTZAL  0b10000
#define RI_BGEZAL  0b10001

//#define RI_BLTZL   0b00010
//#define RI_BGEZL   0b00011
//#define RI_BLTZALL 0b10010
//#define RI_BGEZALL 0b10011

#define RLIKE_CLO   0b100001
#define RLIKE_CLZ   0b100000
#define RLIKE_MUL   0b000010
#define RLIKE_MADD  0b000000
#define RLIKE_MADDU 0b000001
#define RLIKE_MSUB  0b000100
#define RLIKE_MSUBU 0b000101

TYPE resolv_type(Instruction inst);

#endif // INSTRCUTION_H
