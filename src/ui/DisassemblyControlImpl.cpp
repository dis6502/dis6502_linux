/*
** DisassemblyControlImpl.cpp

** Disassembly control implementation used to display a disassembly listing.
*/
#define NOMINMAX

#include "Control.h"
#include "DC.h"
#include "DisassemblyControlImpl.h"
#include "DisassemblyControlTypes.h"
#include "DisassemblyLine.h"
#include "DisassemblyResult.h"
#include "DisassemblySectionType.h"
#include "Memory.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "UI.h"
#include "Window.h"
#include <algorithm>
#include <SegmentTypes.h>
#include <stdexcept>
#include <wchar.h>
#include <Windows.h>
#include <XRef.h>

/*
** maximum history size.
*/
constexpr size_t DIS_MAX_HISTORY = 256;

typedef DWORD DIS_HISTORY[DIS_MAX_HISTORY];

/*
** Structure where buffer size and graphic mode characteristics are saved.
*  Note that the constructor is not actually called.
*  The structure only serves as a template for the access to the window extra data.
*/
class DisStruct {

public:
    DisStruct* disStruct;
    DisassemblyResult* result;                          // address of result to draw
    WORD           wFactor;                             // how much lines represents a scroll of 1 position
    DisassemblyResult::LineNumber scrollLineNumber;     // first line display, one-based
    ScrollPosition wMaxScrollPosition;                  // maximum scroll position
    DisassemblyResult::LineCount  maxLineCount;         // number of lines displayed in the window
    HFONT          hFont;                               // font used to display lines
    WORD           wFontWidth;                          // font width in pixels
    WORD           wFontHeight;                         // font height in pixels
    BOOL           lineNumbersActive;                   // display line number ?
    DisassemblyResult::LineNumber rightClickLineNumber; // line where the user has right-click
    LP_DIS_HISTORY lpHistory;                           // history of jumps
    WORD           historyIndex;                        // index of jump history
};

/*
** notifications found in HIWORD(lParam) in WM_COMMAND message
*/
typedef enum {
    STATE_NORMAL,
    STATE_COMMENT,
    STATE_NUMBER,
    STATE_STRING,
    STATE_INSTRUCTION,
    STATE_UNREFERENCED
} DIS_STATE;

/*
** colors for different part of a disassembly line
*/
COLORREF Colors[] = {
    RGB(0, 0, 0),       // STATE_NORMAL
    RGB(0, 128, 0),     // STATE_COMMENT
    RGB(128, 0, 0),     // STATE_NUMBER
    RGB(128, 0, 128),   // STATE_STRING
    RGB(0, 0, 128),     // STATE_INSTRUCTION
    RGB(192, 192, 192)  // STATE_UNREFERENCED
};

/*
** Are mouse event captured ?
*/
bool bDisCapture = false;
DWORD dwLastLine = 0xFFFFFFFF; // Last line of mouse event

wchar_t szLabelReference[128];
wchar_t szLabelDefinition[128];


/*
** DisassemblyControlImpl window proc.
*/
static LRESULT CALLBACK DisassemblyControWndProc(HWND hWnd, Window::MESSAGE message, WPARAM wParam, LPARAM lParam) {
    DisassemblyControlImpl control = DisassemblyControlImpl(hWnd);
    return control.WndProc(message, wParam, lParam);
}

__declspec(dllexport) void _cdecl  DisassemblyControlImpl::RegisterClassFor(HINSTANCE hInstance) {

    /*
    ** Fill in window class structure with parameters that describe the
    ** disassembly control.
    */
    WNDCLASS wc = {
        .style = CS_DBLCLKS,
        .lpfnWndProc = DisassemblyControWndProc,
        .cbClsExtra = 0,
        .cbWndExtra = sizeof(DisStruct),
        .hInstance = hInstance,
        .hIcon = NULL,
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
        .lpszMenuName = NULL,
        .lpszClassName = DisassemblyControlImpl::CLASS_NAME,
    };

    if (!RegisterClass(&wc)) {
        safeExitWithClassName(CLASS_NAME);
    };
}

DisassemblyControlImpl::DisassemblyControlImpl(HWND hWnd) : Control(hWnd) {}

//DisStruct* DisassemblyControlImpl::GetDisStruct() const {
//    auto result = (DisStruct*)GetWindowLong(hWnd, FIELD_OFFSET(DisStruct, disStruct));
//    if (result == nullptr) {
//        throw std::runtime_error("No state instance assigned to window");
//    }
//    return result;
//}

WORD DisassemblyControlImpl::GetFontHeight() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wFontHeight));
}

WORD DisassemblyControlImpl::GetFontWidth() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wFontWidth));
}

DisassemblyControlImpl::LineCount DisassemblyControlImpl::GetVisibleLineCount() const {
    const auto fontHeight = GetFontHeight();

    // Determine number of visible lines.
    RECT rc{};
    GetClientRect(hWnd, &rc);
    const auto lineCount = (LineCount)(rc.bottom / fontHeight);
    return lineCount;
}

WORD  DisassemblyControlImpl::GetFactor() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wFactor));
}

