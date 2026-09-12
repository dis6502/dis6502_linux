/*
** DisassemblyControl.h

** Disassembly control implementation used to display a disassembly listing.
*/
#pragma once

#include "DisassemblyBuffer.h"
#define NOMINMAX
#include "Control.h"
#include "DC.h"
#include "DisassemblyResult.h"
#include "Font.h"
#include "Memory.h"
#include "SegmentTypes.h"
#include "Window.h"
#include <Windows.h>

constexpr UINT WM_DIS_BASE = (WM_USER + 1234);
typedef DWORD* LP_DIS_HISTORY;

/*
** messages to set or get sprite control parameters
*/
typedef enum {
    WM_DIS_SET_RESULT = WM_DIS_BASE,        // lParam = DisassemblyResult address
    WM_DIS_SET_LINE_NUMBERS_ACTIVE,         // wParam = TRUE if line numbers should be displayed
    WM_DIS_GET_LINE_NUMBERS_ACTIVE,         // return TRUE if line number is displayed
    WM_DIS_SET_SCROLL_LINE_NUMBER,          // lParam = first line to display (DWORD)
    WM_DIS_GET_SCROLL_LINE_NUMBER,          // return first line displayed (DWORD)
    WM_DIS_REFRESH,                         // redraw control
    WM_DIS_SYNC_LINE,                       // center a line in the window. Do not change selection. lParam is the line number
    WM_DIS_SELECT_LINE,                     // center a line in the window. Select line. lParam is the line number
    WM_DIS_SELECT_LINE_WITH_INFO,           // select a line by searching for information in DIS_LINE structure. LOWORD(lParam) = offset and HIWORD(lParam) = segment
    WM_DIS_EXTEND_SEL,                      // extends the selection, if present, to HIWORD(lParam) = segment and LOWORD(lParam) = offset
    WM_DIS_SELECT_DEFINITION,               // select line where a label is defined. lParam = address of label name
    WM_DIS_SELECT_ALL_REFERENCES,           // select all the lines where a label is referenced including definition. lParam = address of label name
    WM_DIS_GET_LABEL_REFERENCE,             // return the label reference in the line under the mouse cursor
    WM_DIS_GET_LABEL_DEFINITION,            // return the label definition in the line under the mouse cursor
    WM_DIS_RESET_HISTORY,                   // reset the history
    WM_DIS_GET_HISTORY_INDEX,               // return the history index (0 for no history)
    WM_DIS_BACK_IN_HISTORY                  // jump to the previous position in history
} DIS_MSG;

class DisStruct;

class DisassemblyControlImpl : public Control {

public:
    static constexpr auto CLASS_NAME = L"DisassemblyControlClass";

    __declspec(dllexport) static void _cdecl RegisterClassFor(HINSTANCE hInstance);

    DisassemblyControlImpl(HWND hWnd);
    LRESULT WndProc(Window::MESSAGE message, WPARAM wParam, LPARAM lParam) override;

private:
    using LineCount = DisassemblyResult::LineCount;
    using LineNumber = DisassemblyResult::LineNumber;

    typedef WORD POSITION;

    //DisStruct* GetDisStruct() const;

    // Instance Attribute Access
    DisassemblyResult* GetResult() const;
    const DisassemblyResult* GetConstResult() const;
    LineCount GetMaxLineCount() const;
    void SetMaxLineCount(LineCount maxLineCount);

    BOOL AreLineNumbersActive() const;

    WORD GetFontHeight() const;
    WORD GetFontWidth() const;
    LineCount GetVisibleLineCount() const;

    WORD GetFactor() const;
    void SetFactor(WORD wFactor);

    // Vertical window scroll bar
    ScrollPosition GetMaxScrollPosition() const;
    void SetMaxScrollPosition(ScrollPosition wMaxScrollPosition);

    LineNumber GetMaxScrollLineNumber() const;
    LineNumber GetScrollLineNumber() const;
    LineNumber GetRightClickLineNumber() const;
    void SetRightClickLineNumber(DisassemblyResult::LineNumber lineNumber);

    void FillRestOfWindow(DC dc);
    void SetScrollLineNumber(LineNumber lineNumber);
    void InitScroll();
    void SetResult(LPARAM lParam);
    void SetLineNumbersActive(BOOL lineNumbersActive);
    void SyncLine(LineNumber lineNumber);
    DIS_LINE* SelectLine(LineNumber lineNumber);


    LRESULT SelectLineWithInfo(SEGMENT_NUMBER segmentNumber, Memory::offset offset);
    LRESULT ExtendSelectionTo(SEGMENT_NUMBER segmentNumber, Memory::offset offset);

    Font* GetFont() const;
    void SetFont(Font* font);
    void Size(); // Adapts number of lines to font and visible client area

    void Create();
    void Destroy();

    LP_DIS_HISTORY GetHistory() const;
    void SetHistory(LP_DIS_HISTORY history);

    WORD GetHistoryIndex() const;
    void SetHistoryIndex(WORD historyIndex);

    void ResetHistory();
    void SaveHistory(LineNumber lineNumber);
    void BackInHistory();

    const DIS_LINE* FindLineUnderCursor(POSITION yPos, LineNumber& cursorLineNumber);
    DIS_LINE* SelectDefinition(const wchar_t* szLabel);
    DIS_LINE* SelectAllReferences(BOOL bSelect, const wchar_t* szLabel);
    void SelectAllReferencesAndNotifyParent(BOOL bSelect, const wchar_t* szText);
    void FindLabelInLine(const wchar_t* szText);
    void SelectDefinitionAndNotifyParent(const wchar_t* szText);
    void FindReference(const wchar_t* szText);
    INT FlushPartOfLine(DC dc, POSITION xPos, POSITION yPos, WORD wState, const wchar_t* szText);
    void PrintOneLineInColor(DC dc, WORD wFontHeight, bool selected, bool referenced, bool withLineNumber, POSITION yPos, const wchar_t* szText);
    void PrintAll(DC dc);
    void Paint();
    void Refresh();

    // Event Handlers
    void MouseMove(WPARAM wParam, LPARAM lParam);
    void LButtonDown(WPARAM wParam, LPARAM lParam);
    void LButtonUp();
    void LButtonDblClk(WPARAM wParam, LPARAM lParam);
    void RButtonDown(WPARAM wParam, LPARAM lParam);
    void VScroll(WPARAM wParam, LPARAM lParam);
};
