#include <cstdio>
#include <cassert>

#include "ui_frontend/MiniAssembler.h"
#include "Application.h"
#include "Workspace.h"
#include "systems/ComputerSystemFactory.h"
#include "WorkspaceLogic.h"
#include "String.h"
#include "ByteArray.h"

int main() {
    ComputerSystemFactory computerSystemFactory;
    Workspace workspace(computerSystemFactory);
    workspace.SetComputerSystemType(ComputerSystemType::ATARI800);

    // Give ourselves a plain raw segment to assemble into - simplest way
    // to get a real, writable Segment without needing a full file load.
    WorkspaceLogic workspaceLogic;
    ByteArray buffer(16);
    workspaceLogic.AddRawSegment(workspace, buffer, 0, 16, 0x0600);

    auto segment = workspace.GetSegmentList()->GetSegment(0);
    Memory::address baseAddress = segment->wBegin;
    std::printf("Segment base address: $%04X, size %zu\n", baseAddress, (size_t)segment->GetSize());

    int failed = 0;
    auto check = [&](const char* desc, bool condition) {
        std::printf("%s %s\n", condition ? "PASS" : "FAIL", desc);
        if (!condition) failed++;
    };

    // LDA #$05 -> opcode 0xA9, operand 0x05 (Immediate mode, 2 bytes)
    {
        auto result = MiniAssembler::Assemble(L"LDA #$05", *segment, 0, baseAddress, workspace);
        std::printf("  message: %s\n", String::wstring_to_utf8(result.message).c_str());
        check("LDA #$05 assembles successfully", result.success);
        check("LDA #$05 writes 2 bytes", result.bytesWritten == 2);
        check("LDA #$05 opcode is 0xA9", segment->GetData(0) == 0xA9);
        check("LDA #$05 operand is 0x05", segment->GetData(1) == 0x05);
    }

    // STA $0600 -> opcode 0x8D, operand $00 $06 (Absolute mode, 3 bytes)
    // (address 0x0600 == the segment's own base address, chosen so this
    // is a realistic self-referential store)
    {
        auto result = MiniAssembler::Assemble(L"STA $0600", *segment, 2, baseAddress + 2, workspace);
        std::printf("  message: %s\n", String::wstring_to_utf8(result.message).c_str());
        check("STA $0600 assembles successfully", result.success);
        check("STA $0600 writes 3 bytes", result.bytesWritten == 3);
        check("STA $0600 opcode is 0x8D", segment->GetData(2) == 0x8D);
        check("STA $0600 low byte is 0x00", segment->GetData(3) == 0x00);
        check("STA $0600 high byte is 0x06", segment->GetData(4) == 0x06);
    }

    // NOP (Implied mode, 1 byte) -> opcode 0xEA
    {
        auto result = MiniAssembler::Assemble(L"NOP", *segment, 5, baseAddress + 5, workspace);
        check("NOP assembles successfully", result.success);
        check("NOP writes 1 byte", result.bytesWritten == 1);
        check("NOP opcode is 0xEA", segment->GetData(5) == 0xEA);
    }

    // Unknown instruction -> should fail with an error, not crash
    {
        auto result = MiniAssembler::Assemble(L"NOTANOPCODE", *segment, 6, baseAddress + 6, workspace);
        std::printf("  message: %s\n", String::wstring_to_utf8(result.message).c_str());
        check("Unknown instruction fails cleanly", !result.success);
    }

    std::printf("\n%s\n", failed == 0 ? "ALL ASSEMBLER CHECKS PASSED" : "SOME ASSEMBLER CHECKS FAILED");
    return failed == 0 ? 0 : 1;
}