void  DisassemblyControlImpl::SetFactor(WORD wFactor) {
    SetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wFactor), wFactor);
}


ScrollPosition DisassemblyControlImpl::GetMaxScrollPosition() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wMaxScrollPosition));
}

void DisassemblyControlImpl::SetMaxScrollPosition(ScrollPosition wMaxScrollPosition) {
    SetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wMaxScrollPosition), wMaxScrollPosition);
}

DisassemblyResult::LineCount  DisassemblyControlImpl::GetMaxLineCount() const {
    return GetWindowLong(hWnd, FIELD_OFFSET(DisStruct, maxLineCount));
}

void DisassemblyControlImpl::SetMaxLineCount(LineCount maxLineCount) {
    SetWindowLong(hWnd, FIELD_OFFSET(DisStruct, maxLineCount), maxLineCount);
}

DisassemblyResult::LineNumber DisassemblyControlImpl::GetMaxScrollLineNumber() const {

    const auto maxLineCount = GetMaxLineCount();
    const auto lineCount = GetConstResult()->GetLineCount();
    LineNumber maxScrollLineNumber;
    if (lineCount <= maxLineCount) {
        maxScrollLineNumber = 1;
    }
    else {
        maxScrollLineNumber = lineCount - maxLineCount + 1;
    }
    return maxScrollLineNumber;
}


DisassemblyResult::LineNumber DisassemblyControlImpl::GetScrollLineNumber() const {
    return GetWindowLong(hWnd, FIELD_OFFSET(DisStruct, scrollLineNumber));
}


DisassemblyResult::LineNumber DisassemblyControlImpl::GetRightClickLineNumber() const {
    return (LineNumber)GetWindowLong(hWnd, FIELD_OFFSET(DisStruct, rightClickLineNumber));
}

void DisassemblyControlImpl::SetRightClickLineNumber(DisassemblyResult::LineNumber lineNumber) {
    SetWindowLong(hWnd, FIELD_OFFSET(DisStruct, rightClickLineNumber), lineNumber);
}


DisassemblyResult* DisassemblyControlImpl::GetResult() const {
    auto result = (DisassemblyResult*)GetWindowLongPtr(hWnd, FIELD_OFFSET(DisStruct, result));
    if (result == nullptr) {
        result = new DisassemblyResult(); // Prevent complex null handling, TODO: Not freed
    }
    return result;
}

const DisassemblyResult* DisassemblyControlImpl::GetConstResult() const {
    return GetResult();
}

/*
** Clear the rest of the window
*/
void DisassemblyControlImpl::FillRestOfWindow(DC dc) {
    RECT rc{};
    GetClientRect(hWnd, &rc);
    const auto wFontHeight = GetFontHeight();
    const auto maxLineCount = (LineCount)rc.bottom / wFontHeight;
    const auto lineCount = GetConstResult()->GetLineCount();
    rc.top = ((lineCount < maxLineCount) ? lineCount : maxLineCount) * wFontHeight;
    dc.FillRect(rc, DC::GetStockBrush(WHITE_BRUSH));
}

/*
** WM_DIS_SET_SCROLL_LINE_NUMBER message
** Set first line to display.
*/
void DisassemblyControlImpl::SetScrollLineNumber(LineNumber lineNumber) {
    const auto maxScrollLineNumber = GetMaxScrollLineNumber();

    // auto requestedLineNumber = lineNumber;
    if (lineNumber < 1) {
        lineNumber = 1;
    }
    if (lineNumber >= maxScrollLineNumber) {
        lineNumber = maxScrollLineNumber;
    }

    SetWindowLong(hWnd, FIELD_OFFSET(DisStruct, scrollLineNumber), lineNumber);

    const auto factor = GetFactor();
    const ScrollPosition scrollPosition = lineNumber / factor;

    // Debug::Log(L"Scroll requested lineNumber=" + std::to_string(requestedLineNumber) + " maxScrollLineNumber=" + std::to_string(maxScrollLineNumber) + " lineNumber=" + std::to_string(lineNumber) + " scrollPosition= " + std::to_string(scrollPosition));

    SetScrollPosition(SB_VERT, scrollPosition, TRUE);
}

/*
** Initialize scroll bar position and range.
*/
void DisassemblyControlImpl::InitScroll() {

    const auto lineCount = GetConstResult()->GetLineCount();
    const auto maxScrollLines = GetMaxScrollLineNumber();
    const auto wFactor = (WORD)(maxScrollLines / 32500) + 1;
    const auto wMaxScrollPosition = (ScrollPosition)(maxScrollLines / wFactor);
    SetFactor(wFactor);
    SetMaxScrollPosition(wMaxScrollPosition);
    SetScrollRange(SB_VERT, 1, wMaxScrollPosition, FALSE);
    SetScrollPosition(SB_VERT, 0, TRUE);
    EnableWindow(hWnd, (lineCount > maxScrollLines));
}

/*
** Handle WM_DIS_SET_RESULT message.
*/
void DisassemblyControlImpl::SetResult(LPARAM lParam) {
    SetWindowLong(hWnd, FIELD_OFFSET(DisStruct, result), (LPARAM)lParam);
}

