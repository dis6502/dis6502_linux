#include "Application.h"
#include "Equate.h"
#include "Character.h"
#include "Workspace.h"
#include "Memory.h"
#include "MemoryInspectorControl.h"
#include "InstructionSet.h"
#include "EditControl.h"
#include "AssembleDialog.h"
#include "String.h"

extern std::unique_ptr<Workspace> g_Workspace;


AssembleDialog::AssembleDialog(Window& parentWindow) : Dialog(parentWindow, L"ASSEMBLEBOX") {
    segment = nullptr;
    memoryInspectorControl = nullptr;
}

bool AssembleDialog::Show(Segment* segment, MemoryInspectorControl* memoryInspectorControl) {
    this->segment = segment;
    this->memoryInspectorControl = memoryInspectorControl;

    const bool bResult = ShowDialogBox();

    this->segment = nullptr;
    this->memoryInspectorControl = nullptr;

    return bResult;
}

LPCWSTR AssembleDialog::GetExpressionValue(LPCWSTR ptr, WORD* value, wstring& error) {
    wchar_t szBuf[64] = { 0 };
    INT index = 0;
    WORD val2 = 0;

    *value = 0;
    while (*ptr == ' ')
        ptr++;

    if (*ptr == '<') {
        ptr = GetExpressionValue(ptr + 1, value, error);
        *value = *value & 0xFF;
    }
    else if (*ptr == '>') {
        ptr = GetExpressionValue(ptr + 1, value, error);
        *value = (*value >> 8) & 0xFF;
    }
    else if ((Character::IsAlpha(*ptr)) || (*ptr == '_') || (*ptr == '@')) {
        szBuf[index++] = *ptr++;

        while ((index < 50) && ((Character::IsAlphaNumeric(*ptr)) || (*ptr == '_') || (*ptr == '@')))
            szBuf[index++] = *ptr++;
        szBuf[index++] = '\0';

        _wcsupr(szBuf);
        const auto equate = ::g_Workspace->GetEquateByLabel(szBuf);
        if (equate != nullptr) {
            *value = equate->GetLabelValue();
        }
        else {
            error = L"ERROR: Unknown label";
        }
    }
    else if (*ptr == '$') {
        ptr++;

        while ((index < 4) && ((Character::IsDigit(*ptr)) || ((*ptr >= 'a') && (*ptr <= 'f')) || ((*ptr >= 'A') && (*ptr <= 'F'))))
            szBuf[index++] = *ptr++;
        szBuf[index++] = '\0';

        swscanf(szBuf, L"%04hX", value);
    }
    else {
        while ((index < 5) && (Character::IsDigit(*ptr)))
            szBuf[index++] = *ptr++;
        szBuf[index++] = '\0';

        swscanf(szBuf, L"%hu", value);
    }

    while (*ptr == ' ')
        ptr++;

    if (*ptr == '+') {
        ptr = GetExpressionValue(ptr + 1, &val2, error);
        *value += val2;
    }
    else if (*ptr == '-') {
        ptr = GetExpressionValue(ptr + 1, &val2, error);
        *value -= val2;
    }
    else if (*ptr == '&') {
        ptr = GetExpressionValue(ptr + 1, &val2, error);
        *value &= val2;
    }
    else if (*ptr == '|') {
        ptr = GetExpressionValue(ptr + 1, &val2, error);
        *value &= val2;
    }

    return ptr;
}

