#include "SceTypes.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <windows.h>

extern "C" int APS5_VABI sceKernelGetModuleInfoForUnwind(uint64_t addr, int flags, ModuleInfoForUnwind* info);

static void Require(bool value) { if (!value) std::abort(); }

__attribute__((section(".ehmeta"), used)) static volatile std::uint32_t ehMeta = 0;

struct alignas(4) FrameHeader {
    std::uint8_t version = 1;
    std::uint8_t framePointerEncoding = 0x1b;
    std::uint8_t countEncoding = 0x03;
    std::uint8_t tableEncoding = 0x3b;
    std::int32_t frames = 0;
    std::uint32_t count = 0;
};

struct alignas(4) Frames {
    std::uint32_t cieLength = 12;
    std::uint32_t cieId = 0;
    std::uint8_t cie[8] = {1, 0, 1, 0x78, 16, 0, 0, 0};
    std::uint32_t terminator = 0;
};

static FrameHeader header;
static Frames frames;

int main() {
    const auto base = reinterpret_cast<std::uint64_t>(GetModuleHandleW(nullptr));
    header.frames = static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(&frames) - reinterpret_cast<std::intptr_t>(&header.frames));
    DWORD old = 0;
    Require(VirtualProtect(const_cast<std::uint32_t*>(&ehMeta), sizeof(ehMeta), PAGE_READWRITE, &old));
    ehMeta = static_cast<std::uint32_t>(reinterpret_cast<std::uint64_t>(&header) - base);

    ModuleInfoForUnwind info{};
    Require(sceKernelGetModuleInfoForUnwind(reinterpret_cast<std::uint64_t>(&main), 0, &info) == 0);
    Require(info.st_size == sizeof(ModuleInfoForUnwind));
    Require(info.eh_frame_hdr_addr == reinterpret_cast<std::uint64_t>(&header));
    Require(info.eh_frame_addr == reinterpret_cast<std::uint64_t>(&frames));
    Require(info.eh_frame_size == 4 + 12);
    Require(info.seg0_addr == base);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    Require(info.seg0_size == nt->OptionalHeader.SizeOfImage);

    ModuleInfoForUnwind host{};
    Require(sceKernelGetModuleInfoForUnwind(reinterpret_cast<std::uint64_t>(&GetModuleHandleW), 0, &host) == 0);
    Require(host.eh_frame_hdr_addr == 0 && host.eh_frame_addr == 0 && host.eh_frame_size == 0);
}
