#include "Application.h"

#include "Text.h"
#include "FindStringDialog.h"
#include "EditControl.h"
#include "MessageBoxDialog.h"
#include "String.h"
#include "DatatypeUtility.h"

FindStringDialog::FindStringDialog(int nMaxChars) {
    this->nMaxChars = nMaxChars;
    nFindAsciiSize = nMaxChars + 1;
    szFindAscii = new wchar_t[nFindAsciiSize];
    szFindAscii[0] = 0;
    nFindHexSize = (nMaxChars * HEX_CHARS_PER_BYTE) + 1;
    szFindHex = new wchar_t[nFindHexSize];
    szFindHex[0] = 0;
}

FindStringDialog::~FindStringDialog() {
    delete szFindAscii;
    delete szFindHex;
}

void FindStringDialog::SetAsciiString(Dialog& dialog, wstring_view asciiString) {
    wcscpy_s(this->szFindAscii, nMaxChars, wstring(asciiString).c_str());
    UpdateAsciiStringField(dialog);
}

LPCWSTR FindStringDialog::GetAsciiString() const {
    return szFindAscii;
}

int FindStringDialog::GetAsciiStringLength() const {
    return String::Length(szFindAscii);
}

bool FindStringDialog::ConvertAsciiStringToHexString() {
    int nIndex = 0;
    for (size_t i = 0; i < String::Length(szFindAscii); i++) {
        wsprintf(&szFindHex[nIndex], L"%02hX ", szFindAscii[i] & 0xFF);
        nIndex += HEX_CHARS_PER_BYTE;
    }

    if (nIndex > 0) { nIndex--; }
    szFindHex[nIndex] = 0;

    return nIndex > 0;
}

bool FindStringDialog::ConvertHexStringToAsciiString() {
    int index;
    wchar_t szConv[5]{ L"0x00" };

    index = 0;
    for (size_t i = 0; i < String::Length(szFindHex);) {
        if (szFindHex[i] == L' ') {
            i++;
            continue;
        }

        if (szFindHex[i + 1] != L' ' &&
            szFindHex[i + 1] != 0) {
            szConv[2] = szFindHex[i];
            szConv[3] = szFindHex[i + 1];
            i = i + 2;
        }
        else {
            szConv[2] = L'0';
            szConv[3] = szFindHex[i];
            i = i + 1;
        }

        szConv[4] = 0;
        byte hexByte;
        if (!DatatypeUtility::ByteFromString(hexByte, szConv)) {
            return false; // No valid hex string.
        }

        szFindAscii[index++] = (wchar_t)hexByte;
    }

    szFindAscii[index] = 0;

    return index > 0;
}

void FindStringDialog::UpdateAsciiStringField(Dialog& dialog) {
    dialog.GetEditControl(IDC_FINDASCII).SetTextSize(nFindAsciiSize - 1);
    dialog.GetEditControl(IDC_FINDASCII).SetText(szFindAscii);
}

void FindStringDialog::UpdateHexStringField(Dialog& dialog) {
    dialog.GetEditControl(IDC_FINDHEX).SetTextSize(nFindHexSize - 1);
    dialog.GetEditControl(IDC_FINDHEX).SetText(szFindHex);
}

bool FindStringDialog::AsciiStringToHexString(Dialog& dialog) {
    auto text = dialog.GetEditControl(IDC_FINDASCII).GetText();
    wcscpy(szFindAscii, text.c_str());
    nFindAsciiSize = text.length();

    const bool bFind = ConvertAsciiStringToHexString();
    UpdateHexStringField(dialog);

    return bFind;
}

bool FindStringDialog::HexStringToAsciiString(Dialog& dialog) {
    auto text = dialog.GetEditControl(IDC_FINDHEX).GetText();
    wcscpy(szFindHex, text.c_str());
    nFindHexSize = text.length();

    const auto result = ConvertHexStringToAsciiString();
    UpdateAsciiStringField(dialog);

    return result;
}

void FindStringDialog::ShowStringNotFoundMessage(Window* parentWindow, wstring_view text) {
    auto title = Text::Get(IDS_FIND_STRING_DIALOG_TITLE);
    auto message = Text::Format(IDS_FIND_STRING_DIALOG_STRING_NOT_FOUND_MESSAGE, text);
    MessageBoxDialog::Show(parentWindow, title, message, MB_OK | MB_ICONEXCLAMATION);
}
