/*
** MemoryInspectorControlImpl.cpp

** Segment memoryInspector control implementation used to display a buffer in hexadecimal and in ASCII/ATASCII.
*/
#define NOMINMAX

#include "MemoryInspectorControlImpl.h"
#include "MemoryInspectorControlTypes.h"
#include "MemoryType.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "Window.h"
#include <algorithm>
#include <wchar.h>
#include <Windows.h>
#include <windowsx.h>

/*
** structure where buffer size and graphic mode characteristics are saved.
*/
typedef struct MemoryInspectorStruct {
    BYTE* buffer;					// buffer address
    BYTE* pType;				// buffer address
    WORD wSize;					// size (in bytes) of the buffer to display
    WORD wStart;				// start address of the buffer to display
    WORD wNumberOfBytesPerLine;	// number of displayed bytes per line
    WORD wScroll;				// first line display (zero based)
    WORD wMaxScroll;			// maximum scroll range
    WORD wMaxLines;				// number of lines displayed in the window
    HFONT hFont;				// font used to display memoryInspector
    WORD wFontWidth;			// font width in pixels
    WORD wFontHeight;			// font height in pixels
    WORD wBeginLine;			// first line of selection
    WORD wBeginRow;				// first row of selection
    WORD wEndLine;				// last line of selection 
    WORD wEndRow;				// last row of selection
    WORD bInternal;				// internal display (use ANTIC code)
    WORD bEditMode;				// edit or read-only mode
    WORD wEditedPart;           // if bEditMode is TRUE. 0=HIGH 4 bits, 1=LOW 4 bits, 2=ASCII)
    WORD wTimerCount;           // if bEditMode is TRUE
} MemoryInspectorStruct;

/*
** Are mouse event captured ?
*/
BOOL bMemoryInspectorCapture = FALSE;

/*
** colors used for different byte type.
*/
static COLORREF dwMemoryInspectorColor[MEMORY_TYPE_ENUM_ITEM_COUNT] = {
    RGB(0, 0, 0),
    RGB(192, 192, 192),
    RGB(128, 128, 128),
    RGB(128, 0, 0),
    RGB(128, 0, 128),
    RGB(128, 128, 0),
    RGB(255, 127, 0),
    RGB(0, 127, 255),
    RGB(0, 128, 0),
    RGB(255, 128, 255),
    RGB(0, 0, 128),
    RGB(255, 0, 128),
    RGB(255, 0, 255)
};


/*
** MemoryInspector window proc.
*/
static LRESULT CALLBACK MemoryInspectorWndProc(HWND hWnd, Window::MESSAGE message, WPARAM wParam, LPARAM lParam) {
    MemoryInspectorControlImpl control = MemoryInspectorControlImpl(hWnd);
    return control.WndProc(message, wParam, lParam);
}

__declspec(dllexport) void _cdecl MemoryInspectorControlImpl::RegisterClassFor(HINSTANCE hInstance) {


    /*
    ** Fill in window class structure with parameters that describe the
    ** MemoryInspector window.
    */
    WNDCLASS wc{
        .style = CS_DBLCLKS,
        .lpfnWndProc = MemoryInspectorWndProc,
        .cbClsExtra = 0,
        .cbWndExtra = sizeof(MemoryInspectorStruct),
        .hInstance = hInstance,
        .hIcon = NULL,
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
        .lpszMenuName = NULL,
        .lpszClassName = MemoryInspectorControlImpl::CLASS_NAME
    };

    if (!RegisterClass(&wc)) {
        safeExitWithClassName(CLASS_NAME);
    };
}

MemoryInspectorControlImpl::MemoryInspectorControlImpl(HWND hWnd) {
    this->hWnd = hWnd;
}

/*
** Sets size of buffer to display.
** wSize is the number of bytes of the buffer.
** This function does not redraw the control.
*/
void MemoryInspectorControlImpl::SetSize(WORD wSize) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wSize), wSize);
    InitScroll();

}

WORD MemoryInspectorControlImpl::GetSize() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wSize));
}

void MemoryInspectorControlImpl::SetStart(WORD wStart) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wStart), wStart);
}

WORD MemoryInspectorControlImpl::GetStart() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wStart));
}


/*
** WM_DUMP_SET_NUMBER_OF_BYTES_PER_LINE message
** set number of bytes to display per line.
** wNumberOfBytesPerLine is the number of bytes per line, at least 8.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetNumberOfBytesPerLine(WORD wNumberOfBytesPerLine) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wNumberOfBytesPerLine), wNumberOfBytesPerLine);
}

/*
** Get the number of ATARI bytes per line.
** An ATARI byte is displayed both as HEX (2 characters and a space) and as ASCII/ATASCII (1 character)
*/
WORD MemoryInspectorControlImpl::GetNumberOfBytesPerLine() {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wNumberOfBytesPerLine));
}

