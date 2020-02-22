#include "instrcution.h"
#include <stdexcept>

TYPE resolv_type(Instruction inst) {
    switch (inst.INST_J.op) {
        case 0:
            return R;
        case OPC_J:
        case OPC_JAL:
            return J;
        case OPC_ADDI:
        case OPC_ADDIU:
        case OPC_ANDI:
        case OPC_BEQ:
        case OPC_BGEZ: //OPI_BLTZ
        case OPC_BGTZ:
        case OPC_BLEZ:
        case OPC_BNE:
        case OPC_LB:
        case OPC_LBU:
        case OPC_LH:
        case OPC_LHU:
        case OPC_LUI:
        case OPC_LW:
        case OPC_ORI:
        case OPC_SB:
        case OPC_SLTI:
        case OPC_SLTIU:
        case OPC_SH:
        case OPC_SW:
        case OPC_SWCL:
        case OPC_XORI:
            return I;
        default:
            throw std::runtime_error{"unrecognized type"};
    }
}
