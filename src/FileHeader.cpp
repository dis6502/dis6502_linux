#include "FileHeader.h"

#include "DatatypeUtility.h"

wstring to_wstring(FileHeader fileHeader) {
   
    return DatatypeUtility::WordToHexString((word)fileHeader, true);
}