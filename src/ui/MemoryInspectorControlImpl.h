/*
** MemoryInspectorControlImpl.h
**
** Segment memoryInspector control implementation used to display a buffer in hexadecimal and in ASCII/ATASCII.
*/
#pragma once

// #include "MemoryInspectorControlTypes.h"
#include "Window.h"
#include <Windows.h>

constexpr UINT WM_DUMP_BASE = (WM_USER + 1234);

/*
** messages to set or get sprite control parameters
*/
typedef enum {
    WM_DUMP_SET_NUMBER_OF_BYTES_PER_LINE = WM_DUMP_BASE,	// lParam = number of bytes per line
    WM_DUMP_SET_BUFFER,										// wParam = buffer size, lParam = buffer addr
    WM_DUMP_SET_TYPE_BUFFER,								// wParam = start address, lParam = type buffer addr
    WM_DUMP_SET_SCROLL,										// wParam = first line to display
    WM_DUMP_GET_SCROLL,										// return first line displayed
    WM_DUMP_SET_INTERNAL,									// wParam = TRUE if internal display
    WM_DUMP_GET_INTERNAL,									// return TRUE if internal display
    WM_DUMP_SET_EDIT_MODE,									// wParam = TRUE if edit mode
    WM_DUMP_GET_EDIT_MODE,									// return TRUE if edit mode
    WM_DUMP_SET_BEGIN_SELECTION,							// wParam = first byte selected
    WM_DUMP_GET_BEGIN_SELECTION,							// return first byte selected
    WM_DUMP_SET_END_SELECTION,								// wParam = last byte selected
    WM_DUMP_GET_END_SELECTION,								// return last byte selected
    WM_DUMP_SEEK_SELECTION,									// set first line to begining of selection
    WM_DUMP_REFRESH											// redraw memoryInspector control
} DUMP_MSG;


class MemoryInspectorControlImpl {
public:
    static constexpr auto CLASS_NAME = L"MemoryInspectorControlClass";

    __declspec(dllexport) static void _cdecl RegisterClassFor(HINSTANCE hInstance);

    MemoryInspectorControlImpl(HWND hWnd);
    LRESULT WndProc(Window::MESSAGE message, WPARAM wParam, LPARAM lParam);

private:
    HWND hWnd;

    void InitScroll();

    void SetBuffer(BYTE* buffer);
    BYTE* GetBuffer();

    void SetTypeBuffer(BYTE* typeBuffer);
    BYTE* GetTypeBuffer();

    void SetSize(WORD wSize);
    WORD GetSize() const;

    void SetStart(WORD wStart);
    WORD GetStart() const;

    void SetNumberOfBytesPerLine(WORD wNumberOfBytesPerLine);
    WORD GetNumberOfBytesPerLine();
    void SetScroll(WORD wScroll, BOOL bRefresh);

    // Private wrapper methods
    void SetBeginLine(WORD wBeginLine);
    WORD GetBeginLine() const;
    void SetBeginRow(WORD wBeginRow);
    WORD GetBeginRow() const;
    void SetEndLine(WORD wEndLine);
    WORD GetEndLine() const;
    void SetEndRow(WORD wEndRow);
    WORD GetEndRow() const;

    void SetBeginSelection(WORD wBegin, BOOL bRefresh);
    void SetEndSelection(WORD wEnd, BOOL bRefresh);
    WORD GetBeginSelection();
    WORD GetEndSelection();
    void FillRestOfWindow(HDC hDC);

    void LButtonDown(WPARAM wParam, LPARAM lParam);

    void SetEndOfSelection(LPARAM lParam);
    void MouseMove(WPARAM wParam, LPARAM lParam);
    void LButtonUp(WPARAM wParam, LPARAM lParam);
    void RButtonDown(WPARAM wParam, LPARAM lParam);
    void LButtonDblClk(WPARAM wParam, LPARAM lParam);

    void SetInternal(BOOL bInternal, BOOL bRefresh);
    BOOL IsInternal() const;

    void SetEditMode(BOOL bEditMode, BOOL bRefresh);
    BOOL IsEditMode() const;
    void SetEditedPart(WORD wEditedPart);
    WORD GetEditedPart() const;
    void SetTimerCount(WORD wTimerCount);
    WORD GetTimerCount();

    void SetFontHeight(WORD wFontHeight);
    WORD GetFontHeight() const;
    void SetFontWidth(WORD wFontWidth);
    WORD GetFontWidth() const;
    void SetFont(HFONT hFont, BOOL bRefresh);


    LONG Size();

    LONG Create();
    LONG Destroy();

    void PrintLine(HDC hDC, WORD wLine, WORD wNbLines);
    void PrintOne(HDC hDC, WORD wLine);
    void PaintAll(HDC hDC); // Private
    LONG Paint();
    void Refresh();

    void Timer(WPARAM wTimerId);
    void SeekSelection();
    BOOL ScrollUp(WORD wScroll, RECT* rc);
    BOOL ScrollDown(WORD wScroll, RECT* rc, WORD wNbLines);
    void VScroll(WPARAM wParam, LPARAM lParam);


    void KeyDown(WPARAM wParam);
    void Char(WPARAM wParam);
};
