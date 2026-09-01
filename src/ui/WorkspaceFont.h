#pragma once

#include "UI.h"

class Workspace;

class WorkspaceFont {
public:
    // Font.
    static HFONT GetResizedFont(const Workspace& workspace);
    static int GetResizedFontWidth(const Workspace& workspace);
    static int GetResizedFontHeight(const Workspace& workspace);
};
