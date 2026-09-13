#include "CommentDialog.h"
#include "Dialog.h"
#include "DisassemblyResult.h"
#include "EditControl.h"
#include "Font.h"
#include "Memory.h"
#include "Resource.h"
#include "SegmentList.h"
#include "SegmentTypes.h"
#include "Window.h"
#include <Windows.h>


CommentDialog::CommentDialog(Window& parentWindow) : Dialog(parentWindow, L"ADDCOMMENTBOX") {}

bool CommentDialog::Show(const DisassemblyResult& disassemblyResult, SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size, Font* font) {
    this->font = font;

    if (size == 0xFFFF) {
        disassemblyResult.FindOffsetAtStartOfInstruction(segmentIndex, offset, size);
    }

    comment = segmentList.GetUserComment(segmentIndex, offset, size);

    const auto result = ShowDialogBox();
    if (result) {
        if (size == 0xFFFF) {
            disassemblyResult.FindOffsetAtStartOfInstruction(segmentIndex, offset, size);
        }
        segmentList.SetUserComment(segmentIndex, offset, size, comment);
    }

    return result;
}

bool CommentDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    switch (command) {
    case IDOK:
        return OnOK();

    case IDCANCEL:
        return OnCancel();

    default:
        return false;
    }
}

bool CommentDialog::InitDialog() {
    GetEditControl(IDC_COMMENT).SetFont(font);
    GetEditControl(IDC_COMMENT).SetText(comment);
    return true;
}

bool CommentDialog::OnOK() {
    comment = GetEditControl(IDC_COMMENT).GetText();
    return EndDialogBox(true);
}
