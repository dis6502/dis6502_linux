#pragma once


#include "Dialog.h"
#include "Memory.h"
#include "SpriteControl.h"
#include "ComboBox.h"
#include "Window.h"
#include <memory>

class MemoryInspectorSelection;
class SpriteControl;


class SelectSpritesDialog: public Dialog {
public:
	SelectSpritesDialog(Window& parentWindow);
	SelectSpritesDialog(const SelectSpritesDialog& a) = delete;			  // copy constructor
	SelectSpritesDialog& operator=(const SelectSpritesDialog&) = delete;  // copy assignment
	SelectSpritesDialog(SelectSpritesDialog&&) = delete;                  // move constructor
	SelectSpritesDialog& operator=(SelectSpritesDialog&&) = delete;       // move assignment

	bool Show(const MemoryInspectorSelection& lpMemoryInspectorSelection);
	void GetSelection(Memory::offset&nBegin, Memory::offset& nEnd) const;

protected:
	bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	typedef wchar_t ITEM_LINE[14];

	std::unique_ptr<ComboBox> comboBox;
    std::unique_ptr <SpriteControl> spriteControl;

	const MemoryInspectorSelection* memoryInspectorSelection;
	Memory::word wSpriteMode;
	Memory::word wSpriteNbBytes;
	Memory::offset wBegin, wEnd;

	void Init();
	void Free();
	void SelectMode(WORD wMode);
	void SelectGoto();
};
