#pragma once

#include "Syntax.h"

#include "Window.h"
#include "Byte.h"
#include "ByteSequence.h"

class SpriteControl : public Window {
public:
	static void RegisterClassFor(HINSTANCE hInstance);

	SpriteControl(Window& parentWindow, ChildID childID);

	void CreateControl(int x, int y, int nWidth, int nHeight);
	void AssignControl(); // If control was already created, for example by dialog

	void SetBuffer(const ByteSequence& byteSequence);
		
	int GetModeNumberOfBytesPerLine() const;
	int GetNumberOfBytesPerLine() const;
	WORD GetMode() const;
	void SetMode(WORD wMode, int nNumberOfBytesPerLine);
	void Refresh() const;

	WORD GetIndex() const;
	void SetIndex(WORD wIndex);
	WORD GetSelection() const;
	void SetSelection(WORD wEnd);

private:
	INT childID;
};
