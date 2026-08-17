#pragma once

#include "Syntax.h"
#include "UI.h"

// Portable placeholder for the Win32 GDI-backed ComputerFont. The real
// implementation created actual HFONTs via CreateFont() for rendering
// the disassembly view in a fixed-width "computer" font (e.g. an Atari
// or C64 character set font). Font rendering is inherently tied to
// whichever UI toolkit replaces Win32 (ImGui uses its own font-atlas
// system, entirely different from GDI fonts) so that part belongs in
// the UI layer, not here. This stub keeps ComputerSystem (which owns a
// ComputerFont member) compiling and testable headlessly.
// See ui_todo/ComputerFont.{h,cpp} for the original GDI implementation
// to reference when building the real UI-layer font handling.
class ComputerFont {
public:
    ComputerFont() = delete;
    ComputerFont(const ComputerFont&) = delete;
    ComputerFont& operator=(const ComputerFont&) = delete;
    ComputerFont(ComputerFont&&) = delete;
    ComputerFont& operator=(ComputerFont&&) = delete;

    explicit ComputerFont(int fontHeight) : fontHeight(fontHeight) {}
    ~ComputerFont() = default;

    void Load(wstring_view fontName) { this->fontName = fontName; }

    HFONT GetFont(bool /*doubleHeight*/) const { return nullptr; } // TODO(UI layer): real font handle

private:
    int fontHeight;
    wstring fontName;
};
