#pragma once

#include "misc.h"
#include <string>
#include <vector>
#include <unordered_map>

class TDisasembler {
    using iterator = std::vector<ui16>::const_iterator;

    std::unordered_map<ui16, std::string> SpecOpNames;
    std::unordered_map<ui16, std::string> OpNames;

    std::string Value(ui16 v, iterator& it, bool isA) const;
    std::string ValueA(ui16 v, iterator& it) const;
    std::string ValueB(ui16 v, iterator& it) const;

public:
    TDisasembler();
    std::string Op(ui16 v, iterator& it) const;
};
