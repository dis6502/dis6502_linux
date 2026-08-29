#include "Application.h"
#include "AssembleDialog.h"
#include "FileSystemLogic.h"
#include "FileType.h"
#include "FindStringDialog.h"
#include "Main.h"
#include "MainDisassembly.h"
#include "MainWindow.h"
#include "MemoryInspector.h"
#include "MemoryInspectorControl.h"
#include "MemoryInspectorControlTypes.h"
#include "MemoryInspectorFindStringDialog.h"
#include "MemoryInspectorSelection.h"
#include "MemoryInspectorWindow.h"
#include "resource.h"
#include "Segment.h"


#include "MainMemoryInspector.h"

extern std::unique_ptr<Main> g_Main;
extern std::unique_ptr<FileSystemLogic> g_FileSystemLogic;
extern std::unique_ptr<MemoryInspector> g_MemoryInspector;

MainMemoryInspector::MainMemoryInspector(Main& main) : MainController(main) {

}

long MainMemoryInspector::Proc(HWND hWnd, WPARAM wParam) {
    switch (HIWORD(wParam)) {
    // User has changed the selection.
    case static_cast<unsigned short>(MemoryInspectorNotification::SELECTION_CHANGED): {
        ::g_MemoryInspector->SelectionChanged();
        break;
    }

    // User has right clicked in the window.
    case static_cast<unsigned short>(MemoryInspectorNotification::RBUTTONDOWN): {
        ::g_MemoryInspector->DrawMenu();
        break;
    }

    // User has left double clicked in the window.
    case static_cast<unsigned short>(MemoryInspectorNotification::LBUTTONDBLCLK): {
        Edit();
        break;
    }

    // User has quit the edit mode.
    case static_cast<unsigned short>(MemoryInspectorNotification::QUIT_EDIT_MODE): {
        QuitEditMode();
        break;
    }
    default:
        break;
    }

    return 0;
}

void MainMemoryInspector::PerformCommands(COMMAND command, bool editMode) {

    // The only command allowed in edit mode is quit edit mode.
    if (editMode) {
        if (command == ID_DUMP_QUIT_EDIT || command == IDM_DUMP_QUIT_EDIT) {
            QuitEditMode();
        }
        return;
    }

    switch (command) {
    case IDM_DUMP_EDIT_COMMENT:
        main->GetMainDisassembly()->AddComment();
        return;

    case ID_DUMP_SELECT_SPRITES:
    case IDM_DUMP_SELECT_SPRITES:
        ::g_MemoryInspector->ShowSelectSpritesDialog();
        return;

    case ID_DUMP_SELECT_ALL:
    case IDM_DUMP_SELECT_ALL:
        SelectAll();
        return;

    case ID_DUMP_SELECT:
    case IDM_DUMP_SELECT_NEXT_UNKNOWN_BLOCK:
        SelectNextUnknownBlock();
        return;

    case ID_DUMP_FIND:
    case IDM_DUMP_FIND:
        Find();
        return;

    case ID_DUMP_FIND_NEXT:
    case IDM_DUMP_FIND_NEXT:
        FindNext();
        return;

    case ID_DUMP_EDIT:
    case IDM_DUMP_EDIT:
        Edit();
        return;

    case ID_DUMP_ASSEMBLE:
    case IDM_DUMP_ASSEMBLE:
        if (::g_MemoryInspector->GetMemoryInspectorSelection()->HasSelection()) {
            AssembleDialog dialog(*mainWindow);
            dialog.Show(::g_MemoryInspector->GetConstMemoryInspectorSelection()->segment, mainWindow->memoryInspectorWindow->GetMemoryInspectorControl());
            UpdateDisassembly();
        }
        return;

    case ID_DUMP_DELETE_SELECTION:
    case IDM_DUMP_DELETE_SELECTION:
        ::g_MemoryInspector->DeleteSelection();
        return;

    case ID_DUMP_CUT_SELECTION:
    case IDM_DUMP_CUT_SELECTION:
        ::g_MemoryInspector->CopySelection();
        ::g_MemoryInspector->DeleteSelection();
        return;

    case ID_DUMP_COPY_SELECTION:
    case IDM_DUMP_COPY_SELECTION:
        ::g_MemoryInspector->CopySelection();
        return;

    case ID_DUMP_PASTE_AT_SELECTION:
    case IDM_DUMP_PASTE_AT_SELECTION:
        ::g_MemoryInspector->PasteAtSelection(false);
        return;

    case IDM_DUMP_PASTE_AFTER_SELECTION:
        ::g_MemoryInspector->PasteAtSelection(true);
        return;

    case IDM_DUMP_SPLIT_AT_SELECTION:
        ::g_MemoryInspector->SplitAtSelection();
        return;

    case IDM_DUMP_SAVE_NO_HEADER:
        SaveWithoutHeader(*mainWindow);
        return;

    case IDM_DUMP_SAVE_HEADER:
        SaveWithHeader(*mainWindow);
        return;

    case ID_DUMP_START_CODE_TRACE:
    case IDM_DUMP_START_CODE_TRACE:
        ::g_MemoryInspector->Guess();
        return;

    case IDM_DUMP_SET_UNKNOWN_BLOCK_TO_BYTE:
        ::g_MemoryInspector->SetUnknownBlockToByte();
        return;

    case IDM_DUMP_SET_TYPE_UNKNOWN:
        ::g_MemoryInspector->SetType(MemoryType::UNKNOWN);
        return;
    case IDM_DUMP_SET_TYPE_LOBYTE:
        ::g_MemoryInspector->SetType(MemoryType::LOBYTE);
        return;
    case IDM_DUMP_SET_TYPE_HIBYTE:
        ::g_MemoryInspector->SetType(MemoryType::HIBYTE);
        return;
    case IDM_DUMP_SET_TYPE_BYTE:
        ::g_MemoryInspector->SetType(MemoryType::BYTE);
        return;
    case IDM_DUMP_SET_TYPE_WORD:
        ::g_MemoryInspector->SetType(MemoryType::WORD);
        return;
    case IDM_DUMP_SET_TYPE_LABEL:
        ::g_MemoryInspector->SetType(MemoryType::LABEL);
        return;
    case IDM_DUMP_SET_TYPE_STRING:
        ::g_MemoryInspector->SetType(MemoryType::STRING);
        return;
    case IDM_DUMP_SET_TYPE_SBYTE:
        ::g_MemoryInspector->SetType(MemoryType::SBYTE);
        return;
    case IDM_DUMP_SET_TYPE_DLIST:
        ::g_MemoryInspector->SetType(MemoryType::DLIST);
        return;
    case IDM_DUMP_SET_TYPE_STORE:
        ::g_MemoryInspector->SetType(MemoryType::STORE);
        return;
    case IDM_DUMP_SET_TYPE_CODE:
        ::g_MemoryInspector->SetType(MemoryType::CODE);
        return;
    case IDM_DUMP_SET_TYPE_SYMBOL:
        ::g_MemoryInspector->SetType(MemoryType::SYMBOL);
        return;
    case IDM_DUMP_SET_TYPE_FIXUP:
        ::g_MemoryInspector->SetType(MemoryType::FIXUP);
        return;

    default:
        break;
    }
}

