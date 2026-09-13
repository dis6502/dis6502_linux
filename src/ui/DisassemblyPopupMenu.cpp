#include "DisassemblyLine.h"
#include "DisassemblyPopupMenu.h"
#include "Equate.h"
#include "MemoryType.h"
#include "PopupMenu.h"
#include "Resource.h"
#include "SegmentTypes.h"
#include "Syntax.h"
#include "Text.h"

DisassemblyPopupMenu::DisassemblyPopupMenu() : PopupMenu(L"DISASSEMBLY_POPUP_MENU") {}

void DisassemblyPopupMenu::Update(bool canFindNext, wstring_view labelDefinition, wstring_view labelReference, size_t historyIndex, const DIS_LINE* lpLine, bool instructionHasImmediateOperand, bool charAllowed, MemoryType immediateMemoryType)
{
    bool isLabelDefinition = false;
    bool isLabelReference = false;

    wstring findDef;
    wstring findRef1;
    wstring findRef2;
    wstring renDef;
    wstring renRef;
    wstring addrRangeDef;
    wstring addrRangeRef;
    int deleteCount = 0;

    if (!labelDefinition.empty()) {
        findRef2 = Text::Format(IDS_DIS_POPUP_MENU_FIND_REF2, labelDefinition);
        renDef = Text::Format(IDS_DIS_POPUP_MENU_REN_DEF, labelDefinition);
        addrRangeDef = Text::Format(IDS_DIS_POPUP_MENU_ADDR_RANGE_DEF, labelDefinition);

        isLabelDefinition = true;
    }

    if (!labelReference.empty()) {
        findDef = Text::Format(IDS_DIS_POPUP_MENU_FIND_DEF, labelReference);
        findRef1 = Text::Format(IDS_DIS_POPUP_MENU_FIND_REF1, labelReference);
        renRef = Text::Format(IDS_DIS_POPUP_MENU_REN_REF, labelReference);
        addrRangeRef = Text::Format(IDS_DIS_POPUP_MENU_ADDR_RANGE_REF, labelReference);

        isLabelReference = true;
    }

    if (isLabelReference) {
        SetText(IDM_DIS_FIND_DEF, findDef);
        SetEnabled(IDM_DIS_FIND_DEF, true);
    }
    else {
        DeleteEntry(IDM_DIS_FIND_DEF);
        deleteCount++;
    }

    SetEnabled(IDM_DIS_BACK_IN_HISTORY, (historyIndex > 0));
    SetEnabled(IDM_DIS_COMMENT, ((lpLine != nullptr) && (lpLine->segmentIndex != NO_SEGMENT_INDEX)));

    SetEnabled(IDM_DIS_FIND, true);
    SetEnabled(IDM_DIS_FIND_NEXT, canFindNext);

    if (isLabelDefinition) {
        SetText(IDM_DIS_FIND_REF2, findRef2);
        SetEnabled(IDM_DIS_FIND_REF2, true);
    }
    else {
        DeleteEntry(IDM_DIS_FIND_REF2);
        deleteCount++;
    }

    if (isLabelReference) {
        SetText(IDM_DIS_FIND_REF1, findRef1);
        SetEnabled(IDM_DIS_FIND_REF1, true);

    }
    else {
        DeleteEntry(IDM_DIS_FIND_REF1);
        deleteCount++;
    }


    const auto labelDefinitionAutomatic = Equate::IsAutomaticLabel(labelDefinition);
    const auto labelReferenceAutomatic = Equate::IsAutomaticLabel(labelReference);
    if (!(labelDefinitionAutomatic || labelReferenceAutomatic)) {
        DeleteSepartor(9 - deleteCount);
        deleteCount++;
    }

    if (labelDefinitionAutomatic) {
        SetText(IDM_DIS_RENAME_DEF, renDef);
        SetEnabled(IDM_DIS_RENAME_DEF, true);
    }
    else {
        DeleteEntry(IDM_DIS_RENAME_DEF);
    }

    if ((labelDefinition != labelReference) && labelReferenceAutomatic) {
        SetText(IDM_DIS_RENAME_REF, renRef);
        SetEnabled(IDM_DIS_RENAME_REF, true);
    }
    else {
        DeleteEntry(IDM_DIS_RENAME_REF);
    }

    if (!(labelDefinitionAutomatic || labelReferenceAutomatic)) {
        DeleteSepartor(12 - deleteCount);
        deleteCount++;
    }

    if (isLabelDefinition) {
        SetText(IDM_DIS_ADDR_RANGE_DEF, addrRangeDef);
        SetEnabled(IDM_DIS_ADDR_RANGE_DEF, true);
    }
    else {
        DeleteEntry(IDM_DIS_ADDR_RANGE_DEF);
    }

    if (isLabelReference) {
        SetText(IDM_DIS_ADDR_RANGE_REF, addrRangeRef);
        SetEnabled(IDM_DIS_ADDR_RANGE_REF, true);
    }
    else {
        DeleteEntry(IDM_DIS_ADDR_RANGE_REF);
    }

    SetEnabled(IDM_DIS_SET_TYPE_CODE, instructionHasImmediateOperand);
    SetEnabled(IDM_DIS_SET_TYPE_LOBYTE, instructionHasImmediateOperand);
    SetEnabled(IDM_DIS_SET_TYPE_HIBYTE, instructionHasImmediateOperand);
    SetEnabled(IDM_DIS_SET_TYPE_STRING, instructionHasImmediateOperand && charAllowed);
    SetEnabled(IDM_DIS_SET_TYPE_UNKNOWN, instructionHasImmediateOperand);

    if (instructionHasImmediateOperand) {
        SetChecked(IDM_DIS_SET_TYPE_CODE, immediateMemoryType == MemoryType::CODE);
        SetChecked(IDM_DIS_SET_TYPE_LOBYTE, immediateMemoryType == MemoryType::LOBYTE);
        SetChecked(IDM_DIS_SET_TYPE_HIBYTE, immediateMemoryType == MemoryType::HIBYTE);
        SetChecked(IDM_DIS_SET_TYPE_STRING, immediateMemoryType == MemoryType::STRING);
        SetChecked(IDM_DIS_SET_TYPE_UNKNOWN, immediateMemoryType == MemoryType::UNKNOWN);
    }
}
