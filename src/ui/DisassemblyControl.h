/*
* Wrapper class for DisassemblyControlImpl.
* This class wrapps the methods implemented as messages in the DisassemblyControlImpl class.
*/
#pragma once

#include "Memory.h"
#include "Syntax.h"

#include "SegmentTypes.h"
#include "DisassemblyResult.h"

#include "Window.h"
#include <cstddef>


class DisassemblyControl : public Window {
public:
    using LineNumber = DisassemblyResult::LineNumber;

	static void RegisterClassFor(HINSTANCE hInstance);

	DisassemblyControl(const Window& parentWindow, ChildID childID);

	void CreateControl(int x, int y, int nWidth, int nHeight);

	bool AreLineNumbersActive() const;
	void SetLineNumbersActive(bool lineNumbersActive);

	void SetResult(DisassemblyResult& result);

	void Refresh();
	void SelectLine(LineNumber lineNumber);
	bool SelectLineWithInfo(SEGMENT_NUMBER segmentNumber, Memory::offset offset);
	bool ExtendSelectionTo(SEGMENT_NUMBER segmentNumber, Memory::offset offset);

	void SetScrollLineNumber(LineNumber lineNumber);
	void SyncLine(LineNumber lineNumber);

	size_t GetHistoryIndex() const;
	void ResetHistory();
	void BackInHistory();

	wstring GetLabelReference() const;
	void SelectLabelDefinition(wstring_view label);

	wstring GetLabelDefinition() const;
	void SelectAllLabelReferences(wstring_view label);

private:
	int childID;
};