BOOL DisassemblyControlImpl::AreLineNumbersActive() const {
    return (BOOL)GetWindowWord(hWnd, FIELD_OFFSET(DisStruct, lineNumbersActive));
}

/*
** Handle WM_DIS_SET_LINE_NUMERS_ACTIVE message.
*/
void DisassemblyControlImpl::SetLineNumbersActive(BOOL lineNumbersActive) {
    SetWindowWord(hWnd, FIELD_OFFSET(DisStruct, lineNumbersActive), (WORD)lineNumbersActive);

}

/*
** Handle WM_DIS_SYNC_LINE message.
** Centers a line in the window.
*/
void DisassemblyControlImpl::SyncLine(LineNumber lineNumber) {

    // Center the selected line in the middle of the window, if there are enough lines to scroll.
    const auto maxLineCount = GetMaxLineCount();
    const auto halfLineCount = maxLineCount / 2;
    if (lineNumber < halfLineCount) {
        // If not, make the selected line the first line.
        lineNumber = 0;
    }
    else {
        lineNumber -= halfLineCount;
    }
    SetScrollLineNumber(lineNumber);
    Refresh();
}

/*
** Handles WM_DIS_SELECT_LINE message.
*/
DIS_LINE* DisassemblyControlImpl::SelectLine(LineNumber lineNumber) {

    const auto selectedDisLine = GetResult()->SelectLine(lineNumber);

    if (selectedDisLine != nullptr) {
        SyncLine(lineNumber);
    }
    return selectedDisLine;
}

/*
** Handles WM_DIS_SELECT_LINE_WITH_INFO message.
*/
LRESULT DisassemblyControlImpl::SelectLineWithInfo(SEGMENT_NUMBER segmentNumber, Memory::offset offset) {

    auto selectedLineNumber = GetResult()->SelectLine(segmentNumber, offset);

    if (selectedLineNumber != 0) {
        SyncLine(selectedLineNumber);
        return TRUE;
    }
    return FALSE;
}

/*
** WM_DIS_EXTEND_SEL message
** Extend the selection uo tp the segment / offset specified, if there is already a selection before.
*/
LRESULT DisassemblyControlImpl::ExtendSelectionTo(SEGMENT_NUMBER segmentNumber, Memory::offset offset) {

    if (GetResult()->ExtendSelectionTo(segmentNumber, offset)) {
        Refresh();
        return TRUE;
    }
    return FALSE;
}

HFONT DisassemblyControlImpl::GetFont() const {

    return (HFONT)GetWindowLongPtr(hWnd, FIELD_OFFSET(DisStruct, hFont));

}


/*
** WM_SETFONT message
** Sets font to use.
*/
void DisassemblyControlImpl::SetFont(HFONT hFont) {

    SetWindowLongPtr(hWnd, FIELD_OFFSET(DisStruct, hFont), (LONG_PTR)hFont);

    auto dc = DC(GetDC(hWnd));
    HFONT hOldFont = NULL_HFONT;
    if (hFont) {
        hOldFont = dc.SelectFont(hFont);
    }
    TEXTMETRIC tm{};
    dc.GetTextMetrics(tm);
    SetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wFontWidth), (WORD)tm.tmAveCharWidth);
    SetWindowWord(hWnd, FIELD_OFFSET(DisStruct, wFontHeight), (WORD)tm.tmHeight);

    if (hFont) {
        dc.SelectFont(hOldFont);
    }
    ReleaseDC(hWnd, dc.hDC);
}


/*
** WM_SIZE message
** Determine new number of lines in the window.
*/
void DisassemblyControlImpl::Size() { // TODO: Rename to AdaptSize()?
    RECT rc{};

    GetClientRect(hWnd, &rc);
    const auto wFontHeight = GetFontHeight();
    const auto maxLineCount = (LineCount)(rc.bottom / wFontHeight);
    SetMaxLineCount(maxLineCount);
    InitScroll();
}

/*
** WM_CREATE message
** Initialization of the window private structure in the extra bytes.
** The default setting is no buffer (0 byte long) with system font.
*/
void DisassemblyControlImpl::Create() {

    SetFont(NULL_HFONT);
    SetFactor(1);
    SetResult(NULL);
    SetScrollLineNumber(0);
    Size();
    SetHistory(new DIS_HISTORY());
}

/*
** WM_DESTROY message
** free stack for position
*/
void DisassemblyControlImpl::Destroy() {
    auto lpHistory = GetHistory();
    delete lpHistory;
}

LP_DIS_HISTORY DisassemblyControlImpl::GetHistory() const {
    return (LP_DIS_HISTORY)GetWindowLongPtr(hWnd, FIELD_OFFSET(DisStruct, lpHistory));
}

void DisassemblyControlImpl::SetHistory(LP_DIS_HISTORY history) {
    SetWindowLongPtr(hWnd, FIELD_OFFSET(DisStruct, lpHistory), (LONG_PTR)history);
}

WORD DisassemblyControlImpl::GetHistoryIndex() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(DisStruct, historyIndex));
}

void DisassemblyControlImpl::SetHistoryIndex(WORD historyIndex) {
    SetWindowWord(hWnd, FIELD_OFFSET(DisStruct, historyIndex), historyIndex);
}

