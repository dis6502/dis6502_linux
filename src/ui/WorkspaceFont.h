#pragma once

#include "Font.h"

class Workspace;

class WorkspaceFont {
public:
    // Font.
    static Font* GetResizedFont(const Workspace& workspace);
    static int GetResizedFontWidth(const Workspace& workspace);
    static int GetResizedFontHeight(const Workspace& workspace);
};
