#pragma once

#include "Dialog.h"
#include "DisassemblyProgressMonitor.h"
#include "SegmentTypes.h"
#include "Syntax.h"
#include "Window.h"

class Disassembly;


class DisassemblyProgressDialog : public Dialog, public DisassemblyProgressMonitor {
public:
    DisassemblyProgressDialog(Window& parentWindow);

    void DisassembleInternal(Disassembly& disassembly) override;
    void SetPass(wstring_view pass) override;
    void SetSegmentNumber(SEGMENT_NUMBER segmentNumber) override;
    bool IsCancelled() override;

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnCancel() override;

private:
    static constexpr int WM_USER_COMMAND = WM_USER + 4321;

    Disassembly* disassembly;
    bool cancelled;
    int updateCount;

    void Init(Disassembly* disassembly);
    void RedrawScreen(HWND hDlg);
};
