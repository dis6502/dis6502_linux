#pragma once

#include "Syntax.h"
#include "Byte.h"

class DisassemblyLineWriter;

class DisassemblyOpcodeBuffer
{
public:
    void Clear();

    /*
    ** Save the last byte as opcode in a circular 3 byte buffer (to display opcodes as comment)
    */
    void SaveLastOpcode(byte bByte);

    void Write(DisassemblyLineWriter& lineWriter, int opcodeSize) const;

private:
    byte opcode0;
    byte opcode1;
    byte opcode2;
};

