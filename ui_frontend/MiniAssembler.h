#pragma once

#include <string>
#include <cwctype>
#include <cwchar>
#include <cstdlib>

#include "Syntax.h"
#include "PlatformCompat.h"
#include "Segment.h"
#include "InstructionSet.h"
#include "Workspace.h"
#include "Character.h"
#include "Memory.h"

// Ported from AssembleDialog.cpp (ui_todo/) - a small interactive
// assembler: given one line of 6502 assembly text and a position in a
// segment, figures out the addressing mode, finds the matching
// Instruction, and writes the resulting opcode+operand bytes directly
// into the segment. The parsing/addressing-mode-detection logic here
// has no UI dependency in the original either (it doesn't touch
// Window/Dialog/EditControl types beyond reading the input text and
// writing the result text) - the actual Windows-specific bits were
// just `_wcsupr`, `swscanf`, and `wsprintf`, all replaced below with
// portable equivalents. Behavior is otherwise unchanged from the
// original, including its quirks (e.g. the same `&` and `|` operators
// both perform bitwise AND - that's in the original source, not a typo
// introduced here).
namespace MiniAssembler {

struct Result {
    bool success = false;
    wstring message;              // "$XX $YY ; text" on success, "ERROR: ..." on failure
    Memory::size bytesWritten = 0; // 0 on failure
};

namespace detail {

