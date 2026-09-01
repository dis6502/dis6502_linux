#pragma once
#include "Syntax.h"
class EquateList;
class EquateListLogic;
class Workspace;
class Window;

class EquateListController final {

public:
    EquateListController(EquateListLogic& equateListLogic);
    ~EquateListController() = default;

    void SetParentWindow(Window& window);

    void LoadUserEquates(Workspace& workspace);
    void Save(const EquateList& equateList, bool xasm) const;

    void Clear(EquateList& equateList);
    void Edit(EquateList& equateList, bool editable, wstring_view address);
    void DefineUserAddressRange(Workspace& workspace, wstring_view address);

private:
    EquateListLogic* equateListLogic;
    Window* parentWindow;

    wstring lastEquateFilePath;

};