bool AssembleDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    wchar_t szAddress[6]{}; // With $
    wchar_t szInst[4]{};
    wchar_t szResult[128]{};
    wstring error;
    OperandMode mode{};
    LPCWSTR ptr{};
    char sign{};
    int index{};
    WORD value{};
    WORD wLen{};
    Memory::offset nBegin{};
    Memory::offset nEnd{};
    Memory::offset nFirst{};
    WORD wAddress{};

    switch (message) {
    case WM_INITDIALOG: {
        wchar_t szBuf[64]{};
        szBuf[0] = 0;
        SetItemTextAndSize(IDC_INSTRUCTION, szBuf);

        memoryInspectorControl->GetNonEmptySelection(nBegin, nEnd);

        nFirst = nBegin;
        wAddress = nFirst + segment->wBegin;
        wsprintf(szAddress, L"$%04hX", wAddress);

        SetItemTextAndSize(IDC_ASMADDR, szAddress);

        szResult[0] = 0;

        return true;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDCANCEL:
            return EndDialogBox(FALSE);

        case IDOK:
            error = L"";

            memoryInspectorControl->GetNonEmptySelection(nBegin, nEnd);

            nFirst = nBegin;
            wAddress = nFirst + segment->wBegin;

            auto instruction = GetEditControl(IDC_INSTRUCTION).GetText();
            auto szBuf = instruction.c_str(); // TODO: Use wstring_view instead of wchar_z*

            ptr = szBuf;
            while (*ptr == ' ') {
                ptr++;
            }

            for (index = 0; (index < 3) && (*ptr != 0); index++) {
                szInst[index] = *ptr++;
            }
            szInst[3] = '\0';

            while (*ptr == ' ') {
                ptr++;
            }

            if (*ptr == '\0') {
                mode = OperandMode::Implied;
            }
            else {
                if (*ptr == '#') {
                    mode = OperandMode::Immediate;
                    ptr = GetExpressionValue(ptr + 1, &value, error);
                }
                else if (((*ptr == 'a') || (*ptr == 'A')) && ((*(ptr + 1) == ' ') || (*(ptr + 1) == '\0'))) {
                    mode = OperandMode::Accumulator;
                    ptr++;

                    while (*ptr == ' ')
                        ptr++;

                    if (*ptr != '\0')
                        error = L"ERROR: garbage at end of line";
                }
                else if (*ptr == '(') {
                    ptr = GetExpressionValue(ptr + 1, &value, error);
                    if (*ptr == ')') {
                        ptr++;

                        while (*ptr == ' ')
                            ptr++;

                        if (*ptr == '\0')
                            mode = OperandMode::Indirect;
                        else if (*ptr == ',') {
                            ptr++;

                            if ((*ptr == 'Y') || (*ptr == 'y')) {
                                mode = OperandMode::IndirectIndexed;
                                ptr++;

                                while (*ptr == ' ')
                                    ptr++;

                                if (*ptr != '\0')
                                    error = L"ERROR: garbage at end of line";
                            }
                        }
                    }
                    else if (*ptr == ',') {
                        ptr++;

                        while (*ptr == ' ')
                            ptr++;

                        if ((*ptr == 'X') || (*ptr == 'x')) {
                            ptr++;

                            while (*ptr == ' ') {
                                ptr++;
                            }

                            if (*ptr == ')') {
                                mode = OperandMode::IndexedIndirect;
                                ptr++;

                                while (*ptr == ' ') {
                                    ptr++;
                                }

                                if (*ptr != '\0') {
                                    error = L"ERROR: garbage at end of line";
                                }
                            }
                            else {
                                error = L"ERROR: missing ')'";
                            }
                        }
                    }
                }
                else if ((*ptr == '$') || (Character::IsAlphaNumeric(*ptr))) {
                    ptr = GetExpressionValue(ptr, &value, error);

                    if (*ptr == '\0') {
                        if ((!_wcsicmp(szInst, L"BEQ")) || (!_wcsicmp(szInst, L"BNE"))
                            || (!_wcsicmp(szInst, L"BCS")) || (!_wcsicmp(szInst, L"BCC"))
                            || (!_wcsicmp(szInst, L"BPL")) || (!_wcsicmp(szInst, L"BMI"))
                            || (!_wcsicmp(szInst, L"BVC")) || (!_wcsicmp(szInst, L"BVS")))
                        {
                            mode = OperandMode::Relative;

                            index = (int)(DWORD)value - (int)(DWORD)wAddress - 2;
                            if ((index < -127) || (index > 127)) {
                                error = L"ERROR: address out of range";
                            }
                            else {
                                value = (WORD)index;
                            }
                        }
                        else {
                            if (value < 256) {
                                mode = OperandMode::ZeroPage;
                            }
                            else {
                                mode = OperandMode::Absolute;
                            }
                        }
                    }
                    else if (*ptr == ',') {
                        ptr++;

                        if ((*ptr == 'X') || (*ptr == 'x')) {
                            ptr++;

                            while (*ptr == ' ') {
                                ptr++;
                            }

                            if (value < 256) {
                                mode = OperandMode::ZeroPageX;
                            }
                            else {
                                mode = OperandMode::AbsoluteX;
                            }

                            if (*ptr != '\0') {
                                error = L"ERROR: garbage at end of line";
                            }
                        }
                        else if ((*ptr == 'Y') || (*ptr == 'y')) {
                            ptr++;

                            while (*ptr == ' ') {
                                ptr++;
                            }

                            if ((value < 256) && (_wcsicmp(szInst, L"LDA"))) {
                                mode = OperandMode::ZeroPageY;
                            }
                            else {
                                mode = OperandMode::AbsoluteY;
                            }

                            if (*ptr != '\0') {
                                error = L"ERROR: garbage at end of line";
                            }
                        }
                        else {
                            error = L"ERROR: X or Y expected";
                        }
                    }
                    else {
                        error = L"ERROR: bad character after address";
                    }
                }
                else if ((*ptr == '+') || (*ptr == '-')) {
                    sign = *ptr;

                    ptr = GetExpressionValue(ptr + 1, &value, error);
                    if (*ptr == '\0') {
                        mode = OperandMode::Relative;

                        if (sign == '-') {
                            value = 0 - value;
                        }
                    }
                    else {
                        error = L"ERROR: garbage at end of line";
                    }
                }
            }

            if (error.empty()) {
                error = L"ERROR: unknown instruction or addr mode";
                const auto& instructionSet = g_Workspace->GetInstructionSet(segment->processorType);
                for (const auto& instruction : instructionSet->GetInstructions()) {
                    wstring instructionName = wstring(instruction.GetName());
                    if ((!_wcsicmp(instructionName.c_str(), szInst)) && (instruction.GetOperandMode() == mode) && (!instruction.IsUnsupportedInstruction())) {
                        wLen = instruction.GetLength();

                        if (nFirst + wLen > segment->GetSize()) {
                            error = L"ERROR: instruction does not fit in segment";
                        }
                        else {
                            const auto opcode = instruction.GetOpcode();
                            segment->SetData(nFirst, opcode);

                            switch (wLen) {
                            case 1:
                                wsprintf(szResult, L"$%02hX ; %s", opcode, szBuf);
                                error = szResult;
                                break;

                            case 2:
                                segment->SetData(nFirst + 1, (byte)(value & 0xFF));
                                wsprintf(szResult, L"$%02hX $%02hX ; %s", opcode, Memory::to_low_byte(value), szBuf);
                                error = szResult;
                                break;

                            case 3:
                                segment->SetData(nFirst + 1, (byte)(value & 0xFF));
                                segment->SetData(nFirst + 2, (byte)((value >> 8) & 0xFF));
                                wsprintf(szResult, L"$%02hX $%02hX $%02hX ; %s", opcode, Memory::to_low_byte(value), Memory::to_high_byte(value), szBuf);
                                error = szResult;
                                break;
                            }

                            nFirst += wLen;
                            nBegin = nEnd = nFirst;
                            memoryInspectorControl->SetSelection(nBegin, nEnd);

                            GetEditControl(IDC_INSTRUCTION).SetText(L"");

                            wAddress += wLen;
                            GetEditControl(IDC_ASMADDR).SetAddress(wAddress);
                        }
                        break;
                    }
                }
            }

            wchar_t szError[1024];
            wcsncpy(szError, error.c_str(), error.size());
            SetItemTextAndSize(ID_RESULT, szError);

            return EndDialogBox(TRUE);
        }
        break;
    }
    }

    return false;
}
