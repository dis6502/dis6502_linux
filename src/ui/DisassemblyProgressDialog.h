#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "DisassemblyProgressMonitor.h"

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

private:
	Disassembly* disassembly;
	bool cancelled ;
	int updateCount;

	void Init(Disassembly* disassembly);
	void RedrawScreen(HWND hDlg);
};