/*
** Reset the history stack.
*/
void DisassemblyControlImpl::ResetHistory() {
    SetHistoryIndex(0);
}

/*
** Save a position in the history stack.
*/
void DisassemblyControlImpl::SaveHistory(LineNumber lineNumber) {

    auto historyIndex = GetHistoryIndex();
    if (historyIndex < DIS_MAX_HISTORY) {
        auto history = GetHistory();
        history[historyIndex] = lineNumber;
        SetHistoryIndex(historyIndex + 1);
    }
}

/*
** WM_DIS_BACK_IN_HISTORY message
** jumps back into history
*/
void DisassemblyControlImpl::BackInHistory() {

    auto historyIndex = GetHistoryIndex();
    if (historyIndex > 0) {
        historyIndex--;
        SetHistoryIndex(historyIndex);
        auto history = GetHistory();
        const auto lineNumber = history[historyIndex];
        auto disLine = SelectLine(lineNumber);
        if (disLine) {
            SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_LBUTTONDOWN), (LPARAM)disLine);
        }
    }
}

/*
** find the line under cursor
*/
const DIS_LINE* DisassemblyControlImpl::FindLineUnderCursor(POSITION yPos, LineNumber& cursorLineNumber) {

    auto scrollLineNumber = GetScrollLineNumber();
    const auto fontHeight = GetFontHeight();
    const auto lineCount = yPos / fontHeight;
    scrollLineNumber += lineCount;

    // find line under cursor
    for (auto i = GetResult()->CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        if (disLine->GetLineNumber() == scrollLineNumber) {
            cursorLineNumber = scrollLineNumber;
            return disLine.get();
        }

    }

    return nullptr;
}

/*
** select label definition
*/
DIS_LINE* DisassemblyControlImpl::SelectDefinition(const wchar_t* szLabel) {

    const auto labelLength = wcslen(szLabel);

    // find line under cursor and send a notification to parent window
    LineNumber selectedLineNumber = 0;
    DIS_LINE* selectedDisLine = nullptr;
    for (auto i = GetResult()->CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        disLine->selected = false;
        if (selectedLineNumber == 0) {
            const auto line = disLine->GetLine();
            if (String::Compare(szLabel, line, labelLength) == 0) {
                if ((line[labelLength] == ' ') || (line[labelLength] == ':')) {
                    selectedDisLine = disLine;
                    selectedLineNumber = disLine->GetLineNumber();
                    disLine->selected = true;
                }
            }
        }
    }

    if (selectedDisLine != nullptr) {
        SyncLine(selectedLineNumber);
    }
    return selectedDisLine;
}

/*
** select all lines where a label is referenced including definition
*/
DIS_LINE* DisassemblyControlImpl::SelectAllReferences(BOOL bSelect, const wchar_t* szLabel) {
    // get buffer characteristics from window extra bytes.
    const auto labelLength = wcslen(szLabel);

    // find line under cursor and send a notification to parent window
    XRef::LineNumber xrefLineNumber = 1;
    LineNumber selectedLineNumber = 0;
    DIS_LINE* selectedDisLine = nullptr;
    for (auto i = GetResult()->CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        disLine->selected = false;
        disLine->xrefLineNumber = 0;
        const auto line = disLine->GetLine();
        if (String::Compare(szLabel, line, labelLength) == 0) {
            if ((line[labelLength] == ' ') || (line[labelLength] == ':')) {
                selectedLineNumber = disLine->GetLineNumber();
                disLine->selected = bSelect;
                disLine->xrefLineNumber = xrefLineNumber++;
            }
        }
        auto pText = line + 1;
        while (auto szFound = String::FindFirst(pText, szLabel)) {
            const wchar_t c = *(szFound - 1);
            if ((c != '@') && (c != '_') && ((c < 'a') || (c > 'z')) && ((c < 'A') || (c > 'Z')) && ((c < '0') || (c > '9'))) {
                const wchar_t c = *(szFound + wcslen(szLabel));
                if ((c != '@') && (c != '_') && ((c < 'a') || (c > 'z')) && ((c < 'A') || (c > 'Z')) && ((c < '0') || (c > '9'))) {
                    disLine->selected = bSelect;
                    if (disLine->xrefLineNumber == 0) {
                        disLine->xrefLineNumber = xrefLineNumber++;
                    }
                }
            }
            pText++;
        }
        if ((selectedDisLine == nullptr) && (disLine->xrefLineNumber)) {
            selectedDisLine = disLine;
        }

    }
    if ((selectedDisLine != nullptr) && (bSelect)) {
        SyncLine(selectedLineNumber);
    }
    return selectedDisLine;
}

/*
** Find all references in the line and display them
*/
void DisassemblyControlImpl::SelectAllReferencesAndNotifyParent(BOOL bSelect, const wchar_t* szText) {

    const auto& disLine = SelectAllReferences(bSelect, szText);
    if (disLine && bSelect) {
        const auto lineNumber = GetRightClickLineNumber();
        SaveHistory(lineNumber);
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_LBUTTONDOWN), (LPARAM)disLine);
    }
}

