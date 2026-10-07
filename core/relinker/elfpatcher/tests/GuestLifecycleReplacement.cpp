#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

extern "C" void APS5_VABI Record(int event) {
    const auto path = std::getenv("ANYPS5_LIFECYCLE_EVENTS");
    if (!path) throw std::runtime_error("Missing lifecycle event path");
    auto* file = std::fopen(path, "ab");
    if (!file) throw std::runtime_error("Cannot open lifecycle event file");
    const auto result = std::fputc(event, file);
    const auto closed = std::fclose(file);
    if (result == EOF || closed != 0) throw std::runtime_error("Cannot write lifecycle event");
}

namespace {

struct Runtime {
    Runtime() { Record('H'); }
    ~Runtime() { Record('h'); }
} runtime;

}
