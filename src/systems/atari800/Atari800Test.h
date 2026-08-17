#pragma once

#include "FileIO.h"
#include "FileType.h"

class ComputerSystem;
class InputStream;
class SegmentList;
class Segment;

class Atari800Test {
public:
    static void AssertSegmentListEquals(const ComputerSystem& computerSystem, FileType fileType, wstring_view filePath, wstring_view outFilePath);
    static void AssertSegmentListEquals(const ComputerSystem& computerSystem, FileType fileType, wstring_view diskImageFilePath, wstring_view fileName, bool expectReadFailure, wstring_view outFilePath);

private:
    static void AssertSegmentListEquals(const ComputerSystem& computerSystem, FileType fileType, wstring_view filePath, InputStream& inputStream, FileIO::FILE_SIZE fileSize, bool expectReadFailure, wstring_view outFilePath);
};