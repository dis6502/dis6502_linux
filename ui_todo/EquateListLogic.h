#pragma once
#include "Syntax.h"

class Window;
class EquateList;
class Workspace;

class EquateListLogic final {

public:
	EquateListLogic();
	~EquateListLogic() = default;

	void SetParentWindow(Window& window);

	void LoadSystemEquates(Workspace& workspace);
	void LoadUserEquates(Workspace& workspace);
	void Save(const EquateList& equateList, bool xasm) const;

	void Clear(EquateList& equateList);
	void Edit(EquateList& equateList, bool editable, wstring_view address);
	void DefineUserAddressRange(Workspace& workspace, wstring_view address);

private:
	Window* parentWindow;

    wstring lastEquateFilePath;

};