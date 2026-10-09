#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libc/include/Shutdown.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
extern "C" int APS5_VABI sceSystemServiceGetAppIdOfRunningBigApp(void);
extern "C" int APS5_VABI sceSystemServiceKillApp(int, int, int, int);
namespace {
bool cleaned = false;
bool finished = false;
void Cleanup() { cleaned = true; }
void VerifyExit() {
    if (!cleaned) std::_Exit(1);
    std::puts("Guest shutdown and atexit completed");
}
void UnexpectedShutdown() {
    if (!finished) std::_Exit(3);
}
void Require(bool value) { if (!value) std::abort(); }
bool Rejects(int appId, int how, int reason, int coreDump) {
    try { sceSystemServiceKillApp(appId, how, reason, coreDump); }
    catch (const std::runtime_error&) { return true; }
    return false;
}
}
int main(int argc, char** argv) {
    const int appId = sceSystemServiceGetAppIdOfRunningBigApp();
    Require(appId > 0 && sceSystemServiceGetAppIdOfRunningBigApp() == appId);
    if (argc > 1 && std::strcmp(argv[1], "--kill") == 0) {
        LibcRegisterShutdown_nid_postfix(Cleanup);
        Require(std::atexit(VerifyExit) == 0);
        sceSystemServiceKillApp(appId, -1, 0, 0);
        return 2;
    }
    LibcRegisterShutdown_nid_postfix(UnexpectedShutdown);
    Require(std::atexit(UnexpectedShutdown) == 0);
    Require(Rejects(appId + 1, -1, 0, 0));
    Require(Rejects(-1, -1, 0, 0));
    Require(Rejects(appId, 0, 0, 0));
    Require(Rejects(appId, -1, 1, 0));
    Require(Rejects(appId, -1, 0, 1));
    finished = true;
}
