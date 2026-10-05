#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"
#include "prx/libkernel/KernelErrors.hpp"
#include "prx/libkernel/Pthread/include/Pthread.hpp"
#include <array>
#include <cstring>
#include <mutex>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif

extern "C" Pthread APS5_VABI scePthreadSelf();

namespace {

struct GuestMcontext {
    std::uint64_t onstack;
    std::uint64_t rdi;
    std::uint64_t rsi;
    std::uint64_t rdx;
    std::uint64_t rcx;
    std::uint64_t r8;
    std::uint64_t r9;
    std::uint64_t rax;
    std::uint64_t rbx;
    std::uint64_t rbp;
    std::uint64_t r10;
    std::uint64_t r11;
    std::uint64_t r12;
    std::uint64_t r13;
    std::uint64_t r14;
    std::uint64_t r15;
    std::uint32_t trapno;
    std::uint16_t fs;
    std::uint16_t gs;
    std::uint64_t addr;
    std::uint32_t flags;
    std::uint16_t es;
    std::uint16_t ds;
    std::uint64_t err;
    std::uint64_t rip;
    std::uint64_t cs;
    std::uint64_t rflags;
    std::uint64_t rsp;
    std::uint64_t ss;
    std::uint64_t len;
    std::uint64_t fpformat;
    std::uint64_t ownedfp;
    std::uint64_t lbrfrom;
    std::uint64_t lbrto;
    std::uint64_t aux1;
    std::uint64_t aux2;
    std::uint64_t fpstate[104];
    std::uint64_t fsbase;
    std::uint64_t gsbase;
    std::uint64_t spare[6];
};

struct GuestUcontext {
    std::uint32_t sigmask[4];
    std::int32_t reserved[12];
    GuestMcontext mcontext;
    GuestUcontext* link;
    void* stackPointer;
    std::uint64_t stackSize;
    std::int32_t stackFlags;
    std::int32_t stackAlign;
    std::int32_t flags;
    std::int32_t spare[4];
    std::int32_t tail[3];
};

static_assert(offsetof(GuestUcontext, mcontext) == 0x40);
static_assert(offsetof(GuestUcontext, mcontext) + offsetof(GuestMcontext, rsp) == 0xf8);

using GuestExceptionHandler = void (APS5_VABI *)(int, void*);

constexpr std::array<int, 6> AllowedSignals{1, 4, 8, 10, 11, 30};

std::mutex handlersLock;
std::array<void*, 32> handlers{};

bool Allowed(int signum) {
    for (const int allowed : AllowedSignals)
        if (allowed == signum) return true;
    return false;
}

GuestExceptionHandler Handler(int signum) {
    std::lock_guard lock(handlersLock);
    return reinterpret_cast<GuestExceptionHandler>(handlers[signum]);
}

#ifdef _WIN32
constexpr std::size_t RedZone = 128;

struct Delivery {
    GuestExceptionHandler handler;
    int signum;
    CONTEXT context;
};

void Deliver(GuestExceptionHandler handler, int signum, CONTEXT& context) {
    GuestUcontext ucontext{};
    auto& m = ucontext.mcontext;
    m.rdi = context.Rdi;
    m.rsi = context.Rsi;
    m.rdx = context.Rdx;
    m.rcx = context.Rcx;
    m.r8 = context.R8;
    m.r9 = context.R9;
    m.rax = context.Rax;
    m.rbx = context.Rbx;
    m.rbp = context.Rbp;
    m.r10 = context.R10;
    m.r11 = context.R11;
    m.r12 = context.R12;
    m.r13 = context.R13;
    m.r14 = context.R14;
    m.r15 = context.R15;
    m.rip = context.Rip;
    m.rsp = context.Rsp;
    m.rflags = context.EFlags;
    m.cs = context.SegCs;
    m.ss = context.SegSs;
    m.len = sizeof(GuestMcontext);
    static_assert(sizeof(context.FltSave) <= sizeof(m.fpstate));
    std::memcpy(m.fpstate, &context.FltSave, sizeof(context.FltSave));
    handler(signum, &ucontext);
    context.Rdi = m.rdi;
    context.Rsi = m.rsi;
    context.Rdx = m.rdx;
    context.Rcx = m.rcx;
    context.R8 = m.r8;
    context.R9 = m.r9;
    context.Rax = m.rax;
    context.Rbx = m.rbx;
    context.Rbp = m.rbp;
    context.R10 = m.r10;
    context.R11 = m.r11;
    context.R12 = m.r12;
    context.R13 = m.r13;
    context.R14 = m.r14;
    context.R15 = m.r15;
    context.Rip = m.rip;
    context.Rsp = m.rsp;
    context.EFlags = static_cast<DWORD>(m.rflags);
    std::memcpy(&context.FltSave, m.fpstate, sizeof(context.FltSave));
}

[[noreturn]] void RedirectedEntry(Delivery* delivery) {
    CONTEXT context = delivery->context;
    const auto handler = delivery->handler;
    const int signum = delivery->signum;
    delete delivery;
    Deliver(handler, signum, context);
    RtlRestoreContext(&context, nullptr);
    std::abort();
}

void CALLBACK WaitingEntry(ULONG_PTR parameter) {
    auto* delivery = reinterpret_cast<Delivery*>(parameter);
    const auto handler = delivery->handler;
    const int signum = delivery->signum;
    delete delivery;
    CONTEXT context{};
    RtlCaptureContext(&context);
    Deliver(handler, signum, context);
}

void RaiseOn(Pthread thread, GuestExceptionHandler handler, int signum) {
    if (thread == scePthreadSelf()) {
        CONTEXT context{};
        RtlCaptureContext(&context);
        Deliver(handler, signum, context);
        return;
    }
    const auto native = static_cast<HANDLE>(thread->nativeHandle);
    if (SuspendThread(native) == static_cast<DWORD>(-1)) throw std::runtime_error("sceKernelRaiseException: cannot suspend the target thread");
    if (thread->waitState != nullptr && thread->waitState->load(std::memory_order_seq_cst) > 0) {
        auto* delivery = new Delivery{handler, signum, {}};
        const bool queued = QueueUserAPC(WaitingEntry, native, reinterpret_cast<ULONG_PTR>(delivery)) != 0;
        ResumeThread(native);
        if (!queued) {
            delete delivery;
            throw std::runtime_error("sceKernelRaiseException: cannot queue delivery to the waiting thread");
        }
        return;
    }
    auto* delivery = new Delivery{handler, signum, {}};
    delivery->context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT;
    if (!GetThreadContext(native, &delivery->context)) {
        ResumeThread(native);
        delete delivery;
        throw std::runtime_error("sceKernelRaiseException: cannot read the target thread context");
    }
    CONTEXT redirected = delivery->context;
    redirected.Rsp = ((delivery->context.Rsp - RedZone) & ~static_cast<DWORD64>(15)) - 8;
    redirected.Rip = reinterpret_cast<DWORD64>(&RedirectedEntry);
    redirected.Rcx = reinterpret_cast<DWORD64>(delivery);
    if (!SetThreadContext(native, &redirected)) {
        ResumeThread(native);
        delete delivery;
        throw std::runtime_error("sceKernelRaiseException: cannot redirect the target thread");
    }
    ResumeThread(native);
}
#endif

}

