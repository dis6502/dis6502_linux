#pragma once

#include <cstdint>
#include <algorithm>

// Ported from ui_todo/SpriteControlImpl.cpp - the original's sprite/
// bitmap preview renders a selected byte range in one of 8 Atari
// graphics modes (roughly ANTIC modes 8-15/GTIA-ish text/graphics
// modes), each with a different bits-per-pixel and pixel aspect ratio.
// This is just the mode tables and the SPRITE_GET_PIXEL_1/2 pixel-
// extraction macros, as pure functions with no drawing dependency -
// the actual drawing (in main.cpp's DrawSpriteViewPanel) is a from-
// scratch ImGui equivalent of the original's manual off-screen-buffer
// blitting, not a port of that part (which was tightly coupled to a
// fixed-stride Win32 DIB buffer via hardcoded byte offsets).
namespace SpriteMode {

struct ModeInfo {
    int numColors;
    int bytesPerLine;
    int pixelsPerByte;
    int pixelWidth;   // horizontal scale factor (accounts for non-square Atari pixels)
    int pixelHeight;  // vertical scale factor
};

// Exact values from wSpriteNbColors/wSpriteNbBytesPerLine/
// wSpriteNbPixelsPerByte/wSpritePixelWidth/wSpritePixelHeight in the
// original (indices 0-7).
inline ModeInfo GetModeInfo(int mode) {
    static const int nbColors[]      = { 4, 2, 4, 2, 2, 4, 4, 1 };
    static const int bytesPerLine[]  = { 10, 10, 20, 20, 20, 40, 40, 40 };
    static const int pixelsPerByte[] = { 4, 8, 4, 8, 8, 4, 4, 8 };
    static const int pixelWidth[]    = { 8, 4, 4, 2, 2, 2, 2, 1 };
    static const int pixelHeight[]   = { 8, 4, 4, 2, 1, 2, 1, 1 };
    mode = std::max(0, std::min(mode, 7));
    return ModeInfo{ nbColors[mode], bytesPerLine[mode], pixelsPerByte[mode], pixelWidth[mode], pixelHeight[mode] };
}

// Extracts the palette index (0-3) for one logical pixel within a byte,
// matching SPRITE_GET_PIXEL_1 (pixelsPerByte==8, 1 bit/pixel -> index
// 0 or 3) and SPRITE_GET_PIXEL_2 (pixelsPerByte==4, 2 bits/pixel ->
// index 0-3 directly) from the original exactly. pixelIndex counts
// left to right within the byte (0 = leftmost/most-significant).
inline int GetPixelPaletteIndex(uint8_t byteValue, int pixelIndex, int pixelsPerByte) {
    if (pixelsPerByte == 8) {
        int bitNumber = 7 - pixelIndex;
        int bit = (byteValue >> bitNumber) & 0x01;
        return bit ? 3 : 0;
    } else {
        int shift = (3 - pixelIndex) * 2;
        return (byteValue >> shift) & 0x03;
    }
}

}
