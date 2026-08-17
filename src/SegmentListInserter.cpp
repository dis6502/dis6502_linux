#include "Application.h"
#include "CommonIO.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SegmentListInserter.h"
#include "Workspace.h"
#include <cassert>

extern std::unique_ptr<Application> g_Application;

SegmentListInserter::SegmentListInserter(SegmentList& segmentList) {
    this->segmentList = &segmentList;
    this->firstIndex = segmentList.GetCount();
    this->currentIndex = this->firstIndex;
    this->segmentList->BeginUpdate();
    this->active = true;
}

SegmentListInserter::SegmentListInserter(const SegmentListInserter& segmentListInserter) :
    SegmentListInserter(*segmentListInserter.segmentList) {

}

SegmentListInserter::~SegmentListInserter() {
    if (active) {
        Cancel();
    }
}

gsl::not_null<Segment*>  SegmentListInserter::InsertSegment() {
    assert(active);

    if (currentIndex == SegmentList::MAX_SEGMENTS) {
        ::g_Application->ThrowErrorMessageWithID(IDS_ERR_NO_FREE_SEG);
    }
    const auto& segment = segmentList->InsertSegmentAt(currentIndex);
    currentIndex++;
    return segment;
}

void SegmentListInserter::RemoveSegment() {
    assert(active);
    assert(currentIndex > 0);

    currentIndex--;
    segmentList->DeleteSegment(currentIndex);
}

void SegmentListInserter::Apply() {
    assert(active);

    if (currentIndex > firstIndex) {

        // Position to first added segment
        segmentList->SetSelectedIndex(firstIndex);

        // Currently the "Add/InsertAt" methods don't fire event directly.
        segmentList->NotifyListeners(SegmentList::Property::SEGMENTS);
        firstIndex = segmentList->GetCount();
        currentIndex = firstIndex;
        segmentList->EndUpdate(); // Commit current transaction
    }
}

void SegmentListInserter::Cancel() {
    assert(active);

    // Remove inserted segments in case there was no Apply().
    if (currentIndex > firstIndex) {

        while (currentIndex > firstIndex) {
            RemoveSegment();
        }
        segmentList->EndUpdate();
    }
    active = false;
}
