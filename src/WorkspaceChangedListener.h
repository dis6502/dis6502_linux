#pragma once

#include <vector>
#include "Workspace.h"
#include "WorkspaceTypes.h"


class WorkspaceChangedListener {
public:
	virtual void HandleWorkspaceChanged(const Workspace& workspace, const std::vector<WorkspaceProperty>& propertyChangeEvents) =0;
};
