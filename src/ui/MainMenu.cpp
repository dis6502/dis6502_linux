#include "AboutDialog.h"
#include "Application.h"
#include "DefaultFolders.h"
#include "EquateListController.h"
#include "FileType.h"
#include "Main.h"
#include "MainController.h"
#include "MainFile.h"
#include "MainMenu.h"
#include "MainWindow.h"
#include "MainWindowMenu.h"
#include "Memory.h"
#include "MemoryInspector.h"
#include "MemoryInspectorSelection.h"
#include "MRUController.h"
#include "MRUEntry.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SegmentTypes.h"
#include "SegmentWriteBootDiskDialog.h"
#include "systems/ComputerSystem.h"
#include "Text.h"
#include "Workspace.h"
#include "WorkspaceDialog.h"
#include <memory>

extern Application* g_Application;

extern std::unique_ptr<Workspace> g_Workspace;

extern std::unique_ptr<MemoryInspector> g_MemoryInspector;
extern std::unique_ptr<EquateListController> g_EquateListController;

extern DefaultFolders* lpDefaultFolders;

extern bool bViewNoDisassembly;
extern bool bViewDoubleHeight;


MainMenu::MainMenu(Main& main, MRUController& mruController) : MainController(main), mruController(&mruController) {}

void MainMenu::PerformFileMenuCommands(COMMAND command, bool editMode) {
    switch (command) {

    case ID_FILE_NEW_WORKSPACE:
    case ID_FILE_NEW:
        if (!editMode) {
            if (main->PromptToClearWorkspace(IDS_MAIN_FILE_NEW_WORKSPACE_TITLE, IDS_MAIN_FILE_NEW_WORKSPACE_MESSAGE, true)) {
                WorkspaceDialog dialog(*mainWindow);
                if (dialog.Show()) {
                    application->SendInfoMessageWithID(IDS_MAIN_FILE_LOG_NEW_WORKSPACE_PREPARED, g_Workspace->GetComputerSystem()->GetTypeInfo()->text);
                }
            }
        }
        return;

    case ID_FILE_OPEN_RAW_FILE:
    case ID_FILE_ADD_RAW_FILE:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::RAW_FILE, command == ID_FILE_ADD_RAW_FILE);
        }
        return;

    case ID_FILE_OPEN_EXECUTABLE_FILE:
    case ID_FILE_ADD_EXECUTABLE_FILE:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::EXECUTABLE_FILE, command == ID_FILE_ADD_EXECUTABLE_FILE);
        }
        return;

    case ID_FILE_OPEN_ROM_IMAGE_FILE:
    case ID_FILE_ADD_ROM_IMAGE_FILE:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::ROM_IMAGE_FILE, command == ID_FILE_ADD_ROM_IMAGE_FILE);
        }
        return;

    case ID_FILE_OPEN_CASSETTE_IMAGE_FILE:
    case ID_FILE_ADD_CASSETTE_IMAGE_FILE:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::CASSETTE_IMAGE_FILE, command == ID_FILE_ADD_CASSETTE_IMAGE_FILE);
        }
        return;

    case ID_FILE_OPEN_DISK_IMAGE_EXECUTABLE_FILE:
    case ID_FILE_ADD_DISK_IMAGE_EXECUTABLE_FILE:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::DISK_IMAGE_EXECUTABLE_FILE, command == ID_FILE_ADD_DISK_IMAGE_EXECUTABLE_FILE);
        }
        return;

    case ID_FILE_OPEN_DISK_IMAGE_BOOT_SECTORS:
    case ID_FILE_ADD_DISK_IMAGE_BOOT_SECTORS:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::DISK_IMAGE_BOOT_SECTORS, command == ID_FILE_ADD_DISK_IMAGE_BOOT_SECTORS);
        }
        return;

    case ID_FILE_OPEN_DISK_IMAGE_SECTORS:
    case ID_FILE_ADD_DISK_IMAGE_SECTORS:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::DISK_IMAGE_SECTORS, command == ID_FILE_ADD_DISK_IMAGE_SECTORS);
        }
        return;

    case ID_FILE_OPEN_WORKSPACE:
        if (!editMode) {
            main->OpenFileWithDialog(FileType::WORKSPACE_FILE, false);
        }
        return;

    case ID_FILE_SAVE_WORKSPACE:
        if (!editMode) {
            main->SaveWorkspaceFile(false);
        }
        return;

    case ID_FILE_SAVE_WORKSPACE_AS:
        if (!editMode) {
            main->SaveWorkspaceFile(true);
        }
        return;

    case ID_FILE_SAVE_DISASSEMBLY_FILES:
        if (!editMode) {
            main->SaveDisassemblyFiles();
        }
        return;

    case ID_FILE_SAVE_DISK_IMAGE_BOOT_SECTORS: // TODO Move to segment context menu
        if (!editMode) {
            const auto segment = ::g_MemoryInspector->GetConstMemoryInspectorSelection()->segment;

            if (segment == nullptr) {
                ::g_Application->SendErrorMessageWithID(IDS_ERR_NO_SEGMENT);
                return;
            }

            bool withRunAddress = false;
            Memory::address runAddress = 0;
            const auto segmentList = ::g_Workspace->GetConstSegmentList();

            for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
                const auto& segment = segmentList->GetConstSegment(segmentIndex);

                if (segment->wBegin == 0x02E0) { // TODO: This is Atari specific
                    withRunAddress = true;
                    runAddress = segment->GetWord(0);
                }
            }

            SegmentWriteBootDiskDialog dialog(*mainWindow);
            dialog.Show(segment, withRunAddress, runAddress);
            return;
        }

    case IDM_MRU_FILE_1:
    case IDM_MRU_FILE_2:
    case IDM_MRU_FILE_3:
    case IDM_MRU_FILE_4:
    case IDM_MRU_FILE_5:
    case IDM_MRU_WORKSPACE_1:
    case IDM_MRU_WORKSPACE_2:
    case IDM_MRU_WORKSPACE_3:
    case IDM_MRU_WORKSPACE_4:
    case IDM_MRU_WORKSPACE_5: {
        const auto fileMenu = mainWindow->mainWindowMenu->GetSubMenuHandle(0);
        const auto entry = mruController->GetMRUEntry(fileMenu, (UINT)command);

        if (entry != nullptr) {
            // Copy because opening will change the entry in the MRU list
            const auto filePath = entry->GetFilePath();
            const auto fileType = entry->GetFileType();
            main->OpenFile(filePath, fileType, false);
        }
        return;
    }

    case ID_FILE_EXIT:
        PostMessage(mainWindow->GetHWnd(), WM_CLOSE, 0, 0L);
        return;

    default:
        break; // Return
    }
}

