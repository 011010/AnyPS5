# Project technical debt

### Build

- Building on Windows requires a specific version of mingw - MinGW-w64 GCC 15.2.0 (`winlibs-gcc15`, `x86_64-ucrt-posix-seh`)
- Even compiled prx libraries on Windows require nearby (static linking of these dependencies causes conflicts):
  - libgcc_s_seh-1.dll
  - libstdc++-6.dll
  - libwinpthread-1.dll

### Silent stubs

Throughout the project, every function at every stage either **does exactly what it's supposed to or throws an exception**. Everywhere... except:
- [libSceSaveDataDialog.native](../core/libs/prx/libSceSaveDataDialog.native/Export.cpp)
- [libSceCommonDialog](../core/libs/prx/libSceCommonDialog/Export.cpp)
- The shader recompiler [skips baryctric coordinates](../core/shader/recompiler/Recompiler.cpp) (is not even passed to SpirvTargetOptions at row 219).

### Unknown function names

- [zARR5aCmkoY](../core/libs/prx/libSceAgc/DcbFlow/src/Control.cpp) (libSceAgc) - unknown signature
- [qj7QZpgr9Uw](../core/libs/prx/libSceAgc/DcbState/src/ContextState.cpp) (libSceAgc)
- [fd5Bp5tGTgo](../core/libs/prx/libSceAgc/Misc/src/ShaderFusion.cpp) (libSceAgc)
- [dolOmWH+huQ](../core/libs/prx/libSceAgc/Misc/src/ShaderFusion.cpp) (libSceAgc)
- [V++UgBtQhn0](../core/libs/prx/libSceAgc/Misc/src/PacketInfo.cpp) (libSceAgc)
- [gQkqkLttcpw](../core/libs/prx/libSceAgc/Acb/src/Control.cpp) (libSceAgc) - unknown signature

### Functional

- There's no way to specify keyboard and mouse input mapping when using a gamepad. The [default mapping](../core/libs/prx/libScePad/include/InputMapping.hpp) is always used.
- [Shader recompilation](../core/shader/recompiler/Recompiler.cpp) currently occurs right before it was transferred to Vulkan with caching, but should be moved to the [relinker](../core/relinker/main.cpp) stage. For this purpose, [shader/recompiler](../core/shader/recompiler) was written completely independently from [libs/prx](../core/libs/prx).
- The executable file that [relinker](../core/relinker/elfpatcher/src/windows/WindowsPeWriter.cpp) generates opens the console when launched, which is inconvenient for playability.
- [Relinker](../core/relinker/elfpatcher/src) doesn't add an icon to the generated executable. This should be done without adding dependencies (only standard).
- `--to-intel` does not lower the register forms of EXTRQ/INSERTQ (`0F 79`, 4 or 5 bytes, too short for a jump) nor MONITORX/MWAITX/CLZERO/RDPRU/MCOMMIT; the [lowering](../core/relinker/codegen/src/x86/Sse4aLowering.cpp) fails the relink instead. SHA-NI is not substituted.
- The length-changing path of the [instruction rewriter](../core/relinker/codegen/src/x86/X64InstructionRewriter.cpp) is not used by the [converter](../core/relinker/codegen/src/Amd64OnlyConverter.cpp): it does not adjust VEX/0F38/0F3A RIP-relative operands, data-to-code references (relocations, FDEs, jump tables) or segment sizes, so every substitution keeps the instruction length.
- The Linux placement of the `--to-intel` stubs in the [ELF patcher](../core/relinker/elfpatcher/src/linux/LinuxElfPatcher.cpp) is covered only by a synthetic test.
- The libc [SSE4a trap emulation](../core/libs/prx/libc/src/specifics/windows/Sse4aEmulation.hpp) on Windows is superseded by `--to-intel` and remains only until the relinked title has been verified without it.
