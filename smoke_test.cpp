// Smoke test for the ported dis6502 core engine on Linux. Not part of
// the official test suite (that's TestUnit/Assert-based and expects
// fixture files under tst/suite/) - this just proves real engine code
// executes correctly after the port, not merely that it compiles.
#include <cassert>
#include <cstdio>
#include <cwchar>

#include "InstructionSet.h"
#include "Strings.h"

int main() {
    InstructionSetMOS6502 instructionSet(L"MOS 6502");

    // Opcode 0xA9 is "LDA #immediate" on the 6502 - a fact that hasn't
    // changed since 1975, so if this doesn't come back right, the port
    // broke something.
    const Instruction& instr = instructionSet.GetInstruction(0xA9);

    std::wstring name(instr.GetName());
    std::printf("Opcode 0xA9 decoded as: %s (length %d bytes)\n",
        String::wstring_to_utf8(name).c_str(), instr.GetLength());

    assert(name == L"LDA");
    assert(instr.GetLength() == 2);

    // Also opcode 0x00 = BRK, length 1, and 0xEA = NOP.
    assert(std::wstring(instructionSet.GetInstruction(0x00).GetName()) == L"BRK");
    assert(std::wstring(instructionSet.GetInstruction(0xEA).GetName()) == L"NOP");

    std::printf("Core engine smoke test PASSED on Linux.\n");
    return 0;
}
