#include <cstdint>
#include <cstddef>
#include <mutex>
#include <stdexcept>
#include <string>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

namespace {

constexpr std::int32_t VOICE_QOS_APP_TYPE_GAME = 0x20000000;

std::mutex g_mutex;
bool g_initialized = false;

}

extern "C" {

int APS5_VABI sceVoiceQoSInit(void* mem_block, uint32_t mem_size, int32_t app_type) {
    if (!mem_block || mem_size == 0) throw std::invalid_argument("sceVoiceQoSInit: null or empty memory block");
    if (app_type != VOICE_QOS_APP_TYPE_GAME) throw std::invalid_argument("sceVoiceQoSInit: unsupported app type " + std::to_string(app_type));
    std::lock_guard lock(g_mutex);
    if (g_initialized) throw std::runtime_error("sceVoiceQoSInit: already initialized");
    g_initialized = true;
    return 0;
}

}