/*
** WM_DUMP_SET_SCROLL message
** set first line to display.
** wScroll is a line number.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetScroll(WORD wScroll, BOOL bRefresh) {
    if (!(GetWindowLong(hWnd, GWL_STYLE) & WS_VSCROLL)) {
        return;
    }
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll), wScroll);
    SetScrollPos(hWnd, SB_VERT, wScroll, TRUE);
    if (bRefresh) {
        Refresh();
    }
}

void MemoryInspectorControlImpl::SetBeginLine(WORD wBeginLine) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wBeginLine), wBeginLine);
}
WORD MemoryInspectorControlImpl::GetBeginLine()  const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wBeginLine));

}
void MemoryInspectorControlImpl::SetBeginRow(WORD wBeginRow) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wBeginRow), wBeginRow);
}
WORD MemoryInspectorControlImpl::GetBeginRow() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wBeginRow));
}
void MemoryInspectorControlImpl::SetEndLine(WORD wEndLine) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wEndLine), wEndLine);

}
WORD MemoryInspectorControlImpl::GetEndLine() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wEndLine));
}
void MemoryInspectorControlImpl::SetEndRow(WORD wEndRow) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wEndRow), wEndRow);
}
WORD MemoryInspectorControlImpl::GetEndRow() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wEndRow));
}
/*
** WM_DUMP_SET_BEGIN_SELECTION message
** wBegin is the first byte selected. It is converted in line/row.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetBeginSelection(WORD wBegin, BOOL bRefresh) {
    WORD wBeginLine;
    WORD wBeginRow;

    if (wBegin == DUMP_NO_SELECTION) {
        wBeginLine = wBeginRow = DUMP_NO_SELECTION;
    }
    else {
        const WORD wNumberOfBytesPerLine = GetNumberOfBytesPerLine();
        wBeginLine = wBegin / wNumberOfBytesPerLine;
        wBeginRow = wBegin % wNumberOfBytesPerLine;
    }
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wBeginLine), wBeginLine);
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wBeginRow), wBeginRow);
    if (bRefresh) {
        Refresh();
    }
}

/*
** WM_DUMP_SET_END_SELECTION message
** wEnd is the last byte selected. It is converted in line/row.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetEndSelection(WORD wEnd, BOOL bRefresh) {
    WORD wEndLine;
    WORD wEndRow;

    if (wEnd == DUMP_NO_SELECTION) {
        wEndLine = wEndRow = DUMP_NO_SELECTION;
    }
    else {
        wEndLine = wEnd / GetNumberOfBytesPerLine();
        wEndRow = wEnd % GetNumberOfBytesPerLine();
    }
    SetEndLine(wEndLine);
    SetEndRow(wEndRow);
    if (bRefresh) {
        Refresh();
    }
}

/*
** WM_DUMP_GET_BEGIN_SELECTION message
** returns the offset of the first byte selected.
*/
WORD MemoryInspectorControlImpl::GetBeginSelection() {

    auto wBeginLine = GetBeginLine();
    auto wBeginRow = GetBeginRow();
    if (wBeginLine == DUMP_NO_SELECTION) {
        return DUMP_NO_SELECTION;
    }
    return wBeginLine * GetNumberOfBytesPerLine() + wBeginRow;
}

/*
** WM_DUMP_GET_END_SELECTION message
** returns offset of the last byte selected.
*/
WORD MemoryInspectorControlImpl::GetEndSelection() {

    auto wEndLine = GetEndLine();
    auto wEndRow = GetEndRow();
    if (wEndLine == DUMP_NO_SELECTION) {
        return DUMP_NO_SELECTION;
    }
    else {
        return wEndLine * GetNumberOfBytesPerLine() + wEndRow;
    }
}

/*
** clear the rest of the window
*/
void MemoryInspectorControlImpl::FillRestOfWindow(HDC hDC) {
    const auto wSize = GetSize();
    const WORD wNbLines = (wSize + GetNumberOfBytesPerLine() - 1) / GetNumberOfBytesPerLine();
    RECT rc;
    GetClientRect(hWnd, &rc);
    const auto wFontHeight = GetFontHeight();
    const auto wMaxLines = (WORD)(rc.bottom / wFontHeight);
    rc.top = ((wNbLines < wMaxLines) ? wNbLines : wMaxLines) * wFontHeight;
    FillRect(hDC, &rc, (HBRUSH)GetStockBrush(WHITE_BRUSH));
}

/*
** Initialize scroll bar position and range.
*/
void MemoryInspectorControlImpl::InitScroll() {
    WORD wNbLines;
    WORD wMaxLines;
    WORD wMaxScroll;

    if (!(GetWindowLong(hWnd, GWL_STYLE) & WS_VSCROLL)) {
        SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxScroll), 0);
        return;
    }
    auto wSize = GetSize();
    wMaxLines = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines));
    const WORD wNumberOfBytesPerLine = GetNumberOfBytesPerLine();
    if (wNumberOfBytesPerLine > 0) {
        wNbLines = (wSize + wNumberOfBytesPerLine - 1) / wNumberOfBytesPerLine;
    }
    else {
        wNbLines = 0;
    }
    if (wNbLines <= wMaxLines) {
        wMaxScroll = 0;
    }
    else {
        wMaxScroll = wNbLines - wMaxLines;
    }
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxScroll), wMaxScroll);
    SetScrollRange(hWnd, SB_VERT, 0, (wMaxScroll ? wMaxScroll : 1), FALSE);
    SetScroll(0, FALSE);
}

/*
** WM_DUMP_SET_BUFFER message
** Set the buffer pointer to the data buffer.
** This function does not redraw  the control.
*/
void MemoryInspectorControlImpl::SetBuffer(BYTE* buffer) {
    SetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, buffer), (LONG_PTR)buffer);
}

BYTE* MemoryInspectorControlImpl::GetBuffer() {
    return  (BYTE*)GetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, buffer));
}

/*
** WM_DUMP_SET_TYPE_BUFFER message
** Sets the pointer to the type buffer.
** Note: the 2 buffers must have the same size given in WM_DUMP_SET_BUFFER.
** This function does not redraw the control.
*/
void MemoryInspectorControlImpl::SetTypeBuffer(BYTE* typeBuffer) {
    SetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, pType), (LONG_PTR)typeBuffer);
}

BYTE* MemoryInspectorControlImpl::GetTypeBuffer() {
    return (BYTE*)GetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, pType));
}

/*
** WM_DUMP_SET_INTERNAL message
** bInternal is TRUE if we want to display bytes with ANTIC internal codes.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetInternal(BOOL bInternal, BOOL bRefresh) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, bInternal), (WORD)bInternal);
    if (bRefresh) {
        Refresh();
    }
}

BOOL MemoryInspectorControlImpl::IsInternal() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, bInternal));
}

/*
** WM_DUMP_SET_EDIT_MODE message
** bEditMode is TRUE if we want to edit bytes.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetEditMode(BOOL bEditMode, BOOL bRefresh) {
    constexpr UINT_PTR timerID = 1248;
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, bEditMode), (WORD)bEditMode);
    if (bEditMode) {
        SetEditedPart(0);
        SetTimerCount(0);
        SetTimer(hWnd, timerID, 250, NULL);
    }
    else {
        KillTimer(hWnd, timerID);
        SetEndLine(GetBeginLine());
        SetEndRow(GetBeginRow());
    }
    if (bRefresh) {
        Refresh();
    }
}

BOOL MemoryInspectorControlImpl::IsEditMode() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, bEditMode));
}

void MemoryInspectorControlImpl::SetEditedPart(WORD wEditedPart) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wEditedPart), wEditedPart);
}
WORD MemoryInspectorControlImpl::GetEditedPart() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wEditedPart));
}
void MemoryInspectorControlImpl::SetTimerCount(WORD wTimerCount) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wTimerCount), wTimerCount);

}
WORD MemoryInspectorControlImpl::GetTimerCount() {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wTimerCount));
}

void MemoryInspectorControlImpl::SetFontHeight(WORD wFontHeight) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wFontHeight), wFontHeight);
}

WORD  MemoryInspectorControlImpl::GetFontHeight() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wFontHeight));
}

void MemoryInspectorControlImpl::SetFontWidth(WORD wFontWidth) {
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wFontWidth), wFontWidth);
}

WORD  MemoryInspectorControlImpl::GetFontWidth() const {
    return GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wFontWidth));
}

/*
** WM_SETFONT message
** set font to use.
** if bRefresh is TRUE, control is redrawn.
*/
void MemoryInspectorControlImpl::SetFont(HFONT hFont, BOOL bRefresh) {
    TEXTMETRIC tm;
    HFONT hOldFont;
    WORD wMaxLines;
    RECT rc;

    SetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, hFont), (LONG_PTR)hFont);
    auto hDC = GetDC(hWnd);
    if (hDC) {
        if (hFont) {
            hOldFont = (HFONT)SelectObject(hDC, hFont);
        }
        GetTextMetrics(hDC, &tm);
        if (hFont) {
            SelectObject(hDC, hOldFont);
        }
        ReleaseDC(hWnd, hDC);
    }
    else {
        tm.tmAveCharWidth = 8;
        tm.tmHeight = 12;
    }
    GetClientRect(hWnd, &rc);
    wMaxLines = (WORD)(rc.bottom / tm.tmHeight);
    SetFontWidth((WORD)tm.tmAveCharWidth);
    SetFontHeight((WORD)tm.tmHeight);
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines), wMaxLines);
    InitScroll();
    if (bRefresh) {
        Refresh();
    }
}

