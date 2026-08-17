#include <cstdio>
#include "ui_frontend/FntFont.h"

void PrintGlyph(const FntFont::Font& font, char c) {
    int index = static_cast<unsigned char>(c) - font.firstChar;
    if (index < 0 || index >= static_cast<int>(font.glyphs.size())) {
        std::printf("Character '%c' (0x%02X) out of range [firstChar=%d, lastChar=%d]\n",
                    c, static_cast<unsigned char>(c), font.firstChar, font.lastChar);
        return;
    }
    const auto& glyph = font.glyphs[index];
    std::printf("Glyph for '%c' (0x%02X), %dx%d:\n", c, static_cast<unsigned char>(c), glyph.width, glyph.height);
    for (int row = 0; row < glyph.height; row++) {
        std::string line;
        for (int col = 0; col < glyph.width; col++) {
            line += glyph.pixels[static_cast<size_t>(row) * glyph.width + col] ? '#' : '.';
        }
        std::printf("  %s\n", line.c_str());
    }
    std::printf("\n");
}

int main() {
    auto fontOpt = FntFont::LoadFromFile("ui_frontend/systems/Atari800/Atari800.fon");

    if (!fontOpt) {
        std::printf("FAILED to parse Atari800.fon - no valid FNT resource found\n");
        return 1;
    }

    const auto& font = *fontOpt;
    std::printf("Parsed font: %dx%d pixels, ascent=%d, chars %d..%d, %zu glyphs\n\n",
                font.pixWidth, font.pixHeight, font.ascent, font.firstChar, font.lastChar, font.glyphs.size());

    // Print a handful of recognizable characters so we can visually
    // confirm the decoding is actually correct, not just "ran without
    // crashing".
    for (char c : {'A', 'B', 'S', '0', '1', ' ', '.', '*'}) {
        PrintGlyph(font, c);
    }

    return 0;
}
