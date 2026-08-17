#pragma once

#include "Syntax.h"

#include "systems/ComputerSystem.h"
#include "Disassembly.h"
#include "DisassemblyOpcodeBuffer.h"
#include "DisassemblyLineWriter.h"

#include "DisassemblySectionType.h"
#include "MemoryType.h"
#include "Profile.h"
#include "Workspace.h"


/**
 * Used in Pass4
 */
class DisassemblyWriter {
public:
    DisassemblyWriter(Disassembly& disassembly, Workspace& workspace);

    /*
** add a line of code to the disassembly listing but, first, flush any pending .byte
** directive.
*/
    void FLUSH_AND_ADD_LINE(const DisassemblyLineWriter& lineWriter, Memory::address wAddr);

    /*
    ** Add a line of code to the disassembly listing but, first, flush any pending .byte
    ** directive.
    */
    void FLUSH_AND_ADD_LINE_WITH_COMMENT(DisassemblyLineWriter& lineWriter, const DisassemblyOpcodeBuffer& opcodeBuffer, int nOpcodeSize, Memory::address wAddr);

    /*
    ** Flush all collected .DS/.BYTE/.SBYTE/.WORD directives in the disassembly listing.
    */
    void FLUSH_BYTES();

    /*
    ** Skip another byte with a .DS directive.
    */
    void DUMP_STORE();

    /*
    ** Save a byte in a temporary buffer. This byte will be written in a .BYTE directive
    ** in an hexadecimal format.
    */
    void DUMP_BYTE(Memory::byte bByte);

    /*
    ** Save a word in a temporary buffer. This word will be written in a .WORD directive
    ** in an hexadecimal format.
    */
    void DUMP_WORD(Memory::word wWord);

    /*
    ** Save a character in a temporary buffer. This byte will be written in a .BYTE directive
    ** in a string format containing ASCII code.
    */
    void DUMP_STRING(byte bChar);

    /*
    ** Save a character in a temporary buffer. This byte will be written in a .SBYTE directive
    ** in a string format containing internal code.
    */
    void DUMP_SBYTE(byte bByte);

    /*
    ** Save a label in a temporary buffer. This label will be written in a .WORD directive.
    */
    void DUMP_LABEL(const wchar_t* szLabel);

    /*
    ** Returns true if the given byte are allowed to be represented as a string
    */
    bool IsByteAllowedInString(byte value) const;

private:
    Disassembly* disassembly;
    const Profile* profile;
    byte returnCharacter;
    byte quoteCharacter;

    Memory::word wDisNbBytes;
    MemoryType memoryType;
    wchar_t szBytes[256];

    const wchar_t* GetLineBuffer() {
        return disassembly->lineWriter.GetLineBuffer();
    }

};
