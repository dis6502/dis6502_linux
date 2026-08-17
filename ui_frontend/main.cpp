// dis6502 - ImGui/SDL2 front end
//
// This replaces the original Win32 MainWindow/DisassemblyWindow/
// DisassemblyControlImpl with an ImGui-based UI, driving the same
// (unmodified) core engine that was ported and verified separately
// (see ../port/ core build + test_runner_bin, which passes the
// project's real unit test suite against real fixture files).
//
// Scope of this skeleton: load a file from the command line, run the
// real Disassembly pass, and render the resulting disassembly listing
// with the same syntax coloring as the original DisassemblyControlImpl
// (via DisassemblyLineColorizer.h, itself verified against real engine
// output in colorizer_verify.cpp). A segment list side panel lets you
// jump between segments. File>Open, memory inspector, and the various
// modal dialogs (equates, comments, etc.) are NOT implemented here -
// see ROADMAP.md for what's next.

#include <SDL.h>
#include <cstdint>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <cstdio>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <iterator>
#include <algorithm>

#include "Application.h"
#include "Workspace.h"
#include "WorkspaceLogic.h"
#include "Disassembly.h"
#include "DisassemblyProgressMonitor.h"
#include "DisassemblyResult.h"
#include "DisassemblyLine.h"
#include "FileType.h"
#include "DisassemblyResultFile.h"
#include "File.h"
#include "IO.h"
#include "OutputStream.h"
#include "Segment.h"
#include "SegmentList.h"
#include "systems/ComputerSystemFactory.h"
#include "systems/ComputerSystem.h"
#include "DefaultFolders.h"
#include "DefaultFoldersLogic.h"
#include "FolderType.h"
#include "Profile.h"
#include "XML.h"
#include "Encoding.h"
#include "MemoryInspectorSelection.h"
#include "MemoryType.h"
#include "EquateList.h"
#include "Equate.h"
#include "MiniAssembler.h"
#include "SpriteMode.h"
#include "String.h"

#include "DisassemblyLineColorizer.h"
#include "FntFont.h"
#include "ComputerFontAtlas.h"
#include "NativeFileDialog.h"

namespace {

ImVec4 ColorFor(DisLineSyntaxClass syntaxClass) {
    // Byte-exact palette, measured directly from a screenshot of the
    // original app (RGB values divided by 255, same convention as
    // ColorForMemoryType below): Comment=(0,128,0), Number=(128,0,0),
    // Instruction=(0,0,128). A prior pass here deliberately dimmed these
    // to ~90/255 on the theory that the full 128/255 "looked not dark
    // enough" - backwards from what the original actually does; String
    // isn't visible in the reference screenshot (no string literals in
    // that sample) but follows the same one-shade-per-class pattern as
    // Number/Instruction, so it gets the analogous (128,0,128).
    switch (syntaxClass) {
        case DisLineSyntaxClass::Normal:       return ImVec4(0.0f,          0.0f,          0.0f,          1.0f);
        case DisLineSyntaxClass::Comment:      return ImVec4(0.0f,          128/255.0f,    0.0f,          1.0f);
        case DisLineSyntaxClass::Number:       return ImVec4(128/255.0f,    0.0f,          0.0f,          1.0f);
        case DisLineSyntaxClass::String:       return ImVec4(128/255.0f,    0.0f,          128/255.0f,    1.0f);
        case DisLineSyntaxClass::Instruction:  return ImVec4(0.0f,          0.0f,          128/255.0f,    1.0f);
        case DisLineSyntaxClass::Unreferenced: return ImVec4(158/255.0f,    158/255.0f,    158/255.0f,    1.0f);
    }
    return ImVec4(1, 1, 1, 1);
}

// Same Colors[] table (by MemoryType enum index) as the original
// MemoryInspectorControlImpl.cpp, RGB values divided by 255.
ImVec4 ColorForMemoryType(MemoryType memoryType) {
    static const ImVec4 colors[] = {
        ImVec4(0,   0,   0,   1),   // UNKNOWN
        ImVec4(192/255.0f, 192/255.0f, 192/255.0f, 1), // LOBYTE
        ImVec4(128/255.0f, 128/255.0f, 128/255.0f, 1), // HIBYTE
        ImVec4(128/255.0f, 0,   0,   1),               // BYTE
        ImVec4(128/255.0f, 0,   128/255.0f, 1),         // WORD
        ImVec4(128/255.0f, 128/255.0f, 0,   1),         // LABEL
        ImVec4(255/255.0f, 127/255.0f, 0,   1),         // STRING
        ImVec4(0,   127/255.0f, 255/255.0f, 1),         // SBYTE
        ImVec4(0,   128/255.0f, 0,   1),                // DLIST
        ImVec4(255/255.0f, 128/255.0f, 255/255.0f, 1),  // STORE
        ImVec4(0,   0,   128/255.0f, 1),                // CODE
        ImVec4(255/255.0f, 0,   128/255.0f, 1),         // SYMBOL
        ImVec4(255/255.0f, 0,   255/255.0f, 1),         // FIXUP
    };
    return colors[static_cast<int>(memoryType)];
}

// Call right after ImGui::Begin() for any floating (non-menu-bar) window.
// Without this, a window can be dragged up so far its title bar slides
// under the main menu bar, which then eats the menu bar's own clicks
// (whichever window is on top gets the mouse). This just snaps the
// window back down to sit right below the menu bar if it's ever dragged
// above that line - it doesn't otherwise restrict where the window can go.
void ClampWindowBelowMenuBar() {
    float menuBarHeight = ImGui::GetFrameHeight();
    ImVec2 pos = ImGui::GetWindowPos();
    if (pos.y < menuBarHeight) {
        ImGui::SetWindowPos(ImVec2(pos.x, menuBarHeight));
    }
}

// Very rough file-type guess from extension, standing in for the real
// FileOpenDialog's logic until a proper Open dialog is wired up (see
// ROADMAP.md - nativefiledialog-extended is the suggested library).
FileType GuessFileType(const std::string& path) {
    auto dot = path.find_last_of('.');
    std::string ext = dot == std::string::npos ? "" : path.substr(dot);
    for (auto& c : ext) c = static_cast<char>(tolower(c));
    if (ext == ".atr" || ext == ".xfd") return FileType::DISK_IMAGE_EXECUTABLE_FILE;
    return FileType::EXECUTABLE_FILE;
}

// Wraps a std::string so every assignment to it also appends to a log
// history - this is how the original's bottom "Log" panel (see the
// reference screenshot) gets its message history, without needing to
// touch every one of the ~25 "statusMessage = ..." call sites scattered
// through this file individually. Reads (empty(), c_str(), implicit
// conversion) behave like a plain std::string.
struct LoggedString {
    std::string value;
    std::vector<std::string>* log = nullptr;

    LoggedString& operator=(const std::string& s) {
        value = s;
        if (log) log->push_back(value);
        return *this;
    }
    LoggedString& operator=(const char* s) { return *this = std::string(s); }

    operator const std::string&() const { return value; }
    const char* c_str() const { return value.c_str(); }
    bool empty() const { return value.empty(); }
};

struct AppState {
    ComputerSystemFactory computerSystemFactory;
    std::unique_ptr<Workspace> workspace;
    WorkspaceLogic workspaceLogic;
    DisassemblyResult::LineNumber selectedLine = 0;
    LoggedString statusMessage;
    std::vector<std::string> logMessages;

    // "Double Font Height" (ID_VIEW_DOUBLE_FONT_HEIGHT in the original)
    // was a genuine UI-only setting there too - it swapped in a taller
    // bitmap font for the disassembly view, not a Workspace/Profile
    // property. ImGui doesn't have a second font loaded for this
    // skeleton, so this just scales the existing font instead - a
    // reasonable ImGui-native equivalent, not a byte-for-byte match.
    bool doubleFontHeight = false;

    // Set from main() after loading the bundled bitmap font into ImGui's
    // atlas (see ComputerFontAtlas.h) - null if that load failed, in
    // which case views fall back to ImGui's default font.
    ImFont* computerFont = nullptr;

    // The About dialog's two bitmaps (see DrawMenuBar's "About DIS6502"
    // popup) - set from main() after loading assets/dis6502-icon.bmp and
    // assets/alfred.bmp via SDL_LoadBMP. Null if a load failed, in which
    // case that image is just skipped rather than shown broken.
    SDL_Texture* aboutIconTexture = nullptr;
    ImVec2 aboutIconSize{};
    SDL_Texture* aboutAlfredTexture = nullptr;
    ImVec2 aboutAlfredSize{};

    bool showProfilePanel = false;
    bool showDefaultFoldersPanel = false;
    DefaultFoldersLogic defaultFoldersLogic;
    std::unique_ptr<DefaultFolders> defaultFolders;

    std::unique_ptr<MemoryInspectorSelection> memoryInspectorSelection;

    // Anchor offset for click-and-drag range selection in the Memory
    // Inspector (see DrawMemoryInspectorPanelContent) - set when the
    // mouse button is first pressed on a byte cell, then used every
    // frame the button stays held to extend the selection to whichever
    // cell is currently under the mouse.
    Memory::offset memoryDragAnchor = 0;

    // Set to true whenever the Memory Inspector selection changes the
    // Disassembly view's target line, so DrawDisassemblyViewContent
    // knows to scroll there on the next frame it renders.
    bool scrollToSelectedLineRequested = false;

    // Set to true whenever a disassembly line selection changes the
    // Memory Inspector's target offset, so DrawMemoryInspectorPanelContent
    // knows to scroll there on the next frame it renders.
    bool scrollToMemorySelectionRequested = false;

    // Jumps the Memory Inspector to the given disassembly line's
    // segment/byte range - shared by both the Cross References panel
    // (which already did this) and clicking a line directly in the
    // Disassembly view (which didn't, until now).
    void SyncMemoryInspectorToDisassemblyLine(const DIS_LINE* line) {
        if (!line || line->segmentIndex == NO_SEGMENT_INDEX) return;
        workspace->GetSegmentList()->SetSelectedIndex(line->segmentIndex);
        if (line->size != 0) {
            memoryInspectorSelection->SetSelection(line->offset, line->offset + line->size - 1);
            scrollToMemorySelectionRequested = true;
        }
    }

    // Finds the disassembly line corresponding to the current Memory
    // Inspector selection's start and jumps the Disassembly view there -
    // the reverse direction of what Cross References already did
    // (selecting a reference jumps the Memory Inspector); this makes
    // selecting bytes in the Memory Inspector jump the Disassembly view
    // to match.
    void SyncDisassemblyToMemorySelection() {
        auto* selection = memoryInspectorSelection.get();
        if (!selection->HasSegment()) return;
        DisassemblyResult::LineNumber lineNumber =
            workspace->GetDisassemblyResult()->SelectLine(selection->GetSegmentIndex(), selection->GetBegin());
        if (lineNumber != 0) {
            SelectDisassemblyLine(lineNumber);
            scrollToSelectedLineRequested = true;
        }
    }

    // XRef panel. Two ways results get populated:
    // 1. Automatically, whenever the Disassembly selection changes (see
    //    RefreshXRefForSelectedLine below) - lists every other line
    //    referencing the selected line's own label as a token.
    // 2. Manually via the Find box below, a plain case-insensitive
    //    substring search across every disassembled line - handy when
    //    the selected line has no label of its own to cross-reference,
    //    or you're hunting for something not related to the current
    //    selection at all. Both share the same xrefResults list.
    std::vector<DisassemblyResult::LineNumber> xrefResults;
    char xrefSearchBuffer[256] = {};
    // What the results currently shown are "References to" - empty when
    // they came from a manual Find instead. Display-only.
    std::string xrefTargetLabel;

