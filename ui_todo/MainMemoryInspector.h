#pragma once

#include "Syntax.h"
#include "MainController.h"

class Window;


class MainMemoryInspector final : public MainController {
public:
	MainMemoryInspector(Main& main);

	long Proc(HWND hWnd, WPARAM wParam);
	void PerformCommands(COMMAND command, bool editMode);

private:
	void SelectAll();
	void SelectNextUnknownBlock();
	void Find();
	void FindNext();
	void Edit();

	void SaveWithoutHeader(const Window& parentWindow);
	void SaveWithHeader(const Window& parentWindow);
};
