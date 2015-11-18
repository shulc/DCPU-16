#pragma once

#include "misc.h"
#include <string>

namespace NDCPU {

class TEmulator;

class TDebugViewer {
    friend class TEmulator;
    const TEmulator* Emulator;

    void DumpRegister(const std::string& name, ui16 value) const;

public:
    TDebugViewer(const TEmulator* emulator);
    void Dump() const;
};
};