    // Same leading-identifier rule as ColorizeDisassemblyLine's label
    // detection (see DisassemblyLineColorizer.h): a line "defines" a
    // label if it starts with a letter, @ or _, up to the first space.
    // Directive continuations, "org", and comment lines don't start
    // this way, so they have no label of their own.
    static std::wstring GetLineLabel(const wchar_t* text) {
        if (!text || !text[0]) return L"";
        wchar_t c = text[0];
        bool isLabelStart = (c == L'@') || (c == L'_') ||
            ((c >= L'a') && (c <= L'z')) || ((c >= L'A') && (c <= L'Z'));
        if (!isLabelStart) return L"";
        size_t end = 0;
        while (text[end] != L'\0' && text[end] != L' ') end++;
        return std::wstring(text, end);
    }

    static bool IsLabelIdentChar(wchar_t c) {
        return ((c >= L'a') && (c <= L'z')) || ((c >= L'A') && (c <= L'Z')) ||
            ((c >= L'0') && (c <= L'9')) || (c == L'_') || (c == L'@');
    }

    // Whether `label` appears in `text` as a standalone token - e.g.
    // "L942E" inside ".word L942E" matches, but inside "L942EXTRA" (a
    // different, longer label) it doesn't.
    static bool ContainsLabelToken(const std::wstring& text, const std::wstring& label) {
        if (label.empty()) return false;
        size_t pos = 0;
        while ((pos = text.find(label, pos)) != std::wstring::npos) {
            bool leftOk = (pos == 0) || !IsLabelIdentChar(text[pos - 1]);
            size_t endPos = pos + label.size();
            bool rightOk = (endPos >= text.size()) || !IsLabelIdentChar(text[endPos]);
            if (leftOk && rightOk) return true;
            pos += 1;
        }
        return false;
    }

    // Populates xrefResults with every line referencing the currently
    // selected line's own label (including the defining line itself, so
    // the panel also confirms where the label is defined) - the engine
    // has no address-level "who references this" index (see the XRef
    // panel comment above), so this works at the text level, the same
    // way ColorizeDisassemblyLine already parses these lines. Clears the
    // results if the selected line has no label to cross-reference.
    void RefreshXRefForSelectedLine() {
        xrefResults.clear();
        xrefTargetLabel.clear();
        auto* result = workspace->GetDisassemblyResult();
        DIS_LINE* line = result->SelectLine(selectedLine);
        if (!line) return;
        std::wstring label = GetLineLabel(line->GetLine());
        if (label.empty()) return;
        xrefTargetLabel = String::wstring_to_utf8(label);
        for (auto it = result->CreateConstLineIterator(); it->HasNext();) {
            const DIS_LINE* candidate = it->Next();
            if (ContainsLabelToken(candidate->GetLine(), label)) {
                xrefResults.push_back(candidate->GetLineNumber());
            }
        }
    }

    // Shared by the Disassembly view, the Segments list, and Memory
    // Inspector sync - anywhere the Disassembly selection changes -
    // so Cross References always reflects whatever's currently selected
    // there, however the selection got there.
    void SelectDisassemblyLine(DisassemblyResult::LineNumber lineNumber) {
        selectedLine = lineNumber;
        RefreshXRefForSelectedLine();
    }

    bool showEquateEditorPanel = false;
    char equateAddBuffer[256] = {};

    bool showDisplaySystemEquatesPanel = false;

    bool showAddressRangePanel = false;
    int addressRangeBaseEquateIndex = 0;
    char addressRangeStartBuffer[16] = {};
    char addressRangeEndBuffer[16] = {};

    bool commentPopupOpen = false;
    char commentBuffer[1024] = {};
    SEGMENT_INDEX commentSegmentIndex = NO_SEGMENT_INDEX;
    Memory::offset commentOffset = 0;
    Memory::size commentSize = 0;

    // Same deferred-open pattern as commentPopupOpen above, and for the
    // same reason: ImGui::OpenPopup("About") can't be called directly
    // from the MenuItem click below (see the About popup's own comment),
    // since that's called from inside BeginMainMenuBar()/BeginMenu("Help")'s
    // ID-stack context - a different one from where BeginPopupModal("About",
    // ...) runs afterward - so the popup IDs would never match and the
    // popup would silently never open.
    bool aboutPopupOpen = false;

    void OpenCommentPopup(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size) {
        commentSegmentIndex = segmentIndex;
        commentOffset = offset;
        commentSize = size;
        wstring existing = workspace->GetSegmentList()->GetUserComment(segmentIndex, offset, size);
        std::string utf8 = String::wstring_to_utf8(existing);
        std::snprintf(commentBuffer, sizeof(commentBuffer), "%s", utf8.c_str());
        commentPopupOpen = true;
    }

    // Byte-pattern search over raw memory (as opposed to the disassembly-
    // text search in the Cross References panel) - see
    // ui_todo/MemoryInspector.cpp's FindString/SearchString/FindNextString
    // for the original this mirrors.
    bool showFindPanel = false;

    bool showSpriteViewPanel = false;
    int spriteViewMode = 7; // one of the 8 modes in SpriteMode.h, default to the plain 1bpp/8-per-byte one
    char findAsciiBuffer[64] = {};
    bool findAllSegments = false;
    struct {
        SEGMENT_INDEX segmentIndex = NO_SEGMENT_INDEX;
        Memory::offset offset = 0;
        Memory::size size = 0;
        bool findWhole = false;
    } finder;

    bool SearchInSegment(SEGMENT_INDEX segmentIndex, const Segment* segment, const std::string& pattern) {
        Memory::size size = segment->GetSize();
        if (finder.size == 0 || size < finder.size) return false;

        for (Memory::offset offset = finder.offset; offset + finder.size <= size; offset++) {
            bool match = true;
            for (Memory::size i = 0; i < finder.size; i++) {
                if (segment->GetData(offset + i) != static_cast<byte>(pattern[i])) { match = false; break; }
            }
            if (match) {
                finder.segmentIndex = segmentIndex;
                finder.offset = offset + 1; // resume just past this match next time
                if (segmentIndex != memoryInspectorSelection->GetSegmentIndex()) {
                    workspace->GetSegmentList()->SetSelectedIndex(segmentIndex);
                }
                memoryInspectorSelection->SetSelection(offset, offset + finder.size - 1);
                return true;
            }
        }
        return false;
    }

    bool FindNextString(const std::string& pattern) {
        if (finder.segmentIndex == NO_SEGMENT_INDEX) return false;
        auto segmentList = workspace->GetSegmentList();
        SEGMENT_COUNT count = segmentList->GetCount();
        for (SEGMENT_INDEX segmentIndex = finder.segmentIndex; segmentIndex < count; segmentIndex++) {
            auto segment = segmentList->GetConstSegment(segmentIndex);
            if (SearchInSegment(segmentIndex, segment, pattern)) {
                statusMessage = "Found";
                return true;
            }
            if (!finder.findWhole) break;
            finder.offset = 0;
        }
        statusMessage = "String not found: " + pattern;
        return false;
    }

    // Starts a fresh search (as opposed to continuing one via
    // FindNextString directly) - mirrors MemoryInspector::FindString().
    void FindString(const std::string& pattern, bool allSegments) {
        finder.segmentIndex = allSegments ? 0 : memoryInspectorSelection->GetSegmentIndex();
        finder.offset = 0;
        finder.size = pattern.size();
        finder.findWhole = allSegments;
        FindNextString(pattern);
    }

    // Byte-type editing (MemoryType::BYTE/WORD/STRING/CODE/etc.) - see
    // ui_todo/MemoryInspector.cpp's SetType/AskByte for the original.
    // LOBYTE/HIBYTE needs a follow-up popup asking for the paired byte's
    // value (see ApplyLowHighByte below and ui_todo/LowHighByteDialog.cpp
    // for the original popup this replaces), so SetMemoryType opens that
    // popup rather than applying immediately in that case.
    bool lowHighBytePopupOpen = false;
    MemoryType lowHighByteType = MemoryType::LOBYTE;
    byte lowHighByteKnownByte = 0;
    char lowHighByteUnknownBuffer[8] = {};

    void SetMemoryType(MemoryType bType) {
        auto* selection = memoryInspectorSelection.get();
        if (selection->IsEmpty()) {
            statusMessage = "No selection";
            return;
        }
        if (!selection->segment->bBinary) {
            statusMessage = "Error: segment is not a binary/code segment";
            return;
        }

        Segment* segment = selection->segment;
        Memory::offset wBegin = selection->GetBegin();
        Memory::size wSize = selection->GetSize();

        if (bType == MemoryType::LOBYTE || bType == MemoryType::HIBYTE) {
            if (wBegin == 0) {
                statusMessage = "Error: cannot set low/high byte type on a segment's first byte";
                return;
            }
            if (wSize != 1) {
                statusMessage = "Error: select exactly one byte to set its low/high byte type";
                return;
            }
            byte previousOpcode = segment->GetData(wBegin - 1);
            auto instructionSet = workspace->GetInstructionSet(segment->processorType);
            if (instructionSet->GetInstruction(previousOpcode).GetOperandMode() != OperandMode::Immediate) {
                statusMessage = "Error: preceding byte is not an immediate-mode opcode";
                return;
            }

            lowHighByteType = bType;
            lowHighByteKnownByte = segment->GetData(wBegin);
            lowHighByteUnknownBuffer[0] = '\0';
            lowHighBytePopupOpen = true;
            return;
        }

        // NOTE: the original's equivalent check here has a comment
        // "&& not at the end of buffer" but never actually implements
        // it, meaning SetType(wBegin + wSize, ...) could write one past
        // the end of the segment's buffer if wBegin+wSize lands exactly
        // on the last byte. That's an out-of-bounds write risk, not just
        // a behavior quirk, so - unlike the deliberately-preserved `|`-
        // means-AND quirk in MiniAssembler - this bound check is added
        // rather than faithfully reproducing the gap.
        if (wBegin > 0) {
            if (segment->IsType(wBegin - 1, MemoryType::LOBYTE) || segment->IsType(wBegin - 1, MemoryType::HIBYTE)) {
                segment->SetType(wBegin - 1, MemoryType::CODE);
            }
            if (wBegin + wSize < segment->GetSize() &&
                (segment->IsType(wBegin + wSize - 1, MemoryType::LOBYTE) || segment->IsType(wBegin + wSize - 1, MemoryType::HIBYTE))) {
                segment->SetType(wBegin + wSize, MemoryType::CODE);
            }
        }

        segment->SetType(wBegin, bType, wSize);
        RunDisassembly();
        statusMessage = "Type updated";
    }

    void ApplyLowHighByte(byte unknownByte) {
        auto* selection = memoryInspectorSelection.get();
        Segment* segment = selection->segment;
        Memory::offset wBegin = selection->GetBegin();
        // The type of the immediate opcode before the known byte is set
        // to LOBYTE/HIBYTE; the user-supplied unknown byte value itself
        // is bit-packed directly into the operand's MemoryType field -
        // matches the original exactly, unusual as that encoding is.
        segment->SetType(wBegin - 1, lowHighByteType);
        segment->SetType(wBegin, static_cast<MemoryType>(unknownByte));
        RunDisassembly();
        statusMessage = "Type updated";
    }

    // Saves the current selection's raw bytes to a file, optionally
    // preceded by a 6-byte Atari binary-load header (0xFFFF marker +
    // little-endian start/end addresses) - the standard format
    // FileHeader::ATARI_BINARY/Segment::IsHeader() elsewhere in the
    // engine already recognizes when loading files back in.
    void SaveSelectionToFile(bool withHeader) {
        auto* selection = memoryInspectorSelection.get();
        if (selection->IsEmpty()) { statusMessage = "No selection"; return; }

        auto path = NativeFileDialog::SaveFile(FileType::RAW_FILE);
        if (!path) return;

        try {
            OutputStream* outputStream = OutputStream::OpenFile(String::utf8_to_wstring(*path), Encoding::BINARY);
            try {
                if (withHeader) {
                    Memory::address startAddr, endAddr;
                    selection->GetAddressRage(startAddr, endAddr);
                    uint8_t header[6] = {
                        0xFF, 0xFF,
                        static_cast<uint8_t>(startAddr & 0xFF), static_cast<uint8_t>((startAddr >> 8) & 0xFF),
                        static_cast<uint8_t>(endAddr & 0xFF), static_cast<uint8_t>((endAddr >> 8) & 0xFF),
                    };
                    outputStream->Write(header, sizeof(header));
                }
                auto bytes = selection->GetByteSequence();
                outputStream->Write(bytes->getConst(), static_cast<long>(bytes->size()));
            } catch (...) {
                delete outputStream;
                throw;
            }
            delete outputStream;
            statusMessage = "Saved selection: " + *path;
        } catch (const IOException& ex) {
            statusMessage = "Failed to save selection: " + String::wstring_to_utf8(ex.GetText());
        }
    }