/*
** Find a reference in the line and display it
*/
void DisassemblyControlImpl::FindLabelInLine(const wchar_t* szText) {
    size_t wSrcIndex = 0;
    size_t wDefIndex = 0;

    szLabelReference[0] = '\0';
    szLabelDefinition[0] = '\0';

    /*
    ** label on column 0 should begin with a letter, an @ or an _ or it is considered
    ** as a comment marker
    */
    auto c = szText[wSrcIndex++];
    if ((c == '@') || (c == '_') || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
        while ((c == '@') || (c == '_') || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9'))) {
            szLabelDefinition[wDefIndex++] = c;
            c = szText[wSrcIndex++];
        }
        while ((c != '\0') && (c != ' ')) {
            c = szText[wSrcIndex++];
        }
        while (c == ' ') {
            c = szText[wSrcIndex++];
        }
        szLabelDefinition[wDefIndex] = '\0';
    }
    else if (c != ' ') {
        return;
    }

    // now we have the instruction
    while (c == ' ') {
        c = szText[wSrcIndex++];
    }
    while ((c != '\0') && (c != ' ')) {
        c = szText[wSrcIndex++];
    }
    while (c == ' ') {
        c = szText[wSrcIndex++];
    }

    // now we have either a parameter or a comment
    if (c == '\"') {
        return;
    }
    if ((c == '#') || (c == '(')) {
        c = szText[wSrcIndex++];
        while (c == ' ') {
            c = szText[wSrcIndex++];
        }
        if ((c == '>') || (c == '<')) {
            c = szText[wSrcIndex++];
            while (c == ' ') {
                c = szText[wSrcIndex++];
            }
        }
    }
    if ((c == '@') || (c == '_') || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
        // Allocate buffer including zero termination.
        LPWSTR szBuf = new WCHAR[String::Length(szText) + 3];
        WORD wDestIndex = 0;
        while ((c == '@') || (c == '_') || ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
        }
        szBuf[wDestIndex] = '\0';
        if (String::Length(szBuf) > 1) {
            wcscpy(szLabelReference, szBuf);
        }
        delete[] szBuf;
    }
}

/*
** Find a reference in the line and display it.
*/
void DisassemblyControlImpl::SelectDefinitionAndNotifyParent(const wchar_t* szText) {

    auto const disLine = SelectDefinition(szText);
    if (disLine) {
        const auto lineNumber = GetRightClickLineNumber();
        SaveHistory(lineNumber);
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_LBUTTONDOWN), (LPARAM)disLine);
    }
}

/*
** Find a reference in the line and display it.
*/
void DisassemblyControlImpl::FindReference(const wchar_t* szText) {
    FindLabelInLine(szText);
    if (String::Length(szLabelReference) > 0) {
        SelectDefinitionAndNotifyParent(szLabelReference);
    }
}

/*
** Repaint one line with syntax coloring.
*/
INT DisassemblyControlImpl::FlushPartOfLine(DC dc, POSITION xPos, POSITION yPos, WORD wState, const wchar_t* szText) {
    constexpr WORD wFontWidth = 8;
    dc.SetTextColor(Colors[wState]);
    dc.TextOut(xPos, yPos, szText, wcslen(szText));
    return xPos + (wcslen(szText) * wFontWidth);
}

