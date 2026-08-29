/*
 * Layout.h
 *
 * Dynamic layout for main window controls.
 */
#pragma once

#include "Syntax.h"
#include "UI.h"
#include <gsl/pointers>

class Layout;

class PartLayout {
public:
	HWND hWnd;
	int left;		// Pixels
	int top;		// Pixels
	int columns;	// Characters
	int width;		// Pixels
	int lines;		// Characters
	int height;		// Pixels

	PartLayout(Layout& layout);
	
	gsl::not_null<Layout*> GetLayout() const;

private:
    gsl::not_null<Layout*> layout;
};

class Layout {
public:
	int fontWidth;
	int fontHeight;
	int mainWidth;
	int mainHeight;
	int memoryInspectorNumberOfBytesPerLine;

	PartLayout segmentList;
	PartLayout memoryInspector;
	PartLayout disassembly;
	PartLayout xrefList;
	PartLayout logList;

	Layout();

	HFONT GetFont() const;
	void SetFont(HFONT hFont, int fontWidth, int fontHeight);
	void ComputeForSize(int mainWidth, int mainHeight);
	void Compute(); // for current size, but potentially changed font

private:
	HFONT font;
};
