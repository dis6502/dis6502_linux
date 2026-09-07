#include "CommonIO.h"
#include "StringUtility.h"
#include "Syntax.h"

wstring IOException::GetText() const {
    return String::utf8_to_wstring(what());
}