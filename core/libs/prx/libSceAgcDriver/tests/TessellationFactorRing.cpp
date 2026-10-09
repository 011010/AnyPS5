#include "SceTypes.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

extern "C" {
int APS5_VABI sceAgcDriverSetTFRing(const volatile void* base, uint32_t size);
int APS5_VABI sceAgcDriverGetTFRing(uintptr_t* base, uint32_t* size);
}

namespace {

void Require(bool value) {
    if (!value) std::abort();
}

template<typename TAction>
bool Rejects(TAction action) {
    try { action(); }
    catch (const std::invalid_argument&) { return true; }
    return false;
}

void RequireRing(uintptr_t expectedBase, uint32_t expectedSize) {
    uintptr_t base = 0;
    uint32_t size = 0;
    Require(sceAgcDriverGetTFRing(&base, &size) == 0);
    Require(base == expectedBase && size == expectedSize);
}

}

int main() {
    alignas(256) static uint8_t ring[0x4000];
    const auto ringBase = reinterpret_cast<uintptr_t>(ring);
    RequireRing(0xff00000000, 0x20000);
    Require(sceAgcDriverSetTFRing(ring, sizeof(ring)) == 0);
    RequireRing(ringBase, sizeof(ring));
    Require(sceAgcDriverSetTFRing(ring + 0x1000, 0x2000) == 0);
    RequireRing(ringBase + 0x1000, 0x2000);
    Require(Rejects([] { sceAgcDriverSetTFRing(nullptr, 0x2000); }));
    Require(Rejects([] { sceAgcDriverSetTFRing(ring, 0); }));
    RequireRing(ringBase + 0x1000, 0x2000);
    uintptr_t base = 0;
    uint32_t size = 0;
    Require(Rejects([&] { sceAgcDriverGetTFRing(nullptr, &size); }));
    Require(Rejects([&] { sceAgcDriverGetTFRing(&base, nullptr); }));
}
