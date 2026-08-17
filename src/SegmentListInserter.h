#pragma once

#include "Syntax.h"

#include "SegmentTypes.h"

class SegmentList;

class SegmentListInserter {
public:

    SegmentListInserter() = delete;                                      // no default constructor
    SegmentListInserter(SegmentList& segmentList);
    SegmentListInserter(const SegmentListInserter& a);                   // have copy constructor for unique_ptr assignment
    SegmentListInserter& operator=(const SegmentListInserter&) = delete; // no copy assignment
    SegmentListInserter(SegmentListInserter&&) = delete;                 // no move constructor
    SegmentListInserter& operator=(SegmentListInserter&&) = delete;      // no move assignment
    ~SegmentListInserter();

    gsl::not_null<Segment*> InsertSegment(); // thows IOException if no further segment can be inserted
    void RemoveSegment();
    void Apply();
    void Cancel();


private:
    
    SegmentList* segmentList;
    SEGMENT_INDEX firstIndex;
    SEGMENT_INDEX currentIndex;
    bool active;
};