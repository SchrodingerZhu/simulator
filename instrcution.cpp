#include "instrcution.h"
#include <stdexcept>

TYPE resolv_type(Instruction inst) {
    switch (inst.INST_J.op) {
        case 0:
            return R;
        case OPJ_J:
        case OPJ_JAL:
            return J;
        case OPI_ADDI:
        case OPI_ADDIU:
        case OPI_ANDI:
        case OPI_BEQ:
        case OPI_BGEZ: //OPI_BLTZ
        case OPI_BGTZ:
        case OPI_BLEZ:
        case OPI_BNE:
        case OPI_LB:
        case OPI_LBU:
        case OPI_LH:
        case OPI_LHU:
        case OPI_LUI:
        case OPI_LW:
        case OPI_ORI:
        case OPI_SB:
        case OPI_SLTI:
        case OPI_SLTIU:
        case OPI_SH:
        case OPI_SW:
        case OPI_SWCL:
        case OPI_XORI:
            return I;
        default:
            throw std::runtime_error{"unrecognized type"};
    }
}
