#include "emulator.h"

namespace NDCPU {

TEmulator::TEmulator()
    : Memory(0x10000)
{
}

TEmulator::TEmulator(const std::vector<ui16>& program)
    : Memory(0x10000)
{
    std::copy(program.begin(), program.end(), Memory.begin());
}

const ui16* TEmulator::GetValueA(ui16 v) {
    return GetValue(v>>10, true);
}

ui16* TEmulator::GetValueB(ui16 v) {
    return GetValue((v >> 5) & 0x1F, false);
}

ui16* TEmulator::GetValue(ui16 v, bool isA) {
    if (v <= 0x07) {
        return &Registers[v];
    } else if (v <= 0x0f) {
        return &Memory[Registers[v - 0x08]];
    } else if (v <= 0x17) {
        Tick(1);
        return &Memory[Registers[v - 0x10] + PC++];
    } else if (v == 0x18) {
        return &Memory[isA ? SP++ : --SP];
    } else if (v == 0x19) {
        return &Memory[SP];
    } else if (v == 0x1a) {
        Tick(1);
        return &Memory[SP + PC++];
    } else if (v == 0x1b) {
        return &SP;
    } else if (v == 0x1c) {
        return &PC;
    } else if (v == 0x1d) {
        return &EX;
    } else if (v == 0x1e) {
        Tick(1);
        return &Memory[Memory[PC++]];
    } else if (v == 0x1f) {
        Tick(1);
        return &Memory[PC++];
    } else if (v >= 0x20 && v <= 0x3f) {
        if (isA) {
            Literal[isA] = FromLiteral(v - 0x21);
            return &Literal[isA];
        }
    }
    return nullptr;
}

void TEmulator::Op(ui16 instuction) {
    const ui16 opcode = instuction & 0x1F;
    // special opcode
    if (opcode == 0x00) {
        const ui16 specOpcode = (instuction >> 5) & 0x1F;
        const ui16* a = GetValueA(instuction);
        switch (specOpcode) {
            case JSR:
                Tick(3);
                // pushes the address of the next instruction to the stack
                Memory[SP--] = PC++;
                // set PC to a
                PC = *a;
                break;
            // TODO:
            // add INT, IAG, IAS, RFI, IAQ, HWN, HWQ, HWI
        }
        return;
    }
    const ui16 *a = GetValueA(instuction);
    ui16       *b = GetValueB(instuction);
    i32 tmp;
    switch (opcode) {
        case SET:
            Tick(1);
            *b = *a;
            break;
        case ADD:
            Tick(2);
            tmp = *a + *b;
            EX = (tmp >> 16) ? 0x0001 : 0x0;
            *b = tmp & 0xFFFF;
            break;
        case SUB:
            Tick(2);
            // TODO:
            // check underflow
            *b -= *a;
            break;
        case MUL:
            Tick(2);
            EX = ((*b * *a) >> 16) & 0xffff;
            *b *= *a;
            break;
        case MLI:
            Tick(2);
            // TODO:
            //  like MUL, but treat b, a as signed
            break;
        case ADX:
            Tick(3);
            {
                tmp = *b + *a + EX;
                EX = tmp >> 16 ? 0x0001 : 0x0;
                *b = tmp & 0xFFFF;
            }
            break;
        case SBX:
            Tick(3);
            {
                tmp = *b - *a + EX;
                EX = tmp >> 16 ? 0xFFFF : 0x0;
                *b = tmp & 0xFFFF;
            }
            break;
        case STI:
            Tick(2);
            *b = *a;
            ++Registers[I];
            ++Registers[J];
            break;
        case STD:
            Tick(2);
            *b = *a;
            --Registers[I];
            --Registers[J];
            break;
    }
}

void TEmulator::Tick(size_t n) {
    Ticks += n;
}

ui16 TEmulator::FromLiteral(ui16 v) const {
    return (v & 0xFF - '0') * 10 + (v >> 8) - '0';
}

void TEmulator::Step() {
    Op(Memory[PC++]);
}

} // namespace NDCPU
