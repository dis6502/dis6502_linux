
#include "PlatformCompat.h"
#include "FileHeader.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SegmentListInserter.h"
#include "Workspace.h"
#include "EquateList.h"
#include "Workspace1X.h"

static constexpr size_t MAX_SEGMENTS10 = 256;
static constexpr size_t MAX_SEGMENTS14 = 256;

// NOTE(Linux port): these were declared `extern` here but never defined
// anywhere in the codebase - grepping the whole source tree found no
// other reference to either symbol. That means the original build could
// never have actually linked a binary that called Load10/Load14/Save14.
// Since nothing else needs these across translation units, they're
// defined here directly instead.
static wstring binPath;  // To be part of workspace, used by "Workspace.cpp"
static wstring diskPath;  // To be part of workspace, used by "Workspace.cpp"

bool Workspace1X::IsNull(const POINTER_32BIT pointer32Bit) {
    return pointer32Bit == 0u;
};

void Workspace1X::Load10(Workspace& workspace, InputStream& inputStream) {
    SEGMENT10 Segments10[MAX_SEGMENTS10];
    FILE_PATH1X szBinPath = { };
    FILE_PATH1X szDiskPath = { };;

    // Read workspace fields.
    inputStream.Read(szBinPath, FILE_PATH1X_SIZE);
    szBinPath[FILE_PATH1X_SIZE - 1] = 0;

    inputStream.Read(szDiskPath, FILE_PATH1X_SIZE);
    szDiskPath[FILE_PATH1X_SIZE - 1] = 0;

    binPath = String::ansi_to_wstring(szBinPath);
    diskPath = String::ansi_to_wstring(szDiskPath);

    const auto& segmentList = workspace.GetSegmentList();

    // Read segment headers.
    memset(&Segments10, 0, sizeof(Segments10));

    auto segmentListInserter = segmentList->CreateInserter();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < MAX_SEGMENTS10; segmentIndex++) {
        auto segment10 = &gsl::at(Segments10, segmentIndex);

        inputStream.Read(segment10, sizeof(SEGMENT10));

        if (!IsNull(segment10->data)) {
            const auto& segment = segmentListInserter->InsertSegment();
            segment->wBegin = segment10->beginAddress;
            segment->wEnd = segment10->endAddress;
            segment->SetFirstLineNumber(segment10->firstLineNumber);
            segment->bBinary = true;
            gsl::at(segment->szTitle, 0) = 0;
        }
    }
    segmentListInserter->Apply();

    // Skip data that potentially cannot be read.
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < 1024 - MAX_SEGMENTS10 - SegmentList::MAX_SEGMENTS; segmentIndex++) { // TODO: Why - SegmentList::MAX_SEGMENTS? the result is always <0?!
        SEGMENT10 Segment10;

        inputStream.Read(&Segment10, sizeof(SEGMENT10));
    }

    // Read memory data and type.
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
        const auto& segment = segmentList->GetSegment(segmentIndex);

        segment->Load14(inputStream); // TODO Should this be Load10??
    }
}

void Workspace1X::Load14(Workspace& workspace, InputStream& inputStream) {
    constexpr auto MAX_INDEX = sizeof(FILE_PATH1X) - 1;
    FILE_PATH1X szBinPath = {};
    FILE_PATH1X	szDiskPath = {};
    SEGMENT14 Segments14[MAX_SEGMENTS14];

    // Read workspace fields.
    inputStream.Read(szBinPath, sizeof(szBinPath));
    szBinPath[MAX_INDEX] = 0; // Safeguard
    inputStream.Read(szDiskPath, sizeof(szDiskPath));
    szDiskPath[MAX_INDEX] = 0; // Safeguard

    binPath = String::ansi_to_wstring(szBinPath);
    diskPath = String::ansi_to_wstring(szDiskPath);

    const auto& segmentList = workspace.GetSegmentList();

    // Read segment headers.
    static constexpr auto segments14Size = sizeof(Segments14);
    memset(&Segments14, 0, segments14Size);

    auto segmentListInserter = segmentList->CreateInserter();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < MAX_SEGMENTS14; segmentIndex++) {
        auto segment14 = &gsl::at(Segments14, segmentIndex);

        inputStream.Read(segment14, sizeof(SEGMENT14));

        if (!IsNull(segment14->data)) {
            const auto& segment = segmentListInserter->InsertSegment();
            segment->SetHeader(FileHeader::ATARI_BINARY);
            segment->wBegin = segment14->beginAddress;
            segment->wEnd = segment14->endAddress;
            segment->SetFirstLineNumber(segment14->firstLineNumber);
            segment->bBinary = (segment14->binary ? true : false);
            segment->szTitle = String::ansi_to_wstring((const char*)segment14->title); // Assumes correct zero termination
        }
    }
    segmentListInserter->Apply();

    // Read user equates.
    workspace.GetUserEquateList()->Load1X(inputStream);

    // Read memory data and type.
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
        const auto& segment = segmentList->GetSegment(segmentIndex);

        segment->Load14(inputStream);
    }
}

void Workspace1X::Save14(const Workspace& workspace, OutputStream& outputStream) {
    const char* szMagic = WORKSPACE_MAGIC14;
    FILE_PATH1X szBinPath;
    FILE_PATH1X	szDiskPath;

    strncpy_s(szBinPath, String::wstring_to_ansi(binPath).c_str(), sizeof(szBinPath));
    strncpy_s(szDiskPath, String::wstring_to_ansi(diskPath).c_str(), sizeof(szDiskPath));

    outputStream.Write(szMagic, strlen(WORKSPACE_MAGIC14));
    outputStream.Write(szBinPath, sizeof(szBinPath));
    outputStream.Write(szDiskPath, sizeof(szDiskPath));

    // Write segment headers.
    SEGMENT14 segment14;

    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < MAX_SEGMENTS10; segmentIndex++) {
        const auto& segment = workspace.GetConstSegment(segmentIndex);
        segment14.beginAddress = segment->wBegin;
        segment14.endAddress = segment->wEnd;
        segment14.firstLineNumber = segment->GetFirstLineNumber();
        segment14.binary = segment->bBinary;

        // Truncate the title if required
        strncpy_s((char*)segment14.title, sizeof(segment14.title), String::wstring_to_ansi(segment->szTitle).c_str(), _TRUNCATE);


        outputStream.Write(&segment14, sizeof(segment14));
    }

    workspace.GetConstUserEquateList()->Save1X(outputStream);

    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < MAX_SEGMENTS14; segmentIndex++) {
        const MemoryBlock& memoryBlock = workspace.GetConstSegment(segmentIndex)->memoryBlock;

        if (!memoryBlock.IsEmpty()) {
            memoryBlock.WriteData(outputStream);
            memoryBlock.WriteType(outputStream);
        }
    }
}