/*
** WM_SIZE message
** Determine new number of lines in the window.
*/
LONG MemoryInspectorControlImpl::Size() {
    RECT rc;

    auto wFontHeight = GetFontHeight();;
    GetClientRect(hWnd, &rc);
    auto wMaxLines = (WORD)(rc.bottom / wFontHeight);
    SetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines), wMaxLines);
    InitScroll();
    return NULL;
}

/*
** WM_CREATE message
** Initialization of the window private structure in the extra bytes.
** The default setting is no buffer (0 byte LONG) with system font.
*/
LONG MemoryInspectorControlImpl::Create() {
    SetBeginLine(DUMP_NO_SELECTION);
    SetBeginRow(DUMP_NO_SELECTION);
    SetEndLine(DUMP_NO_SELECTION);
    SetEndRow(DUMP_NO_SELECTION);
    SetFont(0, FALSE);
    SetBuffer(nullptr);
    SetTypeBuffer(nullptr);
    SetSize(0);
    SetStart(0);
    SetScroll(0, FALSE);
    SetInternal(0, FALSE);
    Size();
    SetEditMode(FALSE, FALSE);
    return (LONG)0;
}

/*
** WM_DESTROY message
*/
LONG MemoryInspectorControlImpl::Destroy() {
    SetEditMode(FALSE, FALSE);
    return (LONG)0;
}

