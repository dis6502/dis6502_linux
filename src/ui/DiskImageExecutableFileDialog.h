#pragma once
#include "Syntax.h"

#include "Dialog.h"

class ListBox;
class AtariDisk;

class DiskImageExecutableFileDialog : public Dialog {
public:
    DiskImageExecutableFileDialog(const Window& parentWindow, HFONT hComputerFont);

    INT_PTR Show(wstring_view diskImageFilePath);
    wstring GetExecutableFilePath() const;

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

    void CreateControls() override;

private:
    HFONT hComputerFont;

    std::unique_ptr<AtariDisk> atariDisk;
    wstring executableFilePath;
};