/*
** Repaint one line with syntax coloring.
*/
void DisassemblyControlImpl::PrintOneLineInColor(DC dc, WORD wFontHeight, bool selected, bool referenced, bool lineNumbersActive, POSITION yPos, const wchar_t* szText) {

    WORD wSrcIndex = 0;
    WORD wDestIndex = 0;
    POSITION xPos = 1;
    WORD wState = STATE_INSTRUCTION;
    wchar_t c, c2;
    BOOL bNumber = FALSE;
    HBRUSH hOldBrush;
    HBRUSH hBrush;
    RECT rc;

    // Use shared buffer
    auto szBuf = String::szBuffer;
    szBuf[0] = 0;

    // fill background in yellow if line is selected
    if (selected) {
        GetClientRect(hWnd, &rc);
        rc.left = 1;
        rc.top = yPos;
        rc.bottom = yPos + wFontHeight;
        hBrush = CreateSolidBrush(RGB(255, 255, 0));
        if (hBrush) {
            hOldBrush = dc.SelectBrush(hBrush);
            dc.FillRect(rc, hBrush);
            dc.SelectBrush(hOldBrush);
            DeleteObject(hBrush);
        }
        dc.SetBkMode(TRANSPARENT);
    }
    else {
        dc.SetBkMode(OPAQUE);
    }

    if (lineNumbersActive) {
        c = szText[wSrcIndex++];
        while ((c >= '0') && (c <= '9')) {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
        }
        szBuf[wDestIndex++] = c;
        szBuf[wDestIndex] = '\0';
        xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
        wDestIndex = 0;
    }

    /*
    ** Label on column 0 should begin with a letter, an @ or an _.
    ** Otherwise it is considered as a comment marker
    */
    c = szText[wSrcIndex++];
    if ((c == '@') || (c == '_') || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
        while ((c != '\0') && (c != ' ')) {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
        }
        while (c == ' ') {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
        }
        szBuf[wDestIndex] = '\0';
        xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
        wDestIndex = 0;
    }
    else if (c != ' ') {
        FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_COMMENT : STATE_UNREFERENCED, &szText[wSrcIndex - 1]);
        return;
    }

    // Now we have the instruction.
    while (c == ' ') {
        szBuf[wDestIndex++] = c;
        c = szText[wSrcIndex++];
    }
    if ((c == '=') || (c == '*')) {
        wState = STATE_NORMAL;
    }
    while ((c != '\0') && (c != ' ')) {
        szBuf[wDestIndex++] = c;
        c = szText[wSrcIndex++];
    }
    while (c == ' ') {
        szBuf[wDestIndex++] = c;
        c = szText[wSrcIndex++];
    }
    szBuf[wDestIndex] = '\0';
    xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? wState : STATE_UNREFERENCED, szBuf);
    wDestIndex = 0;

    // Now we have either a parameter or a comment.
    if ((c == '\"') || (c == '\'')) {
        c2 = c; // Remeber the quote character used.
        szBuf[wDestIndex++] = c;
        szBuf[wDestIndex] = '\0';
        xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
        wDestIndex = 0;
        c = szText[wSrcIndex++];
        // Loop until the end of the line or the 2nd quote.
        while ((c != '\0') && (c != c2)) {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
        }
        szBuf[wDestIndex] = '\0';
        xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_STRING : STATE_UNREFERENCED, szBuf);
        wDestIndex = 0;
    }
    else if (c == '#') {
        szBuf[wDestIndex++] = c;
        szBuf[wDestIndex] = '\0';
        xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
        wDestIndex = 0;
        c = szText[wSrcIndex++];
        while (c == ' ') {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
        }
    }
    do {
        if (c == ',') {
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
            szBuf[wDestIndex] = '\0';
            xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
            wDestIndex = 0;
        }
        bNumber = FALSE;
        if (c == '$') {
            bNumber = TRUE;
            szBuf[wDestIndex++] = c;
            c = szText[wSrcIndex++];
            szBuf[wDestIndex] = '\0';
            xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
            wDestIndex = 0;
        }
        if (bNumber) {
            while (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F'))) {
                szBuf[wDestIndex++] = c;
                c = szText[wSrcIndex++];
            }
            szBuf[wDestIndex] = '\0';
            xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NUMBER : STATE_UNREFERENCED, szBuf);
            wDestIndex = 0;
        }
    } while (c == ',');

    // the rest of the line
    if (c == ';') {
        FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_COMMENT : STATE_UNREFERENCED, &szText[wSrcIndex - 1]);
        return;
    }
    szBuf[wDestIndex++] = c;
    c = szText[wSrcIndex++];
    while ((c != '\0') && (c != ';')) {
        szBuf[wDestIndex++] = c;
        c = szText[wSrcIndex++];
    }
    szBuf[wDestIndex] = '\0';
    xPos = FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_NORMAL : STATE_UNREFERENCED, szBuf);
    //wDestIndex = 0; No further parts
    if (c == ';') {
        FlushPartOfLine(dc, xPos, yPos, referenced ? STATE_COMMENT : STATE_UNREFERENCED, &szText[wSrcIndex - 1]);
    }
}

/*
** Repaint complete control.
*/
void DisassemblyControlImpl::PrintAll(DC dc) {
    HFONT hFont;
    HFONT hOldFont;
    WORD wNbLines;
    wchar_t* szBuf;

    const auto lineNumbersActive = AreLineNumbersActive();
    const auto scrollLineNumber = GetScrollLineNumber();
    const auto wFontWidth = GetFontWidth();
    const auto wFontHeight = GetFontHeight();

    RECT rc{};
    GetClientRect(hWnd, &rc);

    auto wRow = (WORD)(rc.right / wFontWidth);
    wRow = std::min<WORD>(wRow, 256);
    wNbLines = (WORD)(rc.bottom / wFontHeight);
    wNbLines = std::min<WORD>(wNbLines, 256);

    // Allocate buffer including zero termation.
    const size_t iBufSize = std::max(1024, wRow + 1);
    szBuf = new wchar_t[iBufSize];

    // Select font and print all lines.
    hFont = GetFont();
    if (hFont) {
        hOldFont = dc.SelectFont(hFont);
    }

    for (auto i = GetResult()->CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        const auto lineNumber = disLine->GetLineNumber();
        if ((lineNumber >= scrollLineNumber) && (lineNumber - scrollLineNumber < wNbLines)) {
            auto pDisText = disLine->GetLine();
            const size_t pDisTextLength = wcslen(pDisText);
            if (pDisTextLength > iBufSize - 6) { // 4 digit line number plus space plus zero-termination
                throw std::runtime_error("Line too long");
            }
            if (lineNumbersActive) {
                wsprintf(szBuf, L"%04lu %s", lineNumber, pDisText);
            }
            else {
                wcscpy(szBuf, pDisText);
            }

            // Fill rest of buffer with spaces
            auto iMaxLength = std::min<size_t>(wRow + 1, (iBufSize - 1));
            auto iLength = wcslen(szBuf);
            for (; iLength < iMaxLength; iLength++) {
                szBuf[iLength] = ' ';
            }
            szBuf[iLength] = 0;
            if ((iBufSize >= 42) && (szBuf[35] != ';') && (disLine->address) && (disLine->selected)) {
                wsprintf(&szBuf[35], L"; $%04hX", disLine->address);
                szBuf[42] = ' '; // Overwrites 0 from wsprintf, so the rest of the line is displayed
            }

            // All sections other than the system and user equates are never greyed out.
            bool referenced;
            switch (disLine->GetSection()->GetType()) {
            case  DisassemblySectionType::SYSTEM_EQUATES:
            case  DisassemblySectionType::USER_EQUATES:
                referenced = disLine->referenced;
                break;
            default:
                referenced = true;
            }
            PrintOneLineInColor(dc, wFontHeight, disLine->selected, referenced, lineNumbersActive, (WORD)((lineNumber - scrollLineNumber) * wFontHeight), szBuf);

        }
    }

    delete[] szBuf;
    FillRestOfWindow(dc);
    if (hFont) {
        dc.SelectFont(hOldFont);
    }
}

