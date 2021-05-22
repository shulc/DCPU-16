#include "asembler.h"

#include <iostream>

TAsembler::TAsembler(std::istream* in) {
    std::string line;
    while (std::getline(*in, line)) {
        Lines.push_back(line);
    }
}

void TAsembler::Save(std::ostream* out) const {
    for (const auto& line : Lines) {
        *out << line << "\n";
    }
}
