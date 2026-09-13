#include "Application.h"
#include "AssembleDialog.h"
#include "Assembler.h"
#include "Dialog.h"
#include "EditControl.h"
#include "InstructionSet.h"
#include "Memory.h"
#include "MemoryInspectorControl.h"
#include "Segment.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "Text.h"
#include "Word.h"
#include "Workspace.h"
#include <cwchar>

AssembleDialog::AssembleDialog(Window& parentWindow) : Dialog(parentWindow, L"ASSEMBLEBOX") {
    segment = nullptr;
    memoryInspectorControl = nullptr;
}

bool AssembleDialog::Show(Workspace& workspace, Segment& segment, MemoryInspectorControl& memoryInspectorControl) {
    this->workspace = &workspace;
    this->segment = &segment;
    this->memoryInspectorControl = &memoryInspectorControl;

    const bool bResult = ShowDialogBox();

    this->segment = nullptr;
    this->memoryInspectorControl = nullptr;

    return bResult;
}

bool AssembleDialog::InitDialog() {
    wchar_t szBuf[64]{};
    szBuf[0] = 0;
    SetItemTextAndSize(IDC_INSTRUCTION, szBuf);

    Memory::offset beginOffset{};
    Memory::offset endOffset{};
    memoryInspectorControl->GetNonEmptySelection(beginOffset, endOffset);

    const Memory::address address = beginOffset + segment->wBegin;

    wchar_t szAddress[6]{}; // With $
    String::Printf(L"$%04hX", address);

    SetItemTextAndSize(IDC_ASMADDR, szAddress);
    return true;
}


bool AssembleDialog::OnOK() {

    Memory::offset beginOffset{};
    Memory::offset endOffset{};
    memoryInspectorControl->GetNonEmptySelection(beginOffset, endOffset);

    Memory::offset offset = beginOffset;
    Memory::address address = segment->wBegin + offset;

    auto line = GetEditControl(IDC_INSTRUCTION).GetText();


    Assembler::PInstruction instruction;
    word value = 0;
    wstring comment = L"";
    wstring error = L"";
    Assembler::ParseLine(*workspace, line, address, segment->processorType, instruction, value, comment, error);

    wstring result;
    if (error.empty()) {

        if (instruction != nullptr) {
            auto instructionlength = instruction->GetLength();

            if (offset + instructionlength > segment->GetSize()) {
                error = L"ERROR: Instruction does not fit in segment";
            }
            else {
                const auto opcode = instruction->GetOpcode();
                segment->SetData(offset, opcode);
                switch (instructionlength) {
                case 1:
                    result = String::Printf(L"$%02hX ; %s", opcode, comment.c_str());
                    break;

                case 2:
                    segment->SetData(offset + 1, (byte)(value & 0xFF));
                    result = String::Printf(L"$%02hX $%02hX ; %s", opcode, Memory::to_low_byte(value), comment.c_str());
                    break;

                case 3:
                    segment->SetData(offset + 1, (byte)(value & 0xFF));
                    segment->SetData(offset + 2, (byte)((value >> 8) & 0xFF));
                    result = String::Printf(L"$%02hX $%02hX $%02hX ; %s", opcode, Memory::to_low_byte(value), Memory::to_high_byte(value), comment.c_str());
                    break;
                }

                offset += instructionlength;
                beginOffset = endOffset = offset;
                memoryInspectorControl->SetSelection(beginOffset, endOffset);

                GetEditControl(IDC_INSTRUCTION).SetText(L"");

                address += instructionlength;
                GetEditControl(IDC_ASMADDR).SetAddress(address);
            }
        }

        else {
            result = L"ERROR: Unknown instruction or addressing mode.";
        }
    }
    else {
        result = error;
    }

    result = error;
    wchar_t szResult[1024]{};
    wcsncpy(szResult, result.c_str(), result.size());
    SetItemTextAndSize(ID_RESULT, szResult);

    return EndDialogBox(TRUE);
}

bool AssembleDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    switch (command) {
    case IDCANCEL:
        return OnCancel();

    case IDOK:
        return OnOK();
    }
    return false;
}