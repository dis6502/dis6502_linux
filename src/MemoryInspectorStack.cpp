#include "MemoryInspectorStack.h"


MemoryInspectorStack::MemoryInspectorStack() {
	lpFirst = nullptr;
}

MemoryInspectorStack::~MemoryInspectorStack() {
	Clear();
}

/*
** Clear stack.
 * Free all the buffers in a list.
** lpFirst is nullptr when then method returns.
*/
void MemoryInspectorStack::Clear() {
	Entry *lpStack;

	while ((lpStack = lpFirst)) {
		lpFirst = lpStack->pNext;
		delete lpStack;
	}
}

/*
** find the last buffer of a list.
*/
MemoryInspectorStack::Entry* MemoryInspectorStack::FindLastEntry() {
	if (Entry* lpLast = lpFirst; lpLast) {
		while (lpLast->pNext) {
			lpLast = lpLast->pNext;
		}

		return lpLast;
	}

	return nullptr;
}

/*
** allocate an address and add it at the end of a list.
*/
MemoryInspectorStack::Entry* MemoryInspectorStack::AllocateEntry() {
	Entry *lpStack;
	Entry *lpLast;

	// allocate a new buffer and initialize variables.
	lpStack = new Entry();
	lpStack->wLastIndex = 0;
	lpStack->pNext = 0;

	// add this buffer to the end of the list.
	if (lpLast = FindLastEntry(); lpLast) {
		lpLast->pNext = lpStack;
	}
	else {
		lpFirst = lpStack;
	}

	return lpStack;
}

/*
** return the last saved address in the stack
*/
Memory::address MemoryInspectorStack::PopAddress() {
	Entry *lpStack;
	Entry *lpLast;
	Memory::word wIndex;

	lpStack = FindLastEntry();
	if (lpStack == nullptr) {
		return 0xFFFF;
	}

	wIndex = lpStack->wLastIndex;
	if (wIndex > 0) {
		lpStack->wLastIndex = --wIndex;

		return lpStack->pStack[wIndex];
	}

	lpLast = lpFirst;
	while ((lpLast != nullptr) && (lpLast->pNext != lpStack)) {
		lpLast = lpLast->pNext;
	}

	if (lpLast) {
		lpLast->pNext = nullptr;
	}
	else {
		lpFirst = nullptr;
	}

	delete lpStack;
	
	return PopAddress();
}

/*
** Push an address on the stack
*/
void MemoryInspectorStack::PushAddress(Memory::address wAddr) {
    Entry* lpStack = FindLastEntry();
	if (lpStack == nullptr)
		lpStack = AllocateEntry();

	if (lpStack) {
		auto wIndex = lpStack->wLastIndex;

		if (wIndex >= (sizeof(lpStack->pStack) / sizeof(Memory::word))) {
			lpStack = AllocateEntry();
			wIndex = lpStack->wLastIndex;
		}

		if (lpStack) {
			lpStack->pStack[wIndex++] = wAddr;
			lpStack->wLastIndex = wIndex;
		}
	}
}
