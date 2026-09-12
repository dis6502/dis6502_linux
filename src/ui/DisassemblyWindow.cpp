#include "DisassemblyControl.h"
#include "DisassemblyWindow.h"
#include "Layout.h"
#include "PartWindow.h"
#include "Window.h"
#include <memory>
#include <Windows.h>


DisassemblyWindow::DisassemblyWindow(Window& parentWindow, PartLayout& partLayout) : PartWindow(parentWindow, partLayout) {}


void DisassemblyWindow::CreateControl(INT nDisassemblyControlChildID) {
    disassemblyControl = std::make_unique<DisassemblyControl>(*parentWindow, nDisassemblyControlChildID);
    disassemblyControl->CreateControl(partLayout->left, partLayout->top, partLayout->width, partLayout->height);
}

void DisassemblyWindow::ApplyLayout() {
    PartWindow::ApplyLayout();

    disassemblyControl->SetFont(partLayout->GetLayout()->GetFont());
    disassemblyControl->SetPosition(nullptr, partLayout->left, partLayout->top, partLayout->width, partLayout->height, SWP_NOZORDER);
}

DisassemblyControl* DisassemblyWindow::GetDisassemblyControl() const {
    return disassemblyControl.get();
}