/*
** print only one line.
*/
void MemoryInspectorControlImpl::PrintLine(HDC hDC, WORD wLine, WORD wNbLines) {
    wchar_t szBuf[40];
    WORD wScroll;
    WORD wHeight;
    WORD wStart;
    WORD wNbRows;
    WORD wOffset;
    WORD wRow;
    WORD wRowSel;
    byte bByte;
    byte cType, cOldType;
    WORD wPos;
    RECT rc = {};

    // Check if we have a buffer.
    auto buffer = GetBuffer();
    if (buffer == nullptr) {
        return;
    }
    auto lpType = GetTypeBuffer();

    // Get graphic characteristics from window extra bytes.
    auto wFontWidth = GetFontWidth();
    auto wFontHeight = GetFontHeight();
    wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
    wStart = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wStart));
    auto wSize = GetSize();
    auto wBeginLine = GetBeginLine();
    auto wBeginRow = GetBeginRow();
    auto bEditMode = IsEditMode();
    auto bInternal = IsInternal();

    // draw start address
    wHeight = (wLine - wScroll) * wFontHeight;
    const auto wNumberOfBytesPerLine = MemoryInspectorControlImpl::GetNumberOfBytesPerLine();
    wsprintf(szBuf, L"%04hX\x7C", wStart + (wLine * wNumberOfBytesPerLine));
    SetTextColor(hDC, RGB(0, 0, 0));
    TextOut(hDC, 1, wHeight, szBuf, String::Length(szBuf));
    if (wLine == wNbLines - 1) {
        wNbRows = ((wSize - 1) % wNumberOfBytesPerLine) + 1;
    }
    else {
        wNbRows = wNumberOfBytesPerLine;
    }

    // draw hexadecimal and bytes.
    szBuf[0] = 0;
    cOldType = 0xFF;
    for (wRow = 0; wRow < wNbRows; wRow++) {
        wOffset = (wLine * wNumberOfBytesPerLine) + wRow;
        bByte = buffer[wOffset];
        cType = lpType[wOffset];
        if (wOffset > 0) {
            if ((cOldType != static_cast<BYTE>(MemoryType::LOBYTE)) && (cOldType != static_cast<BYTE>(MemoryType::HIBYTE))
                && ((lpType[wOffset - 1] == static_cast<BYTE>(MemoryType::LOBYTE)) || (lpType[wOffset - 1] == static_cast<BYTE>(MemoryType::HIBYTE)))) {
                cType = lpType[wOffset - 1];
            }
            else if ((cType == static_cast<BYTE>(MemoryType::LOBYTE)) || (cType == static_cast<BYTE>(MemoryType::HIBYTE))) {
                cType = static_cast<BYTE>(MemoryType::CODE);
            }
        }
        else if ((cType == static_cast<BYTE>(MemoryType::LOBYTE)) || (cType == static_cast<BYTE>(MemoryType::HIBYTE))) {
            cType = static_cast<BYTE>(MemoryType::CODE);
        }
        else {
            cType = static_cast<BYTE>(MemoryType::UNKNOWN);
        }
        if (cType >= MEMORY_TYPE_ENUM_ITEM_COUNT) {
            cType = static_cast<BYTE>(MemoryType::UNKNOWN);
        }

        cOldType = cType;
        wsprintf(szBuf, L"%02hX ", bByte);
        SetTextColor(hDC, dwMemoryInspectorColor[cType]);
        wPos = (wFontWidth * (5 + (3 * wRow))) + 1;
        TextOut(hDC, wPos, wHeight, szBuf, String::Length(szBuf));

        // is there a selection in edit mode ?
        auto wEditedPart = GetEditedPart();
        auto wTimerCount = GetTimerCount();
        if ((wBeginLine != DUMP_NO_SELECTION) && (bEditMode) && (wLine == wBeginLine) && (wRow == wBeginRow)) {


            INT nW;
            LPWSTR lpChar;

            if (wEditedPart == 0) {
                rc.left = wPos;
                rc.right = rc.left + wFontWidth;
                nW = 1;
                if (wTimerCount == 0) {
                    wcscpy(szBuf, L"  ");
                }
                lpChar = szBuf;
            }
            else if (wEditedPart == 1) {
                rc.left = wPos + wFontWidth;
                rc.right = rc.left + wFontWidth;
                nW = 1;
                if (wTimerCount == 0) {
                    wcscpy(szBuf, L"  ");
                }
                lpChar = szBuf + 1;
                wPos += wFontWidth;
            }
            else {
                rc.left = wPos;
                rc.right = rc.left + (wFontWidth * 2);
                nW = 2;
                lpChar = szBuf;
            }
            rc.top = wHeight;
            rc.bottom = wHeight + wFontHeight;
            SetBkColor(hDC, RGB(255, 255, 0));
            SetTextColor(hDC, RGB(0, 0, 0));
            ExtTextOut(hDC, wPos, wHeight, ETO_OPAQUE, &rc, lpChar, nW, NULL);
            SetTextColor(hDC, dwMemoryInspectorColor[cType]);
            SetBkColor(hDC, RGB(255, 255, 255));
        }

        // display ASCII or ATASCII character
        byte bChar = bByte & 0xff;
        if (bInternal) {
            if (bChar < 64) {
                bChar += 32;
            }
            else if (bChar < 96) {
                bChar -= 64;
            }
            else if ((bChar >= 128) && (bChar < 128 + 64)) {
                bChar += 32;
            }
            else if ((bChar >= 128 + 64) && (bChar < 128 + 96)) {
                bChar -= 64;
            }
        }
        wchar_t cChar = bChar;

        // is there a selection in edit mode ?
        if ((wBeginLine != DUMP_NO_SELECTION) && (bEditMode) && (wLine == wBeginLine) && (wRow == wBeginRow)) {
            rc.left = (wFontWidth * (5 + (wNumberOfBytesPerLine * 3) + wRow)) + 1;
            rc.right = rc.left + wFontWidth;
            rc.top = wHeight;
            rc.bottom = wHeight + wFontHeight;
            SetBkColor(hDC, RGB(255, 255, 0));
            SetTextColor(hDC, RGB(0, 0, 0));
            if ((wEditedPart == 2) && (wTimerCount == 0)) {
                cChar = ' ';
            }
            ExtTextOut(hDC, rc.left, wHeight, ETO_OPAQUE, &rc, &cChar, 1, NULL);
            SetBkColor(hDC, RGB(255, 255, 255));
        }
        else {
            TextOut(hDC, (wFontWidth * (5 + (wNumberOfBytesPerLine * 3) + wRow)) + 1, wHeight, &cChar, 1);
        }
        SetTextColor(hDC, RGB(0, 0, 0));
    }

    // is there a selection in read-only mode ?
    if ((wBeginLine != DUMP_NO_SELECTION) && (!bEditMode)) {
        WORD wFirstLineSelected;
        WORD wLastLineSelected;
        WORD wFirstRowSelected;
        WORD wLastRowSelected;

        auto wEndLine = GetEndLine();
        auto wEndRow = GetEndRow();

        if (wBeginLine > wEndLine) {
            wFirstLineSelected = wEndLine;
            wLastLineSelected = wBeginLine;
            wFirstRowSelected = wEndRow;
            wLastRowSelected = wBeginRow;
        }
        else if (wBeginLine < wEndLine) {
            wFirstLineSelected = wBeginLine;
            wLastLineSelected = wEndLine;
            wFirstRowSelected = wBeginRow;
            wLastRowSelected = wEndRow;
        }
        else {
            wFirstLineSelected = wLastLineSelected = wBeginLine;
            if (wBeginRow > wEndRow) {
                wFirstRowSelected = wEndRow;
                wLastRowSelected = wBeginRow;
            }
            else {
                wFirstRowSelected = wBeginRow;
                wLastRowSelected = wEndRow;
            }
        }
        if (wLine >= wFirstLineSelected && wLine <= wLastLineSelected) {
            if (wLine == wFirstLineSelected) {
                wRowSel = wFirstRowSelected;
            }
            else {
                wRowSel = 0;
            }
            INT nX1, nY, nW1, nH;
            INT nX2, nW2;

            nX1 = (INT)(wFontWidth * (5 + (wRowSel * 3))) + 1;
            nX2 = (INT)(wFontWidth * (5 + (wNumberOfBytesPerLine * 3) + wRowSel)) + 1;
            nY = (INT)wHeight;
            if (wLine == wLastLineSelected) {
                wRowSel = wLastRowSelected + 1 - wRowSel;
            }
            else {
                wRowSel = wNumberOfBytesPerLine - wRowSel;
            }
            nW1 = (int)(wFontWidth * wRowSel * 3) - wFontWidth;
            nW2 = (int)(wFontWidth * wRowSel);
            nH = wFontHeight;

            // invert lines
            PatBlt(hDC, nX1, nY, nW1, nH, DSTINVERT);
            PatBlt(hDC, nX2, nY, nW2, nH, DSTINVERT);
        }
    }

    // fill the rest of line if there is no more byte to display
    if (wRow < wNumberOfBytesPerLine) {
        wchar_t cChar = ' ';
        wcscpy(szBuf, L"   ");
        for (; wRow < wNumberOfBytesPerLine; wRow++) {
            TextOut(hDC, (wFontWidth * (5 + (wRow * 3))) + 1, wHeight, szBuf, String::Length(szBuf));
            TextOut(hDC, (wFontWidth * (5 + (wNumberOfBytesPerLine * 3) + wRow)) + 1, wHeight, &cChar, 1);
        }
    }

    // Write vertical bar to separte hex memoryInspector from string memoryInspector.
    TextOut(hDC, (wFontWidth * (5 + (wNumberOfBytesPerLine * 3) - 1)) + 1, wHeight, L"\x7C", 1);
}

