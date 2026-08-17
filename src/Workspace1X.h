#pragma once

#include "Syntax.h"
#include <cstdint>

constexpr size_t WORKSPACE_MAGIC_SIZE = 12;

constexpr auto WORKSPACE_MAGIC10 = "DIS6502WRK10";
constexpr auto WORKSPACE_MAGIC14 = "DIS6502WRK14";

class Workspace;
class InputStream;
class OutputStream;


class Workspace1X {
public:
    static void Load10(Workspace& workspace, InputStream& inputStream); // throws IOException
    static void Load14(Workspace& workspace, InputStream& inputStream);  // throws IOException
    static void Save14(const Workspace& workspace, OutputStream& outputStream); // throws IOException


    /*
    ** Note: The pointers in the file structure are problematic because the size of a pointer varies on depending on the architecture.
    ** Therefore they have been converted to byte sequences of the right length are are only used as indicators for nullptr / not nullptr.
    */
    typedef uint32_t POINTER_32BIT;

    static bool IsNull(const POINTER_32BIT pointer32Bit);

private:
    // Bool values in version 1X segments are 32 bit Window BOOL
    typedef int32_t BOOL_1X;
    typedef unsigned char CHAR_1X;

    // Addresses in version 1X segments are all 16 bit
    typedef unsigned short ADDRESS_1X;

    typedef unsigned long LINE_NUMBER_1X;

    static constexpr size_t FILE_PATH1X_SIZE = 260;
    typedef char FILE_PATH1X[FILE_PATH1X_SIZE];


    typedef struct SEGMENT10 {
        ADDRESS_1X     beginAddress = 0;
        ADDRESS_1X     endAddress = 0;
        POINTER_32BIT  data = 0;
        POINTER_32BIT  type = 0;
        LINE_NUMBER_1X firstLineNumber = 0;
    } SEGMENT10;

    typedef struct SEGMENT14 {
        ADDRESS_1X     beginAddress = 0;
        ADDRESS_1X     endAddress = 0;
        POINTER_32BIT  data = 0;
        POINTER_32BIT  type = 0;
        LINE_NUMBER_1X firstLineNumber = 0;
        BOOL_1X        binary = 0;
        CHAR_1X        title[22] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    } SEGMENT14;
};
