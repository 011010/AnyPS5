#include <cstdint>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"
#include "prx/libkernel/KernelErrors.hpp"
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <filesystem>
#else
#include <dlfcn.h>
#include <link.h>
#include <unistd.h>
#endif

extern "C" std::int32_t ModuleIdForImage_nid_no_patch(const void* native);

namespace {

constexpr char GuestModuleSuffix[] = ".guest.prx";
constexpr std::int32_t ProtRead = 1;
constexpr std::int32_t ProtWrite = 2;
constexpr std::int32_t ProtExecute = 4;

std::uint32_t ToU32(std::uint64_t value, const char* field) {
    if (value > std::numeric_limits<std::uint32_t>::max())
        throw std::runtime_error(std::string("sceKernelGetModuleInfoFromAddr: ") + field + " exceeds 32 bits");
    return static_cast<std::uint32_t>(value);
}

std::string ModuleName(std::string fileName) {
    if (fileName.size() > sizeof(GuestModuleSuffix) - 1 && fileName.ends_with(GuestModuleSuffix))
        fileName.resize(fileName.size() - (sizeof(GuestModuleSuffix) - 1));
    return fileName;
}

void SetName(ModuleInfoEx& info, const std::string& name) {
    if (name.size() >= sizeof(info.name))
        throw std::runtime_error("sceKernelGetModuleInfoFromAddr: module name too long: " + name);
    std::memcpy(info.name, name.c_str(), name.size() + 1);
}

std::uint64_t EncodedSize(std::uint8_t encoding) {
    switch (encoding & 0x0fu) {
    case 0x00: case 0x04: case 0x0c: return 8;
    case 0x02: case 0x0a: return 2;
    case 0x03: case 0x0b: return 4;
    default: throw std::runtime_error("sceKernelGetModuleInfoFromAddr: unsupported eh_frame_hdr encoding");
    }
}

template<typename TContains>
std::uintptr_t EncodedValue(const TContains& contains, std::uintptr_t field, std::uint8_t encoding) {
    const auto size = EncodedSize(encoding);
    if (!contains(field, size)) throw std::runtime_error("sceKernelGetModuleInfoFromAddr: eh_frame_hdr outside the image");
    const auto* bytes = reinterpret_cast<const void*>(field);
    std::int64_t value = 0;
    switch (encoding & 0x0fu) {
    case 0x02: { std::uint16_t data; std::memcpy(&data, bytes, sizeof(data)); value = data; break; }
    case 0x0a: { std::int16_t data; std::memcpy(&data, bytes, sizeof(data)); value = data; break; }
    case 0x03: { std::uint32_t data; std::memcpy(&data, bytes, sizeof(data)); value = data; break; }
    case 0x0b: { std::int32_t data; std::memcpy(&data, bytes, sizeof(data)); value = data; break; }
    default: std::memcpy(&value, bytes, sizeof(value)); break;
    }
    switch (encoding & 0x70u) {
    case 0x00: return static_cast<std::uintptr_t>(value);
    case 0x10: return field + static_cast<std::uintptr_t>(value);
    default: throw std::runtime_error("sceKernelGetModuleInfoFromAddr: unsupported eh_frame_hdr encoding");
    }
}

template<typename TContains>
std::uintptr_t EhFrameAddress(const TContains& contains, std::uintptr_t header) {
    if (!contains(header, 4)) throw std::runtime_error("sceKernelGetModuleInfoFromAddr: eh_frame_hdr outside the image");
    const auto* bytes = reinterpret_cast<const std::uint8_t*>(header);
    if (bytes[0] != 1) throw std::runtime_error("sceKernelGetModuleInfoFromAddr: unsupported eh_frame_hdr version");
    return EncodedValue(contains, header + 4, bytes[1]);
}

template<typename TContains>
std::uint64_t EhFrameHeaderSize(const TContains& contains, std::uintptr_t header) {
    const auto* bytes = reinterpret_cast<const std::uint8_t*>(header);
    std::uint64_t size = 4 + EncodedSize(bytes[1]);
    if (bytes[2] == 0xff) return size;
    const auto count = static_cast<std::uint64_t>(EncodedValue(contains, header + size, bytes[2] & 0x0fu));
    size += EncodedSize(bytes[2]);
    if (bytes[3] == 0xff) return size;
    const auto entry = 2 * EncodedSize(bytes[3]);
    if (count > (std::numeric_limits<std::uint64_t>::max() - size) / entry || !contains(header, size + count * entry))
        throw std::runtime_error("sceKernelGetModuleInfoFromAddr: eh_frame_hdr table outside the image");
    return size + count * entry;
}

template<typename TContains>
std::uint64_t EhFrameSize(const TContains& contains, std::uintptr_t frame) {
    std::uintptr_t position = frame;
    for (;;) {
        if (!contains(position, 4))
            throw std::runtime_error("sceKernelGetModuleInfoFromAddr: eh_frame outside the image");
        std::uint32_t length;
        std::memcpy(&length, reinterpret_cast<const void*>(position), sizeof(length));
        position += 4;
        if (length == 0) return position - frame;
        std::uint64_t recordLength = length;
        if (length == 0xffffffffu) {
            if (!contains(position, 8))
                throw std::runtime_error("sceKernelGetModuleInfoFromAddr: eh_frame outside the image");
            std::memcpy(&recordLength, reinterpret_cast<const void*>(position), sizeof(recordLength));
            position += 8;
        }
        if (!contains(position, recordLength))
            throw std::runtime_error("sceKernelGetModuleInfoFromAddr: eh_frame record outside the image");
        position += recordLength;
    }
}

#ifndef _WIN32
struct ImageSearch {
    std::uintptr_t address;
    ModuleInfoEx* info;
    bool found;
};

bool Contains(const dl_phdr_info& image, std::uintptr_t begin, std::uint64_t size) {
    for (std::uint16_t i = 0; i < image.dlpi_phnum; ++i) {
        const auto& header = image.dlpi_phdr[i];
        if (header.p_type != PT_LOAD) continue;
        const std::uintptr_t start = image.dlpi_addr + header.p_vaddr;
        if (begin >= start && begin - start <= header.p_memsz && size <= header.p_memsz - (begin - start)) return true;
    }
    return false;
}

std::string ImageName(const dl_phdr_info& image) {
    std::string path = image.dlpi_name ? image.dlpi_name : "";
    if (path.empty()) {
        char executable[4096];
        const auto length = ::readlink("/proc/self/exe", executable, sizeof(executable) - 1);
        if (length < 0) throw std::runtime_error("sceKernelGetModuleInfoFromAddr: cannot resolve the executable path");
        path.assign(executable, static_cast<std::size_t>(length));
    }
    return ModuleName(path.substr(path.find_last_of('/') + 1));
}

void Fill(const dl_phdr_info& image, ModuleInfoEx& info) {
    const auto contains = [&](std::uintptr_t begin, std::uint64_t size) { return Contains(image, begin, size); };
    SetName(info, ImageName(image));
    info.tls_index = ToU32(image.dlpi_tls_modid, "TLS module index");
    for (std::uint16_t i = 0; i < image.dlpi_phnum; ++i) {
        const auto& header = image.dlpi_phdr[i];
        const std::uintptr_t address = image.dlpi_addr + header.p_vaddr;
        if (header.p_type == PT_LOAD && info.segment_count < std::size(info.segments)) {
            auto& segment = info.segments[info.segment_count++];
            segment.address = address;
            segment.size = ToU32(header.p_memsz, "segment size");
            segment.prot = ((header.p_flags & PF_R) ? ProtRead : 0) | ((header.p_flags & PF_W) ? ProtWrite : 0) | ((header.p_flags & PF_X) ? ProtExecute : 0);
        } else if (header.p_type == PT_TLS) {
            info.tls_init_addr = address;
            info.tls_init_size = ToU32(header.p_filesz, "TLS image size");
            info.tls_size = ToU32(header.p_memsz, "TLS size");
            info.tls_align = ToU32(header.p_align, "TLS alignment");
        } else if (header.p_type == PT_GNU_EH_FRAME) {
            info.eh_frame_hdr_addr = address;
            info.eh_frame_hdr_size = ToU32(header.p_memsz, "eh_frame_hdr size");
            info.eh_frame_addr = EhFrameAddress(contains, address);
            info.eh_frame_size = ToU32(EhFrameSize(contains, info.eh_frame_addr), "eh_frame size");
        } else if (header.p_type == PT_DYNAMIC) {
            for (const auto* entry = reinterpret_cast<const ElfW(Dyn)*>(address); entry->d_tag != DT_NULL; ++entry) {
                if (entry->d_tag == DT_INIT) info.init_proc_addr = image.dlpi_addr + entry->d_un.d_ptr;
                else if (entry->d_tag == DT_FINI) info.fini_proc_addr = image.dlpi_addr + entry->d_un.d_ptr;
            }
        }
    }
    info.ref_count = 1;
}

int FindImage(dl_phdr_info* image, std::size_t, void* data) {
    auto& search = *static_cast<ImageSearch*>(data);
    if (!Contains(*image, search.address, 1)) return 0;
    Fill(*image, *search.info);
    search.found = true;
    return 1;
}
#else
bool SectionNamed(const IMAGE_SECTION_HEADER& section, const char* name) {
    return std::strncmp(reinterpret_cast<const char*>(section.Name), name, IMAGE_SIZEOF_SHORT_NAME) == 0;
}

std::int32_t SectionProtection(const IMAGE_SECTION_HEADER& section) {
    const auto characteristics = section.Characteristics;
    return ((characteristics & IMAGE_SCN_MEM_READ) ? ProtRead : 0) | ((characteristics & IMAGE_SCN_MEM_WRITE) ? ProtWrite : 0) | ((characteristics & IMAGE_SCN_MEM_EXECUTE) ? ProtExecute : 0);
}

std::string ImageName(HMODULE module) {
    std::wstring path(MAX_PATH, L'\0');
    for (;;) {
        const auto length = GetModuleFileNameW(module, path.data(), static_cast<DWORD>(path.size()));
        if (length == 0) throw std::runtime_error("sceKernelGetModuleInfoFromAddr: cannot resolve the module path");
        if (length < path.size()) {
            path.resize(length);
            break;
        }
        path.resize(path.size() * 2);
    }
    const auto fileName = std::filesystem::path(path).filename().u8string();
    return ModuleName(std::string(fileName.begin(), fileName.end()));
}

bool Fill(std::uintptr_t address, ModuleInfoEx& info) {
    MEMORY_BASIC_INFORMATION memory{};
    if (VirtualQuery(reinterpret_cast<LPCVOID>(address), &memory, sizeof(memory)) != sizeof(memory) || memory.Type != MEM_IMAGE) return false;
    const auto base = reinterpret_cast<std::uintptr_t>(memory.AllocationBase);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + static_cast<std::uintptr_t>(dos->e_lfanew));
    if (nt->Signature != IMAGE_NT_SIGNATURE || nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return false;
    const std::uint64_t imageSize = nt->OptionalHeader.SizeOfImage;
    const auto contains = [&](std::uintptr_t begin, std::uint64_t size) { return begin >= base && begin - base <= imageSize && size <= imageSize - (begin - base); };
    SetName(info, ImageName(reinterpret_cast<HMODULE>(base)));
    const auto* sections = IMAGE_FIRST_SECTION(nt);
    const auto sectionCount = nt->FileHeader.NumberOfSections;
    bool relinked = false;
    for (unsigned i = 0; i < sectionCount; ++i) relinked = relinked || SectionNamed(sections[i], ".elf0");
    for (unsigned i = 0; i < sectionCount; ++i) {
        const auto& section = sections[i];
        const auto start = base + section.VirtualAddress;
        if (SectionNamed(section, ".ehmeta")) {
            if (!contains(start, 4)) throw std::runtime_error("sceKernelGetModuleInfoFromAddr: .ehmeta outside the image");
            std::uint32_t headerRva;
            std::memcpy(&headerRva, reinterpret_cast<const void*>(start), sizeof(headerRva));
            info.eh_frame_hdr_addr = base + headerRva;
            info.eh_frame_addr = EhFrameAddress(contains, info.eh_frame_hdr_addr);
            info.eh_frame_hdr_size = ToU32(EhFrameHeaderSize(contains, info.eh_frame_hdr_addr), "eh_frame_hdr size");
            info.eh_frame_size = ToU32(EhFrameSize(contains, info.eh_frame_addr), "eh_frame size");
            continue;
        }
        if (SectionNamed(section, ".ehfram") && info.eh_frame_addr == 0) {
            info.eh_frame_addr = start;
            info.eh_frame_size = ToU32(EhFrameSize(contains, start), "eh_frame size");
            continue;
        }
        if (relinked && std::strncmp(reinterpret_cast<const char*>(section.Name), ".elf", 4) != 0) continue;
        const auto prot = SectionProtection(section);
        if (prot == 0 || info.segment_count >= std::size(info.segments)) continue;
        auto& segment = info.segments[info.segment_count++];
        segment.address = start;
        segment.size = ToU32(section.Misc.VirtualSize, "segment size");
        segment.prot = prot;
    }
    info.id = ModuleIdForImage_nid_no_patch(reinterpret_cast<const void*>(base));
    info.ref_count = 1;
    return true;
}
#endif

}

