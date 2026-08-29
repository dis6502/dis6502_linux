#pragma once

#include "Syntax.h"
#include "Dialog.h"

#include "SegmentTypes.h"
#include "Memory.h"

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

	bool Show(const DisassemblyResult& disassemblyResult, SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::offset memoryOffset, Memory::size memorySize, HFONT hFont);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	HFONT hFont = NULL_HFONT;
	wstring comment;
};
