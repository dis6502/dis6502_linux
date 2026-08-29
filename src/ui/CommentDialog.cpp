#include "Application.h"
#include "DisassemblyResult.h"
#include "SegmentList.h"
#include "EditControl.h"
#include "CommentDialog.h"


CommentDialog::CommentDialog(Window& parentWindow) : Dialog(parentWindow, L"ADDCOMMENTBOX") {
}

bool CommentDialog::Show(const DisassemblyResult& disassemblyResult, SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size, HFONT hFont) {
    this->hFont = hFont;

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

bool CommentDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {
    case WM_INITDIALOG:
        GetEditControl(IDC_COMMENT).SetFont(hFont);
        GetEditControl(IDC_COMMENT).SetText(comment);
        return true;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDOK:
            comment = GetEditControl(IDC_COMMENT).GetText();
            return EndDialogBox(true);

        case IDCANCEL:
            return EndDialogBox(false);

        default:
            return false;
        }

    default:
        return false;
    }

    }
}
