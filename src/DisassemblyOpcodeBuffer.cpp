#include "DisassemblyOpcodeBuffer.h"

#include "DisassemblyLineWriter.h"

void DisassemblyOpcodeBuffer::Clear() {
    opcode0 = opcode1 = opcode2 = 0;
}
void DisassemblyOpcodeBuffer::SaveLastOpcode(byte bByte) {
    opcode0 = opcode1;
    opcode1 = opcode2;
    opcode2 = bByte;
}

void DisassemblyOpcodeBuffer::Write(DisassemblyLineWriter& lineWriter, int opcodeSize) const {
    switch (opcodeSize) {
    case 1:
        lineWriter.Byte(opcode2);
        break;
    case 2:
        lineWriter.Byte(opcode1);
        lineWriter.Space();
        lineWriter.Byte(opcode2);
        break;
    case 3:
        lineWriter.Byte(opcode0);
        lineWriter.Space();
        lineWriter.Byte(opcode1);
        lineWriter.Space();
        lineWriter.Byte(opcode2);
        break;
    default:
        throw std::runtime_error("Invalid opcode size");
    }
}