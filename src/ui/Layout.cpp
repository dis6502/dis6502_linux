/*
* Layout.h
* Dynamic layout for main window controls.
*/

#include <algorithm>

#include "Application.h"

#include "Layout.h"

extern std::unique_ptr<Application> g_Application;


PartLayout::PartLayout(Layout& layout) :
    hWnd(NULL_HWND),
    left(0),
    top(0),
    columns(0),
    width(0),
    lines(0),
    height(0),
    layout(&layout) {
}

gsl::not_null<Layout*> PartLayout::GetLayout() const {
    return layout;
}

Layout::Layout() :
    fontWidth(0),
    fontHeight(0),
    mainWidth(0),
    mainHeight(0),
    memoryInspectorNumberOfBytesPerLine(0),
    segmentList(*this),
    memoryInspector(*this),
    disassembly(*this),
    xrefList(*this),
    logList(*this),
    font(NULL_HFONT) {
}

HFONT Layout::GetFont() const {
    return font;
}

void Layout::SetFont(HFONT hFont, int fontWidth, int fontHeight) {
    font = hFont;
    this->fontWidth = std::max(8, fontWidth);
    this->fontHeight = std::max(8, fontHeight);
}

void Layout::ComputeForSize(int mainWidth, int mainHeight) {
    // Screen resolutions mapped to characters.
    // 1024 x 768 = 128 x 96
    // 1280 x 720 = 160 x 90
    // 1440 x 900 = 180 x 112,5
    // 1600 x 900 = 200 x 112,5

    this->mainWidth = std::max(320, mainWidth);
    this->mainHeight = std::max(200, mainHeight);

    Compute();
}

void Layout::Compute() {
    const int effectiveWidth = this->mainWidth;
    const int effectiveHeight = this->mainHeight;

    const int fontWidth = this->fontWidth;
    const int fontHeight = this->fontHeight;

    const int topHeight = fontHeight + 2;

    const int totalColumns = effectiveWidth / fontWidth;
    const int totalLines = effectiveHeight / fontHeight;

    const int segmentLines = std::min(16, std::max(4, totalLines / 7));
    const int segmentHeight = segmentLines * fontHeight + 2;

    const int xrefLines = std::min(16, std::max(4, totalLines / 7));
    const int xrefHeight = xrefLines * fontHeight + 2;

    const int logLines = std::min(8, std::max(4, totalLines / 9));
    const int logHeight = logLines * fontHeight + 2;
    const int logTop = effectiveHeight - logHeight;

    // Left dynamic area
    if (totalColumns >= 72 + 80) {
        this->memoryInspectorNumberOfBytesPerLine = 16;
    }
    else {
        this->memoryInspectorNumberOfBytesPerLine = 8;
    }

    // Computed from: 4 + 1 + (3 * this->memoryInspectorNumberOfBytesPerLine - 1) + 1 + this->memoryInspectorNumberOfBytesPerLine
    const int memoryInspectorColumns = 5 + 4 * this->memoryInspectorNumberOfBytesPerLine + 1;
    const int memoryInspectorWidth = (memoryInspectorColumns * fontWidth) + GetSystemMetrics(SM_CXVSCROLL) + 4;
    const int memoryInspectorHeight = (effectiveHeight - topHeight - segmentHeight - topHeight - topHeight - logHeight);
    const int memoryInspectorLines = memoryInspectorHeight / fontHeight;

    // Right dynamic area
    const int disLeft = memoryInspectorWidth;
    const int disWidth = effectiveWidth - memoryInspectorWidth;
    const int disColumns = (disWidth - (GetSystemMetrics(SM_CXVSCROLL) + 4)) / fontWidth;
    const int disHeight = (effectiveHeight - topHeight - topHeight - xrefHeight - topHeight - logHeight);
    const int disLines = disHeight / fontHeight;

    this->segmentList.left = 0;
    this->segmentList.top = topHeight;
    this->segmentList.columns = memoryInspectorColumns;
    this->segmentList.width = memoryInspectorWidth;
    this->segmentList.lines = segmentLines;
    this->segmentList.height = segmentHeight;

    this->memoryInspector.left = this->segmentList.left;
    this->memoryInspector.top = this->segmentList.top + this->segmentList.height + topHeight;
    this->memoryInspector.columns = memoryInspectorColumns;
    this->memoryInspector.width = memoryInspectorWidth;
    this->memoryInspector.lines = memoryInspectorLines;
    this->memoryInspector.height = memoryInspectorHeight;

    this->disassembly.left = disLeft;
    this->disassembly.top = topHeight;
    this->disassembly.columns = disColumns;
    this->disassembly.width = disWidth;
    this->disassembly.lines = disLines;
    this->disassembly.height = disHeight;

    this->xrefList.left = disLeft;
    this->xrefList.top = this->disassembly.top + this->disassembly.height + topHeight;
    this->xrefList.columns = disColumns;
    this->xrefList.width = disWidth;
    this->xrefList.lines = xrefLines;
    this->xrefList.height = this->xrefList.lines * fontHeight + 2;

    this->logList.left = 0;
    this->logList.top = logTop;
    this->logList.columns = effectiveWidth / fontWidth;
    this->logList.width = effectiveWidth;
    this->logList.lines = logLines;
    this->logList.height = logHeight;

    this->mainHeight = logTop + logHeight;
};
