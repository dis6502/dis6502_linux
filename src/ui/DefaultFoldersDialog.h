#pragma once

#include "Syntax.h"
#include "UI.h"

#include "Dialog.h"

enum class FolderType; 
class DefaultFolders;

class DefaultFoldersDialog : public Dialog {
public:
	DefaultFoldersDialog(Window& parentWindow);

	bool Show(HWND hWndParent, DefaultFolders& lpDefaultFolders);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	DefaultFolders *defaultFolders = nullptr;

	void SetDialogValues(const DefaultFolders& defaultFolders);
	void GetDialogValues(DefaultFolders& defaultFolders);
	bool SelectFolder(const FolderType folderType, Dialog::ITEM_ID itemID);
};
