#include "Application.h"
#include "Layout.h"
#include "MemoryInspector.h"
#include "MemoryInspectorControl.h"
#include "MemoryInspectorSelection.h"
#include "SpriteControl.h"

#include "MemoryInspectorWindow.h"

extern std::unique_ptr<MemoryInspector> g_MemoryInspector;


MemoryInspectorWindow::MemoryInspectorWindow(Window& parentWindow, PartLayout& partLayout) : PartWindow(parentWindow, partLayout),
memoryInspectorControl(nullptr),
memoryInspectorSelection(nullptr)
{
}


void MemoryInspectorWindow::CreateControl(int nMemoryInspectorControlChildId) {
    memoryInspectorControl = std::make_unique<MemoryInspectorControl>(*parentWindow, nMemoryInspectorControlChildId);
    memoryInspectorControl->CreateControl(0, 0, 1, 1);
    memoryInspectorSelection = ::g_MemoryInspector->GetMemoryInspectorSelection();
}

void MemoryInspectorWindow::ApplyLayout() {
    PartWindow::ApplyLayout();

    const auto numberOfBytesPerLine = partLayout->GetLayout()->memoryInspectorNumberOfBytesPerLine;

    memoryInspectorControl->SetNumberOfBytesPerLine(numberOfBytesPerLine);
    memoryInspectorControl->SetFont(partLayout->GetLayout()->GetFont());
    memoryInspectorControl->SetPosition(NULL_HWND, partLayout->left, partLayout->top, partLayout->width, partLayout->height, SWP_NOZORDER);
}

MemoryInspectorControl* MemoryInspectorWindow::GetMemoryInspectorControl() const {
    return memoryInspectorControl.get();
}

void MemoryInspectorWindow::ClearSelection() {
    memoryInspectorControl->ClearSelection();
    memoryInspectorSelection->ClearSelection();
}

void MemoryInspectorWindow::SetSelection(Memory::offset nBegin, Memory::offset nEnd) {
    memoryInspectorControl->SetSelection(nBegin, nEnd);

    if (memoryInspectorControl->GetRawSelection(nBegin, nEnd)) {
        memoryInspectorSelection->SetSelection(nBegin, nEnd);
    }
    else {
        memoryInspectorSelection->ClearSelection();
    }
}
