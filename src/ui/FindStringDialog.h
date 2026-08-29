#pragma once

#include "Syntax.h"

#include "Dialog.h"


class FindStringDialog final {
public:
	static const int HEX_CHARS_PER_BYTE = 3;

	FindStringDialog() = delete;
	FindStringDialog(int nMaxChars);
	~FindStringDialog();

	void SetAsciiString(Dialog& dialog, wstring_view asciiString);
	LPCWSTR GetAsciiString() const;
	int GetAsciiStringLength() const;

	// Updates both szFindAscii and szFindHex.
	// The first field value is taken from the input field, the second field value is computed
	bool AsciiStringToHexString(Dialog& dialog);
	bool HexStringToAsciiString(Dialog& dialog);

	static void ShowStringNotFoundMessage(Window* parentWindow, wstring_view text);

private:
	int nMaxChars;
	LPWSTR szFindAscii;
	int nFindAsciiSize;
	LPWSTR szFindHex;
	int nFindHexSize;

	bool ConvertAsciiStringToHexString();
	bool ConvertHexStringToAsciiString();

	// Updates screen fields only without side effects.
	void UpdateAsciiStringField(Dialog& dialog);
	void UpdateHexStringField(Dialog& dialog);
};
