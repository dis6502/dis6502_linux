#pragma once

#include "Syntax.h"
#include "UI.h"

class Window {
public:
	HWND GetHWnd() const;

	void SetTitle(wstring_view title);
	void SetFont(HFONT hFont);
	
	void SetPosition(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

    bool HasFocus() const;
	void SetFocus();
	
	void SetMouseWheelProc(WNDPROC lpWndProc);
	void SetRButtonDownProc(WNDPROC lpWndProc);

	void SetDragAcceptFiles(bool bAccept);
	void SetDropFilesProc(WNDPROC lpWndProc);

protected:
	const Window* parentWindow;
	HWND hWnd;

    Window();
	Window(const Window& parentWindow);
	virtual ~Window();

	void CreateWindowControl(wstring_view className, wstring_view windowName, DWORD dwStyle, HMENU hMenu, int x, int y, int nWidth, int nHeight);
    void CreateChildControl(wstring_view className, DWORD dwStyle, ChildID childID, int X, int Y, int nWidth, int nHeight);
    void InitControl(wstring_view className, HWND hWnd);

private:
	wstring className;
	WNDPROC lpWndProc;

	WNDPROC lpWM_DROPFILESProc;
	WNDPROC lpWM_MOUSEWHEELProc;
	WNDPROC lpWM_RBUTTONDOWNProc;

	static LRESULT CALLBACK WindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK WindowProcInstance(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
