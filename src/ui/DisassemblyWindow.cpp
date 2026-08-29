#include "Application.h"
#include "DisassemblyControl.h"
#include "Layout.h"
#include "MainWindow.h"

#include "DisassemblyWindow.h"


DisassemblyWindow::DisassemblyWindow(Window& parentWindow, PartLayout& partLayout) : PartWindow(parentWindow, partLayout) {
}


void DisassemblyWindow::CreateControl(INT nDisassemblyControlChildID) {
    disassemblyControl = std::make_unique<DisassemblyControl>(*parentWindow, nDisassemblyControlChildID);
    disassemblyControl->CreateControl(partLayout->left, partLayout->top, partLayout->width, partLayout->height);
}

void DisassemblyWindow::ApplyLayout() {
    PartWindow::ApplyLayout();

    disassemblyControl->SetFont(partLayout->GetLayout()->GetFont());
    disassemblyControl->SetPosition(NULL_HWND, partLayout->left, partLayout->top, partLayout->width, partLayout->height, SWP_NOZORDER);
}

DisassemblyControl* DisassemblyWindow::GetDisassemblyControl() const {
    return disassemblyControl.get();
}
