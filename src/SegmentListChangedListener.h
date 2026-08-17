#pragma once

#include "SegmentList.h"
#include <vector>

class SegmentListChangedListener {
public:
	virtual void HandleSegmentListChanged(const SegmentList& segmentList, const std::vector<SegmentList::Property>& propertyChangeEvents) = 0;
};
