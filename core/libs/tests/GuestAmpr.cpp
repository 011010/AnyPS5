#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libkernel/Apr/include/AprCommandBuffer.hpp"
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <future>
#include <string>
#include <utility>

extern "C" {
int APS5_VABI sceAmprCommandBufferConstructor(Apr::CommandBufferObject*);
int APS5_VABI sceAmprAprCommandBufferConstructor(Apr::CommandBufferObject*, std::uint64_t*, std::uint64_t*);
int APS5_VABI sceAmprCommandBufferSetBuffer(Apr::CommandBufferObject*, void*, std::uint32_t);
std::uint32_t APS5_VABI sceAmprCommandBufferGetCurrentOffset(const Apr::CommandBufferObject*);
std::uint32_t APS5_VABI sceAmprCommandBufferGetNumCommands(const Apr::CommandBufferObject*);
int APS5_VABI sceAmprCommandBufferWriteAddressOnCompletion(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint64_t);
int APS5_VABI sceAmprCommandBufferPushMarker(Apr::CommandBufferObject*, const char*);
int APS5_VABI sceAmprCommandBufferPushMarkerWithColor(Apr::CommandBufferObject*, const char*, std::uint32_t);
int APS5_VABI sceAmprCommandBufferPopMarker(Apr::CommandBufferObject*);
int APS5_VABI sceAmprCommandBufferSetMarker(Apr::CommandBufferObject*, const char*);
int APS5_VABI sceAmprCommandBufferSetMarkerWithColor(Apr::CommandBufferObject*, const char*, const std::uint32_t*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizePushMarker(const char*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizePushMarkerWithColor(const char*, std::uint32_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizePopMarker();
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeSetMarker(const char*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeSetMarkerWithColor(const char*, std::uint32_t);
int APS5_VABI sceKernelAprSubmitCommandBuffer(const Apr::CommandBufferObject*, std::uint32_t);
int APS5_VABI sceAmprCommandBufferWaitOnAddress(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint64_t, std::uint8_t, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWaitOnCounter(Apr::CommandBufferObject*, std::uint8_t, std::uint32_t, std::uint8_t, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWriteCounterOnCompletion(Apr::CommandBufferObject*, std::uint8_t, std::uint32_t);
int APS5_VABI sceAmprCommandBufferWriteAddressFromTimeCounterOnCompletion(Apr::CommandBufferObject*, volatile std::uint64_t*);
int APS5_VABI sceAmprCommandBufferWriteAddressFromCounterOnCompletion(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWriteAddressFromCounterPairOnCompletion(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWriteKernelEventQueueOnCompletion(Apr::CommandBufferObject*, std::uint64_t, std::int32_t, std::uint64_t);
int APS5_VABI sceAmprCommandBufferNop(Apr::CommandBufferObject*, std::uint32_t);
int APS5_VABI sceAmprCommandBufferNopWithData(Apr::CommandBufferObject*, std::uint32_t, const std::uint32_t*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeNop(std::uint32_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeNopWithData(std::uint32_t);
int APS5_VABI sceAmprCommandBufferWaitOnAddress_04_00(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint64_t, std::uint8_t, std::uint8_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeWaitOnAddress_04_00(volatile std::uint64_t*, std::uint64_t, std::uint8_t, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWriteAddressFromTimeCounter_04_00(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint64_t);
int APS5_VABI sceAmprCommandBufferWriteAddressFromCounter_04_00(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint8_t, std::uint64_t);
int APS5_VABI sceAmprCommandBufferWriteAddressFromCounterPair_04_00(Apr::CommandBufferObject*, volatile std::uint64_t*, std::uint8_t, std::uint64_t);
int APS5_VABI sceAmprCommandBufferWriteKernelEventQueue_04_00(Apr::CommandBufferObject*, std::uint64_t, std::int32_t, std::uint64_t, std::uint64_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeWriteAddressFromTimeCounter_04_00(volatile std::uint64_t*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeWriteAddressFromCounter_04_00(volatile std::uint64_t*, std::uint8_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeWriteAddressFromCounterPair_04_00(volatile std::uint64_t*, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWaitOnCounter_04_00(Apr::CommandBufferObject*, std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t);
int APS5_VABI sceAmprCommandBufferWriteCounter_04_00(Apr::CommandBufferObject*, std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t, std::uint8_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeWaitOnCounter_04_00(std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeWriteCounter_04_00(std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t);
int APS5_VABI sceAmprCommandBufferConstructNop(Apr::CommandBufferObject*, std::int16_t, const void*, std::uint32_t, const std::uint32_t*);
int APS5_VABI sceAmprCommandBufferConstructMarker(Apr::CommandBufferObject*, std::uint32_t, const char*, const std::uint32_t*);
int APS5_VABI sceKernelAprResolveFilepathsToIds(const char**, std::uint32_t, std::uint32_t*, std::uint32_t*);
int APS5_VABI sceAmprAprCommandBufferReadFile(Apr::CommandBufferObject*, std::uint64_t*, std::uint64_t*, std::uint32_t, void*, std::uint64_t, std::uint64_t);
int APS5_VABI sceAmprAprCommandBufferReadFileGather(Apr::CommandBufferObject*, std::uint64_t*, std::uint64_t*, std::uint64_t, std::uint64_t);
int APS5_VABI sceAmprAprCommandBufferReadFileScatter(Apr::CommandBufferObject*, std::uint64_t*, std::uint64_t*, void*, std::uint64_t);
int APS5_VABI sceAmprAprCommandBufferReadFileGatherScatter(Apr::CommandBufferObject*, std::uint64_t*, std::uint64_t*, void*, std::uint64_t, std::uint64_t);
int APS5_VABI sceAmprAprCommandBufferResetGatherScatterState(Apr::CommandBufferObject*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeReadFileGather(std::uint64_t, std::uint64_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeReadFileScatter(void*, std::uint64_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeReadFileGatherScatter(void*, std::uint64_t, std::uint64_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeResetGatherScatterState();
std::size_t APS5_VABI sceKernelGetDirectMemorySize();
int APS5_VABI sceAmprAmmCommandBufferConstructor(Apr::CommandBufferObject*);
int APS5_VABI sceAmprAmmCommandBufferMap(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t);
int APS5_VABI sceAmprAmmCommandBufferMapWithGpuMaskId(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t, std::uint8_t);
int APS5_VABI sceAmprAmmCommandBufferMapDirect(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t);
int APS5_VABI sceAmprAmmCommandBufferMapDirectWithGpuMaskId(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t, std::uint8_t);
int APS5_VABI sceAmprAmmCommandBufferUnmap(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t);
std::int64_t APS5_VABI sceAmprAmmMeasureAmmCommandSizeMap(std::uint64_t, std::uint64_t, std::int32_t, std::int32_t);
std::int64_t APS5_VABI sceAmprAmmMeasureAmmCommandSizeMapWithGpuMaskId(std::uint64_t, std::uint64_t, std::int32_t, std::int32_t, std::uint8_t);
std::int64_t APS5_VABI sceAmprAmmMeasureAmmCommandSizeMapDirect(std::uint64_t, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t);
std::int64_t APS5_VABI sceAmprAmmMeasureAmmCommandSizeMapDirectWithGpuMaskId(std::uint64_t, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t, std::uint8_t);
std::int64_t APS5_VABI sceAmprAmmMeasureAmmCommandSizeUnmap(std::uint64_t, std::uint64_t);
int APS5_VABI sceAmprAmmGiveDirectMemory(std::int64_t, std::int64_t, std::size_t, std::size_t, int, std::int64_t*);
int APS5_VABI sceAmprAmmGetVirtualAddressRanges(std::uint64_t*, std::uint64_t*, std::uint64_t*, std::uint64_t*);
int APS5_VABI sceAmprAmmSubmitCommandBuffer(void*, std::uint32_t, std::uint32_t);
int APS5_VABI sceAmprAmmSubmitCommandBuffer2(void*, std::uint32_t, std::uint32_t, std::uint64_t*, std::uint32_t*);
int APS5_VABI sceAmprAmmSubmitCommandBuffer3(void*, std::uint32_t, std::uint32_t, std::uint32_t*);
int APS5_VABI sceAmprAmmWaitCommandBufferCompletion(std::uint32_t);
int APS5_VABI sceAmprAprCommandBufferMapBegin(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t);
int APS5_VABI sceAmprAprCommandBufferMapDirectBegin(Apr::CommandBufferObject*, std::uint64_t, std::uint64_t, std::uint64_t, std::int32_t, std::int32_t);
int APS5_VABI sceAmprAprCommandBufferMapEnd(Apr::CommandBufferObject*);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeMapBegin(std::uint64_t, std::uint64_t, std::uint32_t, std::uint32_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeMapDirectBegin(std::uint64_t, std::uint64_t, std::uint64_t, std::uint32_t, std::uint32_t);
std::uint64_t APS5_VABI sceAmprMeasureCommandSizeMapEnd();
int APS5_VABI sceAmprCommandBufferWriteCounter_04_00(Apr::CommandBufferObject*, std::uint8_t, std::uint8_t, std::uint64_t, std::uint8_t, std::uint8_t);
}

static void Require(bool value) { if (!value) std::abort(); }

namespace {

constexpr int invalidArgument = static_cast<int>(0x80020016);
constexpr int bufferFull = static_cast<int>(0x8002001C);
constexpr std::uint32_t color = 0xFF8040u;

struct Recorder {
    alignas(8) std::array<std::uint8_t, 4096> memory{};
    Apr::CommandBufferObject buffer{};
    std::uint64_t gatherState = 0;
    std::uint64_t scatterState = 0;

    explicit Recorder(std::uint32_t size = 4096) {
        Require(sceAmprCommandBufferConstructor(&buffer) == 0);
        Require(sceAmprAprCommandBufferConstructor(&buffer, &gatherState, &scatterState) == 0);
        Require(sceAmprCommandBufferSetBuffer(&buffer, memory.data(), size) == 0);
    }

    std::uint32_t Offset() const { return sceAmprCommandBufferGetCurrentOffset(&buffer); }
    std::uint32_t Commands() const { return sceAmprCommandBufferGetNumCommands(&buffer); }
};

void RequireAppended(const Recorder& recorder, std::uint32_t offset, std::uint32_t commands, Apr::Opcode opcode, std::uint64_t measured) {
    Require(recorder.Offset() == offset + measured);
    Require(recorder.Commands() == commands + 1);
    Apr::CommandHeader header;
    std::memcpy(&header, recorder.memory.data() + offset, sizeof(header));
    Require(header.opcode == opcode && header.bytes == measured);
}

void RequireRecorded(const Recorder& recorder, std::uint32_t offset, std::uint32_t commands, Apr::Opcode opcode, std::uint64_t measured, const std::string& text) {
    RequireAppended(recorder, offset, commands, opcode, measured);
    Require(measured >= sizeof(Apr::MarkerCommand) + text.size() + 1);
    Require(std::memcmp(recorder.memory.data() + offset + sizeof(Apr::MarkerCommand), text.c_str(), text.size() + 1) == 0);
}

void TestRecording(const std::string& text) {
    Recorder recorder;
    const char* marker = text.c_str();

    auto offset = recorder.Offset();
    auto commands = recorder.Commands();
    Require(sceAmprCommandBufferPushMarker(&recorder.buffer, marker) == 0);
    RequireRecorded(recorder, offset, commands, Apr::Opcode::PushMarker, sceAmprMeasureCommandSizePushMarker(marker), text);

    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferPushMarkerWithColor(&recorder.buffer, marker, color) == 0);
    RequireRecorded(recorder, offset, commands, Apr::Opcode::PushMarker, sceAmprMeasureCommandSizePushMarkerWithColor(marker, color), text);

    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferSetMarker(&recorder.buffer, marker) == 0);
    RequireRecorded(recorder, offset, commands, Apr::Opcode::SetMarker, sceAmprMeasureCommandSizeSetMarker(marker), text);

    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferSetMarkerWithColor(&recorder.buffer, marker, &color) == 0);
    RequireRecorded(recorder, offset, commands, Apr::Opcode::SetMarker, sceAmprMeasureCommandSizeSetMarkerWithColor(marker, color), text);

    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferPopMarker(&recorder.buffer) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::PopMarker, sceAmprMeasureCommandSizePopMarker());
}

void TestRejectedArguments() {
    Recorder recorder;
    Require(sceAmprCommandBufferPushMarker(&recorder.buffer, nullptr) == invalidArgument);
    Require(sceAmprCommandBufferPushMarkerWithColor(&recorder.buffer, nullptr, color) == invalidArgument);
    Require(sceAmprCommandBufferSetMarker(&recorder.buffer, nullptr) == invalidArgument);
    Require(sceAmprCommandBufferSetMarkerWithColor(&recorder.buffer, nullptr, &color) == invalidArgument);
    Require(sceAmprCommandBufferSetMarkerWithColor(&recorder.buffer, "frame", nullptr) == invalidArgument);
    Require(sceAmprCommandBufferPushMarker(nullptr, "frame") == invalidArgument);
    Require(sceAmprCommandBufferPushMarkerWithColor(nullptr, "frame", color) == invalidArgument);
    Require(sceAmprCommandBufferSetMarker(nullptr, "frame") == invalidArgument);
    Require(sceAmprCommandBufferSetMarkerWithColor(nullptr, "frame", &color) == invalidArgument);
    Require(sceAmprCommandBufferPopMarker(nullptr) == invalidArgument);
    Require(recorder.Offset() == 0 && recorder.Commands() == 0);

    const auto rejected = static_cast<std::uint64_t>(static_cast<std::uint32_t>(invalidArgument));
    Require(sceAmprMeasureCommandSizePushMarker(nullptr) == rejected);
    Require(sceAmprMeasureCommandSizePushMarkerWithColor(nullptr, color) == rejected);
    Require(sceAmprMeasureCommandSizeSetMarker(nullptr) == rejected);
    Require(sceAmprMeasureCommandSizeSetMarkerWithColor(nullptr, color) == rejected);
}

void TestFullBuffer() {
    const char* marker = "streaming";
    const auto measured = static_cast<std::uint32_t>(sceAmprMeasureCommandSizePushMarker(marker));
    Recorder exact(measured);
    Require(sceAmprCommandBufferPushMarker(&exact.buffer, marker) == 0);
    Require(exact.Offset() == measured && exact.Commands() == 1);
    Require(sceAmprCommandBufferPopMarker(&exact.buffer) == bufferFull);
    Require(sceAmprCommandBufferSetMarker(&exact.buffer, "") == bufferFull);
    Require(exact.Offset() == measured && exact.Commands() == 1);

    Recorder small(measured - 4);
    Require(sceAmprCommandBufferPushMarker(&small.buffer, marker) == bufferFull);
    Require(sceAmprCommandBufferSetMarkerWithColor(&small.buffer, marker, &color) == bufferFull);
    Require(small.Offset() == 0 && small.Commands() == 0);

    Apr::CommandBufferObject unbound{};
    Require(sceAmprCommandBufferConstructor(&unbound) == 0);
    Require(sceAmprCommandBufferPushMarker(&unbound, marker) == bufferFull);
    Require(sceAmprCommandBufferPopMarker(&unbound) == bufferFull);
}

void TestSubmission() {
    Recorder recorder;
    std::uint64_t first = 0;
    std::uint64_t second = 0;
    Require(sceAmprCommandBufferPushMarker(&recorder.buffer, "level") == 0);
    Require(sceAmprCommandBufferWriteAddressOnCompletion(&recorder.buffer, &first, 0x1111) == 0);
    Require(sceAmprCommandBufferSetMarkerWithColor(&recorder.buffer, "textures", &color) == 0);
    Require(sceAmprCommandBufferPushMarkerWithColor(&recorder.buffer, std::string(200, 'm').c_str(), color) == 0);
    Require(sceAmprCommandBufferSetMarker(&recorder.buffer, "") == 0);
    Require(sceAmprCommandBufferPopMarker(&recorder.buffer) == 0);
    Require(sceAmprCommandBufferPopMarker(&recorder.buffer) == 0);
    Require(sceAmprCommandBufferWriteAddressOnCompletion(&recorder.buffer, &second, 0x2222) == 0);
    Require(recorder.Commands() == 8);
    Require(sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0) == 0);
    Require(first == 0x1111 && second == 0x2222);
}

void TestWaits() {
    Recorder recorder;
    alignas(8) std::uint64_t value = 5;
    alignas(8) std::uint64_t done = 0;
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, &value, 5, 0, 0) == 0);
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, &value, 3, 1, 0) == 0);
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, &value, 9, 2, 1) == 0);
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, &value, 4, 3, 0) == 0);
    Require(sceAmprCommandBufferWriteAddressOnCompletion(&recorder.buffer, &done, 1) == 0);
    auto submitted = std::async(std::launch::async, [&]() { return sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0); });
    Require(submitted.wait_for(std::chrono::seconds(10)) == std::future_status::ready);
    Require(submitted.get() == 0 && done == 1);
}

void TestCounters() {
    Recorder recorder;
    alignas(8) std::uint64_t single = 0;
    alignas(8) std::uint64_t pair = 0;
    Require(sceAmprCommandBufferWriteCounterOnCompletion(&recorder.buffer, 6, 7) == 0);
    Require(sceAmprCommandBufferWriteCounterOnCompletion(&recorder.buffer, 7, 9) == 0);
    Require(sceAmprCommandBufferWaitOnCounter(&recorder.buffer, 6, 7, 0, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter(&recorder.buffer, 7, 8, 1, 1) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&recorder.buffer, &single, 6) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterPairOnCompletion(&recorder.buffer, &pair, 6) == 0);
    Require(sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0) == 0);
    Require(single == 7 && pair == (7ull | (9ull << 32u)));
}

void TestRejectedWaitsAndCounters() {
    Recorder recorder;
    alignas(8) std::uint64_t words[2] = {};
    auto* misaligned = reinterpret_cast<volatile std::uint64_t*>(reinterpret_cast<std::uint8_t*>(words) + 4);
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, &words[0], 0, 4, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, &words[0], 0, 0, 2) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnAddress(&recorder.buffer, misaligned, 0, 0, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnCounter(&recorder.buffer, 128, 0, 0, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnCounter(&recorder.buffer, 0, 0, 4, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnCounter(&recorder.buffer, 0, 0, 0, 2) == invalidArgument);
    Require(sceAmprCommandBufferWriteCounterOnCompletion(&recorder.buffer, 128, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&recorder.buffer, &words[0], 128) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromCounterPairOnCompletion(&recorder.buffer, &words[0], 128) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromCounterPairOnCompletion(&recorder.buffer, &words[0], 7) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&recorder.buffer, nullptr, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&recorder.buffer, misaligned, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromTimeCounterOnCompletion(&recorder.buffer, nullptr) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressOnCompletion(&recorder.buffer, misaligned, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteKernelEventQueueOnCompletion(&recorder.buffer, 0, 1, 0) == invalidArgument);
    Require(recorder.Offset() == 0 && recorder.Commands() == 0);
}

void TestNops() {
    Recorder recorder;
    const std::uint32_t data[3] = {0x11111111u, 0x22222222u, 0x33333333u};
    for (std::uint32_t dwords = 1; dwords <= 16; ++dwords) {
        const auto offset = recorder.Offset();
        const auto commands = recorder.Commands();
        Require(sceAmprCommandBufferNop(&recorder.buffer, dwords) == 0);
        RequireAppended(recorder, offset, commands, Apr::Opcode::Nop, sceAmprMeasureCommandSizeNop(dwords));
    }
    const auto offset = recorder.Offset();
    const auto commands = recorder.Commands();
    Require(sceAmprCommandBufferNopWithData(&recorder.buffer, 3, data) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::Nop, sceAmprMeasureCommandSizeNopWithData(4));
    Require(std::memcmp(recorder.memory.data() + offset + sizeof(Apr::CommandHeader), data, sizeof(data)) == 0);
    Require(sceAmprCommandBufferNopWithData(&recorder.buffer, 0, nullptr) == 0);
    Require(sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0) == 0);

    const auto rejected = static_cast<std::uint64_t>(static_cast<std::uint32_t>(invalidArgument));
    Require(sceAmprCommandBufferNop(&recorder.buffer, 0) == invalidArgument);
    Require(sceAmprCommandBufferNop(&recorder.buffer, 17) == invalidArgument);
    Require(sceAmprCommandBufferNopWithData(&recorder.buffer, 16, data) == invalidArgument);
    Require(sceAmprMeasureCommandSizeNop(0) == rejected && sceAmprMeasureCommandSizeNop(17) == rejected);
    Require(sceAmprMeasureCommandSizeNopWithData(0) == rejected && sceAmprMeasureCommandSizeNopWithData(17) == rejected);
}

void TestVersionedCommands() {
    Recorder recorder;
    alignas(8) std::uint64_t value = 0x8000000000000005ull;
    alignas(8) std::uint64_t single = 0;
    alignas(8) std::uint64_t pair = 0;
    alignas(8) std::uint64_t time = 0;
    Require(sceAmprCommandBufferWaitOnAddress_04_00(&recorder.buffer, &value, 0x8000000000000003ull, 4, 0) == 0);
    Require(sceAmprCommandBufferWaitOnAddress_04_00(&recorder.buffer, &value, 1, 6, 1) == 0);
    Require(sceAmprCommandBufferWaitOnAddress_04_00(&recorder.buffer, &value, 0x8000000000000000ull, 5, 0) == 0);
    Require(sceAmprCommandBufferWriteCounterOnCompletion(&recorder.buffer, 10, 3) == 0);
    Require(sceAmprCommandBufferWriteCounterOnCompletion(&recorder.buffer, 11, 4) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounter_04_00(&recorder.buffer, &single, 10, 1) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterPair_04_00(&recorder.buffer, &pair, 10, 0) == 0);
    Require(sceAmprCommandBufferWriteAddressFromTimeCounter_04_00(&recorder.buffer, &time, 1) == 0);
    auto submitted = std::async(std::launch::async, [&]() { return sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0); });
    Require(submitted.wait_for(std::chrono::seconds(10)) == std::future_status::ready);
    Require(submitted.get() == 0 && single == 3 && pair == (3ull | (4ull << 32u)) && time != 0);

    const auto rejected = static_cast<std::uint64_t>(static_cast<std::uint32_t>(invalidArgument));
    Recorder empty;
    Require(sceAmprCommandBufferWaitOnAddress_04_00(&empty.buffer, nullptr, 0, 0, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnAddress_04_00(&empty.buffer, &value, 0, 7, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnAddress_04_00(&empty.buffer, &value, 0, 0, 2) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromCounterPair_04_00(&empty.buffer, &pair, 11, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteAddressFromTimeCounter_04_00(&empty.buffer, nullptr, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteKernelEventQueue_04_00(&empty.buffer, 0, 1, 0, 0) == invalidArgument);
    Require(empty.Offset() == 0 && empty.Commands() == 0);
    Require(sceAmprMeasureCommandSizeWaitOnAddress_04_00(nullptr, 0, 0, 0) == rejected);
    Require(sceAmprMeasureCommandSizeWaitOnAddress_04_00(&value, 0, 7, 0) == rejected);
    Require(sceAmprMeasureCommandSizeWriteAddressFromTimeCounter_04_00(nullptr) == rejected);
    Require(sceAmprMeasureCommandSizeWriteAddressFromCounter_04_00(&single, 128) == sizeof(Apr::WriteAddressFromCounterCommand));
    Require(sceAmprMeasureCommandSizeWriteAddressFromCounter_04_00(nullptr, 0) == rejected);
    Require(sceAmprMeasureCommandSizeWriteAddressFromCounterPair_04_00(&pair, 128) == sizeof(Apr::WriteAddressFromCounterCommand));
    Require(sceAmprMeasureCommandSizeWriteAddressFromCounterPair_04_00(&pair, 11) == rejected);
    Require(sceAmprMeasureCommandSizeWaitOnAddress_04_00(&value, 0, 6, 1) == sizeof(Apr::WaitCommand));
    Require(sceAmprMeasureCommandSizeWriteAddressFromCounterPair_04_00(&pair, 10) == sizeof(Apr::WriteAddressFromCounterCommand));
}

void SubmitWithin10Seconds(const Recorder& recorder) {
    auto submitted = std::async(std::launch::async, [&]() { return sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0); });
    Require(submitted.wait_for(std::chrono::seconds(10)) == std::future_status::ready);
    Require(submitted.get() == 0);
}

void TestVersionedCounters() {
    enum : std::uint8_t { size8, size4, size2Offset0, size2Offset1, size1Offset0, size1Offset1, size1Offset2, size1Offset3 };
    enum : std::uint8_t { store, atomicOr, atomicAndComplement, atomicXor, atomicAdd };
    Recorder recorder;
    alignas(8) std::uint64_t fields = 0;
    alignas(8) std::uint64_t wide = 0;
    alignas(8) std::uint64_t bits = 0;
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 20, size4, 0x11223344u, store, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 20, size1Offset2, 0x1AAu, store, 1) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 20, size2Offset0, 0xFFFFu, atomicAdd, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 22, size8, 0x0000000500000001ull, store, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 22, size8, 0xFFFFFFFFu, atomicAdd, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 24, size4, 0xF0u, store, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 24, size4, 0x0Fu, atomicOr, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 24, size4, 0x3Cu, atomicAndComplement, 0) == 0);
    Require(sceAmprCommandBufferWriteCounter_04_00(&recorder.buffer, 24, size4, 0xFFu, atomicXor, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 20, size1Offset3, 0x11u, 0, 0, 0, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 20, size1Offset2, 1u, 6, 0, 0, 1) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 20, size2Offset0, 0xF000u, 4, 0, 0, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 20, size2Offset1, 0x11ABu, 2, 0, 0, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 22, size8, 0x0000000600000000ull, 0, 0, 0, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 24, size4, 0xFCu, 0, 1, 0x0Fu, 0) == 0);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&recorder.buffer, 24, size1Offset0, 0x3Cu, 0, 0, 0x0Fu, 0) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&recorder.buffer, &fields, 20) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterPairOnCompletion(&recorder.buffer, &wide, 22) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&recorder.buffer, &bits, 24) == 0);
    Require(recorder.Commands() == 19);
    SubmitWithin10Seconds(recorder);
    Require(fields == 0x11AA3343u && wide == 0x0000000600000000ull && bits == 0x3Cu);

    const auto rejected = static_cast<std::uint64_t>(static_cast<std::uint32_t>(invalidArgument));
    Recorder empty;
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&empty.buffer, 0, 8, 0, 0, 0, 0, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&empty.buffer, 0, size4, 0, 7, 0, 0, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&empty.buffer, 0, size4, 0, 0, 2, 0, 0) == invalidArgument);
    Require(sceAmprCommandBufferWaitOnCounter_04_00(&empty.buffer, 0, size4, 0, 0, 0, 0, 2) == invalidArgument);
    Require(sceAmprCommandBufferWriteCounter_04_00(&empty.buffer, 128, size4, 0, store, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteCounter_04_00(&empty.buffer, 0, 8, 0, store, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteCounter_04_00(&empty.buffer, 0, size4, 0, 5, 0) == invalidArgument);
    Require(sceAmprCommandBufferWriteCounter_04_00(nullptr, 0, size4, 0, store, 0) == invalidArgument);
    Require(empty.Offset() == 0 && empty.Commands() == 0);
    Require(sceAmprMeasureCommandSizeWaitOnCounter_04_00(200, size1Offset3, 0, 6, 1, 0, 1) == sizeof(Apr::WaitCommand));
    Require(sceAmprMeasureCommandSizeWaitOnCounter_04_00(0, 8, 0, 0, 0, 0, 0) == rejected);
    Require(sceAmprMeasureCommandSizeWaitOnCounter_04_00(0, size4, 0, 7, 0, 0, 0) == rejected);
    Require(sceAmprMeasureCommandSizeWaitOnCounter_04_00(0, size4, 0, 0, 2, 0, 0) == rejected);
    Require(sceAmprMeasureCommandSizeWaitOnCounter_04_00(0, size4, 0, 0, 0, 0, 2) == rejected);
    Require(sceAmprMeasureCommandSizeWriteCounter_04_00(127, size8, 0, atomicAdd) == sizeof(Apr::WriteCounterCommand));
    Require(sceAmprMeasureCommandSizeWriteCounter_04_00(128, size4, 0, store) == rejected);
    Require(sceAmprMeasureCommandSizeWriteCounter_04_00(0, 8, 0, store) == rejected);
    Require(sceAmprMeasureCommandSizeWriteCounter_04_00(0, size4, 0, 5) == rejected);
}

void TestConstructed() {
    Recorder recorder;
    const std::uint8_t payload[5] = {1, 2, 3, 4, 5};
    const std::uint32_t word = 0xCAFEF00Du;
    auto offset = recorder.Offset();
    auto commands = recorder.Commands();
    Require(sceAmprCommandBufferConstructNop(&recorder.buffer, 7, payload, sizeof(payload), &word) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::Nop, sceAmprMeasureCommandSizeNopWithData(4));
    const std::uint8_t* data = recorder.memory.data() + offset + sizeof(Apr::CommandHeader);
    const std::uint8_t padding[3] = {};
    Require(std::memcmp(data, &word, sizeof(word)) == 0 && std::memcmp(data + 4, payload, sizeof(payload)) == 0 && std::memcmp(data + 9, padding, sizeof(padding)) == 0);

    const std::array<std::uint8_t, 60> large{};
    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferConstructNop(&recorder.buffer, 0, large.data(), 60, nullptr) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::Nop, sceAmprMeasureCommandSizeNopWithData(16));
    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferConstructNop(&recorder.buffer, 0, nullptr, 0, nullptr) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::Nop, sceAmprMeasureCommandSizeNopWithData(1));

    const std::pair<std::uint32_t, Apr::Opcode> markers[] = {{1, Apr::Opcode::SetMarker}, {2, Apr::Opcode::PushMarker}, {5, Apr::Opcode::SetMarker}, {6, Apr::Opcode::PushMarker}};
    for (const auto& [type, opcode] : markers) {
        offset = recorder.Offset();
        commands = recorder.Commands();
        Require(sceAmprCommandBufferConstructMarker(&recorder.buffer, type, "stream", &color) == 0);
        RequireRecorded(recorder, offset, commands, opcode, sceAmprMeasureCommandSizeSetMarker("stream"), "stream");
    }
    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprCommandBufferConstructMarker(&recorder.buffer, 3, nullptr, nullptr) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::PopMarker, sceAmprMeasureCommandSizePopMarker());
    Require(sceKernelAprSubmitCommandBuffer(&recorder.buffer, 0) == 0);

    Recorder empty;
    Require(sceAmprCommandBufferConstructNop(&empty.buffer, 0, large.data(), 61, nullptr) == invalidArgument);
    Require(sceAmprCommandBufferConstructNop(&empty.buffer, 0, large.data(), 57, &word) == invalidArgument);
    Require(sceAmprCommandBufferConstructNop(nullptr, 0, large.data(), 4, nullptr) == invalidArgument);
    Require(sceAmprCommandBufferConstructMarker(&empty.buffer, 5, "stream", nullptr) == invalidArgument);
    Require(sceAmprCommandBufferConstructMarker(&empty.buffer, 6, "stream", nullptr) == invalidArgument);
    Require(sceAmprCommandBufferConstructMarker(&empty.buffer, 1, nullptr, nullptr) == invalidArgument);
    Require(sceAmprCommandBufferConstructMarker(&empty.buffer, 0, "stream", &color) == invalidArgument);
    Require(sceAmprCommandBufferConstructMarker(&empty.buffer, 4, "stream", &color) == invalidArgument);
    Require(empty.Offset() == 0 && empty.Commands() == 0);
}

std::uint8_t FileByte(std::size_t offset) {
    return static_cast<std::uint8_t>(offset * 7u + 3u);
}

bool MatchesFile(const std::uint8_t* data, std::size_t offset, std::size_t bytes) {
    for (std::size_t index = 0; index < bytes; ++index) {
        if (data[index] != FileByte(offset + index)) return false;
    }
    return true;
}

void TestGatherScatter() {
    const char* path = "ampr_gather_scatter.bin";
    {
        std::ofstream file(path, std::ios::binary);
        for (std::size_t offset = 0; offset < 4096; ++offset) file.put(static_cast<char>(FileByte(offset)));
    }
    std::uint32_t fileId = 0;
    std::uint32_t failed = 0;
    Require(sceKernelAprResolveFilepathsToIds(&path, 1, &fileId, &failed) == 0);

    Recorder recorder;
    auto* buffer = &recorder.buffer;
    auto* map = &recorder.gatherState;
    auto* state = &recorder.scatterState;
    std::array<std::uint8_t, 64> first{};
    std::array<std::uint8_t, 64> second{};
    Require(sceAmprAprCommandBufferReadFileGather(buffer, map, state, 8, 0) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFileScatter(buffer, map, state, second.data(), 8) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFileGatherScatter(buffer, map, state, second.data(), 8, 0) == invalidArgument);
    Require(recorder.Commands() == 0);

    Require(sceAmprAprCommandBufferReadFile(buffer, map, state, fileId, first.data(), 16, 100) == 0);
    auto offset = recorder.Offset();
    auto commands = recorder.Commands();
    Require(sceAmprAprCommandBufferReadFileGather(buffer, map, state, 8, 500) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::ReadFileGather, sceAmprMeasureCommandSizeReadFileGather(8, 500));
    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprAprCommandBufferReadFileScatter(buffer, map, state, second.data(), 8) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::ReadFileScatter, sceAmprMeasureCommandSizeReadFileScatter(second.data(), 8));
    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprAprCommandBufferReadFileGatherScatter(buffer, map, state, second.data() + 32, 4, 1000) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::ReadFileGatherScatter, sceAmprMeasureCommandSizeReadFileGatherScatter(second.data() + 32, 4, 1000));
    Require(sceAmprAprCommandBufferReadFileGather(buffer, map, state, 4, 2000) == 0);
    Require(sceAmprAprCommandBufferReadFileScatter(buffer, map, state, first.data() + 40, 6) == 0);
    offset = recorder.Offset();
    commands = recorder.Commands();
    Require(sceAmprAprCommandBufferResetGatherScatterState(buffer) == 0);
    RequireAppended(recorder, offset, commands, Apr::Opcode::ResetGatherScatterState, sceAmprMeasureCommandSizeResetGatherScatterState());
    Require(sceAmprAprCommandBufferReadFileGather(buffer, map, state, 8, 0) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFileScatter(buffer, map, state, second.data(), 8) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFile(buffer, map, state, fileId, second.data() + 48, 4, 3000) == 0);
    Require(sceAmprAprCommandBufferReadFileScatter(buffer, map, state, second.data() + 56, 4) == 0);
    Require(recorder.Commands() == 9);
    SubmitWithin10Seconds(recorder);

    Require(MatchesFile(first.data(), 100, 16) && MatchesFile(first.data() + 16, 500, 8));
    Require(MatchesFile(second.data(), 508, 8));
    Require(MatchesFile(second.data() + 32, 1000, 4) && MatchesFile(second.data() + 36, 2000, 4));
    Require(MatchesFile(first.data() + 40, 2004, 6));
    Require(MatchesFile(second.data() + 48, 3000, 4) && MatchesFile(second.data() + 56, 3004, 4));
    Require(first[24] == 0 && first[39] == 0 && second[8] == 0 && second[31] == 0 && second[40] == 0);
    std::remove(path);

    const auto rejected = static_cast<std::uint64_t>(static_cast<std::uint32_t>(invalidArgument));
    auto* high = reinterpret_cast<void*>(std::uintptr_t{0xF00000000000ull});
    Recorder empty;
    Require(sceAmprAprCommandBufferReadFile(&empty.buffer, map, state, fileId, first.data(), 0, 0) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFile(&empty.buffer, map, state, fileId, first.data(), 0x100000001ull, 0) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFile(&empty.buffer, map, state, fileId, first.data(), 4, 0x10000000000ull) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFile(&empty.buffer, map, state, fileId, high, 4, 0) == invalidArgument);
    Require(sceAmprAprCommandBufferReadFile(nullptr, map, state, fileId, first.data(), 4, 0) == invalidArgument);
    Require(empty.Offset() == 0 && empty.Commands() == 0);
    Require(sceAmprMeasureCommandSizeReadFileGather(0, 0) == rejected);
    Require(sceAmprMeasureCommandSizeReadFileGather(0x100000000ull, 0x10000000000ull - 1u) == sizeof(Apr::ReadFileCommand));
    Require(sceAmprMeasureCommandSizeReadFileGather(4, 0x10000000000ull) == rejected);
    Require(sceAmprMeasureCommandSizeReadFileScatter(first.data(), 0x100000001ull) == rejected);
    Require(sceAmprMeasureCommandSizeReadFileScatter(high, 4) == rejected);
    Require(sceAmprMeasureCommandSizeReadFileGatherScatter(first.data(), 4, 0x10000000000ull) == rejected);
    Require(sceAmprMeasureCommandSizeReadFileGatherScatter(nullptr, 4, 0) == sizeof(Apr::ReadFileCommand));
}

