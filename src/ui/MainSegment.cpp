#include "systems/ComputerSystem.h"
#include "Application.h"
#include "FileSystemLogic.h"
#include "FileType.h"
#include "Main.h"
#include "MainWindow.h"
#include "MemoryInspector.h"
#include "OutputStream.h"
#include "resource.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SegmentListPopupMenu.h"
#include "SegmentListWindow.h"
#include "SegmentPropertiesDialog.h"
#include "Workspace.h"

#include "MainSegment.h"

extern std::unique_ptr<Main> g_Main;
extern std::unique_ptr<Workspace> g_Workspace;

extern std::unique_ptr<FileSystemLogic> g_FileSystemLogic;
extern std::unique_ptr<MemoryInspector> g_MemoryInspector;


static MainSegment* instance;

MainSegment::MainSegment(Main& main) : MainController(main) {
    segmentList = ::g_Workspace->GetSegmentList();
    segmentListWindow = mainWindow->segmentListWindow.get();
    menu = nullptr;
    updateCounter = 0;
    instance = this;
}


/*
** Reset segment list box, add segment description and set selected index.
*/
void MainSegment::UpdateList() {
    updateCounter++;

    segmentListWindow->ClearList();

    const auto segmentCount = segmentList->GetCount();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++) {
        const auto& segment = segmentList->GetConstSegment(segmentIndex);
        segmentListWindow->AddSegment(segment->ToString());
    }

    int listIndex = segmentList->GetSelectedIndex();
    if (listIndex < 0) {
        listIndex = -1;
    }
    segmentListWindow->SetSelectedIndex(listIndex);

    updateCounter--;
}

long MainSegment::Proc(HWND hWnd, WPARAM wParam) {
    switch (HIWORD(wParam)) {
        // the user has selected a segment.
    case LBN_SELCHANGE: {
        if (updateCounter == 0) {
            Selected();
        }
        break;
    }
    default:
        break;
    }

    return 0;
}

void MainSegment::PerformCommands(COMMAND wCommand, bool editMode) {
    // No actions as long as edit mode is active.
    if (editMode) {
        return;
    }

    const auto selectedIndex = segmentList->GetSelectedIndex();
    switch (wCommand) {
    case IDM_SEGMENT_UP:
        segmentList->MoveSelectedSegmentUp();
        return;

    case IDM_SEGMENT_DOWN:
        segmentList->MoveSelectedSegmentDown();
        return;

    case IDM_SEGMENT_DELETE:
        segmentList->DeleteSelectedSegment();
        return;

    case IDM_SEGMENT_SAVE_NO_HEADER:
        if (selectedIndex >= 0) {
            SaveSegment(selectedIndex, false);
        }
        return;

    case IDM_SEGMENT_SAVE_HEADER:
        if (selectedIndex >= 0) {
            SaveSegment(selectedIndex, true);
        }
        return;

    case IDM_SEGMENT_PROPS:
        if (selectedIndex >= 0) {
            ShowPropertiesDialog(selectedIndex);
        }
        return;

    case IDM_SEGMENT_SAVE_ALL:
        if (segmentList->GetCount() > 0) {
            SaveAllSegments(*segmentList);
        }
        return;

    case IDM_SEGMENT_MERGE:
        if (segmentList->GetCount() > 1) {
            const auto mergedCount = segmentList->MergeSegments();
            application->SendInfoMessageWithID(IDS_LOG_SEGMENTS_MERGED, std::to_wstring(mergedCount));
        }
        return;

    default:
        break;
    }
}

/*
** WM_DROPFILES message, static callback
*/
LRESULT CALLBACK MainSegment::DropFilesProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    instance->DropFilesProc(reinterpret_cast<HDROP>(wParam));
    return 0;
}

void MainSegment::DropFilesProc(HDROP hDrop) {

    // First call to get number of files
    const auto files = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

    if (files == 1) {
        // Second call to get file path of file number 0
        FileIO::FILE_PATH szFilePath{  };
        DragQueryFile(hDrop, 0, szFilePath, sizeof(szFilePath));

        main->OpenFile(szFilePath, FileType::UNKNOWN_FILE, false);
    }
}

/*
** WM_RBUTTONDOWN message, static callback
*/
LRESULT CALLBACK MainSegment::RButtonDownProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    instance->RButtonDownProc();
    return 0;
}

void MainSegment::RButtonDownProc() {
    // Check if we have a segment selected.
    if (segmentList->IsEmpty()) {
        return;
    }

    // If the memory inspector is in edit more, "Quit edit mode" is the only valid context menu action.
    const auto editMode = ::g_MemoryInspector->IsEditMode();
    if (editMode) {
        ::g_MemoryInspector->DrawMenu();
    }
    else {

        if (menu == nullptr) {
            menu = std::make_unique<SegmentListPopupMenu>();
            menu->CreateControl();
        }
        menu->Update(segmentList->GetCount(), segmentList->GetSelectedIndex());

        menu->Track(*mainWindow);
    }
}

void MainSegment::SaveSegment(SEGMENT_INDEX segmentIndex, bool writeHeader) {
    const auto fileType = writeHeader ? FileType::EXECUTABLE_FILE : FileType::RAW_FILE;

    const auto result = ::g_FileSystemLogic->GetSaveFileName(*segmentListWindow, L"", fileType);

    if (result.success) {
        OutputStream* outputStream = nullptr;
        try {
            outputStream = OutputStream::OpenFile(result.filePath, Encoding::BINARY);

            ::g_Workspace->GetComputerSystem()->WriteExecutableFile(*segmentList, segmentIndex, writeHeader, *outputStream);

            delete outputStream;

            // TODO success message
        }
        catch (const IOException& ex) {
            if (outputStream != nullptr) {
                delete outputStream;
            }
            application->SendErrorMessageWithException(ex);

        }
    }
}


void MainSegment::SaveAllSegments(const SegmentList& segmentList) {
    FileSystemLogicResult result = ::g_FileSystemLogic->GetSaveFileName(*segmentListWindow, L"", FileType::EXECUTABLE_FILE);

    if (result.success) {
        OutputStream* outputStream = nullptr;
        try {
            outputStream = OutputStream::OpenFile(result.filePath, Encoding::BINARY);

            g_Workspace->GetComputerSystem()->WriteExecutableFile(segmentList, NO_SEGMENT_INDEX, true, *outputStream);

            delete outputStream;

            // TODO: Success message
        }
        catch (const IOException& ex) {
            if (outputStream != nullptr) {
                delete outputStream;
            }
            application->SendErrorMessageWithException(ex);

        }
    }
}

void MainSegment::ShowPropertiesDialog(SEGMENT_INDEX segmentIndex) {

    SegmentPropertiesDialog dialog(*segmentListWindow);

    if (dialog.Show(*segmentList->GetSegment(segmentIndex))) {
        segmentList->NotifySegmentContentChanged();
    }
}

/*
** Selection has changed in the segment window.
*/
void MainSegment::Selected() {
    updateCounter++;
    QuitEditMode();

    const auto listIndex = segmentListWindow->GetSelectedIndex();
    SEGMENT_INDEX segmentIndex = listIndex;
    if (listIndex < 0) {
        segmentIndex = NO_SEGMENT_INDEX;
    }

    segmentList->SetSelectedIndex(segmentIndex);
    updateCounter--;
}
