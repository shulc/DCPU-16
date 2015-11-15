#pragma once

#include "misc.h"
#include <vector>

class TEmulator {
public:
    // Registers
    enum {
        A = 0,
        B,
        C,
        X,
        Y,
        Z,
        I,
        J,
        NUM_REGISTERS,
    };

    // Basic opcodes
    enum {
        SET = 0x01,
        ADD = 0x02,
        SUB = 0x03,
        MUL = 0x04,
        MLI = 0x05,
        DIV = 0x06,
        DVI = 0x07,
        MOD = 0x08,
        MDI = 0x09,
        AND = 0x0a,
        BOR = 0x0b,
        XOR = 0x0c,
        SHR = 0x0d,
        ASR = 0x0e,
        SHL = 0x0f,
        IFB = 0x10,
        IFC = 0x11,
        IFE = 0x12,
        IFN = 0x13,
        IFG = 0x14,
        IFA = 0x15,
        IFL = 0x16,
        IFU = 0x17,
        ADX = 0x1a,
        SBX = 0x1b,
        STI = 0x1e,
        STD = 0x1f,
    };

    // Special opcodes
    enum {
        JSR = 0x01,
        INT = 0x08,
        IAG = 0x09,
        IAS = 0x0a,
        RFI = 0x0b,
        IAQ = 0x0c,
        HWN = 0x10,
        HWQ = 0x11,
        HWI = 0x12,
    };


    std::vector<ui16> Memory;
    // Registers
    ui16 Registers[NUM_REGISTERS] = {0};
    const char* RegisterNames[NUM_REGISTERS] = {
        "A",
        "B",
        "C",
        "X",
        "Y",
        "Z",
        "I",
        "J"
    };

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
