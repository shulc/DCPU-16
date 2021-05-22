#include "disasembler.h"
#include <sstream>
#include <iomanip>

TDisasembler::TDisasembler(const procVec& program)
    : Program(program)
    , NextOffset(0)
    , NextLabel(0)
{
    SpecOpNames = {{
        {NDCPU::JSR, "JSR"},
        {NDCPU::INT, "INT"},
        {NDCPU::IAG, "IAG"},
        {NDCPU::IAS, "IAS"},
        {NDCPU::RFI, "RFI"},
        {NDCPU::IAQ, "IAQ"},
        {NDCPU::HWN, "HWN"},
        {NDCPU::HWQ, "HWQ"},
        {NDCPU::HWI, "HWI"},
    }};

    OpNames = {{
        {NDCPU::SET, "SET"},
        {NDCPU::ADD, "ADD"},
        {NDCPU::SUB, "SUB"},
        {NDCPU::MUL, "MUL"},
        {NDCPU::MLI, "MLI"},
        {NDCPU::DIV, "DIV"},
        {NDCPU::DVI, "DVI"},
        {NDCPU::MOD, "MOD"},
        {NDCPU::MDI, "MDI"},
        {NDCPU::AND, "AND"},
        {NDCPU::BOR, "BOR"},
        {NDCPU::XOR, "XOR"},
        {NDCPU::SHR, "SHR"},
        {NDCPU::ASR, "ASR"},
        {NDCPU::SHL, "SHL"},
        {NDCPU::IFB, "IFB"},
        {NDCPU::IFC, "IFC"},
        {NDCPU::IFE, "IFE"},
        {NDCPU::IFN, "IFN"},
        {NDCPU::IFG, "IFG"},
        {NDCPU::IFA, "IFA"},
        {NDCPU::IFL, "IFL"},
        {NDCPU::IFU, "IFU"},
        {NDCPU::ADX, "ADX"},
        {NDCPU::SBX, "SBX"},
        {NDCPU::STI, "STI"},
        {NDCPU::STD, "STD"}
    }};
}

std::string Memory(const std::string& name) {
    return std::string("[") + name + "]";
}

std::string Hex(ui16 value, bool hasPrefix = true) {
    std::stringstream out;
    out.setf(std::ios_base::hex, std::ios_base::basefield);
    out.fill('0');
    if (hasPrefix)
        out << "0x";
    out << std::setw(4) << value;
    return out.str();
}

std::string Literal(int v) {
    std::stringstream out;
    out << v;
    return out.str();
}

std::string TDisasembler::ValueA(ui16 v, iterator& it) const {
    return Value(v >> 10, it, true);
}

std::string TDisasembler::ValueB(ui16 v, iterator& it) const {
    return Value((v >> 5) & 0x1F, it, false);
}

std::string TDisasembler::Value(ui16 v, iterator& it, bool isA) const {
    if (v <= 0x07) {
        return NDCPU::RegisterNames[v];
    } else if (v <= 0x0f) {
        return Memory(NDCPU::RegisterNames[v - 0x08]);
    } else if (v <= 0x17) {
        return Memory(NDCPU::RegisterNames[v - 0x10] + " + " + Hex(*it++));
    } else if (v == 0x18) {
        return isA ? "POP" : "PUSH";
    } else if (v == 0x19) {
        return "PEEK";
    } else if (v == 0x1a) {
        return std::string("PICK ") + Hex(*it++);
    } else if (v == 0x1b) {
        return "SP";
    } else if (v == 0x1c) {
        return "PC";
    } else if (v == 0x1d) {
        return "EX";
    } else if (v == 0x1e) {
        return Memory(Hex(*it++));
    } else if (v == 0x1f) {
        return Hex(*it++);
    } else if (v <= 0x3f) {
        if (isA) {
            return Hex(v - 0x21);
        }
    }
    return "";
}

std::string TDisasembler::Op(ui16 instruction, iterator& it) {
    const ui16 opcode = instruction & 0x1F;
    if (!opcode) {
        const ui16 specOpcode = (instruction >> 5) & 0x1F;
        std::string a = ValueA(instruction, it);
        if (specOpcode == NDCPU::JSR)
            a = SetLabel(a);
        return SpecOpNames.at(specOpcode) + " " +  a;
    }
    std::string a = ValueA(instruction, it);
    const std::string b = ValueB(instruction, it);
    if (opcode >= NDCPU::IFN && opcode <= NDCPU::IFU)
        ++NextOffset;
    else
        NextOffset = 0;
    if (opcode == NDCPU::SET && b == "PC" && a.substr(0, 2) == "0x")
        a = SetLabel(a);
    return OpNames.at(opcode) + " " + b + ", " + a;
}

std::string TDisasembler::SetLabel(const std::string& offset) {
    const auto it = Labels.find(offset);
    if (it == Labels.end()) {
        std::stringstream out;
        out << "label" << ++NextLabel;
        return Labels.emplace(offset, out.str()).first->second;
    }
    return it->second;
}

void TDisasembler::Process() {
    iterator begin = Program.begin();
    for (iterator it = Program.begin(), end = Program.end(); it != end;) {
        iterator opBegin = it;
        ui16 v = *it++;
        size_t currentOffset = NextOffset;
        AsmProgram.emplace_back(
                Hex(static_cast<ui16>(opBegin - begin)),
                Op(v, it),
                opBegin, it,
                currentOffset
                );
    }
}

void TDisasembler::Save(std::ostream* out) {
    for (const auto it : AsmProgram) {
        auto labelIt = Labels.find(it.Number);
        *out << std::left << std::setw(10);
        if (labelIt != Labels.end()) {
            *out << labelIt->second  + ":";
        } else {
            *out << " ";
        }
        //*out << it.Number << " : ";
        std::string offset;
        for (size_t i = 0; i < it.Offset; ++i)
            offset += "  ";

        *out << std::left
            << std::setw(30)
            << offset + it.Asm
            << ";";

        for (const auto i : it.Instructions)
            *out << " " << Hex(i, false);
        *out << std::endl;
    }
}
