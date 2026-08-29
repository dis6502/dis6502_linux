#pragma once

#include "Syntax.h"

#include "MainController.h"
#include "SegmentListPopupMenu.h"

class SegmentList;
class SegmentListWindow;

class MainSegment final : public MainController {
public:
    MainSegment(Main& main);

    void UpdateList();

    long Proc(HWND hWnd, WPARAM wParam);
    void PerformCommands(COMMAND command, bool editMode);

    static LRESULT CALLBACK DropFilesProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK RButtonDownProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    SegmentList* segmentList;
    SegmentListWindow* segmentListWindow;
    std::unique_ptr<SegmentListPopupMenu> menu;
    int updateCounter;

    void DropFilesProc(HDROP hDrop);
    void RButtonDownProc();
    void Selected();

    void SaveSegment(SEGMENT_INDEX segmentIndex, bool withHeader);
    void SaveAllSegments(const SegmentList& segmentList);

    void ShowPropertiesDialog(SEGMENT_INDEX segmentIndex);

};