    // Ported from AssembleDialog::GetExpressionValue. Recursive-descent
    // parser for operand expressions: <expr / >expr (low/high byte),
    // labels (via Workspace::GetEquateByLabel), $hex, decimal, and the
    // +,-,&,| binary operators. Returns the remaining unparsed text.
    inline const wchar_t* ParseExpressionValue(const wchar_t* ptr, Workspace& workspace, uint16_t& value, wstring& error) {
        wchar_t buf[64] = { 0 };
        int index = 0;
        uint16_t value2 = 0;

        value = 0;
        while (*ptr == L' ') ptr++;

        if (*ptr == L'<') {
            ptr = ParseExpressionValue(ptr + 1, workspace, value, error);
            value = value & 0xFF;
        } else if (*ptr == L'>') {
            ptr = ParseExpressionValue(ptr + 1, workspace, value, error);
            value = (value >> 8) & 0xFF;
        } else if (Character::IsAlpha(*ptr) || *ptr == L'_' || *ptr == L'@') {
            buf[index++] = *ptr++;
            while (index < 50 && (Character::IsAlphaNumeric(*ptr) || *ptr == L'_' || *ptr == L'@')) {
                buf[index++] = *ptr++;
            }
            buf[index] = L'\0';
            for (int i = 0; buf[i]; i++) buf[i] = static_cast<wchar_t>(towupper(buf[i]));

            const auto* equate = workspace.GetEquateByLabel(buf);
            if (equate != nullptr) {
                value = equate->GetLabelValue();
            } else {
                error = L"ERROR: Unknown label";
            }
        } else if (*ptr == L'$') {
            ptr++;
            while (index < 4 && (Character::IsDigit(*ptr) || (*ptr >= L'a' && *ptr <= L'f') || (*ptr >= L'A' && *ptr <= L'F'))) {
                buf[index++] = *ptr++;
            }
            buf[index] = L'\0';
            value = static_cast<uint16_t>(wcstoul(buf, nullptr, 16));
        } else {
            while (index < 5 && Character::IsDigit(*ptr)) {
                buf[index++] = *ptr++;
            }
            buf[index] = L'\0';
            value = static_cast<uint16_t>(wcstoul(buf, nullptr, 10));
        }

        while (*ptr == L' ') ptr++;

        if (*ptr == L'+') {
            ptr = ParseExpressionValue(ptr + 1, workspace, value2, error);
            value += value2;
        } else if (*ptr == L'-') {
            ptr = ParseExpressionValue(ptr + 1, workspace, value2, error);
            value -= value2;
        } else if (*ptr == L'&') {
            ptr = ParseExpressionValue(ptr + 1, workspace, value2, error);
            value &= value2;
        } else if (*ptr == L'|') {
            ptr = ParseExpressionValue(ptr + 1, workspace, value2, error);
            value &= value2; // matches the original exactly - '|' performs AND there too
        }

        return ptr;
    }

} // namespace detail

// currentAddress is the memory address the instruction will be written
// at (segment.wBegin + segmentOffset) - needed for computing relative
// branch offsets.
inline Result Assemble(wstring_view instructionText, Segment& segment, Memory::offset segmentOffset,
                        Memory::address currentAddress, Workspace& workspace) {
    Result result;
    wstring error;
    OperandMode mode{};
    uint16_t value = 0;
    wchar_t instName[4] = {};

    wstring text(instructionText);
    const wchar_t* ptr = text.c_str();

    while (*ptr == L' ') ptr++;
    int index = 0;
    for (; index < 3 && *ptr != 0; index++) {
        instName[index] = *ptr++;
    }
    instName[3] = L'\0';
    while (*ptr == L' ') ptr++;

    if (*ptr == L'\0') {
        mode = OperandMode::Implied;
    } else if (*ptr == L'#') {
        mode = OperandMode::Immediate;
        ptr = detail::ParseExpressionValue(ptr + 1, workspace, value, error);
    } else if ((*ptr == L'a' || *ptr == L'A') && (*(ptr + 1) == L' ' || *(ptr + 1) == L'\0')) {
        mode = OperandMode::Accumulator;
        ptr++;
        while (*ptr == L' ') ptr++;
        if (*ptr != L'\0') error = L"ERROR: garbage at end of line";
    } else if (*ptr == L'(') {
        ptr = detail::ParseExpressionValue(ptr + 1, workspace, value, error);
        if (*ptr == L')') {
            ptr++;
            while (*ptr == L' ') ptr++;
            if (*ptr == L'\0') {
                mode = OperandMode::Indirect;
            } else if (*ptr == L',') {
                ptr++;
                if (*ptr == L'Y' || *ptr == L'y') {
                    mode = OperandMode::IndirectIndexed;
                    ptr++;
                    while (*ptr == L' ') ptr++;
                    if (*ptr != L'\0') error = L"ERROR: garbage at end of line";
                }
            }
        } else if (*ptr == L',') {
            ptr++;
            while (*ptr == L' ') ptr++;
            if (*ptr == L'X' || *ptr == L'x') {
                ptr++;
                while (*ptr == L' ') ptr++;
                if (*ptr == L')') {
                    mode = OperandMode::IndexedIndirect;
                    ptr++;
                    while (*ptr == L' ') ptr++;
                    if (*ptr != L'\0') error = L"ERROR: garbage at end of line";
                } else {
                    error = L"ERROR: missing ')'";
                }
            }
        }
    } else if (*ptr == L'$' || Character::IsAlphaNumeric(*ptr)) {
        ptr = detail::ParseExpressionValue(ptr, workspace, value, error);

        if (*ptr == L'\0') {
            if (!_wcsicmp(instName, L"BEQ") || !_wcsicmp(instName, L"BNE") ||
                !_wcsicmp(instName, L"BCS") || !_wcsicmp(instName, L"BCC") ||
                !_wcsicmp(instName, L"BPL") || !_wcsicmp(instName, L"BMI") ||
                !_wcsicmp(instName, L"BVC") || !_wcsicmp(instName, L"BVS")) {
                mode = OperandMode::Relative;
                int offset = static_cast<int>(value) - static_cast<int>(currentAddress) - 2;
                if (offset < -127 || offset > 127) {
                    error = L"ERROR: address out of range";
                } else {
                    value = static_cast<uint16_t>(offset);
                }
            } else {
                mode = (value < 256) ? OperandMode::ZeroPage : OperandMode::Absolute;
            }
        } else if (*ptr == L',') {
            ptr++;
            if (*ptr == L'X' || *ptr == L'x') {
                ptr++;
                while (*ptr == L' ') ptr++;
                mode = (value < 256) ? OperandMode::ZeroPageX : OperandMode::AbsoluteX;
                if (*ptr != L'\0') error = L"ERROR: garbage at end of line";
            } else if (*ptr == L'Y' || *ptr == L'y') {
                ptr++;
                while (*ptr == L' ') ptr++;
                mode = (value < 256 && _wcsicmp(instName, L"LDA")) ? OperandMode::ZeroPageY : OperandMode::AbsoluteY;
                if (*ptr != L'\0') error = L"ERROR: garbage at end of line";
            } else {
                error = L"ERROR: X or Y expected";
            }
        } else {
            error = L"ERROR: bad character after address";
        }
    } else if (*ptr == L'+' || *ptr == L'-') {
        wchar_t sign = *ptr;
        ptr = detail::ParseExpressionValue(ptr + 1, workspace, value, error);
        if (*ptr == L'\0') {
            mode = OperandMode::Relative;
            if (sign == L'-') value = static_cast<uint16_t>(0 - value);
        } else {
            error = L"ERROR: garbage at end of line";
        }
    }

    if (!error.empty()) {
        result.message = error;
        return result;
    }

    error = L"ERROR: unknown instruction or addr mode";
    const auto& instructionSet = workspace.GetInstructionSet(segment.processorType);
    for (const auto& instruction : instructionSet->GetInstructions()) {
        wstring instructionName(instruction.GetName());
        if (!_wcsicmp(instructionName.c_str(), instName) &&
            instruction.GetOperandMode() == mode &&
            !instruction.IsUnsupportedInstruction()) {

            unsigned short length = instruction.GetLength();
            if (segmentOffset + length > segment.GetSize()) {
                error = L"ERROR: instruction does not fit in segment";
                break;
            }

            byte opcode = instruction.GetOpcode();
            segment.SetData(segmentOffset, opcode);

            wchar_t resultText[128];
            switch (length) {
                case 1:
                    swprintf(resultText, 128, L"$%02hX ; %.*ls", opcode, static_cast<int>(text.size()), text.c_str());
                    break;
                case 2:
                    segment.SetData(segmentOffset + 1, static_cast<byte>(value & 0xFF));
                    swprintf(resultText, 128, L"$%02hX $%02hX ; %.*ls", opcode, Memory::to_low_byte(value), static_cast<int>(text.size()), text.c_str());
                    break;
                case 3:
                    segment.SetData(segmentOffset + 1, static_cast<byte>(value & 0xFF));
                    segment.SetData(segmentOffset + 2, static_cast<byte>((value >> 8) & 0xFF));
                    swprintf(resultText, 128, L"$%02hX $%02hX $%02hX ; %.*ls", opcode, Memory::to_low_byte(value), Memory::to_high_byte(value), static_cast<int>(text.size()), text.c_str());
                    break;
                default:
                    resultText[0] = L'\0';
                    break;
            }

            result.success = true;
            result.message = resultText;
            result.bytesWritten = length;
            return result;
        }
    }

    result.message = error;
    return result;
}

} // namespace MiniAssembler
