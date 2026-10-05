#include "prx/libc/include/General.hpp"
#include "prx/libc/include/HeapDiagnostics.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace {

std::uint64_t mspaceAtomicIdMask = 0;
std::array<std::uint64_t, 64> mstateTable{};

static_assert(sizeof(LibcHeapInfo) == 32);
static_assert(offsetof(LibcHeapInfo, mspace_atomic_id_mask) == 16);
static_assert(offsetof(LibcHeapInfo, mstate_table) == 24);

}

extern "C" {

void LibcHeapTraceInfo_nid_no_patch(LibcHeapInfo* info) {
    if (info == nullptr)
        throw std::invalid_argument("heap trace info: null output");
    if (info->size != sizeof(LibcHeapInfo))
        throw std::invalid_argument("heap trace info: unsupported structure size");
    info->unknown2 = 0;
    info->mspace_atomic_id_mask = &mspaceAtomicIdMask;
    info->mstate_table = mstateTable.data();
}

// unknown signature
void APS5_VABI sceLibcInternalBacktraceForGame_nid_postfix(const char* heapName) {
    (void)heapName;
    NotImplemented_nid_no_patch(__func__);
}

// unknown signature
void APS5_VABI sceLibcInternalHeapErrorReportForGame_nid_postfix(void* heap, void* block, std::uint32_t error) {
    (void)heap;
    (void)block;
    (void)error;
    NotImplemented_nid_no_patch(__func__);
}

APS5_EXPORT("BnMAMrsfVWo", sceLibcUnknown_BnMAMrsfVWo);
int32_t APS5_VABI sceLibcUnknown_BnMAMrsfVWo(void) {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

APS5_EXPORT("OQ-dzhlnM28", sceLibcInternalExtUnknown_OQ_dzhlnM28);
int APS5_VABI sceLibcInternalExtUnknown_OQ_dzhlnM28(void) {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

APS5_EXPORT("qBS714-Jr3g", sceLibcInternalExtUnknown_qBS714_Jr3g);
int APS5_VABI sceLibcInternalExtUnknown_qBS714_Jr3g(void) {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

APS5_EXPORT("tB59hFLH3SA", sceLibcInternalExtUnknown_tB59hFLH3SA);
int APS5_VABI sceLibcInternalExtUnknown_tB59hFLH3SA(void) {
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

}
