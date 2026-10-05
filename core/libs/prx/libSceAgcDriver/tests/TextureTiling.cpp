#include "GraphicsTests.hpp"
#include "prx/libSceAgcDriver/Graphics/include/TextureSwizzleEquations.hpp"
#include "prx/libSceAgcDriver/Graphics/include/TextureTiling.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

using namespace AgcDriver::Graphics;

template<typename TAction>
void reject(TAction action, std::string_view reason) {
    try {
        action();
    } catch (const std::runtime_error& error) {
        Require(std::string_view(error.what()).find(reason) != std::string_view::npos, std::string("unexpected texture tiling test error: ") + error.what());
        return;
    }
    throw std::runtime_error(std::string("expected texture tiling rejection: ") + std::string(reason));
}

std::uint64_t equationOffset(const TextureSwizzleEquation& equation, std::uint32_t x, std::uint32_t y, std::uint32_t z) {
    std::uint64_t offset = 0;
    for (std::uint32_t bit = 0; bit < 16; ++bit) {
        const auto mask = equation.bits[bit];
        const auto selected = (x & (mask & 0xfffu)) ^ ((y << 12) & (mask & 0xfff000u)) ^ ((z << 24) & (mask & 0xff000000u));
        offset |= static_cast<std::uint64_t>(std::popcount(selected) & 1u) << bit;
    }
    return offset;
}

}

