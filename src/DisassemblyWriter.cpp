#include "DisassemblyWriter.h"
#include "Strings.h"
#ifdef _WIN32
#include <Windows.h>
#endif

DisassemblyWriter::DisassemblyWriter(Disassembly& disassembly, Workspace& workspace) :
    wDisNbBytes(0) {
    this->disassembly = &disassembly;
    returnCharacter = workspace.GetComputerSystem()->GetReturnCharacter();
    profile = workspace.GetConstProfile();
    quoteCharacter = profile->quoteForASCIIStrings.at(0);
    memoryType = MemoryType::UNKNOWN;
}
/*
** Returns true if a character can be insterted in a string.
** Return falses if the caracter should appear as a hex byte.
*/
bool DisassemblyWriter::IsByteAllowedInString(byte value) const {
    if ((value == 0) || (value == returnCharacter) || (value == quoteCharacter)) {
        return false;
    }

    // TODO: This actually depends on the character set of the computersystm
    if ((profile->showNonASCIIChararactersAsBytes) && ((value < 0x20) || (value >= 0x7D) || (value == 0x60) || (value == 0x7B))) {
        return false;
    }

    return true;
}


void DisassemblyWriter::FLUSH_AND_ADD_LINE(const DisassemblyLineWriter& lineWriter, Memory::address wAddr) {

    FLUSH_BYTES();
    disassembly->absoluteAddress = wAddr;
    disassembly->AddLineWriter();
}


void DisassemblyWriter::FLUSH_AND_ADD_LINE_WITH_COMMENT(DisassemblyLineWriter& lineWriter, const DisassemblyOpcodeBuffer& opcodeBuffer, int nOpcodeSize, Memory::address wAddr) {

    if (profile->showOpcodeAsComment) {
        lineWriter.SpaceUntil34();
        lineWriter.String(profile->commentPrefix);
        lineWriter.Space();
        opcodeBuffer.Write(lineWriter, nOpcodeSize);
    }
    FLUSH_AND_ADD_LINE(lineWriter, wAddr);
}


void DisassemblyWriter::FLUSH_BYTES() {
    if (wDisNbBytes > 0) {
        const auto wSaveSize = disassembly->markSize;

        switch (memoryType) {
        case MemoryType::STRING:
        case MemoryType::SBYTE:
            wcscat(szBytes, profile->quoteForASCIIStrings.c_str());
            break;
        case MemoryType::STORE:
            wsprintf(szBytes + wcslen(szBytes), L"%hu", wDisNbBytes);
            break;
        default:
            break;
        }

        disassembly->markSize = wDisNbBytes;
        disassembly->AddLine(szBytes);
        strclr(szBytes);

        if (wSaveSize >= wDisNbBytes) {
            disassembly->markSize = wSaveSize - wDisNbBytes;
        }
        else {
            disassembly->markSize = 0;
        }

        wDisNbBytes = 0;
        memoryType = MemoryType::UNKNOWN;
    }
}


void DisassemblyWriter::DUMP_STORE() {
    if (memoryType != MemoryType::STORE) {
        FLUSH_BYTES();
    }

    memoryType = MemoryType::STORE;

    if (wDisNbBytes == 0) {
        wsprintf(szBytes, L"%s%s ", GetLineBuffer(), profile->directiveDS.c_str());
    }

    wDisNbBytes++;
}


void DisassemblyWriter::DUMP_BYTE(Memory::byte bByte) {
    if (wDisNbBytes >= profile->directiveBYTENumberOfBytesPerLine) {
        FLUSH_BYTES();
    }
    else if (memoryType != MemoryType::BYTE) {
        FLUSH_BYTES();
    }

    memoryType = MemoryType::BYTE;

    if (profile->useHexNotation) {
        if (wDisNbBytes == 0) {
            wsprintf(szBytes, L"%s%s %s%02hX", GetLineBuffer(), profile->directiveBYTE.c_str(), profile->hexNotationPrefix.c_str(), bByte);
        }
        else {
            wsprintf(szBytes + wcslen(szBytes), L"%s%s%02hX", profile->directiveBYTESeparator.c_str(), profile->hexNotationPrefix.c_str(), bByte);
        }
    }
    else {
        if (wDisNbBytes == 0) {
            wsprintf(szBytes, L"%s%s %hu", GetLineBuffer(), profile->directiveBYTE.c_str(), bByte);
        }
        else {
            wsprintf(szBytes + wcslen(szBytes), L"%s%hu", profile->directiveBYTESeparator.c_str(), bByte);
        }
    }

    wDisNbBytes++;
}