extern "C" {

int APS5_VABI sceKernelInstallExceptionHandler(int signum, void* handler) {
 if (!Allowed(signum) || handler == nullptr) return SCE_KERNEL_ERROR_EINVAL;
 std::lock_guard lock(handlersLock);
 if (handlers[signum] != nullptr) return SCE_KERNEL_ERROR_EAGAIN;
 handlers[signum] = handler;
 return 0;
}

int APS5_VABI sceKernelRemoveExceptionHandler(int signum) {
 if (!Allowed(signum)) return SCE_KERNEL_ERROR_EINVAL;
 std::lock_guard lock(handlersLock);
 handlers[signum] = nullptr;
 return 0;
}

int APS5_VABI sceKernelRaiseException(Pthread thread, int signum) {
 if (signum != 30) return SCE_KERNEL_ERROR_EINVAL;
 if (thread == nullptr) return SCE_KERNEL_ERROR_ESRCH;
 const auto handler = Handler(signum);
 if (handler == nullptr) throw std::runtime_error("sceKernelRaiseException: no handler installed for the signal");
#ifdef _WIN32
 RaiseOn(thread, handler, signum);
 return 0;
#else
 NotImplemented_nid_no_patch(__func__);
 return 0;
#endif
}

void APS5_VABI sceKernelDebugRaiseException(int c1, int c2) {
  APS5_LOG_OUT("sceKernelDebugRaiseException c1=%d c2=%d", c1, c2);
}

void APS5_VABI sceKernelDebugRaiseExceptionOnReleaseMode(int c1, int c2) {
  APS5_LOG_OUT("sceKernelDebugRaiseExceptionOnReleaseMode c1=%d c2=%d", c1, c2);
}

}