/*
** draw one line
*/
void MemoryInspectorControlImpl::PrintOne(HDC hDC, WORD wLine) {
    HFONT hFont;
    HFONT hOldFont;
    WORD wNbLines;

    // Check if we have a buffer.
    auto buffer = GetBuffer();
    if (buffer == nullptr) {
        return;
    }


    // select font and print all lines
    hFont = (HFONT)GetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, hFont));
    if (hFont) {
        hOldFont = (HFONT)SelectObject(hDC, hFont);
    }

    auto wSize = GetSize();
    wNbLines = (wSize + MemoryInspectorControlImpl::GetNumberOfBytesPerLine() - 1) / MemoryInspectorControlImpl::GetNumberOfBytesPerLine();
    PrintLine(hDC, wLine, wNbLines);
    if (hFont) {
        SelectObject(hDC, hOldFont);
    }
}

/*
** repaint all control.
*/
void MemoryInspectorControlImpl::PaintAll(HDC hDC) {
    HFONT hFont;
    HFONT hOldFont;
    WORD wScroll;
    WORD wLine;
    WORD wNbLines;
    WORD wMaxLine;
    WORD wMaxLineInWindow;

    // Check if we have a buffer.
    if (GetBuffer() == nullptr) {
        return;
    }

    // Select font and print all lines.
    hFont = (HFONT)GetWindowLongPtr(hWnd, FIELD_OFFSET(MemoryInspectorStruct, hFont));
    if (hFont) {
        hOldFont = (HFONT)SelectObject(hDC, hFont);
    }

    wMaxLineInWindow = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines));
    wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
    auto wSize = GetSize();
    wNbLines = (wSize + GetNumberOfBytesPerLine() - 1) / GetNumberOfBytesPerLine();
    wMaxLine = std::min<WORD>(wNbLines, wScroll + wMaxLineInWindow + 1);
    for (wLine = wScroll; wLine < wMaxLine; wLine++) {
        PrintLine(hDC, wLine, wNbLines);
    }
    FillRestOfWindow(hDC);
    if (hFont) {
        SelectObject(hDC, hOldFont);
    }
}

/*
** WM_PAINT message
** redraw the buffer
*/
LONG MemoryInspectorControlImpl::Paint() {
    PAINTSTRUCT ps;
    HDC hDC;

    hDC = BeginPaint(hWnd, &ps);
    PaintAll(hDC);
    EndPaint(hWnd, &ps);
    return NULL;
}

/*
** WM_DUMP_REFRESH message
** refresh window
*/
void MemoryInspectorControlImpl::Refresh() {
    HDC hDC;

    hDC = GetDC(hWnd);
    if (hDC) {
        PaintAll(hDC);
        ReleaseDC(hWnd, hDC);
    }
}

/*
** WM_TIMER message
** refresh memoryInspector window
*/
void MemoryInspectorControlImpl::Timer(WPARAM wTimerId) {
    if (IsEditMode()) {
        auto wTimerCount = GetTimerCount();
        wTimerCount = (WORD)((wTimerCount + 1) % 4);
        SetTimerCount(wTimerCount);
        Refresh();
    }
}

/*
** WM_DUMP_SEEK_SELECTION message
** set first line to begining of selection
*/
void MemoryInspectorControlImpl::SeekSelection() {

    // if no selection, nothing to do.
    const auto wBeginLine = GetBeginLine();
    if (wBeginLine == DUMP_NO_SELECTION) {
        Refresh();
        return;
    }

    // We must scroll but we have to check if the selection can be drawn on the first line.
    const auto wMaxScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxScroll));
    const auto wScroll = std::min(wBeginLine, wMaxScroll);

    SetScroll(wScroll, TRUE);
}

/*
** scroll one line up (with BitBlt)
*/
BOOL MemoryInspectorControlImpl::ScrollUp(WORD wScroll, RECT* rc) {
    HDC hDC;

    hDC = GetDC(hWnd);
    if (hDC) {
        auto wFontHeight = GetFontHeight();
        BitBlt(hDC, rc->left, rc->top + wFontHeight, rc->right, rc->bottom - wFontHeight, hDC, 0, 0, SRCCOPY);
        PrintOne(hDC, wScroll);
        FillRestOfWindow(hDC);
        return TRUE;
    }
    return FALSE;
}

/*
** scroll one line down (with BitBlt)
*/
BOOL MemoryInspectorControlImpl::ScrollDown(WORD wScroll, RECT* rc, WORD wNbLines) {
    HDC hDC;

    hDC = GetDC(hWnd);
    if (hDC) {
        auto wFontHeight = GetFontHeight();
        BitBlt(hDC, rc->left, rc->top, rc->right, rc->bottom - wFontHeight, hDC, 0, wFontHeight, SRCCOPY);
        PrintOne(hDC, (WORD)(wScroll + wNbLines - 1));
        FillRestOfWindow(hDC);
        ReleaseDC(hWnd, hDC);
        return TRUE;
    }
    return FALSE;
}

/*
** WM_VSCROLL message
*/
void MemoryInspectorControlImpl::VScroll(WPARAM wParam, LPARAM lParam) {
    WORD wOldScroll;
    WORD wScroll;
    WORD wMaxScroll;
    WORD wNbLines;
    RECT rc;

    wOldScroll = wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
    wMaxScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxScroll));
    auto wFontHeight = GetFontHeight();
    GetClientRect(hWnd, &rc);
    wNbLines = (WORD)(rc.bottom / wFontHeight);
    switch (LOWORD(wParam)) {
    case SB_TOP:
        if (wScroll) {
            wScroll = 0;
            SetScroll(wScroll, TRUE);
        }
        break;

    case SB_BOTTOM:
        if (wScroll != wMaxScroll) {
            wScroll = wMaxScroll;
            SetScroll(wScroll, TRUE);
        }
        break;

    case SB_LINEUP:
        if (wScroll) {
            wScroll--;
            SetScroll(wScroll, FALSE);
            if (!ScrollUp(wScroll, &rc)) {
                Refresh();
            }
        }
        break;

    case SB_LINEDOWN:
        if (wScroll != wMaxScroll) {
            wScroll++;
            SetScroll(wScroll, FALSE);
            if (!ScrollDown(wScroll, &rc, wNbLines)) {
                Refresh();
            }
        }
        break;

    case SB_PAGEUP:
        if (wScroll) {
            wScroll -= std::min(wScroll, wNbLines);
            SetScroll(wScroll, TRUE);
        }
        break;

    case SB_PAGEDOWN:
        if (wScroll != wMaxScroll) {
            wScroll += std::min<WORD>(wMaxScroll - wScroll, wNbLines);
            SetScroll(wScroll, TRUE);
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        if ((wScroll != HIWORD(wParam)) && (HIWORD(wParam) <= wMaxScroll)) {
            wScroll = HIWORD(wParam);
            SetScroll(wScroll, TRUE);
        }
        break;
    }
    if (wOldScroll != wScroll) {
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), MemoryInspectorNotification::SCROLL_CHANGED), (LONG_PTR)hWnd);
    }
}