/*
** WM_PAINT message
** redraw the buffer
*/
void DisassemblyControlImpl::Paint() {
    PAINTSTRUCT ps{};

    auto dc = DC(BeginPaint(hWnd, &ps));
    PrintAll(dc);
    EndPaint(hWnd, &ps);
}

/*
** WM_DIS_REFRESH message
** refresh window
*/
void DisassemblyControlImpl::Refresh() {

    auto dc = DC(GetDC(hWnd));
    PrintAll(dc);
    ReleaseDC(hWnd, dc.hDC);
}

/*
** WM_MOUSEMOVE message
** wParam Indicates whether various virtual keys are down. This parameter can be one or more of the following values.
** lParam The low-order word specifies the x-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area.
** The high-order word specifies the y-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area.
*/
void DisassemblyControlImpl::MouseMove(WPARAM wParam, LPARAM lParam) {

    if (bDisCapture == false) {
        return;
    }

    BOOL bRefresh = FALSE;
    auto scrollLineNumber = GetScrollLineNumber();
    const auto fontHeight = GetFontHeight();
    const auto wNbLines = HIWORD(lParam) / fontHeight;
    scrollLineNumber += wNbLines;

    // find line under cursor and send a notification to parent window
    for (auto i = GetResult()->CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        disLine->selected = false;
        if (disLine->GetLineNumber() == scrollLineNumber) {
            if (disLine->GetLineNumber() != dwLastLine) {
                bRefresh = TRUE;
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_LBUTTONDOWN), (LPARAM) & (*disLine));
                FindLabelInLine(disLine->GetLine());
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_XREF), (LPARAM) & (*disLine));
            }
            dwLastLine = disLine->GetLineNumber();
            disLine->selected = true;
        }
    }

    if (bRefresh) {
        Refresh();
    }
}

/*
** WM_LBUTTONDOWN message
*/
void DisassemblyControlImpl::LButtonDown(WPARAM wParam, LPARAM lParam) {
    SetFocus();

    // Capture mouse input.
    if (bDisCapture == false) {
        SetCapture(hWnd);
    }
    bDisCapture = true;
    dwLastLine = 0xFFFFFFFF;

    // Find line under cursor and send a notification to parent window
    MouseMove(wParam, lParam);
}

/*
** WM_LBUTTONUP message
*/
void DisassemblyControlImpl::LButtonUp() {
    /*
    ** Release mouse input.
    */
    if (bDisCapture) {
        ReleaseCapture();
    }
    bDisCapture = false;
    dwLastLine = 0xFFFFFFFF;
}

/*
** WM_LBUTTONDBLCLK message
*/
void DisassemblyControlImpl::LButtonDblClk(WPARAM wParam, LPARAM lParam) {
    LineNumber lineNumber;

    // find line under cursor and display line referenced by this one
    auto disLine = FindLineUnderCursor(HIWORD(lParam), lineNumber);
    if (disLine) {
        SetRightClickLineNumber(lineNumber);
        FindReference(disLine->GetLine());
    }
}

/*
** WM_RBUTTONDOWN message
*/
void DisassemblyControlImpl::RButtonDown(WPARAM wParam, LPARAM lParam) {

    // Check if we have a buffer.
    SetFocus();

    /*
    ** search the line under cursor and extract in a global variable the
    ** referenced label.
    */
    LineNumber lineNumber = 0;
    auto disLine = FindLineUnderCursor(HIWORD(lParam), lineNumber);
    if (disLine) {
        SetRightClickLineNumber(lineNumber);
        FindLabelInLine(disLine->GetLine());
    }

    // notify parent of the right button click.
    SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_RBUTTONDOWN), (LONG_PTR)disLine);
}

