#pragma once

#include "Syntax.h"
#include <vector>

enum class ProcessorType {
    UNKNOWN, 
    MOS6502,
    MOS65C02
};


struct ProcessorTypeInfo {
	const ProcessorType processorType;
	const wstring key;
	const wstring text;

	ProcessorTypeInfo(const ProcessorType processorType, const wstring_view key, const wstring_view text) :
        processorType{ processorType }, key{ key }, text{ text }
	{
	}
};

class ProcessorTypeFactory {
public:
	static const ProcessorTypeInfo& GetInfo(const wstring_view key);
	static const ProcessorTypeInfo& GetInfo(const ProcessorType processorType);
    static int GetIndex(const ProcessorType processorType, const std::vector<ProcessorType> processorTypes, int defaultIndex);
};
