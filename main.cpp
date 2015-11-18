#include "emulator.h"
#include "view.h"

#include <vector>

int main(int, char*[]) {
    std::vector<ui16> program { {
        0x7c01, 0x0030, 0x7fc1, 0x0020, 0x1000, 0x7803, 0x1000, 0xc413,
        0x7f81, 0x0019, 0xacc1, 0x7c01, 0x2000, 0x22c1, 0x2000, 0x88c3,
        0x84d3, 0xbb81, 0x9461, 0x7c20, 0x0017, 0x7f81, 0x0019, 0x946f,
        0x6381, 0xeb81} };

    NDCPU::TEmulator emulator(program);
    NDCPU::TDebugViewer view(&emulator);

    // dump initial state
    view.Dump();

    for (size_t i = 0; i < 4; ++i) {
        emulator.Step();
        view.Dump();
    }

    return 0;
}