void RunTextureTilingTests() {
    {
        const auto mips = ComputeMipLayout(TextureTileMode::kLinear, 1, 4, 4, 2);
        Require(mips.size() == 2, "linear mip chain must contain the requested mip count");
        Require(mips[0].tiledOffset == 512 && mips[0].tiledSize == 1024, "linear mip 0 offset or size changed");
        Require(mips[0].width == 4 && mips[0].height == 4, "linear mip 0 dimensions changed");
        Require(mips[0].blocksPerRow == 256 && mips[0].pitchBytes == 256, "linear mip 0 row layout changed");
        Require(!mips[0].tail, "linear mips must never fall into a mip tail");
        Require(mips[1].tiledOffset == 0 && mips[1].tiledSize == 512, "linear mip 1 offset or size changed");
        Require(mips[1].width == 2 && mips[1].height == 2, "linear mip 1 dimensions changed");
        Require(mips[1].linearOffset == mips[1].tiledOffset && mips[1].linearSize == mips[1].tiledSize, "linear tiling must keep linear and tiled layout identical");
    }
    {
        const auto mips = ComputeMipLayout(TextureTileMode::kLinear, 169, 8, 8, 1);
        Require(mips.size() == 1, "compressed linear layout must contain one mip");
        Require(mips[0].width == 2 && mips[0].height == 2, "compressed linear mip block dimensions changed");
        Require(mips[0].blocksPerRow == 32 && mips[0].pitchBytes == 256, "compressed linear mip row layout changed");
        Require(mips[0].tiledSize == 512 && mips[0].linearSize == 512, "compressed linear mip size changed");
    }
    {
        const auto mips = ComputeMipLayout(TextureTileMode::kStandard256B, 1, 64, 64, 1);
        Require(mips.size() == 1, "standard 256B layout must contain one mip");
        Require(mips[0].tiledOffset == 0 && mips[0].tiledSize == 4096, "standard 256B mip 0 offset or size changed");
        Require(mips[0].width == 64 && mips[0].height == 64, "standard 256B mip 0 dimensions changed");
        Require(mips[0].blocksPerRow == 4 && mips[0].pitchBytes == 64, "standard 256B mip 0 row layout changed");
        Require(!mips[0].tail, "standard 256B textures must never use a mip tail");

        const auto surfaceSize = ComputeSurfaceSize(mips, 3);
        Require(surfaceSize == 4096ull * 3ull, "surface size must multiply the slice size by the array layer count");
    }
    {
        const auto mips = ComputeMipLayout(TextureTileMode::kStandard256B, 1, 32, 32, 6);
        Require(mips.size() == 6, "standard 256B mip chain must contain the requested mip count");
        for (const auto& mip : mips) Require(!mip.tail, "standard 256B tile mode must never produce a mip tail");
    }
    {
        const auto mips = ComputeMipLayout(TextureTileMode::kStandard64KB, 1, 1024, 1024, 11);
        Require(mips.size() == 11, "standard 64KB mip chain must contain the requested mip count");
        auto tailSeen = false;
        for (const auto& mip : mips) {
            Require(mip.width != 0 && mip.height != 0, "every standard 64KB mip must have nonzero dimensions");
            Require(mip.tiledSize != 0 && mip.linearSize != 0, "every standard 64KB mip must have a nonzero size");
            if (mip.tail) {
                tailSeen = true;
                Require(mip.blocksPerRow == 1, "mip tail levels must report a single block per row");
                Require(mip.tiledOffset == 0, "mip tail levels must share the tiled tail block offset");
            }
        }
        Require(tailSeen, "a deep standard 64KB mip chain must fall into the mip tail");
        Require(!mips.front().tail, "the base level of a deep mip chain must not be in the mip tail");
    }
    {
        const auto mips = ComputeMipLayout(TextureTileMode::kStandard4KB, 1, 512, 512, 10);
        Require(mips.size() == 10, "standard 4KB mip chain must contain the requested mip count");
        auto tailSeen = false;
        for (const auto& mip : mips) {
            if (mip.tail) tailSeen = true;
        }
        Require(tailSeen, "a deep standard 4KB mip chain must fall into the mip tail");
    }

    {
        const auto mips = ComputeMipLayout(TextureTileMode::RenderTarget64KB, 56, 257, 129, 1);
        Require(mips[0].blocksPerRow == 3 && mips[0].tiledSize == 393216, "render target surfaces must pad to complete 128 by 128 blocks for 32-bit pixels");
        Require(mips[0].pitchBytes == 1536 && mips[0].linearSize == 1536u * 129u, "detiled render target rows must span the padded block width");
        Require(ComputeSurfaceSize(mips, 6) == 2359296, "render target cube faces must retain the padded guest slice stride");
    }
    for (const auto format : std::array<std::uint32_t, 5>{1, 7, 56, 71, 77}) {
        const auto mips = ComputeMipLayout(TextureTileMode::RenderTarget64KB, format, 1024, 513, 11);
        std::vector<std::pair<std::uint64_t, std::uint64_t>> ranges;
        bool tailSeen = false;
        for (const auto& mip : mips) {
            Require(mip.linearOffset % 4 == 0, "detiled mip levels must start word-aligned");
            Require(mip.linearSize >= static_cast<std::uint64_t>(mip.pitchBytes) * mip.height, "detiled mip allocation must contain every row");
            Require(mip.linearSize % 4 == 0, "detiled mip sizes must preserve word alignment between array layers");
            ranges.emplace_back(mip.linearOffset, mip.linearOffset + mip.linearSize);
            if (mip.tail) {
                tailSeen = true;
                Require(mip.tiledOffset == 0 && mip.tiledSize == 65536, "render target mip tails must share one guest 64KB block");
            }
        }
        std::sort(ranges.begin(), ranges.end());
        for (std::size_t index = 1; index < ranges.size(); ++index) Require(ranges[index].first >= ranges[index - 1].second, "detiled mip levels must occupy separate ranges");
        Require(tailSeen && !mips.front().tail, "render target mip chains must cover both regular blocks and mip tails");
    }
    const auto compressed = ComputeMipLayout(TextureTileMode::RenderTarget64KB, 169, 64, 64, 1);
    Require(compressed.size() == 1 && compressed[0].tiledSize == 65536 && compressed[0].linearSize != 0, "block compressed render target layout is wrong");
    Require(ComputeMipLayout(TextureTileMode::RenderTarget64KB, 132, 64, 64, 1).size() == 1, "format 132 render target layout is missing");
    reject([] { ComputeMipLayout(TextureTileMode::RenderTarget64KB, 74, 64, 64, 1); }, "unsupported bytes per element");

    reject([] { ComputeMipLayout(TextureTileMode::kLinear, 1, 0, 4, 1); }, "zero-sized texture");
    reject([] { ComputeMipLayout(TextureTileMode::kLinear, 1, 4, 0, 1); }, "zero-sized texture");
    reject([] { ComputeMipLayout(TextureTileMode::kLinear, 1, 4, 4, 0); }, "mip count is out of range");
    reject([] { ComputeMipLayout(TextureTileMode::kLinear, 1, 4, 4, 17); }, "mip count is out of range");

    {
        GuestTextureResource volume{};
        volume.width = 64;
        volume.height = 64;
        volume.depthOrLastArray = 31;
        volume.mipCount = 3;
        volume.tileMode = TextureTileMode::kStandard4KB;
        volume.dimension = TextureDimension::k3D;
        volume.format = 56;
        const auto geometry = DescribeSurface(volume);
        Require(geometry.thick && geometry.blockDepth == 8 && geometry.mips.size() == 3, "mipmapped 3D texture geometry changed");
        Require(geometry.mips[2].tiledOffset == 0 && geometry.mips[2].tiledSize == 8192, "3D mip 2 must lead each slab");
        Require(geometry.mips[1].tiledOffset == 8192 && geometry.mips[1].tiledSize == 32768, "3D mip 1 offset or size changed");
        Require(geometry.mips[0].tiledOffset == 40960 && geometry.mips[0].tiledSize == 131072, "3D mip 0 must end each slab");
        Require(geometry.layerBytes == 172032 && geometry.guestBytes == 172032ull * 4, "3D slabs must hold the whole mip chain");
        Require(geometry.HasLayer(1, 15) && !geometry.HasLayer(1, 16) && !geometry.HasLayer(2, 8), "3D mips must halve their depth");
        volume.width = 33;
        volume.height = 20;
        volume.depthOrLastArray = 11;
        volume.mipCount = 2;
        const auto odd = DescribeSurface(volume);
        Require(odd.mips[1].width == 16 && odd.mips[1].blocksPerRow == 3 && odd.mips[1].tiledSize == 12288, "a 3D level must be padded from its size rounded up, as addrlib does");
        Require(odd.mips[0].blocksPerRow == 5 && odd.mips[0].tiledOffset == 12288 && odd.mips[0].tiledSize == 40960, "3D mip 0 of a non-power-of-two volume changed");
        volume.width = 8;
        volume.height = 8;
        volume.depthOrLastArray = 7;
        volume.mipCount = 2;
        reject([&] { DescribeSurface(volume); }, "3D texture mip tails are not implemented");
    }

    {
        // CoveredMipBytes against the XOR address equations: every element slot of a covered range
        // holds exactly one element of the mip, and every tile block left out holds a slot no
        // element does (the bytes a write-back must keep).
        constexpr std::array<TextureTileMode, 4> modes{TextureTileMode::kZ64KBX, TextureTileMode::kS64KBX, TextureTileMode::kD64KBX, TextureTileMode::kR64KBX};
        constexpr std::array<std::pair<std::uint32_t, std::uint32_t>, 6> sizes{{{200, 150}, {1920, 1080}, {2432, 1368}, {960, 540}, {256, 128}, {300, 1}}};
        for (const auto mode : modes) {
            for (std::uint32_t bytesPerElement = 1; bytesPerElement <= 16; bytesPerElement *= 2) {
                const auto* equation = FindTextureSwizzleEquation(XorSwizzleMode(mode), bytesPerElement);
                Require(equation != nullptr, "missing XOR swizzle equation");
                const auto block = ThinBlockLayout(mode, bytesPerElement);
                for (const auto& [width, height] : sizes) {
                    if (static_cast<std::uint64_t>(width) * height > (1u << 20) && mode != TextureTileMode::kR64KBX) continue;
                    const auto what = "XOR mode " + std::to_string(XorSwizzleMode(mode)) + ", " + std::to_string(bytesPerElement) + " bytes, " + std::to_string(width) + "x" + std::to_string(height);
                    for (const auto& mip : ComputeElementMipLayout(mode, bytesPerElement, width, height, 3)) {
                        const auto covered = CoveredMipBytes(mode, bytesPerElement, mip);
                        if (mip.tail) {
                            Require(covered.empty(), what + ": a tail mip has covered bytes");
                            continue;
                        }
                        std::vector<std::uint8_t> held(static_cast<std::size_t>(mip.tiledSize / bytesPerElement), 0);
                        for (std::uint32_t y = 0; y < mip.height; ++y) {
                            for (std::uint32_t x = 0; x < mip.width; ++x) {
                                const auto offset = (static_cast<std::uint64_t>(y / block[2]) * mip.blocksPerRow + x / block[1]) * block[0] + equationOffset(*equation, x, y, 0);
                                Require(offset % bytesPerElement == 0 && offset < mip.tiledSize, what + ": an element lies outside the mip");
                                held[static_cast<std::size_t>(offset / bytesPerElement)] += 1;
                            }
                        }
                        std::vector<bool> inCovered(static_cast<std::size_t>(mip.tiledSize / block[0]), false);
                        std::uint64_t previous = 0;
                        for (const auto& [begin, end] : covered) {
                            Require(begin >= previous && begin < end && end <= mip.tiledSize && begin % block[0] == 0 && end % block[0] == 0, what + ": covered ranges are not ascending whole blocks");
                            previous = end;
                            for (auto slot = begin / bytesPerElement; slot < end / bytesPerElement; ++slot) Require(held[static_cast<std::size_t>(slot)] == 1, what + ": a covered byte holds no element or several");
                            for (auto at = begin; at < end; at += block[0]) inCovered[static_cast<std::size_t>(at / block[0])] = true;
                        }
                        for (std::size_t index = 0; index < inCovered.size(); ++index) {
                            if (inCovered[index]) continue;
                            const auto first = index * block[0] / bytesPerElement;
                            const auto last = (index + 1) * block[0] / bytesPerElement;
                            Require(std::any_of(held.begin() + static_cast<std::ptrdiff_t>(first), held.begin() + static_cast<std::ptrdiff_t>(last), [](std::uint8_t count) { return count == 0; }), what + ": a block every byte of which holds an element is left out");
                        }
                    }
                }
            }
        }
        const auto linear = ComputeElementMipLayout(TextureTileMode::kLinear, 4, 200, 3, 1).front();
        const auto rows = CoveredMipBytes(TextureTileMode::kLinear, 4, linear);
        Require(linear.pitchBytes > 800 && rows.size() == 3 && rows[1].first == linear.pitchBytes && rows[1].second == linear.pitchBytes + 800, "linear covered bytes are not the rows without their pitch padding");
        const auto dense = ComputeElementMipLayout(TextureTileMode::kLinear, 4, 256, 3, 1).front();
        Require(dense.pitchBytes == 1024 && CoveredMipBytes(TextureTileMode::kLinear, 4, dense) == std::vector<std::pair<std::uint64_t, std::uint64_t>>{{0, 3072}}, "linear rows without padding are not one range");
    }

    reject([] { ComputeSurfaceSize({}, 1); }, "empty mip chain");
    reject([] { ComputeSurfaceSize(ComputeMipLayout(TextureTileMode::kLinear, 1, 4, 4, 1), 0); }, "zero array layers");
}
