#include "IO.h"
#include "Strings.h"

wstring IOException::GetText() const {
    return String::utf8_to_wstring(what());
}