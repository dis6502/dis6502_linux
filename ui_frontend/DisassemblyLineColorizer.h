#pragma once

#include <string>
#include <vector>

// Which syntax class a run of characters in a disassembly line belongs
// to - directly mirrors the original DisassemblyControlImpl.cpp's
// STATE_* enum, so behavior matches the Win32 version exactly. The
// actual color values (Colors[] in the original) are a UI concern and
// live in whichever renderer consumes this (see DisLineColorToImGui
// for the ImGui mapping).
enum class DisLineSyntaxClass {
    Normal,
    Comment,
    Number,
    String,
    Instruction,
    Unreferenced
};

struct DisLineSegment {
    DisLineSyntaxClass syntaxClass;
    std::wstring text;
};

// Ports PrintOneLineInColor's character-scanning state machine
// (DisassemblyControlImpl.cpp) into a pure function: given one line of
// already-formatted disassembly text (as produced by DisassemblyLine/
// DIS_LINE::GetLine()) and whether the underlying instruction is
// "referenced", returns the ordered list of colored runs that make up
// that line. No drawing, no selection-highlight rect (that's just
// ImGui::Selectable's `selected` flag in the ImGui renderer) - only the
// per-character-class text splitting, which is the part that's genuinely
// shared logic rather than UI.
inline std::vector<DisLineSegment> ColorizeDisassemblyLine(const wchar_t* szText, bool referenced) {
    std::vector<DisLineSegment> result;
    auto flush = [&](DisLineSyntaxClass syntaxClass, const std::wstring& text) {
        if (!text.empty()) {
            result.push_back({ referenced ? syntaxClass : DisLineSyntaxClass::Unreferenced, text });
        }
    };

    size_t srcIndex = 0;
    std::wstring buf;
    wchar_t c, c2;

    // Label on column 0 should begin with a letter, an @ or an _.
    // A leading '.' is a directive with no label (this port's default
    // Profile directives - .byte, .word, etc. - all start with '.', and
    // an un-labeled data line is written straight to column 0 with no
    // leading space), so it also falls through to instruction parsing
    // rather than being mistaken for a label. Anything else unexpected
    // at column 0 is treated as a comment marker.
    c = szText[srcIndex++];
    if ((c == L'@') || (c == L'_') || ((c >= L'a') && (c <= L'z')) || ((c >= L'A') && (c <= L'Z'))) {
        while ((c != L'\0') && (c != L' ')) { buf += c; c = szText[srcIndex++]; }
        while (c == L' ') { buf += c; c = szText[srcIndex++]; }
        flush(DisLineSyntaxClass::Normal, buf);
        buf.clear();
    } else if ((c != L' ') && (c != L'.')) {
        flush(DisLineSyntaxClass::Comment, &szText[srcIndex - 1]);
        return result;
    }

    // The instruction mnemonic.
    DisLineSyntaxClass instructionClass = DisLineSyntaxClass::Instruction;
    while (c == L' ') { buf += c; c = szText[srcIndex++]; }
    if ((c == L'=') || (c == L'*')) {
        instructionClass = DisLineSyntaxClass::Normal;
    }
    while ((c != L'\0') && (c != L' ')) { buf += c; c = szText[srcIndex++]; }
    while (c == L' ') { buf += c; c = szText[srcIndex++]; }
    flush(instructionClass, buf);
    buf.clear();

    // Either a parameter or a comment.
    if ((c == L'\"') || (c == L'\'')) {
        c2 = c;
        buf += c;
        flush(DisLineSyntaxClass::Normal, buf);
        buf.clear();
        c = szText[srcIndex++];
        while ((c != L'\0') && (c != c2)) { buf += c; c = szText[srcIndex++]; }
        flush(DisLineSyntaxClass::String, buf);
        buf.clear();
    } else if (c == L'#') {
        buf += c;
        flush(DisLineSyntaxClass::Normal, buf);
        buf.clear();
        c = szText[srcIndex++];
        while (c == L' ') { buf += c; c = szText[srcIndex++]; }
    }

    bool isNumber;
    do {
        if (c == L',') {
            buf += c;
            c = szText[srcIndex++];
            flush(DisLineSyntaxClass::Normal, buf);
            buf.clear();
        }
        isNumber = false;
        if (c == L'$') {
            isNumber = true;
            buf += c;
            c = szText[srcIndex++];
            flush(DisLineSyntaxClass::Normal, buf);
            buf.clear();
        }
        if (isNumber) {
            while (((c >= L'0') && (c <= L'9')) || ((c >= L'a') && (c <= L'f')) || ((c >= L'A') && (c <= L'F'))) {
                buf += c;
                c = szText[srcIndex++];
            }
            flush(DisLineSyntaxClass::Number, buf);
            buf.clear();
        }
    } while (c == L',');

    // The rest of the line.
    if (c == L';') {
        flush(DisLineSyntaxClass::Comment, &szText[srcIndex - 1]);
        return result;
    }
    // Guard against reading past the terminator: if the operand loop
    // above already consumed exactly to end-of-string, c is '\0' here,
    // and unconditionally doing buf += c; c = szText[srcIndex++] (as
    // this used to) appends the NUL into buf and then reads one slot
    // past it - an out-of-bounds read into whatever heap memory follows
    // the caller's string, which could render as garbage text.
    if (c != L'\0') {
        buf += c;
        c = szText[srcIndex++];
        while ((c != L'\0') && (c != L';')) { buf += c; c = szText[srcIndex++]; }
    }
    flush(DisLineSyntaxClass::Normal, buf);
    if (c == L';') {
        flush(DisLineSyntaxClass::Comment, &szText[srcIndex - 1]);
    }

    return result;
}
