#pragma once
#include "Dialog.h"
#include "Font.h"
#include "Syntax.h"
#include "Window.h"
#include <memory>

class ListBox;
class AtariDisk;

class DiskImageExecutableFileDialog : public Dialog {
public:
    DiskImageExecutableFileDialog(const Window& parentWindow, Font* computerFont);

    DialogFuncResult Show(wstring_view diskImageFilePath);
    wstring GetExecutableFilePath() const;

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

    void CreateControls() override;

private:
    Font* computerFont;

    std::unique_ptr<AtariDisk> atariDisk;
    wstring executableFilePath;
};