constexpr std::uint64_t page = 0x4000;
constexpr int permissionDenied = static_cast<int>(0x80020001);
constexpr int busy = static_cast<int>(0x80020010);
constexpr int noSuchSubmission = static_cast<int>(0x80020003);
constexpr std::int32_t cpuReadWrite = 0x03;
constexpr std::int32_t cpuGpuReadWrite = 0x33;
constexpr std::int32_t amprReadWrite = 0xC0;

std::uint64_t& At(std::uint64_t address) {
    return *reinterpret_cast<std::uint64_t*>(address);
}

void TestAmm() {
    std::uint64_t start = 0;
    std::uint64_t end = 0;
    std::uint64_t multimapStart = 0;
    std::uint64_t multimapEnd = 0;
    Require(sceAmprAmmGetVirtualAddressRanges(&start, &end, &multimapStart, &multimapEnd) == 0);
    Require(start != 0 && start % page == 0 && start < end && end <= multimapStart && multimapStart < multimapEnd);

    const auto directMemory = static_cast<std::int64_t>(sceKernelGetDirectMemorySize());
    std::int64_t pool = -1;
    std::int64_t direct = -1;
    Require(sceAmprAmmGiveDirectMemory(0, directMemory, 7 * page, page, 1, &pool) == 0 && pool >= 0);
    Require(sceAmprAmmGiveDirectMemory(0, directMemory, 2 * page, page, 0, &direct) == 0 && direct >= 0);
    Require(sceAmprAmmGiveDirectMemory(0, directMemory, page, page, 2, &direct) == invalidArgument);
    Require(sceAmprAmmGiveDirectMemory(0, directMemory, page, page, 1, nullptr) == invalidArgument);

    const std::uint64_t automatic = start;
    const std::uint64_t first = start + 0x10000;
    const std::uint64_t alias = start + 0x20000;
    const std::uint64_t reused = start + 0x30000;
    const std::uint64_t region = start + 0x60000;
    const std::uint64_t directRegion = start + 0x70000;

    Recorder maps;
    Require(sceAmprAmmCommandBufferConstructor(&maps.buffer) == 0);
    auto offset = maps.Offset();
    auto commands = maps.Commands();
    Require(sceAmprAmmCommandBufferMap(&maps.buffer, automatic, 2 * page, 0, cpuReadWrite) == 0);
    RequireAppended(maps, offset, commands, Apr::Opcode::AmmMap, sceAmprAmmMeasureAmmCommandSizeMap(automatic, 2 * page, 0, cpuReadWrite));
    offset = maps.Offset();
    commands = maps.Commands();
    Require(sceAmprAmmCommandBufferMapDirect(&maps.buffer, first, direct, 2 * page, 0, cpuGpuReadWrite) == 0);
    RequireAppended(maps, offset, commands, Apr::Opcode::AmmMapDirect, sceAmprAmmMeasureAmmCommandSizeMapDirect(first, direct, 2 * page, 0, cpuGpuReadWrite));
    Require(sceAmprAmmCommandBufferMapDirectWithGpuMaskId(&maps.buffer, alias, direct, 2 * page, 0, amprReadWrite, 3) == 0);
    std::uint32_t id = 0;
    Require(sceAmprAmmSubmitCommandBuffer3(maps.memory.data(), maps.Offset(), 0, &id) == 0 && id != 0);
    Require(sceAmprAmmWaitCommandBufferCompletion(id) == 0);
    Require(sceAmprAmmWaitCommandBufferCompletion(id + 1000) == noSuchSubmission);
    At(automatic) = 0x1111;
    At(automatic + 2 * page - 8) = 0x2222;
    Require(At(automatic) == 0x1111 && At(automatic + 2 * page - 8) == 0x2222);
    At(first + page) = 0x3333;
    Require(At(alias + page) == 0x3333);
    At(alias) = 0x4444;
    Require(At(first) == 0x4444);

    Recorder remaps;
    Require(sceAmprAmmCommandBufferConstructor(&remaps.buffer) == 0);
    offset = remaps.Offset();
    commands = remaps.Commands();
    Require(sceAmprAmmCommandBufferUnmap(&remaps.buffer, automatic, 2 * page) == 0);
    RequireAppended(remaps, offset, commands, Apr::Opcode::AmmUnmap, sceAmprAmmMeasureAmmCommandSizeUnmap(automatic, 2 * page));
    Require(sceAmprAmmCommandBufferMapWithGpuMaskId(&remaps.buffer, reused, 6 * page, 0, cpuReadWrite, 1) == 0);
    std::uint64_t result = ~0ull;
    Require(sceAmprAmmSubmitCommandBuffer2(remaps.memory.data(), remaps.Offset(), 0, &result, &id) == 0 && result == 0);
    Require(sceAmprAmmWaitCommandBufferCompletion(id) == 0);
    At(reused + 6 * page - 8) = 0x5555;
    Require(At(reused + 6 * page - 8) == 0x5555);

    Recorder apr;
    alignas(8) std::uint64_t done = 0;
    offset = apr.Offset();
    commands = apr.Commands();
    Require(sceAmprAprCommandBufferMapBegin(&apr.buffer, region, page, 0, amprReadWrite) == 0);
    RequireAppended(apr, offset, commands, Apr::Opcode::AmmMap, sceAmprMeasureCommandSizeMapBegin(region, page, 0, amprReadWrite));
    Require(sceAmprAprCommandBufferMapBegin(&apr.buffer, region, page, 0, amprReadWrite) == permissionDenied);
    Require(sceAmprAprCommandBufferMapDirectBegin(&apr.buffer, directRegion, direct, page, 0, amprReadWrite) == permissionDenied);
    Require(sceAmprCommandBufferWriteAddressOnCompletion(&apr.buffer, &done, 1) == permissionDenied);
    Require(sceAmprCommandBufferWriteCounterOnCompletion(&apr.buffer, 30, 1) == permissionDenied);
    Require(sceAmprCommandBufferWriteKernelEventQueueOnCompletion(&apr.buffer, 1, 1, 0) == permissionDenied);
    Require(sceAmprCommandBufferWriteAddressFromCounter_04_00(&apr.buffer, &done, 30, 0) == permissionDenied);
    Require(sceAmprCommandBufferWriteCounter_04_00(&apr.buffer, 30, 1, 5, 0, 0) == permissionDenied);
    Require(sceAmprCommandBufferWriteCounter_04_00(&apr.buffer, 30, 1, 5, 0, 1) == 0);
    offset = apr.Offset();
    commands = apr.Commands();
    Require(sceAmprAprCommandBufferMapEnd(&apr.buffer) == 0);
    RequireAppended(apr, offset, commands, Apr::Opcode::MapEnd, sceAmprMeasureCommandSizeMapEnd());
    Require(sceAmprAprCommandBufferMapEnd(&apr.buffer) == permissionDenied);
    Require(sceAmprAprCommandBufferMapDirectBegin(&apr.buffer, directRegion, direct, page, 0, cpuReadWrite) == 0);
    Require(sceAmprAprCommandBufferMapEnd(&apr.buffer) == 0);
    Require(sceAmprCommandBufferWriteAddressFromCounterOnCompletion(&apr.buffer, &done, 30) == 0);
    SubmitWithin10Seconds(apr);
    Require(done == 5);
    At(region + page - 8) = 0x6666;
    Require(At(region + page - 8) == 0x6666);
    Require(At(directRegion) == 0x4444);

    const auto rejected = static_cast<std::uint64_t>(static_cast<std::uint32_t>(invalidArgument));
    const std::int64_t ammRejected = invalidArgument;
    Recorder empty;
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, start, page, 0, 0x04) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, start, page, 0, 0x400) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, start + 0x1000, page, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, start, page + 0x1000, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, start, 0, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, 0, page, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(&empty.buffer, ~(page - 1), 2 * page, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAmmCommandBufferMapDirect(&empty.buffer, start, direct + 0x1000, page, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAmmCommandBufferUnmap(&empty.buffer, start + 8, page) == invalidArgument);
    Require(sceAmprAmmCommandBufferMap(nullptr, start, page, 0, cpuReadWrite) == invalidArgument);
    Require(sceAmprAprCommandBufferMapBegin(&empty.buffer, start, page, 0, 0x04) == invalidArgument);
    Require(sceAmprAprCommandBufferMapEnd(nullptr) == invalidArgument);
    Require(empty.Offset() == 0 && empty.Commands() == 0);

    Apr::CommandBufferObject unbound{};
    Require(sceAmprCommandBufferConstructor(&unbound) == 0);
    Require(sceAmprAmmCommandBufferMap(&unbound, start, page, 0, cpuReadWrite) == permissionDenied);
    Require(sceAmprAmmCommandBufferUnmap(&unbound, start, page) == permissionDenied);
    const auto measured = static_cast<std::uint32_t>(sceAmprAmmMeasureAmmCommandSizeUnmap(start, page));
    Recorder exact(measured);
    Require(sceAmprAmmCommandBufferUnmap(&exact.buffer, start, page) == 0);
    Require(sceAmprAmmCommandBufferUnmap(&exact.buffer, start, page) == busy);
    Require(sceAmprAmmCommandBufferMap(&exact.buffer, start, page, 0, cpuReadWrite) == busy);
    Require(sceAmprAmmSubmitCommandBuffer(nullptr, 0, 0) == permissionDenied);
    Require(sceAmprAmmSubmitCommandBuffer(exact.memory.data(), 0, 3) == invalidArgument);
    Require(sceAmprAmmSubmitCommandBuffer3(nullptr, 0, 3, nullptr) == invalidArgument);
    Require(sceAmprAmmSubmitCommandBuffer(exact.memory.data(), 0, 2) == 0);

    Require(sceAmprAmmMeasureAmmCommandSizeMap(start, page, 0, 0x04) == ammRejected);
    Require(sceAmprAmmMeasureAmmCommandSizeMapWithGpuMaskId(start + 8, page, 0, cpuReadWrite, 0) == ammRejected);
    Require(sceAmprAmmMeasureAmmCommandSizeMapWithGpuMaskId(start, page, 0, cpuReadWrite, 0) == std::int64_t{sizeof(Apr::AmmMapCommand)});
    Require(sceAmprAmmMeasureAmmCommandSizeMapDirect(start, direct + 8, page, 0, cpuReadWrite) == ammRejected);
    Require(sceAmprAmmMeasureAmmCommandSizeMapDirectWithGpuMaskId(start, direct, page, 0, cpuReadWrite, 0) == std::int64_t{sizeof(Apr::AmmMapCommand)});
    Require(sceAmprAmmMeasureAmmCommandSizeUnmap(start, 0) == ammRejected);
    Require(sceAmprMeasureCommandSizeMapBegin(start, page, 0, 0x04) == rejected);
    Require(sceAmprMeasureCommandSizeMapDirectBegin(start, direct + 8, page, 0, cpuReadWrite) == rejected);
    Require(sceAmprMeasureCommandSizeMapDirectBegin(start, direct, page, 0, cpuReadWrite) == sizeof(Apr::AmmMapCommand));
}

}

int main() {
    TestRecording("frame");
    TestRecording("");
    TestRecording("1234567");
    TestRecording("12345678");
    TestRecording(std::string(300, 'a'));
    TestRejectedArguments();
    TestFullBuffer();
    TestSubmission();
    TestWaits();
    TestCounters();
    TestRejectedWaitsAndCounters();
    TestNops();
    TestVersionedCommands();
    TestVersionedCounters();
    TestConstructed();
    TestGatherScatter();
    TestAmm();
    return 0;
}
