#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <vector>

extern "C" {
int APS5_VABI sceVoiceQoSInit(void*, std::uint32_t, std::int32_t);
}

static void Require(bool value) { if (!value) std::abort(); }

template<typename TException, typename TFunction>
static bool Throws(TFunction function) {
    try {
        function();
    } catch (const TException&) {
        return true;
    }
    return false;
}

int main() {
    std::vector<std::uint8_t> memory(0x40000);
    Require(Throws<std::invalid_argument>([&] { sceVoiceQoSInit(nullptr, 0x40000, 0x20000000); }));
    Require(Throws<std::invalid_argument>([&] { sceVoiceQoSInit(memory.data(), 0, 0x20000000); }));
    Require(Throws<std::invalid_argument>([&] { sceVoiceQoSInit(memory.data(), 0x40000, 0); }));
    Require(sceVoiceQoSInit(memory.data(), 0x40000, 0x20000000) == 0);
    Require(Throws<std::runtime_error>([&] { sceVoiceQoSInit(memory.data(), 0x40000, 0x20000000); }));
}