/*
** WM_LBUTTONDOWN message
*/
void MemoryInspectorControlImpl::LButtonDown(WPARAM wParam, LPARAM lParam) {
    WORD wScroll;
    WORD wBeginLine;
    WORD wBeginRow;
    BOOL bEditMode;

    // Check if we have a buffer.
    if (GetBuffer() == nullptr) {
        return;
    }

    // Capture mouse input.
    if (bMemoryInspectorCapture == FALSE) {
        SetCapture(hWnd);
    }
    bMemoryInspectorCapture = TRUE;

    // Get first line of selection
    wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
    auto wFontWidth = GetFontWidth();
    auto wFontHeight = GetFontHeight();
    wBeginLine = wScroll + (HIWORD(lParam) / wFontHeight);
    bEditMode = IsEditMode();
    WORD wEditedPart = 0;
    if (LOWORD(lParam) < (5 * wFontWidth)) {
        wBeginRow = 0;
    }
    else {
        const WORD wStartOfAtasciiPaneInPixel = ((GetNumberOfBytesPerLine() * 3) + 5) * wFontWidth;
        if (LOWORD(lParam) > wStartOfAtasciiPaneInPixel) {
            wBeginRow = (LOWORD(lParam) - 1 - wStartOfAtasciiPaneInPixel) / wFontWidth;
            wEditedPart = 2;
        }
        else {
            wBeginRow = (LOWORD(lParam) - 1 - (4 * wFontWidth) - (wFontWidth / 2)) / (3 * wFontWidth);
            if (((LOWORD(lParam) - 1 - (4 * wFontWidth) - (wFontWidth / 2)) % (3 * wFontWidth)) > ((3 * wFontWidth) / 2)) {
                wEditedPart = 1;
            }
        }
        if (wBeginRow > GetNumberOfBytesPerLine() - 1) {
            wBeginRow = GetNumberOfBytesPerLine() - 1;
        }
    }

    // Save new selection and redraw control.
    SetBeginLine(wBeginLine);
    SetBeginRow(wBeginRow);

    // Set end of selection to beginning of selection
    if (bEditMode) {
        SetEditedPart(wEditedPart);
    }
    else {
        SetEndLine(wBeginLine);
        SetEndRow(wBeginRow);
    }

    Refresh();
}

/*
** Set end of selection (scroll window if selection extends outside window).
*/
void MemoryInspectorControlImpl::SetEndOfSelection(LPARAM lParam) {
    WORD wEndLine;
    WORD wEndRow;
    RECT rc;
    WORD wEditedPart;

    // determine new end of selection
    auto wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
    auto wFontWidth = GetFontWidth();
    auto wFontHeight = GetFontHeight();
    auto wMaxLines = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines));
    auto bEditMode = IsEditMode();
    wEditedPart = 0;
    GetClientRect(hWnd, &rc);
    if ((short)HIWORD(lParam) < (short)0) {
        VScroll(SB_LINEUP, 0);
        wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
        wEndLine = wScroll;
    }
    else if (HIWORD(lParam) > (WORD)rc.bottom) {
        VScroll(SB_LINEDOWN, 0);
        wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
        wEndLine = wScroll + wMaxLines - 1;
    }
    else {
        wEndLine = wScroll + (HIWORD(lParam) / wFontHeight);
    }
    if (LOWORD(lParam) < (5 * wFontWidth)) {
        wEndRow = 0;
    }
    else {
        const WORD wStartOfAtasciiPaneInPixel = ((GetNumberOfBytesPerLine() * 3) + 5) * wFontWidth;
        if (LOWORD(lParam) > wStartOfAtasciiPaneInPixel) {
            wEndRow = (LOWORD(lParam) - 1 - wStartOfAtasciiPaneInPixel) / wFontWidth;
            wEditedPart = 2;
        }
        else {
            wEndRow = (LOWORD(lParam) - 1 - (4 * wFontWidth) - (wFontWidth / 2)) / (3 * wFontWidth);
            if (((LOWORD(lParam) - 1 - (4 * wFontWidth) - (wFontWidth / 2)) % (3 * wFontWidth)) > ((3 * wFontWidth) / 2)) {
                wEditedPart = 1;
            }
        }
        if (wEndRow > GetNumberOfBytesPerLine() - 1) {
            wEndRow = GetNumberOfBytesPerLine() - 1;
        }
    }

    // Save new end of selection and redraw control.
    if (bEditMode) {
        SetBeginLine(wEndLine);
        SetBeginRow(wEndRow);
        SetEditedPart(wEditedPart);
        SetEndRow(wEndRow); // This probably has no effect
    }
    else {
        SetEndLine(wEndLine);
        SetEndRow(wEndRow);
    }
    Refresh();
}

/*
** WM_MOUSEMOVE message
*/
void MemoryInspectorControlImpl::MouseMove(WPARAM wParam, LPARAM lParam) {
    // Check if we have a buffer.
    if (GetBuffer() == nullptr) {
        return;
    }

    // check we are in selection mode.
    if (!bMemoryInspectorCapture) {
        return;
    }

    // determine new end of selection
    SetEndOfSelection(lParam);
}

/*
** WM_LBUTTONUP message
*/
void MemoryInspectorControlImpl::LButtonUp(WPARAM wParam, LPARAM lParam) {
    // Check if we have a buffer.
    if (GetBuffer() == nullptr) {
        return;
    }

    // Release mouse input.
    if (bMemoryInspectorCapture) {
        ReleaseCapture();
    }
    bMemoryInspectorCapture = FALSE;

    // determine new end of selection
    SetEndOfSelection(lParam);

    // notify parent of the selection change.
    SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), MemoryInspectorNotification::SELECTION_CHANGED), (LONG_PTR)hWnd);
}

