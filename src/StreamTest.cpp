#include "StreamTest.h"

#include "Byte.h"
#include "InputStream.h"
#include "Assert.h"

void StreamTest::AssertInputStreamsEqual(InputStream& actualInputStream, InputStream& expectedInputStream, Stream::stream_size expectedStreamSize) {
    for (InputStream::stream_offset position = 0; position < expectedStreamSize; position++) {
        byte actualByte;
        actualByte = 0;
        try {
            actualInputStream.Read(&actualByte, sizeof(actualByte));
        }
        catch (const IOException& ex) {
            Assert::LogValue(L"Premature end of actual stream at position ", std::to_wstring(position));
            Assert::Fail(ex);

        }
        byte expectedByte;
        expectedByte = 0;
        try {
            expectedInputStream.Read(&expectedByte, sizeof(expectedByte));
        }
        catch (const IOException& ex) {
            {}
            Assert::LogValue(L"Premature end of expected stream at position", std::to_wstring(position));
            Assert::Fail(ex);
        }
        if (actualByte != expectedByte) {
            Assert::LogValue(L"Difference at stream position", std::to_wstring(position));

            Assert::LogValue(L"Actual Input Stream Byte  ", std::to_wstring(actualByte));
            Assert::LogValue(L"Expected Input Stream Byte", std::to_wstring(expectedByte));
            Assert::Fail(L"Different input stream content");

        }
        //Debug::LogValue("Actual File Input Stream Byte  ", std::to_string(position)+"/"+std::to_string(actualByte) + "/" + std::to_string(actualByte));

    }
}