    bool assemblePopupOpen = false;
    char assembleInstructionBuffer[128] = {};
    std::string assembleResultMessage;

    void OpenDefaultFoldersPanel() {
        if (!defaultFolders) {
            auto typeInfo = *workspace->GetComputerSystem()->GetTypeInfo();
            defaultFolders = defaultFoldersLogic.CreateDefaultFolders(typeInfo);
            defaultFoldersLogic.Load(*defaultFolders);
        }
        showDefaultFoldersPanel = true;
    }

    AppState() {
        statusMessage.log = &logMessages;
        NewWorkspace();
    }

    void OpenFile(FileType fileType) {
        auto path = NativeFileDialog::OpenFile(fileType);
        if (path) LoadFile(*path, fileType);
    }

    void OpenWorkspace() {
        auto path = NativeFileDialog::OpenFile(FileType::WORKSPACE_FILE);
        if (path) OpenWorkspaceFrom(*path);
    }

    // saveAs mirrors the original MainFile::SaveWorkspaceFile(bSaveAs):
    // if the workspace already has a file path and this isn't "Save As",
    // save straight back to it without prompting.
    void SaveWorkspace(bool saveAs) {
        wstring existingPath = workspace->GetFilePath();
        if (!existingPath.empty() && !saveAs) {
            SaveWorkspaceTo(String::wstring_to_utf8(existingPath));
            return;
        }
        auto path = NativeFileDialog::SaveFile(FileType::WORKSPACE_FILE);
        if (path) SaveWorkspaceTo(*path);
    }

    void SaveDisassemblyFiles() {
        auto path = NativeFileDialog::SaveFile(FileType::DISASSEMBLY_FILE);
        if (path) SaveDisassemblyFilesTo(*path);
    }

    // Generic Yes/No confirmation, used before destructive actions like
    // clearing equates (see ui_todo/EquateListLogic.cpp's Clear(), which
    // shows a MessageBox before proceeding - this is that confirmation,
    // ported to a real popup rather than skipped).
    bool confirmPopupOpen = false;
    std::string confirmMessage;
    std::function<void()> confirmAction;

    void RequestConfirm(const std::string& message, std::function<void()> action) {
        confirmMessage = message;
        confirmAction = std::move(action);
        confirmPopupOpen = true;
    }

    void ClearSystemEquates() {
        workspace->GetSystemEquateList()->Clear();
        statusMessage = "Cleared system equates";
    }

    void ClearUserEquates() {
        workspace->GetUserEquateList()->Clear();
        statusMessage = "Cleared user equates";
    }

    void OpenUserEquates() {
        auto path = NativeFileDialog::OpenFile(FileType::EQUATES_FILE);
        if (path) OpenUserEquatesFrom(*path);
    }

    void SaveUserEquates() {
        auto path = NativeFileDialog::SaveFile(FileType::EQUATES_FILE);
        if (path) SaveUserEquatesTo(*path, false);
    }

    void ExportUserEquates() {
        auto path = NativeFileDialog::SaveFile(FileType::EQUATES_FILE);
        if (path) SaveUserEquatesTo(*path, true);
    }

    void OpenUserEquatesFrom(const std::string& path) {
        bool ok = workspace->GetUserEquateList()->Load(String::utf8_to_wstring(path));
        statusMessage = ok ? ("Opened user equates: " + path) : ("Failed to open user equates: " + path);
    }

    // xasm selects the file format Save() writes: false is the native
    // dis6502 format (used by "Save User Equates..."), true writes an
    // xasm-assembler-compatible listing (used by "Export User Equates...").
    void SaveUserEquatesTo(const std::string& path, bool xasm) {
        workspace->GetUserEquateList()->Save(String::utf8_to_wstring(path), xasm);
        statusMessage = (xasm ? "Exported user equates: " : "Saved user equates: ") + path;
    }

    // Same Load/Save split as OpenUserEquates/SaveUserEquates above, using
    // XML::Load/XML::Save directly on the Workspace's Profile (see
    // ui_todo/ProfileLogic.cpp's Load(profile, filePath)/Save(profile,
    // parentWindow), which round-trip through the same "Profile" XML
    // element via Profile::SerializeTo/DeserializeFrom in Profile.cpp).
    void LoadProfile() {
        auto path = NativeFileDialog::OpenFile(FileType::PROFILE_FILE);
        if (path) LoadProfileFrom(*path);
    }

    void LoadProfileFrom(const std::string& path) {
        auto result = XML::Load(*workspace->GetProfile(), L"Profile", String::utf8_to_wstring(path));
        statusMessage = (result == tinyxml2::XML_SUCCESS)
            ? ("Loaded profile: " + path)
            : ("Failed to load profile: " + path);
    }

    void SaveProfile() {
        auto path = NativeFileDialog::SaveFile(FileType::PROFILE_FILE);
        if (path) SaveProfileTo(*path);
    }

    void SaveProfileTo(const std::string& path) {
        auto result = XML::Save(*workspace->GetProfile(), L"Profile", String::utf8_to_wstring(path));
        statusMessage = (result == tinyxml2::XML_SUCCESS)
            ? ("Saved profile: " + path)
            : ("Failed to save profile: " + path);
    }

    // Backing store for the Profile panel's Cancel button. Profile
    // publicly inherits XML::Serializable (see Profile.h), whose copy
    // constructor/assignment are explicitly deleted (see XML.h), so a
    // second Profile can't just be copy-assigned as a backup - instead
    // this snapshots the same "Profile" XML element Save/Load use, in
    // memory, via an XMLPrinter round-trip, and Cancel deserializes it
    // straight back into the live Profile the panel was editing.
    std::string profileSnapshotXml;

    void SnapshotProfile() {
        tinyxml2::XMLDocument doc;
        doc.SetBOM(false);
        auto* element = doc.NewElement("Profile");
        workspace->GetProfile()->SerializeTo(*element);
        doc.InsertFirstChild(element);
        tinyxml2::XMLPrinter printer;
        doc.Print(&printer);
        profileSnapshotXml = printer.CStr();
    }

    void RestoreProfileSnapshot() {
        if (profileSnapshotXml.empty()) return;
        tinyxml2::XMLDocument doc;
        if (doc.Parse(profileSnapshotXml.c_str()) == tinyxml2::XML_SUCCESS) {
            workspace->GetProfile()->DeserializeFrom(*doc.RootElement());
        }
    }

    void NewWorkspace() {
        workspace = std::make_unique<Workspace>(computerSystemFactory);
        workspace->SetComputerSystemType(ComputerSystemType::ATARI800); // TODO: let the user pick
        memoryInspectorSelection = std::make_unique<MemoryInspectorSelection>(*workspace);
        xrefResults.clear();
        selectedLine = 0;
        statusMessage = "New workspace";
    }

    void OpenWorkspaceFrom(const std::string& path) {
        NewWorkspace();
        bool ok = workspaceLogic.Load(*workspace, String::utf8_to_wstring(path));
        statusMessage = ok ? ("Opened workspace: " + path) : ("Failed to open workspace: " + path);
        if (ok) selectedLine = 1;
    }

    void SaveWorkspaceTo(const std::string& path) {
        // WORKSPACE36 is the current/modern format (see ui_todo/MainFile.cpp's
        // SaveWorkspaceFile, which always saves as WORKSPACE36 - WORKSPACE14
        // is a legacy format Workspace1X.cpp reads for backward compatibility).
        bool ok = workspaceLogic.Save(*workspace, String::utf8_to_wstring(path), Workspace::Format::WORKSPACE36);
        statusMessage = ok ? ("Saved workspace: " + path) : ("Failed to save workspace: " + path);
    }

    void SaveDisassemblyFilesTo(const std::string& path) {
        DisassemblyResultFile disassemblyResultFile;
        try {
            disassemblyResultFile.SaveListing(*workspace->GetConstDisassemblyResult(),
                                               *workspace->GetConstProfile(),
                                               File(String::utf8_to_wstring(path)));
            statusMessage = "Saved disassembly listing: " + path;
        } catch (const IOException& ex) {
            statusMessage = "Failed to save disassembly listing: " + String::wstring_to_utf8(ex.GetText());
        }
    }

    void LoadFile(const std::string& path) {
        LoadFile(path, GuessFileType(path));
    }

    void LoadFile(const std::string& path, FileType fileType) {
        bool ok = workspaceLogic.AddFile(*workspace, fileType, String::utf8_to_wstring(path));
        if (!ok) {
            statusMessage = "Failed to load: " + path;
            return;
        }

        RunDisassembly();
        statusMessage = "Loaded: " + path;
        selectedLine = 1;
    }

    // Runs (or re-runs) the disassembly pass over the current workspace.
    // Needed not just after loading a file, but any time something that
    // affects disassembly output changes - e.g. adding an equate/label
    // or defining an address range - since Disassembly reads the
    // equate/segment state fresh each time rather than incrementally
    // updating an existing result.
    void RunDisassembly() {
        Disassembly disassembly;
        DisassemblyProgressMonitor progressMonitor;
        disassembly.SetProgressMonitor(progressMonitor);
        disassembly.SetWorkspace(*workspace);
        disassembly.StartDisassembly();
    }
};

void DrawSegmentListContent(AppState& state) {
    const auto& segmentList = state.workspace->GetSegmentList();
    SEGMENT_INDEX count = segmentList->GetCount();

    // Cyan-ish selected-row highlight, matching the reference screenshot
    // (the original uses a similar teal/cyan bar for the selected segment).
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.75f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.85f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 0.65f, 0.65f, 1.0f));
    // Own nested scrolling region, so the "Segments" header the caller
    // draws above this stays fixed/visible instead of scrolling away
    // with the list (a real bug reported after testing: "top info bar
    // is cut off and not accessible" - the pane's own header was
    // scrolling out of view along with its content).
    ImGui::BeginChild("SegmentsScroll", ImVec2(0, 0), false);
    for (SEGMENT_INDEX i = 0; i < count; i++) {
        auto segment = segmentList->GetSegment(i);
        bool selected = (segmentList->GetSelectedIndex() == i);
        std::string title = String::wstring_to_utf8(segment->szTitle);
        if (title.empty()) title = "(segment " + std::to_string(i) + ")";
        if (ImGui::Selectable(title.c_str(), selected)) {
            segmentList->SetSelectedIndex(i);
            state.SelectDisassemblyLine(segment->GetFirstLineNumber());
            state.scrollToSelectedLineRequested = true;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor(3);
}

void DrawDisassemblyViewContent(AppState& state) {
    auto* result = state.workspace->GetDisassemblyResult();
    auto lineCount = result->GetLineCount();

    if (ImGui::Button("Comment...")) {
        DIS_LINE* line = result->SelectLine(state.selectedLine);
        if (line && line->segmentIndex != NO_SEGMENT_INDEX) {
            state.OpenCommentPopup(line->segmentIndex, line->offset, line->size);
        }
    }

    ImGui::BeginChild("DisassemblyScroll", ImVec2(0, 0), false,
                       ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::SetWindowFontScale(state.doubleFontHeight ? 2.0f : 1.0f);
    if (state.computerFont) ImGui::PushFont(state.computerFont);

    if (state.scrollToSelectedLineRequested) {
        // Row height is uniform (one NewLine() per rendered row below),
        // so the target line's Y position is just its index times that
        // height - no need to actually render up to it first.
        float rowHeight = ImGui::GetTextLineHeightWithSpacing();
        float targetY = static_cast<float>(state.selectedLine - 1) * rowHeight;
        ImGui::SetScrollY(std::max(0.0f, targetY - ImGui::GetWindowHeight() * 0.5f));
        state.scrollToSelectedLineRequested = false;
    }

    // Yellow selected-line highlight, matching the reference screenshot.
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.9f, 0.9f, 0.0f, 1.0f));

    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(lineCount));
    while (clipper.Step()) {
        for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
            DisassemblyResult::LineNumber lineNumber = row + 1; // lines are 1-based
            DIS_LINE* line = result->SelectLine(lineNumber);
            if (!line) continue;

            bool selected = (lineNumber == state.selectedLine);
            ImGui::PushID(row);
            if (ImGui::Selectable("##row", selected, ImGuiSelectableFlags_AllowItemOverlap)) {
                state.SelectDisassemblyLine(lineNumber);
                state.SyncMemoryInspectorToDisassemblyLine(line);
            }
            ImGui::PopID();
            ImGui::SameLine(0, 0);

            // DIS_LINE::referenced ("Displayed in grey if not referenced",
            // see DisassemblyLine.h) is only ever actually computed for
            // SYSTEM_EQUATES lines (see Disassembly::
            // SetSystemEquateLinesReferencedBySystemAddress/
            // SetNearestSystemEquateLineReferencedByAddress in
            // Disassembly.cpp) - an unused system-defined constant gets
            // grayed out as informational noise reduction. It's never set
            // for CODE_LINES/USER_EQUATES/CODE_EQUATES lines, so those
            // default to false and would otherwise all render in that
            // same "unreferenced" gray regardless of section, discarding
            // the normal Instruction/Number/Comment coloring entirely.
            bool referenced = line->referenced ||
                (line->GetSection()->GetType() != DisassemblySectionType::SYSTEM_EQUATES);
            auto segments = ColorizeDisassemblyLine(line->GetLine(), referenced);
            for (auto& seg : segments) {
                if (seg.text.empty()) continue;
                ImGui::PushStyleColor(ImGuiCol_Text, ColorFor(seg.syntaxClass));
                ImGui::TextUnformatted(String::wstring_to_utf8(seg.text).c_str());
                ImGui::PopStyleColor();
                ImGui::SameLine(0, 0);
            }
            ImGui::NewLine();
        }
    }
    ImGui::PopStyleColor(3);
    if (state.computerFont) ImGui::PopFont();
    ImGui::EndChild();
}

