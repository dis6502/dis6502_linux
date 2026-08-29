#include "Application.h"
#include "ComboBox.h"
#include "MemoryInspector.h"
#include "MemoryInspectorSelection.h"
#include "Segment.h"
#include "SpriteControl.h"
#include "EditControl.h"
#include "TextLabel.h"

#include "SpriteControlImpl.h"

#include "SelectSpritesDialog.h"


SelectSpritesDialog::SelectSpritesDialog(Window& parentWindow) : Dialog(parentWindow, L"FINDSPRITESBOX") {
    comboBox = nullptr;
    spriteControl = nullptr;

    wSpriteMode = 15;
    wSpriteNbBytes = 40;

    memoryInspectorSelection = nullptr;
    wBegin = 0;
    wEnd = 0;
}

bool SelectSpritesDialog::Show(const MemoryInspectorSelection& memoryInspectorSelection) {
    this->memoryInspectorSelection = &memoryInspectorSelection;

    return ShowDialogBox();
}

void SelectSpritesDialog::GetSelection(Memory::offset& nBegin, Memory::offset& nEnd) const {
    nBegin = this->wBegin;
    nEnd = this->wEnd;
}

void SelectSpritesDialog::Init() {
    RECT rc, rcDlg;

    HWND hWndComboBox = GetDlgItem(hDlg, IDC_GRAPHICCOMBO);

    comboBox = std::make_unique<ComboBox>(hWndComboBox);
    comboBox->AddString(IDS_SPRITE_ANTIC_8); // ANTIC 8 (40x24x4)
    comboBox->AddString(IDS_SPRITE_ANTIC_9); // ANTIC 9 (80x48x2)
    comboBox->AddString(IDS_SPRITE_ANTIC_A); // ANTIC A (80x48x4)
    comboBox->AddString(IDS_SPRITE_ANTIC_B); // ANTIC B (160x96x2)
    comboBox->AddString(IDS_SPRITE_ANTIC_C); // ANTIC C (160x192x2)
    comboBox->AddString(IDS_SPRITE_ANTIC_D); // ANTIC D (160x96x4)
    comboBox->AddString(IDS_SPRITE_ANTIC_E); // ANTIC E (160x192x4)
    comboBox->AddString(IDS_SPRITE_ANTIC_F); // ANTIC F (320x192x2)

    spriteControl = std::make_unique<SpriteControl>(*this, IDC_GRAPHIC);
    spriteControl->AssignControl();

    GetClientRect(spriteControl->GetHWnd(), &rc);

    // TODO: Test this, the actual dialog is too small!
    const int adjustWidth = 320 - rc.right + GetSystemMetrics(SM_CXVSCROLL);
    const int adjustHeight = 192 - rc.bottom;

    GetWindowRect(hDlg, &rcDlg);
    GetClientRect(hDlg, &rc);
    ClientToScreen(hDlg, (POINT*)&rc);

    const int clientLeft = rc.left;
    const int clientTop = rc.top;

    MoveWindow(hDlg, rcDlg.left, rcDlg.top, rcDlg.right - rcDlg.left + adjustWidth + 1, rcDlg.bottom - rcDlg.top + adjustHeight + 1, false);

    GetWindowRect(spriteControl->GetHWnd(), &rc);
    MoveWindow(spriteControl->GetHWnd(), rc.left - clientLeft, rc.top - clientTop, rc.right - rc.left + adjustWidth + 1, rc.bottom - rc.top + adjustHeight + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDC_ADDRESSTEXT), &rc);
    MoveWindow(GetDlgItem(hDlg, IDC_ADDRESSTEXT), rc.left - clientLeft, rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDC_ADDRESSSTART), &rc);
    MoveWindow(GetDlgItem(hDlg, IDC_ADDRESSSTART), rc.left - clientLeft, rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDC_SPRITETEXT), &rc);
    MoveWindow(GetDlgItem(hDlg, IDC_SPRITETEXT), rc.left - clientLeft + adjustWidth + 1, rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDC_SPRITEWIDTH), &rc);
    MoveWindow(GetDlgItem(hDlg, IDC_SPRITEWIDTH), rc.left - clientLeft + adjustWidth + 1, rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDC_GRAPHICTEXT), &rc);
    MoveWindow(GetDlgItem(hDlg, IDC_GRAPHICTEXT), rc.left - clientLeft, rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDC_GRAPHICCOMBO), &rc);
    MoveWindow(GetDlgItem(hDlg, IDC_GRAPHICCOMBO), rc.left - clientLeft, rc.top - clientTop + adjustHeight, rc.right - rc.left + adjustWidth + 1, (rc.bottom - rc.top + 1) * 8, false);

    GetWindowRect(GetDlgItem(hDlg, IDOK), &rc);
    MoveWindow(GetDlgItem(hDlg, IDOK), rc.left - clientLeft + (adjustWidth / 2), rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    GetWindowRect(GetDlgItem(hDlg, IDCANCEL), &rc);
    MoveWindow(GetDlgItem(hDlg, IDCANCEL), rc.left - clientLeft + (adjustWidth / 2), rc.top - clientTop + adjustHeight, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);

    const auto& segment = memoryInspectorSelection->segment;
    const auto& data = segment->memoryBlock.GetData();
    spriteControl->SetBuffer(*data);

    SelectGoto();
    SelectMode(wSpriteMode);
    comboBox->SetSelectedIndex(wSpriteMode - 8);
}