void MainMenu::PerformLabelsMenuCommands(COMMAND command) {
    switch (command) {
    case ID_EQUATES_CLEAR_SYSTEM_EQUATES:
        ::g_EquateListController->Clear(*::g_Workspace->GetSystemEquateList());
        return;

    case ID_EQUATES_DISPLAY_SYSTEM_EQUATES:
        ::g_EquateListController->Edit(*::g_Workspace->GetSystemEquateList(), false, L"");
        return;

    case ID_EQUATES_CLEAR_USER_EQUATES:
        ::g_EquateListController->Clear(*::g_Workspace->GetUserEquateList());
        return;

    case ID_EQUATES_EDIT_USER_EQUATES:
        ::g_EquateListController->Edit(*g_Workspace->GetUserEquateList(), true, L"");
        return;

    case ID_EQUATES_DEFINE_USER_ADDRESS_RANGE:
        ::g_EquateListController->DefineUserAddressRange(*::g_Workspace, L"");
        return;

    case ID_EQUATES_OPEN_USER_EQUATES:
        ::g_EquateListController->LoadUserEquates(*::g_Workspace);
        return;

    case ID_EQUATES_SAVE_USER_EQUATES:
        ::g_EquateListController->Save(*::g_Workspace->GetUserEquateList(), false);
        return;

    case ID_EQUATES_EXPORT_USER_EQUATES: // TODO: Select type (XASM/normal) rather in Save Dialog
        ::g_EquateListController->Save(*::g_Workspace->GetUserEquateList(), true);
        return;
    }
}

void MainMenu::PerformViewMenuCommands(COMMAND command) {
    switch (command) {
    case ID_VIEW_DISPLAYASSCREENCODE:
        ::g_MemoryInspector->ToggleDisplayAsScreenCode();
        return;

    case ID_VIEW_NODISASSEMBLY:
        main->ToggleViewDisassembly();
        return;

    case ID_VIEW_DOUBLE_FONT_HEIGHT:
        main->ToggleViewDoubleFontHeight();
        return;

    case ID_VIEW_DEFAULT_FOLDERS:
        main->ShowDefaultFoldersDialog();
        return;

    case ID_VIEW_PROFILE:
        main->ShowProfileDialog();
        return;
    }
}

void MainMenu::PerformHelpMenuCommands(COMMAND command) {
    switch (command) {
    case ID_HELP_ABOUT:
        AboutDialog dialog(*mainWindow);
        dialog.Show();
        return;
    }
}

void MainMenu::PerformCommands(COMMAND command, bool editMode) {
    PerformFileMenuCommands(command, editMode);
    PerformLabelsMenuCommands(command);
    PerformViewMenuCommands(command);
    PerformHelpMenuCommands(command);
}
