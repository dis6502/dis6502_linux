#pragma once

#include <array>

#include "Syntax.h"
#include "Byte.h"

#include "LabelAccess.h"


enum class OperandMode {
    Immediate,
    Absolute,
    ZeroPage,
    Accumulator,
    Implied,
    IndexedIndirect,
    IndirectIndexed,
    ZeroPageX,
    ZeroPageY,
    AbsoluteX,
    AbsoluteY,
    Relative,
    Indirect,
    ZeroPageIndirect,
    ZeroPageRelative,
    IndexedIndirectAbsolute,
    ReservedNop1Byte,
    ReservedNop2Byte,
    ReservedNop3Byte
};

class Instruction {

public:
    Instruction(byte opcode, wstring_view, bool illegal, LabelAccess labelAccess, OperandMode mode);

    byte GetOpcode() const;
    wstring_view GetName() const;
    unsigned short GetLength() const;
    LabelAccess GetLabelAccess() const;
    OperandMode GetOperandMode() const;
    bool IsImmediateMode() const;
    bool IsUnsupportedInstruction() const;

private:
    const byte opcode;
    wstring name;
    const bool illegal;
    const LabelAccess labelAccess;
    const OperandMode mode;
};

class InstructionSet // abstract
{
public:
    InstructionSet(wstring_view name, std::array<Instruction, 256> instructionArray);

    InstructionSet() = delete;									// default constructor
    InstructionSet(const InstructionSet&) = delete;				// copy constructor		
    InstructionSet& operator=(const InstructionSet&) = delete;	// copy assignment
    InstructionSet(InstructionSet&&) = delete;                  // move constructor
    InstructionSet& operator=(InstructionSet&&) = delete;       // move assignment
    virtual ~InstructionSet() = default;

    const std::array<Instruction, 256>& GetInstructions() const;
    const Instruction& GetInstruction(byte opcode) const;
    const wstring& GetName() const;
    bool IsDualAddressingMode(byte opcode) const;

private:
    wstring name;
    std::array<Instruction, 256> instructionArray;
};

class InstructionSetMOS6502 final : public InstructionSet
{
public:
    InstructionSetMOS6502(wstring_view name);
    ~InstructionSetMOS6502() = default;

};

class InstructionSetMOS65C02 final : public InstructionSet
{
public:
    InstructionSetMOS65C02(wstring_view name);
    ~InstructionSetMOS65C02() = default;

};