extern "C" {

int APS5_VABI sceKernelGetModuleInfoFromAddr(std::uint64_t address, int flags, ModuleInfoEx* info) {
    if (!info) return SCE_KERNEL_ERROR_EFAULT;
    if (flags != 2) throw std::invalid_argument("sceKernelGetModuleInfoFromAddr: unsupported flags " + std::to_string(flags));
    if (info->st_size != sizeof(ModuleInfoEx))
        throw std::invalid_argument("sceKernelGetModuleInfoFromAddr: unsupported st_size " + std::to_string(info->st_size));
    ModuleInfoEx result{};
    result.st_size = sizeof(ModuleInfoEx);
#ifdef _WIN32
    if (!Fill(static_cast<std::uintptr_t>(address), result)) return SCE_KERNEL_ERROR_ESRCH;
#else
    Dl_info symbol{};
    link_map* native = nullptr;
    if (!dladdr1(reinterpret_cast<const void*>(address), &symbol, reinterpret_cast<void**>(&native), RTLD_DL_LINKMAP) || !native)
        return SCE_KERNEL_ERROR_ESRCH;
    ImageSearch search{static_cast<std::uintptr_t>(address), &result, false};
    dl_iterate_phdr(FindImage, &search);
    if (!search.found) return SCE_KERNEL_ERROR_ESRCH;
    result.id = ModuleIdForImage_nid_no_patch(native);
#endif
    *info = result;
    return 0;
}

}