/*
** WM_RBUTTONDOWN message
*/
void MemoryInspectorControlImpl::RButtonDown(WPARAM wParam, LPARAM lParam) {
    // Check if we have a buffer.
    auto buffer = GetBuffer();
    if (buffer == nullptr) {
        return;
    }

    // notify parent of the right button click.
    SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), MemoryInspectorNotification::RBUTTONDOWN), (LONG_PTR)hWnd);
}

/*
** WM_LBUTTONDBLCLK message
*/
void MemoryInspectorControlImpl::LButtonDblClk(WPARAM wParam, LPARAM lParam) {
    // Notify parent of the left button double click.
    SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), MemoryInspectorNotification::LBUTTONDBLCLK), (LONG_PTR)hWnd);
    SetEndOfSelection(lParam);
}

/*
** WM_KEYDOWN message
*/
void MemoryInspectorControlImpl::KeyDown(WPARAM wParam) {
    BOOL bRedraw;
    WORD wScroll;
    WORD wMaxLineInWindow;

    // Check if we have a buffer.
    auto buffer = GetBuffer();
    if (buffer == nullptr) {
        return;
    }

    bRedraw = FALSE;
    auto wBegin = GetBeginSelection();
    auto wSize = GetSize();
    auto bEditMode = IsEditMode();

    if (!bEditMode) {
        switch (wParam) {
        case VK_NEXT:
            return VScroll(SB_PAGEDOWN, 0);
        case VK_PRIOR:
            return VScroll(SB_PAGEUP, 0);
        case VK_UP:
            return VScroll(SB_LINEUP, 0);
        case VK_DOWN:
            return VScroll(SB_LINEDOWN, 0);
        case VK_END:
            return VScroll(SB_BOTTOM, 0);
        case VK_HOME:
            return VScroll(SB_TOP, 0);
        default:
            break;
        }
        return;
    }
    if ((wBegin != DUMP_NO_SELECTION) && (wBegin < wSize)) {
        auto wEditedPart = GetEditedPart();
        if (wEditedPart < 2) {
            if (wParam == VK_UP) {
                if (wBegin >= MemoryInspectorControlImpl::GetNumberOfBytesPerLine()) {
                    wEditedPart = 0;
                    wBegin -= GetNumberOfBytesPerLine();
                }
                bRedraw = TRUE;
            }
            else if (wParam == VK_DOWN) {
                if (wBegin + GetNumberOfBytesPerLine() < wSize) {
                    wEditedPart = 0;
                    wBegin += GetNumberOfBytesPerLine();
                }
                bRedraw = TRUE;
            }
            else if (wParam == VK_LEFT) {
                if (wEditedPart == 1) {
                    wEditedPart = 0;
                    bRedraw = TRUE;
                }
                else if (wBegin > 0) {
                    wBegin--;
                    wEditedPart = 1;
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_RIGHT) {
                if (wEditedPart == 0) {
                    wEditedPart = 1;
                    bRedraw = TRUE;
                }
                else if (wBegin < (WORD)(wSize - 1)) {
                    wBegin++;
                    wEditedPart = 0;
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_HOME) {
                if ((wBegin != 0) || (wEditedPart == 1)) {
                    wBegin = 0;
                    wEditedPart = 0;
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_END) {
                if ((wBegin != (WORD)(wSize - 1)) || (wEditedPart == 1)) {
                    wBegin = (WORD)(wSize - 1);
                    wEditedPart = 0;
                    bRedraw = TRUE;
                }
            }
        }
        else {
            if (wParam == VK_UP) {
                if (wBegin >= GetNumberOfBytesPerLine()) {
                    wBegin -= GetNumberOfBytesPerLine();
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_DOWN) {
                if (wBegin + GetNumberOfBytesPerLine() < wSize) {
                    wBegin += GetNumberOfBytesPerLine();
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_LEFT) {
                if (wBegin > 0) {
                    wBegin--;
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_RIGHT) {
                if (wBegin < (WORD)(wSize - 1)) {
                    wBegin++;
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_HOME) {
                if (wBegin != 0) {
                    wBegin = 0;
                    bRedraw = TRUE;
                }
            }
            else if (wParam == VK_END) {
                if (wBegin != (WORD)(wSize - 1)) {
                    wBegin = (WORD)(wSize - 1);
                    bRedraw = TRUE;
                }
            }
        }
        if (bRedraw) {
            SetEditedPart(wEditedPart);
            SetBeginSelection(wBegin, TRUE);
            wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
            wMaxLineInWindow = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines));
            if ((wBegin / GetNumberOfBytesPerLine()) < wScroll) {
                SetScroll((WORD)(wBegin / GetNumberOfBytesPerLine()), TRUE);
            }
            else if ((wBegin / GetNumberOfBytesPerLine()) >= (wScroll + wMaxLineInWindow)) {
                SetScroll((WORD)((wBegin / GetNumberOfBytesPerLine()) - wMaxLineInWindow + 1), TRUE);
            }
        }
    }
}

/*
** WM_CHAR message
*/
void MemoryInspectorControlImpl::Char(WPARAM wParam) {
    BYTE cChar;
    BYTE cType;
    WORD wScroll;
    WORD wMaxLineInWindow;

    // Check if we have a buffer.
    auto buffer = GetBuffer();
    if (buffer == nullptr) {
        return;
    }

    auto typeBuffer = GetTypeBuffer();
    BOOL bRedraw = FALSE;
    auto wBegin = GetBeginSelection();
    auto wSize = GetSize();
    auto bEditMode = IsEditMode();
    if ((bEditMode) && (wBegin != DUMP_NO_SELECTION) && (wBegin < wSize)) {
        cChar = buffer[wBegin];
        auto wEditedPart = GetEditedPart();
        if (wEditedPart < 2) {
            if (wParam == VK_TAB) {
                wEditedPart = 2;
                bRedraw = TRUE;
            }
            else if ((wParam >= '0') && (wParam <= '9')) {
                if (wEditedPart == 0) {
                    cChar = (BYTE)((cChar & 0x0F) | (((wParam - '0') & 0x0F) << 4));
                }
                else {
                    cChar = (BYTE)((cChar & 0xF0) | ((wParam - '0') & 0x0F));
                }
                buffer[wBegin] = cChar;
                wEditedPart = (WORD)((wEditedPart + 1) & 0x01);
                if (wEditedPart == 0) {
                    wBegin++;
                }
                bRedraw = TRUE;
            }
            else if ((wParam >= 'a') && (wParam <= 'f')) {
                if (wEditedPart == 0) {
                    cChar = (BYTE)((cChar & 0x0F) | (((wParam - 'a' + 10) & 0x0F) << 4));
                }
                else {
                    cChar = (BYTE)((cChar & 0xF0) | ((wParam - 'a' + 10) & 0x0F));
                }
                buffer[wBegin] = cChar;
                wEditedPart = (WORD)((wEditedPart + 1) & 0x01);
                if (wEditedPart == 0) {
                    wBegin++;
                }
                bRedraw = TRUE;
            }
            else if ((wParam >= 'A') && (wParam <= 'F')) {
                if (wEditedPart == 0) {
                    cChar = (BYTE)((cChar & 0x0F) | (((wParam - 'A' + 10) & 0x0F) << 4));
                }
                else {
                    cChar = (BYTE)((cChar & 0xF0) | ((wParam - 'A' + 10) & 0x0F));
                }
                buffer[wBegin] = cChar;
                wEditedPart = (WORD)((wEditedPart + 1) & 0x01);
                if (wEditedPart == 0) {
                    wBegin++;
                }
                bRedraw = TRUE;
            }
        }
        else {
            if (wParam == VK_TAB) {
                wEditedPart = 0;
                bRedraw = TRUE;
            }
            else if ((wParam >= ' ') && (wParam < 128) && (wParam != '~') && (wParam != '{') && (wParam != '}')) {
                cChar = (BYTE)wParam;
                cType = typeBuffer[wBegin];
                if (cType == static_cast<BYTE>(MemoryType::SBYTE)) {
                    if (cChar < 32) {
                        cChar += 64;
                    }
                    else if (cChar < 96) {
                        cChar -= 32;
                    }
                }
                buffer[wBegin] = cChar;
                wBegin++;
                bRedraw = TRUE;
            }
            else if (wParam == VK_RETURN) {
                cChar = (BYTE)0x9B;
                buffer[wBegin] = cChar;
                wBegin++;
                bRedraw = TRUE;
            }
        }
        if (bRedraw) {
            SetEditedPart(wEditedPart);
            if (wBegin >= wSize) {
                wBegin = DUMP_NO_SELECTION;
                SetEditMode(FALSE, FALSE);
            }
            SetBeginSelection(wBegin, TRUE);
            if (wBegin != DUMP_NO_SELECTION) {
                wScroll = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));
                wMaxLineInWindow = GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wMaxLines));
                if ((wBegin / GetNumberOfBytesPerLine()) < wScroll) {
                    SetScroll((WORD)(wBegin / GetNumberOfBytesPerLine()), TRUE);
                }
                else if ((wBegin / GetNumberOfBytesPerLine()) >= (wScroll + wMaxLineInWindow)) {
                    SetScroll((WORD)((wBegin / GetNumberOfBytesPerLine()) - wMaxLineInWindow + 1), TRUE);
                }
            }
            else {
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), MemoryInspectorNotification::QUIT_EDIT_MODE), (LONG_PTR)hWnd);
            }
        }
    }
}