// Real hex-dump view over the currently selected segment, using the
// already-portable MemoryInspectorSelection/Segment core classes.
// Bytes are colored by MemoryType using the same table as the original
// MemoryInspectorControlImpl. Click a byte to select it; shift-click
// extends the selection - a simpler interaction than the original's
// full click-drag range select, but drives the same underlying
// MemoryInspectorSelection::SetSelection().
// Right-click context menu for the Memory Inspector's hex grid, matching
// the original's exact menu structure/labels/shortcuts (confirmed
// against a screenshot of the real app). Several items are grayed-out
// stubs rather than real implementations - the user confirmed these
// aren't needed: Start code trace, Set Unknown block to Byte, Edit
// bytes at selection, Cut/Copy/Paste/Delete/Split, Select next Unknown
// block, and Select all. Everything else here is real and working.
void DrawMemoryInspectorContextMenu(AppState& state, MemoryInspectorSelection* selection) {
    bool hasSelection = selection->HasSelection();

    ImGui::MenuItem("Start code trace at selection", "Ctrl-T", false, false); // not implemented (not needed)

    if (ImGui::BeginMenu("Change type of selected bytes to", hasSelection)) {
        static const std::pair<const char*, MemoryType> typeOptions[] = {
            {"Code", MemoryType::CODE},
            {"Code with Low Byte", MemoryType::LOBYTE},
            {"Code with High Byte", MemoryType::HIBYTE},
            {"Byte", MemoryType::BYTE},
            {"Word", MemoryType::WORD},
            {"Label", MemoryType::LABEL},
            {"SpartaDos X Label", MemoryType::SYMBOL},
            {"SpartaDos X Address Fix-Up", MemoryType::FIXUP},
            {"String", MemoryType::STRING},
            {"Screen Byte", MemoryType::SBYTE},
            {"Display List", MemoryType::DLIST},
            {"Data Store", MemoryType::STORE},
        };
        MemoryType currentType = hasSelection ? selection->segment->GetType(selection->GetBegin()) : MemoryType::UNKNOWN;
        for (auto& opt : typeOptions) {
            if (ImGui::MenuItem(opt.first, nullptr, currentType == opt.second)) state.SetMemoryType(opt.second);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Unknown", nullptr, currentType == MemoryType::UNKNOWN)) state.SetMemoryType(MemoryType::UNKNOWN);
        ImGui::EndMenu();
    }

    ImGui::MenuItem("Set current block of Unknown type to Byte", nullptr, false, false); // not implemented (not needed)

    ImGui::Separator();
    if (ImGui::MenuItem("Add/Edit comment...", nullptr, false, hasSelection)) {
        state.OpenCommentPopup(selection->GetSegmentIndex(), selection->GetBegin(), selection->GetSize());
    }
    ImGui::MenuItem("Edit bytes at selection (same as double-click)", "F2", false, false); // not implemented (not needed)
    if (ImGui::MenuItem("Assemble at selection...", "F8", false, hasSelection)) {
        state.assembleResultMessage.clear();
        state.assembleInstructionBuffer[0] = '\0';
        state.assemblePopupOpen = true;
    }

    ImGui::Separator();
    ImGui::MenuItem("Cut", "Ctrl-X", false, false);                       // not implemented (not needed)
    ImGui::MenuItem("Copy", "Ctrl-C", false, false);                      // not implemented (not needed)
    ImGui::MenuItem("Paste (insert before selection)", "Ctrl-V", false, false); // not implemented (not needed)
    ImGui::MenuItem("Paste (insert after selection)", nullptr, false, false);   // not implemented (not needed)
    ImGui::MenuItem("Delete", "Del", false, false);                       // not implemented (not needed)
    ImGui::MenuItem("Split at selection", nullptr, false, false);         // not implemented (not needed)

    ImGui::Separator();
    if (ImGui::MenuItem("Find...", "Ctrl-F")) state.showFindPanel = true;
    if (ImGui::MenuItem("Find next", "F3", false, state.findAsciiBuffer[0] != '\0')) {
        state.FindNextString(state.findAsciiBuffer);
    }

    ImGui::Separator();
    ImGui::MenuItem("Select next block of Unknown type", "F5", false, false); // not implemented (not needed)
    if (ImGui::MenuItem("Select Sprites...", "F4", false, hasSelection)) state.showSpriteViewPanel = true;
    ImGui::MenuItem("Select all", "Ctrl-A", false, false);                    // not implemented (not needed)

    ImGui::Separator();
    if (ImGui::MenuItem("Save selection without header...", nullptr, false, hasSelection)) state.SaveSelectionToFile(false);
    if (ImGui::MenuItem("Save selection with header...", nullptr, false, hasSelection)) state.SaveSelectionToFile(true);
}

void DrawMemoryInspectorPanelContent(AppState& state) {
    auto selection = state.memoryInspectorSelection.get();
    auto segmentList = state.workspace->GetSegmentList();
    selection->SetSegmentIndex(segmentList->GetSelectedIndex());

    if (!selection->HasSegment()) {
        ImGui::TextUnformatted("No segment selected - pick one in the Segments panel.");
        return;
    }

    const Segment* segment = selection->GetConstSegment();
    Memory::size size = segment->GetSize();
    const int bytesPerRow = 8; // matches the reference screenshot's layout
    int rowCount = static_cast<int>((size + bytesPerRow - 1) / bytesPerRow);



    // The original MemoryInspectorControlImpl assumed a white background
    // (standard for a Win32 control) with its Colors[] table designed
    // against that - including pure black for MemoryType::UNKNOWN. The
    // global white ChildBg (see main()) already matches that; this panel
    // still needs its own black default text override, since ImGui's
    // light theme default text isn't pure black.
    ImGui::BeginChild("MemoryScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
    if (state.computerFont) ImGui::PushFont(state.computerFont);

    if (state.scrollToMemorySelectionRequested) {
        // Same idea as the Disassembly view's scroll-to-line: row height
        // is uniform, so the target row's Y position is just its index
        // (offset / bytesPerRow) times that height.
        float rowHeight = ImGui::GetTextLineHeightWithSpacing();
        float targetY = static_cast<float>(selection->GetBegin() / bytesPerRow) * rowHeight;
        ImGui::SetScrollY(std::max(0.0f, targetY - ImGui::GetWindowHeight() * 0.5f));
        state.scrollToMemorySelectionRequested = false;
    }

    if (ImGui::BeginPopupContextWindow("MemoryInspectorContextMenu")) {
        DrawMemoryInspectorContextMenu(state, selection);
        ImGui::EndPopup();
    }

    ImGuiListClipper clipper;
    clipper.Begin(rowCount);
    while (clipper.Step()) {
        for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
            Memory::offset rowStart = static_cast<Memory::offset>(row * bytesPerRow);
            Memory::address rowAddress = segment->wBegin + rowStart;

            ImGui::Text("%04X:", rowAddress);
            for (int col = 0; col < bytesPerRow; col++) {
                Memory::offset offset = rowStart + col;
                if (offset >= size) { ImGui::SameLine(0, 4); ImGui::TextUnformatted("   "); continue; }

                byte value = segment->GetData(offset);
                MemoryType memoryType = segment->GetType(offset);

                bool selected = selection->HasSelection() && offset >= selection->GetBegin() && offset <= selection->GetEnd();

                ImGui::SameLine(0, 4);
                ImGui::PushID(offset);
                char label[8];
                std::snprintf(label, sizeof(label), "%02X", value);
                ImGui::PushStyleColor(ImGuiCol_Text, ColorForMemoryType(memoryType));
                ImGui::Selectable(label, selected, ImGuiSelectableFlags_None, ImVec2(20, 0));

                bool selectionChanged = false;
                if (ImGui::IsItemActivated()) {
                    // Mouse button just went down on this cell.
                    if (ImGui::IsKeyDown(ImGuiMod_Shift) && selection->HasSelection()) {
                        // Shift-click: extend the existing selection and
                        // anchor the drag at whichever existing endpoint
                        // is farther from this click, so dragging from
                        // here continues to extend/shrink sensibly.
                        Memory::offset begin = selection->GetBegin() < offset ? selection->GetBegin() : offset;
                        Memory::offset end = selection->GetEnd() > offset ? selection->GetEnd() : offset;
                        selection->SetSelection(begin, end);
                        state.memoryDragAnchor = (offset == begin) ? end : begin;
                    } else {
                        state.memoryDragAnchor = offset;
                        selection->SetSelection(offset, offset);
                    }
                    selectionChanged = true;
                } else if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    // Button still held (click-and-drag) while hovering
                    // this cell - extend the selection from the anchor
                    // to here, covering however many bytes that spans.
                    Memory::offset begin = std::min(state.memoryDragAnchor, offset);
                    Memory::offset end = std::max(state.memoryDragAnchor, offset);
                    selection->SetSelection(begin, end);
                    selectionChanged = true;
                }
                if (selectionChanged) state.SyncDisassemblyToMemorySelection();
                ImGui::PopStyleColor();
                ImGui::PopID();
            }

            // Character-glyph column: each byte's raw value rendered
            // directly as a glyph in the same bitmap font (matching the
            // reference screenshot's divider + per-byte character
            // representation) - this is the "Display as Screen Code"
            // idea applied directly to raw memory, not through
            // disassembly text. Uses ImFont::RenderChar to draw by raw
            // ImWchar codepoint rather than building a text string,
            // since byte values 0x80-0xFF would otherwise be
            // misinterpreted as UTF-8 continuation bytes if passed
            // through ImGui's normal (UTF-8-decoding) text functions.
            if (state.computerFont) {
                ImGui::SameLine(0, 8);
                ImGui::TextUnformatted("|");
                ImGui::SameLine(0, 8);

                ImDrawList* drawList = ImGui::GetWindowDrawList();
                const float glyphCellWidth = 10.0f;
                for (int col = 0; col < bytesPerRow; col++) {
                    Memory::offset offset = rowStart + col;
                    if (col > 0) ImGui::SameLine(0, 0);
                    if (offset >= size) { ImGui::Dummy(ImVec2(glyphCellWidth, state.computerFont->FontSize)); continue; }

                    byte value = segment->GetData(offset);
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    state.computerFont->RenderChar(drawList, state.computerFont->FontSize, pos,
                                                    IM_COL32(0, 0, 0, 255), static_cast<ImWchar>(value));
                    ImGui::Dummy(ImVec2(glyphCellWidth, state.computerFont->FontSize));
                }
            }
        }
    }
    if (state.computerFont) ImGui::PopFont();
    ImGui::PopStyleColor(); // black default text
    ImGui::EndChild();
}

