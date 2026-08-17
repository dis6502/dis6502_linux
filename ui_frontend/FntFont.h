#pragma once

#include <cstdint>
#include <vector>
#include <optional>
#include <fstream>
#include <string>

// Parses the legacy Windows 3.x NE-format .FON file (an embedded FNT
// bitmap font resource) that the original dis6502 bundled per computer
// system (Atari800.fon, C64.fon, etc.) for rendering the disassembly
// view in that system's actual character set.
//
// Rather than fully parse the NE executable's resource table (a
// separate, more involved format) to locate the embedded FNT resource,
// this scans for it directly: an FNT header starts with a version word
// (0x0200 or 0x0300) whose declared size, added to the candidate start
// offset, must exactly equal the file size - a strong, effectively
// unique validating condition for files (like these) known to contain
// exactly one FNT resource. This was cross-checked by hand against a
// hex dump of the real Atari800.fon before being written as code:
// candidate offset 0x1B0 gave dfSize=0x0EA0 (0x1B0+0x0EA0 = 0x1050 =
// 4176 = the real file size), the copyright string matched exactly at
// the expected header offset, and dfPixWidth/dfPixHeight both came out
// as 8 (matching the "FONTRES ...:Atari800 8" string elsewhere in the
// file). See fnt_verify.cpp for the actual decoded-glyph verification.
namespace FntFont {

struct Glyph {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> pixels; // row-major, 1 byte per pixel (0 or 1), size width*height
};

struct Font {
    int pixWidth = 0;
    int pixHeight = 0;
    int ascent = 0;
    int firstChar = 0;
    int lastChar = 0;
    std::vector<Glyph> glyphs; // index 0 == firstChar
};

namespace detail {
    inline uint16_t ReadU16(const std::vector<uint8_t>& b, size_t offset) {
        return static_cast<uint16_t>(b[offset] | (b[offset + 1] << 8));
    }
    inline uint32_t ReadU32(const std::vector<uint8_t>& b, size_t offset) {
        return static_cast<uint32_t>(b[offset]) | (static_cast<uint32_t>(b[offset + 1]) << 8) |
               (static_cast<uint32_t>(b[offset + 2]) << 16) | (static_cast<uint32_t>(b[offset + 3]) << 24);
    }

    // Returns the file offset of the embedded FNT resource, or nullopt.
    inline std::optional<size_t> FindFntResourceOffset(const std::vector<uint8_t>& b) {
        for (size_t offset = 0; offset + 6 < b.size(); offset++) {
            uint16_t version = ReadU16(b, offset);
            if (version != 0x0200 && version != 0x0300) continue;
            uint32_t size = ReadU32(b, offset + 2);
            if (size == 0 || offset + size != b.size()) continue;

            // Extra sanity checks on the fields that matter for decoding,
            // to rule out a version-word-shaped false positive elsewhere
            // in the file.
            size_t pixWidthOffset = offset + 0x56;
            size_t pixHeightOffset = offset + 0x58;
            size_t firstCharOffset = offset + 0x5F;
            size_t lastCharOffset = offset + 0x60;
            if (lastCharOffset >= b.size()) continue;

            uint16_t pixWidth = ReadU16(b, pixWidthOffset);
            uint16_t pixHeight = ReadU16(b, pixHeightOffset);
            uint8_t firstChar = b[firstCharOffset];
            uint8_t lastChar = b[lastCharOffset];
            if (pixWidth == 0 || pixWidth > 64 || pixHeight == 0 || pixHeight > 64) continue;
            if (firstChar > lastChar) continue;

            return offset;
        }
        return std::nullopt;
    }
}

inline std::optional<Font> Load(const std::vector<uint8_t>& fileBytes) {
    auto resourceOffsetOpt = detail::FindFntResourceOffset(fileBytes);
    if (!resourceOffsetOpt) return std::nullopt;
    size_t r = *resourceOffsetOpt;

    uint16_t version = detail::ReadU16(fileBytes, r);
    uint16_t ascent = detail::ReadU16(fileBytes, r + 0x4A);
    uint16_t pixWidth = detail::ReadU16(fileBytes, r + 0x56);
    uint16_t pixHeight = detail::ReadU16(fileBytes, r + 0x58);
    uint8_t firstChar = fileBytes[r + 0x5F];
    uint8_t lastChar = fileBytes[r + 0x60];

    // Char table starts right after the fixed-size header: 0x76 bytes
    // for version 0x0200, 0x94 bytes for version 0x0300 (the extra
    // 0x1E bytes are dfFlags/dfAspace/dfBspace/dfCspace/dfColorPointer/
    // dfReserved1). Each entry is {WORD width; WORD offset} for 0x0200
    // (4 bytes) or {WORD width; DWORD offset} for 0x0300 (6 bytes) -
    // offsets are relative to r (the start of this FNT resource).
    size_t headerSize = (version == 0x0300) ? 0x94 : 0x76;
    size_t entrySize = (version == 0x0300) ? 6 : 4;
    size_t charTableOffset = r + headerSize;

    Font font;
    font.pixWidth = pixWidth;
    font.pixHeight = pixHeight;
    font.ascent = ascent;
    font.firstChar = firstChar;
    font.lastChar = lastChar;

    int charCount = lastChar - firstChar + 1;
    font.glyphs.reserve(charCount);

    for (int i = 0; i < charCount; i++) {
        size_t entryOffset = charTableOffset + i * entrySize;
        if (entryOffset + entrySize > fileBytes.size()) break;

        uint16_t width = detail::ReadU16(fileBytes, entryOffset);
        uint32_t glyphOffset = (version == 0x0300)
            ? detail::ReadU32(fileBytes, entryOffset + 2)
            : detail::ReadU16(fileBytes, entryOffset + 2);

        Glyph glyph;
        glyph.width = width;
        glyph.height = pixHeight;
        glyph.pixels.assign(static_cast<size_t>(width) * pixHeight, 0);

        // Row-major: one byte per row for widths up to 8 pixels (bit 7
        // = leftmost column), rows stored top to bottom. Confirmed by
        // manually decoding the real Atari800.fon's 'A' glyph bytes
        // (0x00 0x18 0x3C 0x66 0x66 0x7E 0x66 0x00) this way and getting
        // a recognizable letter A - an earlier column-major attempt
        // produced a plausible-looking but wrong shape instead (see
        // fnt_verify.cpp for the check this was caught with). For
        // pixWidth > 8 this would need bytesPerRow = ceil(width/8)
        // bytes per row instead of the single-byte-per-row case
        // implemented here, but every font this app bundles is 8
        // pixels wide, so that case is untested.
        int bytesPerRow = (width + 7) / 8;
        size_t glyphFileOffset = r + glyphOffset;

        for (int row = 0; row < pixHeight; row++) {
            for (int byteIndex = 0; byteIndex < bytesPerRow; byteIndex++) {
                size_t byteOffset = glyphFileOffset + row * bytesPerRow + byteIndex;
                if (byteOffset >= fileBytes.size()) continue;
                uint8_t byteValue = fileBytes[byteOffset];
                for (int bit = 0; bit < 8; bit++) {
                    int col = byteIndex * 8 + bit;
                    if (col >= width) break;
                    bool set = (byteValue & (0x80 >> bit)) != 0;
                    glyph.pixels[static_cast<size_t>(row) * width + col] = set ? 1 : 0;
                }
            }
        }

        font.glyphs.push_back(std::move(glyph));
    }

    return font;
}

inline std::optional<Font> LoadFromFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return std::nullopt;
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return Load(bytes);
}

} // namespace FntFont
