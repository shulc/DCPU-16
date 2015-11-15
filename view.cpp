#include "view.h"
#include "emulator.h"

#include <iostream>
#include <iomanip>

TDebugViewer::TDebugViewer(const TEmulator* emulator)
    : Emulator(emulator)
{
}

void TDebugViewer::DumpRegister(const std::string& name, ui16 value) const {
    std::cout
        << std::setw(4)
        << name
        << " : ";

    std::ios_base::fmtflags flags = std::cout.setf(
            std::ios_base::hex, std::ios_base::basefield);
    char fill = std::cout.fill('0');
    std::cout << "0x" << std::setw(4) << value;
    std::cout.fill(fill);
    std::cout.setf(flags, std::ios_base::basefield);
}

void TDebugViewer::Dump() const {
    std::cout << "Cycles : " << Emulator->Ticks << std::endl;
    DumpRegister("PC", Emulator->PC);
    DumpRegister("SP", Emulator->SP);
    DumpRegister("EX", Emulator->EX);
    DumpRegister("IA", Emulator->IA);
    std::cout << std::endl;

    for (auto i = 0; i < TEmulator::NUM_REGISTERS; ++i) {
        DumpRegister(Emulator->RegisterNames[i], Emulator->Registers[i]);
        if (i % 4 == 3)
            std::cout << std::endl;
    }
}
