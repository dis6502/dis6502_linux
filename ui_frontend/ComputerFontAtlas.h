#pragma once

#include "imgui.h"
#include "FntFont.h"

#include <vector>

// Wires a parsed FntFont (see FntFont.h) into ImGui's font atlas via
// ImFontAtlas::AddCustomRectFontGlyph - the standard, long-documented
// mechanism for injecting custom bitmap glyphs (used in imgui_demo.cpp's
// own "Custom Rectangles" section), rather than a hand-rolled texture.
//
// IMPORTANT ORDERING REQUIREMENT: RegisterGlyphs() must be called AFTER
// ImGui::CreateContext() but BEFORE the render backend's init call
// (ImGui_ImplSDLRenderer2_Init in main.cpp) - that call uploads whatever
// is in the atlas at that moment as a GPU texture, so the custom rects
// and their pixel data need to already exist by then. BlitPixels() does
// the actual pixel copy and must run after RegisterGlyphs() but can run
// either before or after the backend init, since it only touches the
// CPU-side atlas buffer that GetTexDataAsAlpha8() returns (which forces
// the atlas to build if it hasn't already).
//
// Like the rest of ui_frontend/, this could not be run against a real
// ImGui build in the sandbox this was written in (no network access to
// fetch ImGui/SDL2) - unlike FntFont.h itself, which *was* verified by
// actually parsing and visually inspecting decoded glyphs from the real
// bundled .fon files (see fnt_verify.cpp). The custom-rect-glyph API
// used here has been stable for a long time and is exactly the pattern
// imgui_demo.cpp itself demonstrates, but treat this integration layer
// with the same "verify before fully trusting" approach as the rest of
// this front end.
struct ComputerFontAtlas {
    ImFont* font = nullptr;
    FntFont::Font fntFont;
    std::vector<int> rectIds; // one per glyph, indexed the same as fntFont.glyphs

    // Call before the render backend's init call. Adds one custom rect
    // per glyph to the atlas; pixel data isn't copied in yet (see
    // BlitPixels below) - AddCustomRectFontGlyph only reserves the
    // rect's position/size in the atlas layout.
    // IMPORTANT: call this only after a separate, untouched default font
    // has already been registered for general UI text (see main.cpp) -
    // otherwise this AddFontDefault() call would be the *only* font in
    // the atlas and would become the app's global default everywhere
    // (menu bar, Segments list, Log, etc.), not just wherever
    // ImGui::PushFont(this->font) is explicitly used. That was a real
    // bug in an earlier version of this file: every pane's text ended
    // up using these 8x8 bitmap glyphs, not just Disassembly/Memory
    // Inspector, making all text look too wide/oddly spaced everywhere.
    void RegisterGlyphs(FntFont::Font parsedFont) {
        fntFont = std::move(parsedFont);
        ImGuiIO& io = ImGui::GetIO();
        font = io.Fonts->AddFontDefault(); // a SEPARATE font instance from the general-UI default

        rectIds.resize(fntFont.glyphs.size());
        for (size_t i = 0; i < fntFont.glyphs.size(); i++) {
            int codepoint = fntFont.firstChar + static_cast<int>(i);
            const auto& glyph = fntFont.glyphs[i];
            rectIds[i] = io.Fonts->AddCustomRectFontGlyph(
                font, static_cast<ImWchar>(codepoint), glyph.width, glyph.height,
                static_cast<float>(glyph.width + 1)); // +1px advance so characters aren't touching
        }
    }

    // Call after RegisterGlyphs(). Forces the atlas to build (if it
    // hasn't already) and copies each glyph's decoded pixels into its
    // reserved rect in the atlas's single-channel alpha buffer.
    void BlitPixels() {
        ImGuiIO& io = ImGui::GetIO();
        unsigned char* pixels = nullptr;
        int texWidth = 0, texHeight = 0;
        io.Fonts->GetTexDataAsAlpha8(&pixels, &texWidth, &texHeight);
        if (!pixels) return;

        for (size_t i = 0; i < fntFont.glyphs.size(); i++) {
            const ImFontAtlasCustomRect* rect = io.Fonts->GetCustomRectByIndex(rectIds[i]);
            if (!rect) continue;
            const auto& glyph = fntFont.glyphs[i];
            for (int y = 0; y < glyph.height; y++) {
                for (int x = 0; x < glyph.width; x++) {
                    int texX = rect->X + x;
                    int texY = rect->Y + y;
                    if (texX < 0 || texX >= texWidth || texY < 0 || texY >= texHeight) continue;
                    pixels[static_cast<size_t>(texY) * texWidth + texX] = glyph.pixels[static_cast<size_t>(y) * glyph.width + x] ? 255 : 0;
                }
            }
        }
    }
};