// Cross-reference list. Automatically populated from the Disassembly
// selection (AppState::RefreshXRefForSelectedLine, called by
// SelectDisassemblyLine anywhere the Disassembly selection changes) -
// every line referencing the selected line's own label as a token,
// including the defining line itself. The Find box below is a second,
// independent way to populate the same list: a plain case-insensitive
// substring search, for when the selected line has no label of its own,
// or you're looking for something unrelated to the current selection.
// Unlike the original (which marks disLine->xrefLineNumber during
// Pass1/label-reference resolution and lists exactly those pre-marked
// lines - see ui_todo/MainDisassembly.cpp's RefreshXRef), both paths
// here work at the text level, since the engine has no address-level
// reference index to draw on (see RefreshXRefForSelectedLine's comment).
void DrawXRefPanelContent(AppState& state) {
    bool search = ImGui::InputText("Find", state.xrefSearchBuffer, sizeof(state.xrefSearchBuffer),
                                    ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Search") || search) {
        state.xrefResults.clear();
        state.xrefTargetLabel.clear();
        std::string needle = state.xrefSearchBuffer;
        for (auto& c : needle) c = static_cast<char>(tolower(c));
        if (!needle.empty()) {
            auto* result = state.workspace->GetDisassemblyResult();
            for (auto it = result->CreateConstLineIterator(); it->HasNext();) {
                const DIS_LINE* line = it->Next();
                std::string text = String::wstring_to_utf8(line->GetLine());
                for (auto& c : text) c = static_cast<char>(tolower(c));
                if (text.find(needle) != std::string::npos) {
                    state.xrefResults.push_back(line->GetLineNumber());
                }
            }
        }
    }

    if (!state.xrefTargetLabel.empty()) {
        ImGui::TextDisabled("References to: %s", state.xrefTargetLabel.c_str());
    } else if (state.xrefResults.empty()) {
        ImGui::TextDisabled("Select a labeled line in Disassembly, or search above.");
    }

    ImGui::Separator();
    ImGui::BeginChild("XRefResults", ImVec2(0, 0), false);
    for (auto lineNumber : state.xrefResults) {
        DIS_LINE* line = state.workspace->GetDisassemblyResult()->SelectLine(lineNumber);
        if (!line) continue;
        std::string text = String::wstring_to_utf8(line->GetLine());
        ImGui::PushID(static_cast<int>(lineNumber));
        if (ImGui::Selectable(text.c_str(), lineNumber == state.selectedLine)) {
            // Jump the Disassembly view there and follow the segment/byte
            // range in Memory Inspector - deliberately doesn't call
            // SelectDisassemblyLine (which would re-run
            // RefreshXRefForSelectedLine and replace this very list out
            // from under you), so you can click through every result
            // without the reference list changing underneath you.
            state.selectedLine = lineNumber;
            state.scrollToSelectedLineRequested = true;
            state.SyncMemoryInspectorToDisassemblyLine(line);
        }
        ImGui::PopID();
    }
    ImGui::EndChild();
}

// Draws a full-width colored bar with a label, matching each pane's
// title color in the original screenshots (Segments=yellow,
// Disassembly=green, Memory Inspector=cyan, Cross References=pink/red,
// Log=lavender). These are approximate colors picked by eye from photos
// of the real app, not measured/exact values - same caveat as the other
// approximate color-matches elsewhere in this port.
void DrawColoredHeaderBar(const char* label, ImU32 bgColor, ImU32 textColor = IM_COL32(0, 0, 0, 255)) {
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    float width = ImGui::GetContentRegionAvail().x;
    float height = ImGui::GetTextLineHeightWithSpacing();
    ImGui::GetWindowDrawList()->AddRectFilled(p0, ImVec2(p0.x + width, p0.y + height), bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
}

// Fixed split-pane layout matching the original: Segments (top-left) +
// Memory Inspector (bottom-left) in a narrow left column, Disassembly
// (top-right) + Cross References (bottom-right) filling the rest, and
// a full-width Log strip along the bottom - one non-movable container
// window instead of the independent floating windows this used to be,
// since that's a closer match to the original's actual fixed layout
// (it isn't a set of movable/dockable panes either).
void DrawMainLayoutAndLog(AppState& state) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 pos = viewport->WorkPos;
    ImVec2 windowSize = viewport->WorkSize;

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("##MainLayout", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings);

    // Use the actual usable content area (windowSize minus this window's
    // own padding/border) rather than the raw window size - using the
    // raw size here was a real bug: it made the sum of the child pane
    // widths/heights slightly exceed what actually fit, pushing the
    // right column (and its scrollbar) past the visible edge of the
    // window ("disassembly frame too wide, vertical scrollbar off
    // screen" after testing).
    ImVec2 size = ImGui::GetContentRegionAvail();

    const float logHeight = 140.0f;
    // Widened from an earlier 300px - that was too narrow for the
    // Memory Inspector's 16-bytes-per-row hex grid (~450px minimum),
    // which is what "memory inspector window is too narrow" was
    // reporting.
    const float leftWidth = 480.0f;
    float contentHeight = size.y - logHeight;
    float topHeight = contentHeight * 0.65f;
    float bottomHeight = contentHeight - topHeight;
    float rightWidth = size.x - leftWidth - ImGui::GetStyle().ItemSpacing.x;

    // NoScrollbar/NoScrollWithMouse on all four outer panes: each pane's
    // header (drawn here) should never scroll away with its content -
    // every content function below manages its own inner scrolling
    // region sized to fit under the header instead.
    const ImGuiWindowFlags paneFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("SegmentsPane", ImVec2(leftWidth, topHeight), true, paneFlags);
    DrawColoredHeaderBar("Segments", IM_COL32(255, 215, 0, 255));
    ImGui::Separator();
    DrawSegmentListContent(state);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("DisassemblyPane", ImVec2(rightWidth, topHeight), true, paneFlags);
    DrawColoredHeaderBar("Disassembly", IM_COL32(0, 220, 0, 255));
    ImGui::Separator();
    DrawDisassemblyViewContent(state);
    ImGui::EndChild();

    ImGui::BeginChild("MemoryInspectorPane", ImVec2(leftWidth, bottomHeight), true, paneFlags);
    DrawColoredHeaderBar("Memory Inspector", IM_COL32(64, 224, 208, 255));
    ImGui::Separator();
    DrawMemoryInspectorPanelContent(state);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("XRefPane", ImVec2(rightWidth, bottomHeight), true, paneFlags);
    DrawColoredHeaderBar("Cross References", IM_COL32(255, 160, 160, 255));
    ImGui::Separator();
    DrawXRefPanelContent(state);
    ImGui::EndChild();

    // Log strip, full width, matching the original's bottom Log panel -
    // shows the message history LoggedString feeds (see its definition
    // above) rather than just the single latest status message.
    ImGui::BeginChild("LogPane", ImVec2(0, logHeight), true, paneFlags);
    DrawColoredHeaderBar("Log", IM_COL32(200, 200, 255, 255));
    ImGui::Separator();
    ImGui::BeginChild("LogScroll", ImVec2(0, 0), false);
    for (auto& msg : state.logMessages) {
        ImGui::TextUnformatted(msg.c_str());
    }
    if (!state.logMessages.empty()) ImGui::SetScrollHereY(1.0f); // auto-scroll to the latest entry
    ImGui::EndChild();
    ImGui::EndChild();

    ImGui::End();
}

