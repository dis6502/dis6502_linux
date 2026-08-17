#include "ComputerSystemType.h"

ComputerSystemTypeInfo::ComputerSystemTypeInfo(ComputerSystemType type, wstring_view key, wstring_view text, wstring_view fileName) :
    type{ type }, id{ key }, text{ text }, fileName{ fileName } {
}