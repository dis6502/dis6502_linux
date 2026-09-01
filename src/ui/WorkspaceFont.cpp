#include "ComputerFont.h"
#include "Workspace.h"
#include "WorkspaceFont.h"
#include <Windows.h>

HFONT WorkspaceFont::GetResizedFont(const Workspace& workspace) {

    return ComputerFont::Get(*workspace.GetComputerSystem()).GetFont(workspace.IsViewDoubleHeight());

}

int WorkspaceFont::GetResizedFontWidth(const Workspace& workspace) {

    return 8;

}

int WorkspaceFont::GetResizedFontHeight(const Workspace& workspace) {

    return workspace.IsViewDoubleHeight() ? 16 : 8;

}