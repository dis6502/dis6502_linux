#pragma once

class EquateList;
#include "WorkspaceTypes.h"


class EquateListChangedListener {
public:
    virtual void HandleEquateListChanged(const EquateList& EquateList, const WorkspaceProperty workspaceProperty) = 0;
};
