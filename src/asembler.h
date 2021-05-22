#pragma once

#include <iosfwd>
#include <string>
#include <vector>

class TAsembler {
public:
    explicit TAsembler(std::istream* in);
    void Save(std::ostream* out) const;

private:
    std::vector<std::string> Lines;
};
