#pragma once

#include <gsl/gsl>
#include <vector>

#include "Syntax.h"
#include "Dialog.h"

class ListBox;


class AboutDialog : public Dialog {
public:
	AboutDialog(Window& parentWindow);
	AboutDialog(const AboutDialog& a) = delete;			  // copy constructor
	AboutDialog& operator=(const AboutDialog&) = delete;  // copy assignment
	AboutDialog(AboutDialog&&) = delete;                  // move constructor
	AboutDialog& operator=(AboutDialog&&) = delete;        // move assignment

	void Show();

protected:
	bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	class Module;

	std::vector<std::unique_ptr<Module>> modules;

	void GetModuleVersion(Module& module);
	void GetAllModuleVersions();
};


class AboutDialog::Module {
public:
	Module(wstring_view name);

	wstring name;
	wstring version;
	wstring description;
};