void DisassemblyWriter::DUMP_WORD(Memory::word wWord) {
    if (wDisNbBytes >= (profile->directiveWORDNumberOfWordsPerLine * 2u)) {
        FLUSH_BYTES();
    }
    else if (memoryType != MemoryType::WORD) {
        FLUSH_BYTES();
    }

    memoryType = MemoryType::WORD;

    if (profile->useHexNotation) {
        if (wDisNbBytes == 0) {
            wsprintf(szBytes, L"%s%s %s%04hX", GetLineBuffer(), profile->directiveWORD.c_str(), profile->hexNotationPrefix.c_str(), wWord);
        }
        else {
            wsprintf(szBytes + wcslen(szBytes), L"%s%s%04hX", profile->directiveBYTESeparator.c_str(), profile->hexNotationPrefix.c_str(), wWord);
        }
    }
    else {
        if (wDisNbBytes == 0) {
            wsprintf(szBytes, L"%s%s %hu", GetLineBuffer(), profile->directiveWORD.c_str(), wWord);
        }
        else {
            wsprintf(szBytes + wcslen(szBytes), L"%s%hu", profile->directiveBYTESeparator.c_str(), wWord);
        }
    }

    wDisNbBytes++;
    wDisNbBytes++;
}

void DisassemblyWriter::DUMP_STRING(byte bChar) {
    if (!IsByteAllowedInString(bChar)) {
        DUMP_BYTE(bChar);
    }
    else {
        if (wDisNbBytes >= profile->directiveBYTENumberOfCharactersPerString) {
            FLUSH_BYTES();
        }
        else if (memoryType != MemoryType::STRING) {
            FLUSH_BYTES();
        }

        memoryType = MemoryType::STRING;

        if (wDisNbBytes == 0) {
            wsprintf(szBytes, L"%s%s %s%c", GetLineBuffer(), profile->directiveBYTE.c_str(), profile->quoteForASCIIStrings.c_str(), bChar);
        }
        else {
            auto len = wcslen(szBytes);
            szBytes[len] = bChar;
            szBytes[len + 1] = 0;
        }

        wDisNbBytes++;
    }
}


void DisassemblyWriter::DUMP_SBYTE(byte bByte) {
    byte cInternal;
    cInternal = bByte;

    if (cInternal < 64) {
        cInternal += 32;
    }
    else if (cInternal < 96) {
        cInternal -= 64;
    }
    else if ((cInternal >= 128) && (cInternal < 128 + 64)) {
        cInternal += 32;
    }
    else if ((cInternal >= 128 + 64) && (cInternal < 128 + 96)) {
        cInternal -= 64;
    }

    if ((cInternal == returnCharacter) || (cInternal == 0x22) || (cInternal == 0)) {
        DUMP_BYTE(bByte);
    }
    else {
        if (wDisNbBytes >= profile->directiveBYTENumberOfCharactersPerString) {
            FLUSH_BYTES();
        }
        else if (memoryType != MemoryType::SBYTE) {
            FLUSH_BYTES();
        }

        memoryType = MemoryType::SBYTE;

        if (wDisNbBytes == 0) {
            wsprintf(szBytes, L"%s%s %s%c", GetLineBuffer(), profile->directiveSBYTE.c_str(), profile->quoteForASCIIStrings.c_str(), cInternal);
        }
        else {
            auto len = wcslen(szBytes);
            szBytes[len] = cInternal;
            szBytes[len + 1] = 0;
        }

        wDisNbBytes++;
    }
}


void DisassemblyWriter::DUMP_LABEL(const wchar_t* szLabel) {
    if (wDisNbBytes >= (profile->directiveWORDNumberOfWordsPerLine * 2u)) {
        FLUSH_BYTES();
    }
    else if (memoryType != MemoryType::LABEL) {
        FLUSH_BYTES();
    }

    memoryType = MemoryType::LABEL;

    if (wDisNbBytes == 0) {
        wsprintf(szBytes, L"%s%s %s", GetLineBuffer(), profile->directiveWORD.c_str(), szLabel);
    }
    else {
        wsprintf(szBytes + wcslen(szBytes), L"%s%s", profile->directiveBYTESeparator.c_str(), szLabel);
    }

    wDisNbBytes++;
    wDisNbBytes++;
}
