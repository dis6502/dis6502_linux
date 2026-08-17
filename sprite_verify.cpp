#include <cstdio>
#include <cassert>
#include "ui_frontend/SpriteMode.h"

int main() {
    // 1bpp mode (pixelsPerByte=8): 0xFF -> all pixels index 3
    for (int i = 0; i < 8; i++) assert(SpriteMode::GetPixelPaletteIndex(0xFF, i, 8) == 3);
    // 0x00 -> all pixels index 0
    for (int i = 0; i < 8; i++) assert(SpriteMode::GetPixelPaletteIndex(0x00, i, 8) == 0);
    // 0x80 = 10000000 -> pixel 0 (MSB) = 1 -> index 3, rest = 0
    assert(SpriteMode::GetPixelPaletteIndex(0x80, 0, 8) == 3);
    for (int i = 1; i < 8; i++) assert(SpriteMode::GetPixelPaletteIndex(0x80, i, 8) == 0);
    // 0x01 = 00000001 -> pixel 7 (LSB) = 1 -> index 3, rest = 0
    assert(SpriteMode::GetPixelPaletteIndex(0x01, 7, 8) == 3);
    for (int i = 0; i < 7; i++) assert(SpriteMode::GetPixelPaletteIndex(0x01, i, 8) == 0);

    // 2bpp mode (pixelsPerByte=4): 0b11100100 = 0xE4
    // pixel0 = bits 7-6 = 11 = 3
    // pixel1 = bits 5-4 = 10 = 2
    // pixel2 = bits 3-2 = 01 = 1
    // pixel3 = bits 1-0 = 00 = 0
    assert(SpriteMode::GetPixelPaletteIndex(0xE4, 0, 4) == 3);
    assert(SpriteMode::GetPixelPaletteIndex(0xE4, 1, 4) == 2);
    assert(SpriteMode::GetPixelPaletteIndex(0xE4, 2, 4) == 1);
    assert(SpriteMode::GetPixelPaletteIndex(0xE4, 3, 4) == 0);

    // Mode table sanity checks against the original arrays
    auto m0 = SpriteMode::GetModeInfo(0);
    assert(m0.numColors == 4 && m0.bytesPerLine == 10 && m0.pixelsPerByte == 4 && m0.pixelWidth == 8 && m0.pixelHeight == 8);
    auto m7 = SpriteMode::GetModeInfo(7);
    assert(m7.numColors == 1 && m7.bytesPerLine == 40 && m7.pixelsPerByte == 8 && m7.pixelWidth == 1 && m7.pixelHeight == 1);

    std::printf("ALL SPRITE MODE CHECKS PASSED\n");
    return 0;
}