void MainMemoryInspector::Edit() {
    ::g_MemoryInspector->SetEditMode(true);
}

void MainMemoryInspector::SelectAll() {
    ::g_MemoryInspector->SelectAll();
}

void MainMemoryInspector::SelectNextUnknownBlock() {
    ::g_MemoryInspector->SelectNextUnknownBlock();
}

void MainMemoryInspector::Find() {
    if (::g_MemoryInspector->GetConstMemoryInspectorSelection()->segment != nullptr) {
        MemoryInspectorFindStringDialog dialog(*mainWindow);
        dialog.Show(*::g_MemoryInspector.get());
    }
}

void MainMemoryInspector::FindNext() {
    if (::g_MemoryInspector->HasFindString()) {
        ::g_MemoryInspector->FindNextString();
    }
    else {
        MemoryInspectorFindStringDialog dialog(*mainWindow);
        dialog.Show(*::g_MemoryInspector.get());
    }
}

void MainMemoryInspector::SaveWithoutHeader(const Window& parentWindow) {

    auto result = ::g_FileSystemLogic->GetSaveFileName(parentWindow, L"", FileType::RAW_FILE);

    if (result.success) {
        if (auto fp = FileIO::OpenFile(result.filePath, L"w+b"); fp) { // TODO Use OutputStream
            const auto& inspectorSelection = ::g_MemoryInspector->GetConstMemoryInspectorSelection();
            const auto& byteSequence = inspectorSelection->GetByteSequence();
            fwrite(byteSequence->getConst(), sizeof(byte), byteSequence->size(), fp);

            fclose(fp);
        }
    }
}

void MainMemoryInspector::SaveWithHeader(const Window& parentWindow) {
    auto result = ::g_FileSystemLogic->GetSaveFileName(parentWindow, L"", FileType::EXECUTABLE_FILE);

    if (result.success) {
        auto fp = FileIO::OpenFile(result.filePath, L"w+b"); // TODO Use OutputStream

        if (fp != nullptr) {
            const auto& inspectorSelection = ::g_MemoryInspector->GetConstMemoryInspectorSelection();

            const auto header = FileHeader::ATARI_BINARY;
            Memory::address wBegin;
            Memory::address wEnd;
            inspectorSelection->GetAddressRage(wBegin, wEnd);
            fwrite(&header, sizeof(byte), sizeof(header), fp);
            fwrite(&wBegin, sizeof(byte), sizeof(wBegin), fp);
            fwrite(&wEnd, sizeof(byte), sizeof(wEnd), fp);

            const auto byteSequence = inspectorSelection->GetByteSequence();
            fwrite(byteSequence->getConst(), sizeof(byte), byteSequence->size(), fp);

            fclose(fp);
        }
    }
}
