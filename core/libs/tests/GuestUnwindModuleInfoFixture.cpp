#include "tests/GuestUnwindModuleInfoFixture.hpp"

#include <cstdint>
#include <windows.h>

__attribute__((section(".ehmeta"), used)) static volatile std::uint32_t ehMeta = 0;

extern "C" IMAGE_DOS_HEADER __ImageBase;

static UnwindFixture fixture;

extern "C" __declspec(dllexport) UnwindFixture* GetUnwindFixture() {
    const auto base = reinterpret_cast<std::uint64_t>(&__ImageBase);
    fixture.header.frames = static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(&fixture.frames) - reinterpret_cast<std::intptr_t>(&fixture.header.frames));
    DWORD old = 0;
    if (!VirtualProtect(const_cast<std::uint32_t*>(&ehMeta), sizeof(ehMeta), PAGE_READWRITE, &old)) return nullptr;
    ehMeta = static_cast<std::uint32_t>(reinterpret_cast<std::uint64_t>(&fixture.header) - base);
    return &fixture;
}
