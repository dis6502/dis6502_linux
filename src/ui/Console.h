#pragma once

#include "Syntax.h"

class Console {
public:
	Console();
	void Write(wstring_view message);
	string ReadLine();

private:
	bool allocated;

	void Allocate();
};
