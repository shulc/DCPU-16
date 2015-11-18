#pragma once
#include <inttypes.h>
#include <array>
#include <string>

using ui16 = uint16_t;
using i16  = int16_t;

using ui32 = uint32_t;
using i32  = int32_t;

namespace NDCPU {
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

    static std::array<std::string, NUM_REGISTERS> RegisterNames = {{
        "A",
        "B",
        "C",
        "X",
        "Y",
        "Z",
        "I",
        "J"
    }};
};
