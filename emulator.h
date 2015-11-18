#pragma once

#include "misc.h"
#include <vector>

namespace NDCPU {

class TEmulator {
public:
    std::vector<ui16> Memory;
    // Registers
    ui16 Registers[NUM_REGISTERS] = {0};

    ui16 PC = 0;        // program counter
    ui16 SP = 0xFFFF;        // stack pointer
    ui16 EX = 0;        // extra/excess
    ui16 IA = 0;        // interrupt address

    ui16 Literal[2]; // temp registers for literal values

    size_t Ticks = 0;

    void Tick(size_t n);
    const ui16* GetValueA(ui16 v);
    ui16* GetValueB(ui16 v);
    ui16* GetValue(ui16 v, bool isA);

    ui16 FromLiteral(ui16 n) const ;

    TEmulator();
    TEmulator(const std::vector<ui16>& program);
    void Op(ui16 opcode);
    void Step();
};
};
