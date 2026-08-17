#pragma once

#include "Syntax.h"
#include "Memory.h"

class MemoryInspectorStack {
public:
	MemoryInspectorStack();
	virtual ~MemoryInspectorStack();

	void Clear();
	Memory::address PopAddress();
	void PushAddress(Memory::address wAddr);

private:
	/*
	** Structure of one stack entry. This is a linked list with only a pointer on the next buffer.
	*/
	class Entry;

	Entry* lpFirst;

	Entry* FindLastEntry();
	Entry* AllocateEntry();
};


class MemoryInspectorStack::Entry {
public:
	Memory::word wLastIndex;
	Entry* pNext;
	Memory::word pStack[1024];
};