/*
** MemoryInspectorControlImpl window proc.
*/
LRESULT MemoryInspectorControlImpl::WndProc(Window::MESSAGE message, WPARAM wParam, LPARAM lParam) {
    switch (message) {

    case WM_DUMP_SET_NUMBER_OF_BYTES_PER_LINE:
        SetNumberOfBytesPerLine((WORD)lParam);
        return NULL;

    case WM_DUMP_SET_BUFFER:
        SetBuffer((BYTE*)lParam);
        SetSize((WORD)wParam);
        return NULL;

    case WM_DUMP_SET_TYPE_BUFFER:
        SetTypeBuffer((BYTE*)lParam);
        SetStart((WORD)wParam);
        return NULL;

    case WM_DUMP_SET_SCROLL:
        SetScroll((WORD)wParam, (BOOL)lParam);
        return NULL;

    case WM_DUMP_GET_SCROLL:
        return (LONG)(DWORD)GetWindowWord(hWnd, FIELD_OFFSET(MemoryInspectorStruct, wScroll));

    case WM_DUMP_SET_INTERNAL:
        SetInternal(wParam, (BOOL)lParam);
        return NULL;

    case WM_DUMP_GET_INTERNAL:
        return (LONG)IsInternal();

    case WM_DUMP_SET_EDIT_MODE:
        SetEditMode(wParam, TRUE);
        return NULL;

    case WM_DUMP_GET_EDIT_MODE:
        return (LONG)IsEditMode();

    case WM_DUMP_SET_BEGIN_SELECTION:
        SetBeginSelection((WORD)wParam, (BOOL)lParam);
        return NULL;

    case WM_DUMP_GET_BEGIN_SELECTION:
        return GetBeginSelection();

    case WM_DUMP_SET_END_SELECTION:
        SetEndSelection((WORD)wParam, (BOOL)lParam);
        return NULL;

    case WM_DUMP_GET_END_SELECTION:
        return GetEndSelection();

    case WM_DUMP_SEEK_SELECTION:
        SeekSelection();
        return NULL;

    case WM_DUMP_REFRESH:
        Refresh();
        return NULL;

    case WM_SETFONT:
        SetFont((HFONT)wParam, (BOOL)lParam);
        return NULL;

    case WM_GETFONT:
        return GetWindowLong(hWnd, FIELD_OFFSET(MemoryInspectorStruct, hFont));

    case WM_CREATE:
        return Create();

    case WM_SIZE:
        return Size();

    case WM_PAINT:
        return Paint();

    case WM_VSCROLL:
        VScroll(wParam, lParam);
        return NULL;

    case WM_LBUTTONDOWN:
        SetFocus(hWnd);
        LButtonDown(wParam, lParam);
        return NULL;

    case WM_LBUTTONDBLCLK:
        LButtonDblClk(wParam, lParam);
        return NULL;

    case WM_MOUSEMOVE:
        MouseMove(wParam, lParam);
        return NULL;

    case WM_LBUTTONUP:
        LButtonUp(wParam, lParam);
        return NULL;

    case WM_RBUTTONDOWN:
        SetFocus(hWnd);
        RButtonDown(wParam, lParam);
        return NULL;

    case WM_TIMER:
        Timer(wParam);
        return NULL;

    case WM_CHAR:
        Char(wParam);
        return NULL;

    case WM_KEYDOWN:
        KeyDown(wParam);
        return NULL;

    case WM_DESTROY:
        return Destroy();

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
