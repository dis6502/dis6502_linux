#include "IO.h"
#include "String.h"

wstring IOException::GetText() const {
    return String::utf8_to_wstring(what());
}