void SelectSpritesDialog::Free() {
    spriteControl.reset();
    comboBox.reset();
}

bool SelectSpritesDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    switch (message) {
    case WM_INITDIALOG:
        Init();
        return true;

    case WM_DESTROY:
        Free();
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
        {
            const auto wIndex = spriteControl->GetIndex();
            auto wEnd = spriteControl->GetSelection();

            if ((wEnd != SPRITE_NO_SELECTION) && (wEnd >= wIndex)) {
                wBegin = wIndex;
            }
            else {
                wBegin = wEnd = dis_k::DUMP_NO_SELECTION;
            }

            wSpriteMode = spriteControl->GetMode();
            wSpriteNbBytes = spriteControl->GetNumberOfBytesPerLine();

            return EndDialogBox(TRUE);
        }

        case IDCANCEL: {
            return EndDialogBox(FALSE);
        }

        case IDC_GRAPHICCOMBO: {
            switch (HIWORD(wParam)) {
            case CBN_SELCHANGE: {
                const auto index = GetComboBox(IDC_GRAPHICCOMBO).GetSelectedIndex(); 
                if (index != LB_ERR) {
                    SelectMode((WORD)(index + 8));
                    spriteControl->Refresh();
                }

                return true;
            }

            default:
                return false;
            }
            break;
        }

        case IDC_GRAPHIC: {
            switch (HIWORD(wParam)) {
            case SPRITE_INDEX_CHANGED:
            case SPRITE_SELECTION_CHANGED: {
                const auto wIndex = spriteControl->GetIndex();
                const auto wEnd = spriteControl->GetSelection();
                ITEM_LINE szLine;
                // TODO Have dedicated memory formatter, see memory:to_hex_string
                if ((wEnd != SPRITE_NO_SELECTION) && (wEnd >= wIndex)) {
                    wsprintf(szLine, L"$%04hX - $%04hX", memoryInspectorSelection->segment->wBegin + wIndex, memoryInspectorSelection->segment->wBegin + wEnd);
                }
                else {
                    wsprintf(szLine, L"$%04hX", memoryInspectorSelection->segment->wBegin + wIndex);
                }

                GetEditControl(IDC_ADDRESSSTART).SetText(szLine);
                return true;
            }

            case SPRITE_NBBYTES_CHANGED: {
                GetTextLabel(IDC_SPRITEWIDTH).SetNumber(spriteControl->GetNumberOfBytesPerLine());
                return true;
            }

            default:
                return false;
            }
            break;
        default:
            return false;
        }
        }
        break;
    default:
        return false;

    }

}

void SelectSpritesDialog::SelectMode(WORD wMode) {
    wSpriteMode = wMode;
    spriteControl->SetMode(wMode, wSpriteNbBytes);

    GetTextLabel(IDC_SPRITEWIDTH).SetNumber(wSpriteNbBytes); 
    // TODO wMode => wSPriteMode
}

void SelectSpritesDialog::SelectGoto() {
    Memory::offset wIndex = 0;
    Memory::offset wEnd = SPRITE_NO_SELECTION;

    if (memoryInspectorSelection->HasSelection()) {
        wIndex = memoryInspectorSelection->GetBegin();
        wEnd = memoryInspectorSelection->GetEnd();
    }

    spriteControl->SetIndex(wIndex);
    spriteControl->SetSelection(wEnd);

    ITEM_LINE szLine;
    if ((wEnd != SPRITE_NO_SELECTION) && (wEnd >= wIndex)) {
        wsprintf(szLine, L"$%04hX - $%04hX", memoryInspectorSelection->segment->wBegin + wIndex, memoryInspectorSelection->segment->wBegin + wEnd);
    }
    else {
        wsprintf(szLine, L"$%04hX", memoryInspectorSelection->segment->wBegin + wIndex);
    }
    SetDlgItemText(hDlg, IDC_ADDRESSSTART, szLine);
}
