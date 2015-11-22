#pragma once

#include "misc.h"
#include <string>
#include <vector>
#include <unordered_map>

class TDisasembler {
    using procVec = std::vector<ui16>;
    using iterator = procVec::const_iterator;

    struct TAsmLine {
        TAsmLine(const std::string& number, const std::string& a, size_t offset = 0)
            : Number(number)
            , Asm(a)
            , Offset(offset) {
        }

        std::string Number;
        std::string Asm;
        size_t Offset;
    };

    using TAsmVec = std::vector<TAsmLine>;

    const procVec& Program;
    std::unordered_map<ui16, std::string> SpecOpNames;
    std::unordered_map<ui16, std::string> OpNames;
    std::unordered_map<std::string, std::string> Labels;
    TAsmVec AsmProgram;

    std::string SetLabel(const std::string& offset);
    std::string Value(ui16 v, iterator& it, bool isA) const;
    std::string ValueA(ui16 v, iterator& it) const;
    std::string ValueB(ui16 v, iterator& it) const;
    std::string Op(ui16 v, iterator& it);
    size_t NextOffset;
    size_t NextLabel;

public:
    TDisasembler(const procVec& program);
    void Process();
    void Save(std::ostream* out);
};
