/*
 * Layout.h
 *
 * Dynamic layout for main window controls.
 */
#pragma once

#include <gsl/pointers>
#include <Windows.h>

class Layout;
class Font;

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

    Font* GetFont() const;
    void SetFont(Font* font, int fontWidth, int fontHeight);
    void ComputeForSize(int mainWidth, int mainHeight);
    void Compute(); // for current size, but potentially changed font

private:
    Font* font;
};