// Straightforward field-by-field editor for Profile's ~30 public
// settings (see Profile.h). Grouped under the same section headings as
// the struct's own comments, so it reads the same as the source. wstring
// fields go through a small fixed buffer since ImGui::InputText needs a
// char* it can write into directly.
void DrawProfilePanel(AppState& state) {
    if (!state.showProfilePanel) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Profile", &state.showProfilePanel);
    ClampWindowBelowMenuBar();

    Profile* profile = state.workspace->GetProfile();

    auto textField = [](const char* label, wstring& field) {
        static char buf[512];
        std::string utf8 = String::wstring_to_utf8(field);
        std::snprintf(buf, sizeof(buf), "%s", utf8.c_str());
        if (ImGui::InputText(label, buf, sizeof(buf))) {
            field = String::utf8_to_wstring(buf);
        }
    };

    auto intField = [](const char* label, word& field) {
        static char buf[16];
        std::snprintf(buf, sizeof(buf), "%u", field);
        if (ImGui::InputText(label, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
            field = static_cast<word>(std::strtoul(buf, nullptr, 10));
        }
    };

    ImGui::SeparatorText("Source Layout");
    ImGui::Checkbox("Use Line Numbers", &profile->useLineNumbers);
    ImGui::Checkbox("Align Instructions", &profile->alignInstructions);
    ImGui::Checkbox("Show Lower Case Instructions", &profile->showLowerCaseInstructions);

    ImGui::SeparatorText("Opcodes");
    ImGui::Checkbox("Use Illegal Opcodes", &profile->useIllegalOpcodes);
    ImGui::Checkbox("Show A In Accumulator Mode", &profile->showAInAccumulatorMode);
    ImGui::Checkbox("Show Colon After Label", &profile->showColonAfterLabel);
    ImGui::Checkbox("Show Opcode As Comment", &profile->showOpcodeAsComment);
    ImGui::Checkbox("Show BRK As Byte 0", &profile->showBRKAsByte0);
    ImGui::Checkbox("Show ZP Absolute As Byte", &profile->showZPAbsoluteAsByte);
    textField("Directive Force Absolute", profile->directiveForceAbsolute);

    ImGui::SeparatorText("Comments");
    textField("Comment Prefix", profile->commentPrefix);

    ImGui::SeparatorText("Numbers");
    ImGui::Checkbox("Use Hex Notation", &profile->useHexNotation);
    textField("Hex Notation Prefix", profile->hexNotationPrefix);

    ImGui::SeparatorText("Strings");
    ImGui::Checkbox("Show Non-ASCII Characters As Bytes", &profile->showNonASCIIChararactersAsBytes);
    textField("Quote For ASCII Strings", profile->quoteForASCIIStrings);

    ImGui::SeparatorText("Directives");
    textField("LOW Head", profile->directiveLOWHead);
    textField("LOW Tail", profile->directiveLOWTail);
    textField("HIGH Head", profile->directiveHIGHHead);
    textField("HIGH Tail", profile->directiveHIGHTail);
    textField("BYTE", profile->directiveBYTE);
    textField("BYTE Separator", profile->directiveBYTESeparator);
    ImGui::Checkbox("BYTE Only Numbers Allowed", &profile->directiveBYTEOnlyNumbersAllowed);
    intField("BYTE Number Of Bytes Per Line", profile->directiveBYTENumberOfBytesPerLine);
    intField("BYTE Number Of Characters Per String", profile->directiveBYTENumberOfCharactersPerString);
    ImGui::Checkbox("SBYTE Allowed", &profile->directiveSBYTEAllowed);
    textField("SBYTE", profile->directiveSBYTE);
    ImGui::Checkbox("WORD Allowed", &profile->directiveWORDAllowed);
    textField("WORD", profile->directiveWORD);
    intField("WORD Number Of Words Per Line", profile->directiveWORDNumberOfWordsPerLine);
    ImGui::Checkbox("DS Allowed", &profile->directiveDSAllowed);
    textField("DS", profile->directiveDS);

    ImGui::SeparatorText("Source Structure");
    textField("ORG", profile->directiveORG);
    textField("EQU", profile->directiveEQU);
    textField("END Head", profile->directiveENDHead);
    ImGui::Checkbox("END Needs Filename", &profile->directiveENDNeedsFilename);
    textField("END Tail", profile->directiveENDTail);

    ImGui::SeparatorText("Disassembly Listing");
    ImGui::Checkbox("Omit Unreferenced System Labels", &profile->omitUnreferencedSystemLabels);
    ImGui::Checkbox("INCLUDE Allowed", &profile->directiveINCLUDEAllowed);
    textField("INCLUDE Head", profile->directiveINCLUDEHead);
    textField("INCLUDE Tail", profile->directiveINCLUDETail);
    ImGui::Checkbox("INCLUDE All Equates In One File", &profile->directiveINCLUDEAllEquatesInOneIncludeFile);
    ImGui::Checkbox("INCLUDE All Includes In Main File", &profile->directiveINCLUDEAllIncludesInMainFile);
    intField("INCLUDE Maximum Number Of Lines Per File", profile->directiveINCLUDEMaximumNumberOfLinesPerFile);

    static const char* encodingNames[] = { "Unknown", "ASCII", "ATASCII", "Binary", "UTF8" };
    int encodingIndex = static_cast<int>(profile->outputEncoding);
    if (ImGui::Combo("Output Encoding", &encodingIndex, encodingNames, static_cast<int>(std::size(encodingNames)))) {
        profile->outputEncoding = static_cast<Encoding>(encodingIndex);
    }

    // Load/Save round-trip a whole Profile through a .prf file (see
    // AppState::LoadProfile/SaveProfile above). Fields above are edited
    // directly on the live Profile as you type, so Load and Save both
    // take effect immediately - there's no separate "apply" step.
    ImGui::Separator();
    if (ImGui::Button("Load Profile...")) state.LoadProfile();
    ImGui::SameLine();
    if (ImGui::Button("Save Profile...")) state.SaveProfile();

    // OK/Cancel bracket the whole editing session: OK just closes,
    // keeping whatever's in the fields (and whatever Load Profile most
    // recently loaded); Cancel restores the snapshot taken when the
    // panel was opened (see SnapshotProfile/RestoreProfileSnapshot),
    // discarding both field edits and any mid-session Load.
    ImGui::SameLine();
    if (ImGui::Button("OK")) {
        state.statusMessage = "Profile updated";
        state.showProfilePanel = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        state.RestoreProfileSnapshot();
        state.statusMessage = "Profile changes cancelled";
        state.showProfilePanel = false;
    }

    ImGui::End();
}

// Editor for the current computer system's default folder paths
// (DefaultFolders/DefaultFoldersLogic - both already portable core
// logic, see src/DefaultFoldersLogic.cpp). One text field per
// FolderType, a Save button that writes them back via
// DefaultFoldersLogic::Save (backed by the portable INI-file
// ApplicationSettingsSection from the core-engine porting work).
void DrawDefaultFoldersPanel(AppState& state) {
    if (!state.showDefaultFoldersPanel || !state.defaultFolders) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 350), ImGuiCond_FirstUseEver);
    ImGui::Begin("Default Folders", &state.showDefaultFoldersPanel);
    ClampWindowBelowMenuBar();

    static const FolderType folderTypes[] = {
        FolderType::RAW_FILES, FolderType::EXECUTABLE_FILES, FolderType::ROM_IMAGE_FILES,
        FolderType::CASSETTE_IMAGE_FILES, FolderType::DISK_IMAGE_FILES, FolderType::WORKSPACE_FILES,
        FolderType::EQUATES_FILES, FolderType::PROFILE_FILES, FolderType::DISASSEMBLY_FILES,
    };

    for (FolderType folderType : folderTypes) {
        auto info = FolderTypeFactory::GetInfo(folderType);
        std::string label = String::wstring_to_utf8(info.text);

        static char buf[1024];
        std::string utf8 = String::wstring_to_utf8(state.defaultFolders->GetFolderPath(folderType));
        std::snprintf(buf, sizeof(buf), "%s", utf8.c_str());
        ImGui::PushID(static_cast<int>(folderType));
        if (ImGui::InputText(label.c_str(), buf, sizeof(buf))) {
            state.defaultFolders->SetFolderPath(folderType, String::utf8_to_wstring(buf));
        }
        ImGui::PopID();
    }

    if (ImGui::Button("Save")) {
        state.defaultFoldersLogic.Save(*state.defaultFolders);
        state.statusMessage = "Saved default folders";
    }

    ImGui::End();
}

// Real "Add equate" support via EquateList::AddEquate(wstring_view line),
// same parser the original file-loading path uses (see
// ui_todo/EquateDialog.cpp for the Win32 version). Deletion of a single
// equate is NOT implemented: EquateList's public API only offers
// Clear() (wipe everything - already wired to "Clear User Equates") or
// the private RemoveRange() used internally by SetRange() - there's no
// public "remove this one equate" method to call. Note the original
// dialog's own Add-equate handler was entirely commented out in the
// source (a half-finished feature even there), so this actually goes
// further than the shipped Win32 version in that respect.
// Read-only viewer for the system/CPU-defined equates (as opposed to
// user equates, which DrawEquateEditorPanel below can add to) - these
// come from the computer system definition itself (e.g. Atari OS
// addresses) rather than anything the user defines, so there's nothing
// to edit here, just list them.
void DrawDisplaySystemEquatesPanel(AppState& state) {
    if (!state.showDisplaySystemEquatesPanel) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("System Equates", &state.showDisplaySystemEquatesPanel);
    ClampWindowBelowMenuBar();

    for (const auto& equate : state.workspace->GetSystemEquateList()->GetEquates()) {
        std::string label = String::wstring_to_utf8(equate->GetLabel());
        std::string comment = String::wstring_to_utf8(equate->GetComment());
        ImGui::Text("%-20s $%04X  %s", label.c_str(), equate->GetLabelValue(), comment.c_str());
    }

    ImGui::End();
}

void DrawEquateEditorPanel(AppState& state) {
    if (!state.showEquateEditorPanel) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Edit User Equates", &state.showEquateEditorPanel);
    ClampWindowBelowMenuBar();

    ImGui::TextUnformatted("Format: LABEL = $ADDRESS  (e.g. SCREEN = $2000)");
    bool enterPressed = ImGui::InputText("##addEquate", state.equateAddBuffer, sizeof(state.equateAddBuffer),
                                          ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Add") || enterPressed) {
        EquateList* userEquateList = state.workspace->GetUserEquateList();
        Equate* added = userEquateList->AddEquate(String::utf8_to_wstring(state.equateAddBuffer));
        if (added) {
            state.equateAddBuffer[0] = '\0';
            state.RunDisassembly();
            state.statusMessage = "Added equate";
        } else {
            state.statusMessage = "Could not parse equate line";
        }
    }

    ImGui::Separator();
    ImGui::BeginChild("EquateListScroll", ImVec2(0, 0), false);
    for (const auto& equate : state.workspace->GetUserEquateList()->GetEquates()) {
        std::string label = String::wstring_to_utf8(equate->GetLabel());
        std::string comment = String::wstring_to_utf8(equate->GetComment());
        ImGui::Text("%-20s $%04X  %s", label.c_str(), equate->GetLabelValue(), comment.c_str());
    }
    ImGui::EndChild();
    ImGui::End();
}

