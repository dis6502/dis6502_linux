#include "FileHeader.h"

#include "DatatypeUtility.h"
#include "Syntax.h"
#include "Word.h"

wstring to_wstring(FileHeader fileHeader) {
   
    return DatatypeUtility::WordToHexString((word)fileHeader, true);
}