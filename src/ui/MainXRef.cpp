#include "MainController.h"
#include "MainDisassembly.h"
#include "MainWindow.h"
#include "MainXRef.h"
#include "Syntax.h"
#include "Text.h"
#include "XRefListWindow.h"
#include <string>
#include <vector>

MainXRef::MainXRef(Main& main) : MainController(main) {

    updateCounter = 0;
}

void MainXRef::SetMainDisassembly(MainDisassembly& mainDisassembly) {
    this->mainDisassembly = &mainDisassembly;
}

long MainXRef::Proc(Window::WindowHandle hWnd, Window::WPARAM wParam) {
    switch (HIWORD(wParam)) {
        // the user has selected a reference.
    case LBN_SELCHANGE:
        if (updateCounter == 0) {
            HandleSelectionChanged();
        }
        break;
    }

    return 0;
}

void MainXRef::HandleSelectionChanged() {
    updateCounter++;
    QuitEditMode();
    auto hXRefList = mainWindow->xrefListWindow->GetHWnd();
    const DWORD dwIndex = SendMessage(hXRefList, LB_GETCURSEL, 0, 0);
    const WORD wLine = (WORD)(dwIndex + 1);
    mainDisassembly->XRefSelected(wLine);
    updateCounter--;
}

void MainXRef::UpdateList(wstring_view findString, std::vector<wstring> references) {
    updateCounter++;
    auto hXRefList = mainWindow->xrefListWindow->GetHWnd();

    // TODO: Local events!
    SendMessage(hXRefList, LB_RESETCONTENT, 0, 0);
    if (findString.empty()) {
        mainWindow->xrefListWindow->SetTitle(Text::Get(IDS_XREF_TITLE_NO_LABEL_SELECTED));
    }
    else {
        const auto size = references.size();
        const auto textID = (size == 1 ? IDS_XREF_TITLE_LABEL_REFERENCE : IDS_XREF_TITLE_LABEL_REFERENCES);
        mainWindow->xrefListWindow->SetTitle(Text::Format(textID, std::to_wstring(size), findString));
        for (auto& reference : references) {
            SendMessage(hXRefList, LB_ADDSTRING, 0, (LPARAM)reference.c_str());
        }
    }

    InvalidateRect(mainWindow->GetHWnd(), nullptr, false);
    updateCounter--;
}