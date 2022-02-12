#ifndef OPS_H

const auto NibbleMask = 0x000F;
const auto AddrMask   = 0x0FFF;
const auto ByteMask   = 0x00FF;

enum OpCode {
    SYS,
    CLS,
    RET,
    JP,
    CALL,
    SE_BY,
    SNE_BY,
    SE_VY,
    LD_BY,
    ADD,
    LD_VX,
    OR,
    AND,
    XOR,
    ADD_VX,
    SUB_VX,
    SHR_VX,
    SUBN,
    SHL,
    SNE_VX,
    LD_I,
    JP_V,
    RND,
    DRW,
    SKP,
    SKNP,
    LD_VX_DT,
    LD_VX_K,
    LD_DT,
    LD_ST,
    ADD_I,
    LD_F,
    LD_B,
    LD_I_VX,
    LD_VX_I
};

#endif // !OPS_H
