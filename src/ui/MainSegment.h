#pragma once

#include "MainController.h"
#include "SegmentListPopupMenu.h"
#include "SegmentTypes.h"
#include "Window.h"
#include <memory>
#include <Windows.h>

class Main;
class SegmentList;
class SegmentListWindow;

class MainSegment final : public MainController {
public:
    MainSegment(Main& main);

    void UpdateList();

    long Proc(Window::WindowHandle hWnd, Window::WPARAM wParam);
    void PerformCommands(COMMAND command, bool editMode);

    static Window::LRESULT CALLBACK DropFilesProc(Window::WindowHandle hWnd, Window::MESSAGE message, Window::WPARAM wParam, Window::LPARAM lParam);
    static Window::LRESULT CALLBACK RButtonDownProc(Window::WindowHandle hWnd, Window::MESSAGE message, Window::WPARAM wParam, Window::LPARAM lParam);

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
