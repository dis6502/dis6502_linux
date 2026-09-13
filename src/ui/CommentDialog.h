#pragma once

#include "Dialog.h"
#include "Font.h"
#include "Memory.h"
#include "SegmentTypes.h"
#include "Syntax.h"
#include "Window.h"

class DisassemblyResult;
class SegmentList;
/*
** Comments can be added in the memoryInspector and in the disassembly area.
*/
class CommentDialog : public Dialog {
public:
    CommentDialog() = delete;
    CommentDialog(Window& parentWindow);
    CommentDialog(const CommentDialog& a) = delete;			  // No copy constructor
    CommentDialog& operator=(const CommentDialog&) = delete;  // No copy assignment
    CommentDialog(CommentDialog&) = delete;                   // No move constructor
    CommentDialog& operator=(CommentDialog&&) = delete;       // No move assignment

    bool Show(const DisassemblyResult& disassemblyResult, SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::offset memoryOffset, Memory::size memorySize, Font* font);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

private:
    Font* font;
    wstring comment;
};
