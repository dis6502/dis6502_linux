#include "CommonIO.h"
#include "StringUtility.h"

wstring IOException::GetText() const {
    return String::utf8_to_wstring(what());
}