// Real implementation via EquateList::SetRange(), which defines a
// sub-range of an existing equate's address space (e.g. splitting a
// table into per-entry labels) - see ui_todo/EquateRangeDialog.cpp for
// the original combo-box-based Win32 version.
void DrawAddressRangePanel(AppState& state) {
    if (!state.showAddressRangePanel) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(450, 250), ImGuiCond_FirstUseEver);
    ImGui::Begin("Define Address Range", &state.showAddressRangePanel);
    ClampWindowBelowMenuBar();

    EquateList* userEquateList = state.workspace->GetUserEquateList();
    const auto& equates = userEquateList->GetEquates();

    if (equates.empty()) {
        ImGui::TextUnformatted("No user equates yet - add one first (Equates > Edit User Equates...).");
        ImGui::End();
        return;
    }

    if (state.addressRangeBaseEquateIndex >= static_cast<int>(equates.size())) {
        state.addressRangeBaseEquateIndex = 0;
    }

    std::string previewLabel = String::wstring_to_utf8(equates[state.addressRangeBaseEquateIndex]->GetLabel());
    if (ImGui::BeginCombo("Base Equate", previewLabel.c_str())) {
        for (int i = 0; i < static_cast<int>(equates.size()); i++) {
            bool selected = (i == state.addressRangeBaseEquateIndex);
            std::string label = String::wstring_to_utf8(equates[i]->GetLabel());
            if (ImGui::Selectable(label.c_str(), selected)) {
                state.addressRangeBaseEquateIndex = i;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::InputText("Start Address (hex)", state.addressRangeStartBuffer, sizeof(state.addressRangeStartBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputText("End Address (hex)", state.addressRangeEndBuffer, sizeof(state.addressRangeEndBuffer),
                      ImGuiInputTextFlags_CharsHexadecimal);

    if (ImGui::Button("Apply")) {
        Equate* baseEquate = equates[state.addressRangeBaseEquateIndex];
        Memory::address startAddr = static_cast<Memory::address>(std::strtoul(state.addressRangeStartBuffer, nullptr, 16));
        Memory::address endAddr = static_cast<Memory::address>(std::strtoul(state.addressRangeEndBuffer, nullptr, 16));
        userEquateList->SetRange(baseEquate->GetLabel(), baseEquate->GetLabelValue(), startAddr, endAddr);
        state.RunDisassembly();
        state.statusMessage = "Address range applied";
    }

    ImGui::End();
}

// Real implementation via SegmentList::GetUserComment()/SetUserComment(),
// operating on the currently selected disassembly line's segment/offset/
// size (see ui_todo/CommentDialog.cpp for the original, which also
// supports an auto-detect-instruction-bounds mode via
// DisassemblyResult::FindOffsetAtStartOfInstruction() for contexts where
// the exact offset/size isn't already known - not needed here since a
// selected disassembly line always has both already).
void DrawCommentPopup(AppState& state) {
    if (state.commentPopupOpen) {
        ImGui::OpenPopup("Edit Comment");
        state.commentPopupOpen = false;
    }
    if (ImGui::BeginPopupModal("Edit Comment", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputTextMultiline("##comment", state.commentBuffer, sizeof(state.commentBuffer), ImVec2(400, 100));
        if (ImGui::Button("Save")) {
            if (state.commentSegmentIndex != NO_SEGMENT_INDEX) {
                state.workspace->GetSegmentList()->SetUserComment(state.commentSegmentIndex, state.commentOffset, state.commentSize, String::utf8_to_wstring(state.commentBuffer));
                state.RunDisassembly();
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

// Real implementation via MiniAssembler.h (ported from
// ui_todo/AssembleDialog.cpp, verified separately in
// ../assembler_verify.cpp against the real 6502 instruction set).
// Operates on the Memory Inspector's current non-empty byte selection,
// same requirement as the original (GetNonEmptySelection()). On success,
// advances the selection forward past the newly-written bytes and
// clears the input, so typing several instructions in a row (like the
// original) works the same way.
void DrawAssemblePopup(AppState& state) {
    if (state.assemblePopupOpen) {
        ImGui::OpenPopup("Assemble");
        state.assemblePopupOpen = false;
    }
    if (ImGui::BeginPopupModal("Assemble", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        auto* selection = state.memoryInspectorSelection.get();

        if (!selection->HasSegment() || !selection->HasSelection()) {
            ImGui::TextUnformatted("Selection lost - close this and select a byte again.");
        } else {
            Memory::address address = selection->GetConstSegment()->wBegin + selection->GetBegin();
            ImGui::Text("Address: $%04X", address);

            bool enterPressed = ImGui::InputText("Instruction", state.assembleInstructionBuffer,
                                                  sizeof(state.assembleInstructionBuffer),
                                                  ImGuiInputTextFlags_EnterReturnsTrue);
            if (ImGui::Button("Assemble") || enterPressed) {
                auto result = MiniAssembler::Assemble(String::utf8_to_wstring(state.assembleInstructionBuffer),
                                                       *selection->segment, selection->GetBegin(), address, *state.workspace);
                state.assembleResultMessage = String::wstring_to_utf8(result.message);
                if (result.success) {
                    Memory::offset next = selection->GetBegin() + result.bytesWritten;
                    selection->SetSelection(next, next);
                    state.assembleInstructionBuffer[0] = '\0';
                    state.RunDisassembly();
                }
            }
            if (!state.assembleResultMessage.empty()) {
                ImGui::TextUnformatted(state.assembleResultMessage.c_str());
            }
        }

        ImGui::Separator();
        if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

// Byte-pattern search over raw memory - see AppState::FindString/
// FindNextString above, ported from ui_todo/MemoryInspector.cpp's
// FindString/SearchString/FindNextString. Unlike the original (which
// syncs a hex-string field alongside the ASCII one via
// ui_todo/FindStringDialog.cpp's two-way conversion), this only takes
// the ASCII text directly as the search pattern - simpler, though it
// means you can't search for a byte sequence that isn't printable
// ASCII the way the original's hex field allowed.
void DrawFindPanel(AppState& state) {
    if (!state.showFindPanel) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 130), ImGuiCond_FirstUseEver);
    ImGui::Begin("Find", &state.showFindPanel);
    ClampWindowBelowMenuBar();

    bool enterPressed = ImGui::InputText("Text", state.findAsciiBuffer, sizeof(state.findAsciiBuffer),
                                          ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::Checkbox("Search all segments", &state.findAllSegments);

    std::string pattern = state.findAsciiBuffer;
    if ((ImGui::Button("Find") || enterPressed) && !pattern.empty()) {
        state.FindString(pattern, state.findAllSegments);
    }
    ImGui::SameLine();
    ImGui::BeginDisabled(state.finder.segmentIndex == NO_SEGMENT_INDEX);
    if (ImGui::Button("Find Next") && !pattern.empty()) {
        state.FindNextString(pattern);
    }
    ImGui::EndDisabled();

    ImGui::End();
}

// Ported from ui_todo/LowHighByteDialog.cpp - shown when setting a
// byte's type to LOBYTE/HIBYTE (see AppState::SetMemoryType above),
// asking for the paired byte's value that can't be inferred
// automatically (it lives at a different, non-adjacent memory location
// - e.g. a table of low bytes and a separate table of high bytes).
void DrawLowHighBytePopup(AppState& state) {
    if (state.lowHighBytePopupOpen) {
        ImGui::OpenPopup("Low/High Byte");
        state.lowHighBytePopupOpen = false;
    }
    if (ImGui::BeginPopupModal("Low/High Byte", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        const char* knownLabel = (state.lowHighByteType == MemoryType::LOBYTE) ? "Low byte (known)" : "High byte (known)";
        const char* unknownLabel = (state.lowHighByteType == MemoryType::LOBYTE) ? "High byte" : "Low byte";

        ImGui::Text("%s: $%02X", knownLabel, state.lowHighByteKnownByte);
        bool enterPressed = ImGui::InputText(unknownLabel, state.lowHighByteUnknownBuffer,
                                              sizeof(state.lowHighByteUnknownBuffer),
                                              ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue);

        bool hasText = state.lowHighByteUnknownBuffer[0] != '\0';
        ImGui::BeginDisabled(!hasText);
        if (ImGui::Button("OK") || (enterPressed && hasText)) {
            byte unknownByte = static_cast<byte>(std::strtoul(state.lowHighByteUnknownBuffer, nullptr, 16));
            state.ApplyLowHighByte(unknownByte);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

// Generic Yes/No confirmation popup - see AppState::RequestConfirm above.
void DrawConfirmPopup(AppState& state) {
    if (state.confirmPopupOpen) {
        ImGui::OpenPopup("Confirm");
        state.confirmPopupOpen = false;
    }
    if (ImGui::BeginPopupModal("Confirm", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted(state.confirmMessage.c_str());
        if (ImGui::Button("Yes")) {
            if (state.confirmAction) state.confirmAction();
            state.confirmAction = nullptr;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) {
            state.confirmAction = nullptr;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

// From-scratch ImGui rendering of the SpriteMode.h pixel extraction -
// NOT a port of the original's Paint()/PaintAll() (which manually blits
// into a fixed-stride Win32 DIB buffer via hardcoded byte offsets like
// "lpScreen + 960" for row replication - not something meaningful to
// carry over to ImGui's retained draw-list model). Draws each logical
// pixel as a scaled filled rect via ImDrawList, using SpriteMode's
// pixel-extraction logic (verified separately - see the description of
// this feature in README.md) for the actual bit-to-palette-index work.
void DrawSpriteViewPanel(AppState& state) {
    if (!state.showSpriteViewPanel) return;
    ImGui::SetNextWindowPos(ImVec2(300, ImGui::GetFrameHeight() + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Sprite View", &state.showSpriteViewPanel);
    ClampWindowBelowMenuBar();

    auto* selection = state.memoryInspectorSelection.get();
    if (!selection->HasSegment() || !selection->HasSelection()) {
        ImGui::TextUnformatted("No selection - pick a byte range in the Memory Inspector first.");
        ImGui::End();
        return;
    }

    static const char* modeNames[] = {
        "Mode 0 (40x24, 4 colors)", "Mode 1 (80x48, 2 colors)", "Mode 2 (80x48, 4 colors)",
        "Mode 3 (160x96, 2 colors)", "Mode 4 (160x192, 2 colors)", "Mode 5 (160x96, 4 colors)",
        "Mode 6 (160x192, 4 colors)", "Mode 7 (320x192, monochrome)"
    };
    ImGui::Combo("Mode", &state.spriteViewMode, modeNames, static_cast<int>(std::size(modeNames)));

    auto modeInfo = SpriteMode::GetModeInfo(state.spriteViewMode);
    const Segment* segment = selection->GetConstSegment();
    Memory::offset begin = selection->GetBegin();
    Memory::offset end = selection->GetEnd();
    Memory::size selectionSize = selection->GetSize(); // GetSize() already accounts for the inclusive end (GetEnd()-GetBegin()+1)

    // Extra on-screen zoom on top of the mode's own pixel aspect-ratio
    // scaling (pixelWidth/pixelHeight can be as small as 1, which would
    // otherwise render as a single, hard-to-see screen pixel).
    const int zoom = 4;
    int cellWidth = modeInfo.pixelWidth * zoom;
    int cellHeight = modeInfo.pixelHeight * zoom;

    // Simple 4-shade grayscale palette (index 0 = black through index 3
    // = white) - not an attempt at accurate GTIA color reproduction,
    // just enough contrast to make the shape readable.
    static const ImU32 palette[4] = {
        IM_COL32(0, 0, 0, 255), IM_COL32(85, 85, 85, 255),
        IM_COL32(170, 170, 170, 255), IM_COL32(255, 255, 255, 255)
    };

    ImGui::BeginChild("SpriteScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();

    int rowCount = static_cast<int>((selectionSize + modeInfo.bytesPerLine - 1) / modeInfo.bytesPerLine);
    for (int row = 0; row < rowCount; row++) {
        for (int byteCol = 0; byteCol < modeInfo.bytesPerLine; byteCol++) {
            Memory::offset offset = begin + static_cast<Memory::offset>(row) * modeInfo.bytesPerLine + byteCol;
            if (offset > end || offset >= segment->GetSize()) continue;
            byte value = segment->GetData(offset);

            for (int pixel = 0; pixel < modeInfo.pixelsPerByte; pixel++) {
                int paletteIndex = SpriteMode::GetPixelPaletteIndex(value, pixel, modeInfo.pixelsPerByte);
                float px = static_cast<float>((byteCol * modeInfo.pixelsPerByte + pixel) * cellWidth);
                float py = static_cast<float>(row * cellHeight);
                ImVec2 p0(origin.x + px, origin.y + py);
                ImVec2 p1(p0.x + cellWidth, p0.y + cellHeight);
                drawList->AddRectFilled(p0, p1, palette[paletteIndex]);
            }
        }
    }

    // Reserve layout space for the whole image so scrolling/sizing works.
    float totalWidth = static_cast<float>(modeInfo.bytesPerLine * modeInfo.pixelsPerByte * cellWidth);
    float totalHeight = static_cast<float>(rowCount * cellHeight);
    ImGui::Dummy(ImVec2(totalWidth, totalHeight));

    ImGui::EndChild();
    ImGui::End();
}

void DrawMenuBar(AppState& state, bool& done) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("Open File")) {
                if (ImGui::MenuItem("Open Cassette Image File...")) state.OpenFile(FileType::CASSETTE_IMAGE_FILE);
                if (ImGui::MenuItem("Open Disk Image Executable File...")) state.OpenFile(FileType::DISK_IMAGE_EXECUTABLE_FILE);
                if (ImGui::MenuItem("Open Disk Image Boot Sectors...")) state.OpenFile(FileType::DISK_IMAGE_BOOT_SECTORS);
                if (ImGui::MenuItem("Open Disk Image Sectors...")) state.OpenFile(FileType::DISK_IMAGE_SECTORS);
                if (ImGui::MenuItem("Open Executable File...")) state.OpenFile(FileType::EXECUTABLE_FILE);
                if (ImGui::MenuItem("Open Raw File...")) state.OpenFile(FileType::RAW_FILE);
                if (ImGui::MenuItem("Open ROM Image File...")) state.OpenFile(FileType::ROM_IMAGE_FILE);
                ImGui::EndMenu();
            }
            ImGui::Separator();
            // TODO(ROADMAP.md): Recent Files/Workspaces (MRU) - see
            // ui_todo/MainFile.cpp's mruController for the original.
            if (ImGui::MenuItem("New Workspace")) state.NewWorkspace();
            if (ImGui::MenuItem("Open Workspace...")) state.OpenWorkspace();
            if (ImGui::MenuItem("Save Workspace")) state.SaveWorkspace(false);
            if (ImGui::MenuItem("Save Workspace As...")) state.SaveWorkspace(true);
            if (ImGui::MenuItem("Save Disassembly Files...")) state.SaveDisassemblyFiles();
            ImGui::MenuItem("Save Disk Image Boot Sectors...", nullptr, false, false); // TODO(ROADMAP.md)
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) done = true;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Equates")) {
            if (ImGui::MenuItem("Clear System Equates")) {
                state.RequestConfirm("Clear all system equates?", [&state] { state.ClearSystemEquates(); });
            }
            if (ImGui::MenuItem("Display System Equates")) state.showDisplaySystemEquatesPanel = true;
            ImGui::Separator();
            if (ImGui::MenuItem("Clear User Equates")) {
                state.RequestConfirm("Clear all user equates?", [&state] { state.ClearUserEquates(); });
            }
            if (ImGui::MenuItem("Edit User Equates...")) state.showEquateEditorPanel = true;
            if (ImGui::MenuItem("Define Address Range...")) state.showAddressRangePanel = true;
            ImGui::Separator();
            if (ImGui::MenuItem("Open User Equates...")) state.OpenUserEquates();
            if (ImGui::MenuItem("Save User Equates...")) state.SaveUserEquates();
            ImGui::Separator();
            if (ImGui::MenuItem("Export User Equates...")) state.ExportUserEquates();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            bool displayAsScreenCode = state.workspace->IsViewDisplayAsScreenCode();
            if (ImGui::MenuItem("Display as Screen Code", nullptr, displayAsScreenCode)) {
                state.workspace->SetViewDisplayAsScreenCode(!displayAsScreenCode);
            }
            bool noDisassembly = state.workspace->isViewNoDisassembly();
            if (ImGui::MenuItem("No Disassembly", nullptr, noDisassembly)) {
                state.workspace->SetViewNoDisassembly(!noDisassembly);
            }
            if (ImGui::MenuItem("Double Font Height", nullptr, state.doubleFontHeight)) {
                state.doubleFontHeight = !state.doubleFontHeight;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Default Folders...")) state.OpenDefaultFoldersPanel();
            if (ImGui::MenuItem("Profile...")) { state.SnapshotProfile(); state.showProfilePanel = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About...")) state.aboutPopupOpen = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // OpenPopup deferred to here - see aboutPopupOpen's declaration in
    // AppState for why it can't be called directly from the MenuItem
    // click above.
    if (state.aboutPopupOpen) {
        ImGui::OpenPopup("About DIS6502");
        state.aboutPopupOpen = false;
    }
    if (ImGui::BeginPopupModal("About DIS6502", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Text/layout/version number match the original Win32 app's own
        // About dialog (dis6502.rc) as closely as ImGui reasonably
        // allows. CenterInAvailWidth centers relative to whatever the
        // current cursor/content region is - works both for whole-window
        // centering below and for the header table's middle column
        // further down, unlike a plain window-width-based center, which
        // would come out wrong inside a table cell.
        auto CenterInAvailWidth = [](const char* text) {
            float avail = ImGui::GetContentRegionAvail().x;
            float textWidth = ImGui::CalcTextSize(text).x;
            float offset = std::max(0.0f, (avail - textWidth) * 0.5f);
            if (offset > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
        };

        // Header row: DIS6502 icon | title/subtitle | Alfred - same
        // left-icon/centered-title/right-bitmap arrangement as
        // ABOUTBOX's ICON IDI_DIS6502 (13,9,21,20) / CTEXT "DIS6502"
        // (70,10,85,9) / CONTROL IDB_BITMAP1 (168,7,51,34) in dis6502.rc.
        // Both bitmaps were pre-flattened onto plain white at packaging
        // time (see LoadAboutTexture in main()), which is why this
        // dialog's white ChildBg (see main()'s global style override)
        // needs to stay white for them to blend in seamlessly.
        if (ImGui::BeginTable("AboutHeader", 3, ImGuiTableFlags_None)) {
            ImGui::TableSetupColumn("icon", ImGuiTableColumnFlags_WidthFixed, 56.0f);
            ImGui::TableSetupColumn("title", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("alfred", ImGuiTableColumnFlags_WidthFixed, 90.0f);
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            if (state.aboutIconTexture) {
                ImGui::Image((ImTextureID)(intptr_t)state.aboutIconTexture, ImVec2(48, 48));
            }

            ImGui::TableSetColumnIndex(1);
            ImGui::PushFont(nullptr); // default font at default size for the title, regardless of any smaller UI font active elsewhere
            CenterInAvailWidth("DIS6502");
            ImGui::TextUnformatted("DIS6502");
            ImGui::PopFont();
            CenterInAvailWidth("The 6502 Disassembler");
            ImGui::TextUnformatted("The 6502 Disassembler");

            ImGui::TableSetColumnIndex(2);
            if (state.aboutAlfredTexture) {
                float aspect = state.aboutAlfredSize.y / state.aboutAlfredSize.x;
                float w = 80.0f;
                ImGui::Image((ImTextureID)(intptr_t)state.aboutAlfredTexture, ImVec2(w, w * aspect));
            }

            ImGui::EndTable();
        }
        ImGui::Spacing();

        const char* url = "http://sourceforge.net/projects/dis6502";
        CenterInAvailWidth(url);
        ImGui::TextUnformatted(url);
        if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsItemClicked()) SDL_OpenURL(url);
        ImGui::Spacing();

        CenterInAvailWidth("(c) 1997-2024 Eric Bacher, atari@ebacher.info");
        ImGui::TextUnformatted("(c) 1997-2024 Eric Bacher, atari@ebacher.info");
        CenterInAvailWidth("Win32 Port - 2005 by James Wilkinson, james@slor.net");
        ImGui::TextUnformatted("Win32 Port - 2005 by James Wilkinson, james@slor.net");
        CenterInAvailWidth("Win32 Fixes - 2015-2024 by Peter Dell, jac@wudsn.com");
        ImGui::TextUnformatted("Win32 Fixes - 2015-2024 by Peter Dell, jac@wudsn.com");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Matches dis6502.rc's ABOUTBOX text verbatim, including its
        // "an 6502" (not "a 6502") wording.
        ImGui::TextWrapped("The purpose of this software is to disassemble an 6502 binary file and generate a listing ready to assemble.");
        ImGui::Spacing();
        ImGui::TextWrapped("Feel free to send any comments, new ideas, or bug reports on SourceForge.");
        ImGui::Spacing();

        // dis6502.rc's ABOUTBOX declares the IDC_LIST_VERSION listbox
        // itself but populates its rows from code that isn't in the .rc
        // file, so the exact original row text (DISCTL.dll etc.) isn't
        // available here. Kept the same four rows from before - the
        // .exe plus the three original ActiveX/OCX controls (disassembly
        // view, memory dump view, sprite view), relabeled as the modules
        // they correspond to within this single-binary Linux/ImGui port
        // rather than claiming DLLs that don't exist in this build - but
        // updated to the version dis6502.rc's VERSIONINFO actually
        // declares now: FILEVERSION/PRODUCTVERSION 4,0,0,0, FileVersion/
        // ProductVersion string "4.0 Beta".
        if (ImGui::BeginTable("AboutVersions", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Module");
            ImGui::TableSetupColumn("Version");
            ImGui::TableSetupColumn("Description");
            ImGui::TableHeadersRow();

            auto Row = [](const char* module, const char* version, const char* description) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(module);
                ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(version);
                ImGui::TableSetColumnIndex(2); ImGui::TextUnformatted(description);
            };
            Row("dis6502_imgui", "4.0 Beta", "6502 Disassembler (Linux/ImGui port)");
            Row("Disassembly view", "4.0 Beta", "DIS6502 Disassembly Control (ported)");
            Row("Memory Inspector", "4.0 Beta", "DIS6502 Dump Control (ported)");
            Row("Sprite View", "4.0 Beta", "DIS6502 Sprite Display Control (ported)");
            ImGui::EndTable();
        }
        ImGui::Spacing();
        // __DATE__/__TIME__ are filled in by the preprocessor at compile
        // time - genuinely when this binary was built, distinct from the
        // 4.0 Beta version above (which is the original app's, carried
        // forward unchanged, not tracked separately for this port).
        ImGui::TextDisabled("This build: ImGui/SDL2 Linux port, built %s %s", __DATE__, __TIME__);
        ImGui::Spacing();

        float okWidth = 120.0f;
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - okWidth - ImGui::GetStyle().WindowPadding.x);
        if (ImGui::Button("OK", ImVec2(okWidth, 0))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

} // namespace

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    NativeFileDialog::Init();

    // Wire the engine's clipboard hook to real SDL clipboard access.
    Application::clipboardTextSetter = [](wstring_view text) {
        SDL_SetClipboardText(String::wstring_to_utf8(text).c_str());
    };

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("dis6502", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           1280, 800, windowFlags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();

    // Force pure white for every window/child background, matching the
    // original's white panels - covers all fixed frame panes (Segments,
    // Disassembly, Memory Inspector, Cross References, Log) and every
    // floating dialog (Profile, Default Folders, Equates, Find, Sprite
    // View, popups, etc.) from this one spot, instead of each needing
    // its own PushStyleColor(ImGuiCol_ChildBg, ...) override.
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImGuiIO& io = ImGui::GetIO();

    // Register the general-UI default font FIRST, as its own untouched
    // font instance - this is what the menu bar, Segments list, Log,
    // and everything else that never calls PushFont uses. Must happen
    // before ComputerFontAtlas::RegisterGlyphs() below, which adds a
    // SEPARATE font instance for its 8x8 bitmap glyphs - if this call
    // didn't happen first, that one would end up as the only (and
    // therefore global default) font instead (see ComputerFontAtlas.h).
    io.Fonts->AddFontDefault();

    // Load the original's bundled bitmap font for the disassembly/hex
    // views, matching the computer system AppState currently hardcodes
    // (ComputerSystemType::ATARI800 - see the "TODO: let the user pick"
    // note there; this font choice should track that once it's
    // selectable, rather than being a second hardcoded default).
    ComputerFontAtlas computerFontAtlas;
    {
        wstring fontPath = g_Application->GetModuleFilePath(L"systems/Atari800/Atari800.fon");
        auto fntFontOpt = FntFont::LoadFromFile(String::wstring_to_utf8(fontPath));
        if (fntFontOpt) {
            computerFontAtlas.RegisterGlyphs(std::move(*fntFontOpt));
            computerFontAtlas.BlitPixels();
        } else {
            std::fprintf(stderr, "Could not load bundled font at %s - falling back to ImGui's default font\n",
                         String::wstring_to_utf8(fontPath).c_str());
        }
    }

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    AppState state;
    state.computerFont = computerFontAtlas.font;

    // About dialog bitmaps - loaded via plain SDL_LoadBMP (both were
    // pre-flattened onto a plain white background at packaging time, to
    // avoid pulling in an extra PNG-decoding dependency for what the
    // dialog only ever shows against its own white background anyway;
    // see assets/dis6502-icon.bmp and assets/alfred.bmp).
    auto LoadAboutTexture = [&](const wchar_t* relativePath, SDL_Texture** outTexture, ImVec2* outSize) {
        wstring path = g_Application->GetModuleFilePath(relativePath);
        SDL_Surface* surface = SDL_LoadBMP(String::wstring_to_utf8(path).c_str());
        if (!surface) {
            std::fprintf(stderr, "Could not load %s: %s\n", String::wstring_to_utf8(path).c_str(), SDL_GetError());
            return;
        }
        *outTexture = SDL_CreateTextureFromSurface(renderer, surface);
        *outSize = ImVec2(static_cast<float>(surface->w), static_cast<float>(surface->h));
        SDL_FreeSurface(surface);
    };
    LoadAboutTexture(L"assets/dis6502-icon.bmp", &state.aboutIconTexture, &state.aboutIconSize);
    LoadAboutTexture(L"assets/alfred.bmp", &state.aboutAlfredTexture, &state.aboutAlfredSize);

    if (argc > 1) {
        state.LoadFile(argv[1]);
    } else {
        state.statusMessage = "Usage: dis6502_imgui <file.xex|.atr|...>";
    }

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
            else if (event.type == SDL_DROPFILE) {
                // event.drop.file is a heap-allocated C string SDL gives us
                // ownership of - must be freed with SDL_free(), not free().
                if (event.drop.file) {
                    state.LoadFile(event.drop.file); // guesses FileType from extension
                    SDL_free(event.drop.file);
                }
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        DrawMenuBar(state, done);
        DrawMainLayoutAndLog(state);
        DrawProfilePanel(state);
        DrawDefaultFoldersPanel(state);
        DrawDisplaySystemEquatesPanel(state);
        DrawEquateEditorPanel(state);
        DrawAddressRangePanel(state);
        DrawCommentPopup(state);
        DrawAssemblePopup(state);
        DrawFindPanel(state);
        DrawSpriteViewPanel(state);
        DrawLowHighBytePopup(state);
        DrawConfirmPopup(state);

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    if (state.aboutIconTexture) SDL_DestroyTexture(state.aboutIconTexture);
    if (state.aboutAlfredTexture) SDL_DestroyTexture(state.aboutAlfredTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    NativeFileDialog::Shutdown();
    SDL_Quit();
    return 0;
}