/*
** WM_VSCROLL message
*/
void DisassemblyControlImpl::VScroll(WPARAM wParam, LPARAM lParam) {

    const auto scrollLineNumber = GetScrollLineNumber();
    const auto wFactor = GetFactor();
    ScrollPosition wScrollPosition = (ScrollPosition)(scrollLineNumber / wFactor);
    const auto wOldScrollPosition = wScrollPosition;
    const auto wMaxScrollPosition = GetMaxScrollPosition();

    // Determine number of visible lines
    const auto lineCount = (INT)GetVisibleLineCount(); // TODO: I think, here the factor is missing
    switch (LOWORD(wParam)) {
    case SB_TOP:
        if (wScrollPosition != 0) {
            wScrollPosition = 0;
        }
        break;

    case SB_BOTTOM:
        if (wScrollPosition != wMaxScrollPosition) {
            wScrollPosition = wMaxScrollPosition;
        }
        break;

    case SB_LINEUP:
        if (wScrollPosition > 0) {
            wScrollPosition--;
        }
        break;

    case SB_LINEDOWN:
        if (wScrollPosition != wMaxScrollPosition) {
            wScrollPosition++;
        }
        break;

    case SB_PAGEUP:
        if (wScrollPosition) {
            wScrollPosition -= std::min(wScrollPosition, lineCount);
        }
        break;

    case SB_PAGEDOWN:
        if (wScrollPosition != wMaxScrollPosition) {
            wScrollPosition += std::min(wMaxScrollPosition - wScrollPosition, lineCount);
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        if (wScrollPosition != HIWORD(wParam)) {
            wScrollPosition = HIWORD(wParam);
        }
        break;
    }
    if (wOldScrollPosition != wScrollPosition) {
        const auto newScrollLineNumber = (LineNumber)(wScrollPosition * wFactor);
        SetScrollLineNumber(newScrollLineNumber);
        Refresh();
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), DIS_SCROLL_CHANGED), (LONG_PTR)hWnd);
    }
}

/*
** Dis window proc.
*/
LRESULT DisassemblyControlImpl::WndProc(Window::MESSAGE message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DIS_SET_RESULT:
        SetResult(lParam);
        InitScroll();
        break;

    case WM_DIS_SET_LINE_NUMBERS_ACTIVE:
        SetLineNumbersActive((BOOL)wParam);
        break;

    case WM_DIS_SYNC_LINE:
        SyncLine(lParam);
        break;

    case WM_DIS_SELECT_LINE:
        SelectLine(lParam);
        break;

    case WM_DIS_SELECT_LINE_WITH_INFO:
        return SelectLineWithInfo(HIWORD(lParam), LOWORD(lParam));

    case WM_DIS_EXTEND_SEL:
        return ExtendSelectionTo(HIWORD(lParam), LOWORD(lParam));

    case WM_DIS_SELECT_DEFINITION:
        SelectDefinitionAndNotifyParent((const wchar_t*)lParam);
        break;

    case WM_DIS_SELECT_ALL_REFERENCES:
        SelectAllReferencesAndNotifyParent((BOOL)wParam, (const wchar_t*)lParam);
        break;

    case WM_DIS_GET_LABEL_REFERENCE:
        return (LRESULT)szLabelReference;  // TODO: Works only because they are global/static

    case WM_DIS_GET_LABEL_DEFINITION:
        return (LRESULT)szLabelDefinition; // TODO: Works only because they are global/static

    case WM_DIS_GET_LINE_NUMBERS_ACTIVE:
        return AreLineNumbersActive();

    case WM_DIS_SET_SCROLL_LINE_NUMBER:
        SetScrollLineNumber(lParam);
        break;

    case WM_DIS_GET_SCROLL_LINE_NUMBER:
        return GetScrollLineNumber();

    case WM_DIS_RESET_HISTORY:
        ResetHistory();
        break;

    case WM_DIS_GET_HISTORY_INDEX:
        return GetHistoryIndex();

    case WM_DIS_BACK_IN_HISTORY:
        BackInHistory();
        break;

    case WM_DIS_REFRESH:
        Refresh();
        break;

    case WM_LBUTTONDOWN:
        LButtonDown(wParam, lParam);
        break;

    case WM_MOUSEMOVE:
        MouseMove(wParam, lParam);
        break;

    case WM_LBUTTONUP:
        LButtonUp();
        break;


    case WM_LBUTTONDBLCLK:
        LButtonDblClk(wParam, lParam);
        break;


    case WM_RBUTTONDOWN:
        RButtonDown(wParam, lParam);
        break;


    case WM_SETFONT:
        SetFont((HFONT)wParam);
        Size();
        break;

    case WM_GETFONT:
        return (LPARAM)GetFont();

    case WM_CREATE:
        Create();
        break;

    case WM_DESTROY:
        Destroy();
        break;

    case WM_SIZE:
        Size();
        break;

    case WM_PAINT:
        Paint();
        break;

    case WM_VSCROLL:
        VScroll(wParam, lParam);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_NEXT:
            VScroll(SB_PAGEDOWN, lParam);
            break;
        case VK_PRIOR:
            VScroll(SB_PAGEUP, lParam);
            break;
        case VK_UP:
            VScroll(SB_LINEUP, lParam);
            break;
        case VK_DOWN:
            VScroll(SB_LINEDOWN, lParam);
            break;
        case VK_HOME:
            VScroll(SB_TOP, lParam);
            break;
        case VK_END:
            VScroll(SB_BOTTOM, lParam);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return LRESULT_0;
}
