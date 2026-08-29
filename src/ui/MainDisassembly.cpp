#include "Application.h"
#include "CommentDialog.h"
#include "Dialog.h"
#include "Disassembly.h"
#include "DisassemblyLine.h" 
#include "DisassemblyPopupMenu.h"
#include "DisassemblyProgressDialog.h"
#include "DisassemblyProgressMonitor.h"
#include "DisassemblyResult.h"
#include "DisassemblyWriter.h"

#include "DisassemblyFindStringDialog.h"
#include "FindStringDialog.h"
#include "EquateListLogic.h"
#include "LowHighByteDialog.h"
#include "Main.h"
#include "MainWindow.h"
#include "MemoryInspector.h"
#include "MemoryInspectorSelection.h"
#include "resource.h"

#include "Segment.h"
#include "SegmentList.h"
#include "Workspace.h"
#include "SegmentListWindow.h"

#include "MainDisassembly.h"

extern HWND hDisWnd;

extern std::unique_ptr<Workspace> g_Workspace;
extern std::unique_ptr<MemoryInspector> g_MemoryInspector; // TODO Make explicit references
extern std::unique_ptr<EquateListLogic> g_EquateListLogic;

MainDisassembly::MainDisassembly(Main& main) : MainController(main) {
    disassemblyControl = nullptr;
    mainXRef = nullptr;
}

void MainDisassembly::SetControl() {
    disassemblyControl = mainWindow->disassemblyWindow->GetDisassemblyControl();
}

void MainDisassembly::SetMainXRef(MainXRef& mainXRef) {
    this->mainXRef = &mainXRef;
}


long MainDisassembly::Proc(HWND hWnd, WPARAM wParam, LPARAM lParam) {


    switch (HIWORD(wParam)) {
        // The user has changed the selection
    case DIS_SCROLL_CHANGED:
        break;

        // The cross reference must be updated.
    case DIS_XREF:
    {
        auto label = disassemblyControl->GetLabelReference();

        if (label.empty()) {
            label = disassemblyControl->GetLabelDefinition();
        }

        if (!label.empty()) {
            disassemblyControl->SelectAllLabelReferences(label);
        }

        RefreshXRef(label);
    }
    break;

    // User has clicked in the disassembly window.
    case DIS_LBUTTONDOWN:
    {
        const auto editMode = ::g_MemoryInspector->IsEditMode();
        const DIS_LINE* pDis = (DIS_LINE*)lParam;

        // TODO: Implemente click to edit comments
        if ((pDis->segmentIndex != NO_SEGMENT_INDEX) && (pDis->segmentIndex < ::g_Workspace->GetConstSegmentList()->GetCount()) && (!editMode)) {
            ::g_MemoryInspector->GetMemoryInspectorSelection()->SetSegmentIndex(pDis->segmentIndex);
            SendMessage(mainWindow->segmentListWindow->GetHWnd(), LB_SETCURSEL, ::g_MemoryInspector->GetMemoryInspectorSelection()->GetSegmentIndex(), 0L);
            ::g_MemoryInspector->SegmentChanged();
            InvalidateRect(mainWindow->GetHWnd(), nullptr, false);

            if (pDis->size != 0) {
                ::g_MemoryInspector->Select(pDis->offset, (WORD)(pDis->offset + pDis->size + -1));
            }
        }
    }
    break;

    // User has right-clicked in the window.
    case DIS_RBUTTONDOWN:
    {
        if (::g_MemoryInspector->IsEditMode()) {
            ::g_MemoryInspector->DrawMenu();
        }
        else {
            DrawMenu((DIS_LINE*)lParam);
        }
        break;
    }
    }

    return 0;
}
// TODO Make Private
static MemoryType decodeMsg(WORD wCommand)
{
    switch (wCommand) {
    case IDM_DIS_SET_TYPE_CODE:
        return MemoryType::CODE;
    case IDM_DIS_SET_TYPE_LOBYTE:
        return MemoryType::LOBYTE;
    case IDM_DIS_SET_TYPE_HIBYTE:
        return MemoryType::HIBYTE;
    case IDM_DIS_SET_TYPE_STRING:
        return MemoryType::STRING;
    case IDM_DIS_SET_TYPE_UNKNOWN:
        return MemoryType::UNKNOWN;
    }
    return MemoryType::UNKNOWN;
}
void MainDisassembly::PerformCommands(WORD wCommand, bool bEditMode) {
    switch (wCommand) {
    case ID_DIS_BACK_IN_HISTORY:
    case IDM_DIS_BACK_IN_HISTORY:
        BackInHistory();
        return;

    case IDM_DIS_COMMENT:
        AddComment();
        return;

    case ID_DIS_FIND_DEF:
    case IDM_DIS_FIND_DEF:
        FindDef();
        return;

    case IDM_DIS_FIND_REF1:
        FindRef1();
        return;

    case IDM_DIS_FIND_REF2:
        FindRef2();
        return;

    case ID_DIS_FIND:
    case IDM_DIS_FIND:
        Find();
        return;

    case ID_DIS_FIND_NEXT:
    case IDM_DIS_FIND_NEXT:
        FindNextString(false);
        return;

    case IDM_DIS_RENAME_DEF:
        RenameDef();
        return;

    case IDM_DIS_RENAME_REF:
        RenameRef();
        return;

    case IDM_DIS_ADDR_RANGE_DEF:
        AddrRangeDef();
        return;

    case IDM_DIS_ADDR_RANGE_REF:
        AddrRangeRef();
        return;

    case IDM_DIS_SET_TYPE_CODE:
    case IDM_DIS_SET_TYPE_LOBYTE:
    case IDM_DIS_SET_TYPE_HIBYTE:
    case IDM_DIS_SET_TYPE_STRING:
    case IDM_DIS_SET_TYPE_UNKNOWN:
        if (SetImmediateType(decodeMsg(wCommand))) {
            ::g_MemoryInspector->Refresh();
        }
        return;
    }
}

void MainDisassembly::AddComment() {

    if ((disSelection.segmentIndex != NO_SEGMENT_INDEX) && (disSelection.offset != 0xFFFF)) { // TODO: Have constant
        CommentDialog dialog(*mainWindow);
        auto hFont = ::g_Workspace->GetResizedFont();

        if (dialog.Show(*::g_Workspace->GetConstDisassemblyResult(), *::g_Workspace->GetSegmentList(),
            disSelection.segmentIndex,
            disSelection.offset,
            disSelection.size, hFont)) {
            ::g_MemoryInspector->Disassemble();
        }
    }
}

void MainDisassembly::BackInHistory() {
    disassemblyControl->BackInHistory();
}

void MainDisassembly::DrawMenu(const DIS_LINE* lpLine) {

    // Get label definition and reference to build menu dynamically.
    const auto labelReference = disassemblyControl->GetLabelReference();
    const auto labelDefinition = disassemblyControl->GetLabelDefinition();

    // Check if there is a navigation history available.
    const auto historyIndex = disassemblyControl->GetHistoryIndex();

    bool instructionHasImmediateOperand = false;
    bool charAllowed = false;
    MemoryType immediateMemoryType = MemoryType::UNKNOWN;

    // Remember item under cursor for "Add/Edit Comment" option.
    if ((lpLine != nullptr) && (lpLine->segmentIndex != NO_SEGMENT_INDEX)) {
        disSelection.segmentIndex = lpLine->segmentIndex;
        disSelection.offset = lpLine->offset;
        byte immediateValue;
        instructionHasImmediateOperand = Disassembly::IsInstructionWithImmediate(*::g_Workspace, lpLine->segmentIndex, lpLine->offset, immediateValue, immediateMemoryType);

        if (instructionHasImmediateOperand) {
            // TODO: It should not be required to create a disassembly here.
            Disassembly disassembly;
            DisassemblyWriter disassemblyWriter(disassembly, *::g_Workspace);
            charAllowed = disassemblyWriter.IsByteAllowedInString(immediateValue);
        }
    }
    else {
        disSelection.segmentIndex = NO_SEGMENT_INDEX;
        disSelection.offset = 0xFFFF;
    }

    disSelection.size = 0xFFFF;

    // Create popup menu opened on right button click.
    DisassemblyPopupMenu disassemblyPopupMenu;
    disassemblyPopupMenu.Update(CanFind(false), labelDefinition, labelReference, historyIndex, lpLine, instructionHasImmediateOperand, charAllowed, immediateMemoryType);
    disassemblyPopupMenu.Track(*mainWindow);
}


void MainDisassembly::Find() {
    DisassemblyFindStringDialog dialog(*mainWindow);

    if (dialog.Show(findString)) {
        FindString(dialog.GetFindAscii());
    }
}

void MainDisassembly::FindDef() {
    auto labelReference = disassemblyControl->GetLabelReference();
    disassemblyControl->SelectLabelDefinition(labelReference);
}

void MainDisassembly::FindRef1() {

    auto labelReference = disassemblyControl->GetLabelReference();
    disassemblyControl->SelectAllLabelReferences(labelReference);
    RefreshXRef(labelReference);
}

void MainDisassembly::FindRef2() {
    auto labelDefinition = disassemblyControl->GetLabelDefinition();
    disassemblyControl->SelectAllLabelReferences(labelDefinition);
    RefreshXRef(labelDefinition);
}


void MainDisassembly::RenameDef() {
    auto labelDefinition = disassemblyControl->GetLabelDefinition();
    auto address = Equate::ExtractAddress(labelDefinition);
    ::g_EquateListLogic->Edit(*::g_Workspace->GetUserEquateList(), TRUE, address);
}

void MainDisassembly::RenameRef() {
    auto labelReference = disassemblyControl->GetLabelReference();
    auto address = Equate::ExtractAddress(labelReference);
    ::g_EquateListLogic->Edit(*::g_Workspace->GetUserEquateList(), TRUE, address);
}

void MainDisassembly::AddrRangeDef() {
    auto labelDefinition = disassemblyControl->GetLabelDefinition();
    auto address = Equate::ExtractAddress(labelDefinition);
    ::g_EquateListLogic->DefineUserAddressRange(*::g_Workspace, address);
}

void MainDisassembly::AddrRangeRef() {
    auto labelReference = disassemblyControl->GetLabelReference();
    auto address = Equate::ExtractAddress(labelReference);
    ::g_EquateListLogic->DefineUserAddressRange(*::g_Workspace, address);
}


bool MainDisassembly::CanFind(bool first) const {
    return first || !findString.empty();
}

wstring MainDisassembly::GetFindString() const {
    return findString;
}

bool MainDisassembly::FindString(wstring_view findString) {

    findFirstLineNumber = 0;
    this->findString = findString;
    const auto found = FindNextString(true);
    RefreshXRef(this->findString);

    return found;
}

bool MainDisassembly::FindNextString(bool first) {
    auto found = ::g_Workspace->GetDisassemblyResult()->FindAndSelectLines(first, findFirstLineNumber, findString);
    if (found) {
        disassemblyControl->SyncLine(findFirstLineNumber);
    }
    else {
        FindStringDialog::ShowStringNotFoundMessage(disassemblyControl, findString);
    }

    return found;
}


/*
** Scroll to a specific line in the disassembly listing.
*/
void MainDisassembly::SelectLine(DisassemblyResult::LineNumber lineNumber) {
    disassemblyControl->SetResult(*::g_Workspace->GetDisassemblyResult());
    disassemblyControl->SetScrollLineNumber(lineNumber);
}


void MainDisassembly::SetSelection(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size) {
    disSelection.segmentIndex = segmentIndex;
    disSelection.offset = offset;
    disSelection.size = size;
}

bool MainDisassembly::SelectLineWithInfo(SEGMENT_NUMBER segmentNumber, Memory::offset offset) {
    return disassemblyControl->SelectLineWithInfo(segmentNumber, offset);
}

bool MainDisassembly::ExtendSelectionTo(SEGMENT_NUMBER segmentNumber, Memory::offset offset) {
    return disassemblyControl->ExtendSelectionTo(segmentNumber, offset);
}

/*
** Start the disassembly by opening the dialog box that shows the process.
*/
void MainDisassembly::Process(MemoryInspector& memoryInspector, const Segment* segment) {
    Disassembly disassembly;
    DisassemblyProgressDialog disassemblyProgressMonitor(*mainWindow);
    disassembly.SetProgressMonitor(disassemblyProgressMonitor);
    disassembly.SetWorkspace(*::g_Workspace);
    disassembly.StartDisassembly();

    RefreshDisControl();
    InvalidateRect(disassemblyControl->GetHWnd(), nullptr, true);

    if (segment == nullptr) {
        SelectLine(0);
    }
    else {
        SelectLine(segment->GetFirstLineNumber());
        // Move the disassembler window again to the selection.
        memoryInspector.SelectionChanged();
    }

    RefreshXRef(L"");
    disassemblyControl->ResetHistory();
}

void MainDisassembly::RefreshDisControl() {
    disassemblyControl->SetLineNumbersActive(::g_Workspace->GetConstProfile()->useLineNumbers);
    auto result = ::g_Workspace->GetDisassemblyResult();
    disassemblyControl->SetResult(*result);
}

void MainDisassembly::ResetDisControl() {
    disassemblyControl->Refresh();
    disassemblyControl->SetScrollLineNumber(0);
    RefreshXRef(L"");
    disassemblyControl->ResetHistory();
}

void MainDisassembly::RefreshXRef(wstring_view findString) {
    std::vector<wstring> references;

    // Scan all the buffers to reset all the selection flags except the line that matches the string.
    if (findString.empty()) {
        return;
    }

    for (auto i = ::g_Workspace->GetDisassemblyResult()->CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();

        // Add source line to reference window if it is marked.
        if (disLine->xrefLineNumber) {
            references.push_back(wstring(disLine->GetLine()));
        }
    }

    mainXRef->UpdateList(findString, references);
}

void MainDisassembly::XRefSelected(XRef::LineNumber xrefLineNumber) {
    for (auto i = ::g_Workspace->GetDisassemblyResult()->CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();

        // Do we have the line ?
        if ((disLine->xrefLineNumber == xrefLineNumber) && (disLine->segmentIndex != NO_SEGMENT_INDEX)) {
            // Change segment, select bytes in memory inspector and select line in disassembly.
            g_Workspace->GetSegmentList()->SetSelectedIndex(disLine->segmentIndex);

            if (disLine->size != 0) {
                g_MemoryInspector->Select(disLine->offset, (disLine->offset + disLine->size + -1));
            }
            else {
                g_MemoryInspector->ClearSelection();
            }

            disassemblyControl->SelectLine(disLine->GetLineNumber());
            InvalidateRect(mainWindow->GetHWnd(), nullptr, false);
            return;
        }
    }
}

bool MainDisassembly::AskByte(MemoryType bType, byte cKnownByte, byte& unknownByte) {
    LowHighByteDialog dialog(*mainWindow);

    const auto result = dialog.Show(bType, cKnownByte);
    if (result) {
        unknownByte = dialog.GetUnknownByte();
    }

    return result;
}

/*
** Set the type of the current immediate instruction byte
*/
bool MainDisassembly::SetImmediateType(MemoryType immType) {
    auto segmentIndex = disSelection.segmentIndex;
    auto offset = disSelection.offset;

    if (segmentIndex == NO_SEGMENT_INDEX) {
        return false;
    }

    gsl::not_null<const SegmentList*> segmentList = g_Workspace->GetConstSegmentList();
    gsl::not_null<Segment*> segment = segmentList->GetSegment(segmentIndex);

    if (segment->bBinary && !segment->IsEmpty()) {
        byte opc = segment->GetData(offset);

        const auto& instructionSet = g_Workspace->GetInstructionSet(segment->processorType);

        if (instructionSet->GetInstruction(opc).GetOperandMode() != OperandMode::Immediate) {
            return false;
        }

        BYTE bUnknownByte;
        if (immType == MemoryType::LOBYTE || immType == MemoryType::HIBYTE) {
            if (!AskByte(immType, segment->GetData(offset + 1), bUnknownByte)) {
                return false;
            }
        }

        MemoryType instrImmType = segment->GetType(offset);
        if (instrImmType == MemoryType::LOBYTE || immType == MemoryType::HIBYTE) {
            segment->SetType(offset, MemoryType::UNKNOWN);
        }

        switch (immType) {
        case MemoryType::LOBYTE:
        case MemoryType::HIBYTE:
            segment->SetType(offset, immType);
            segment->SetType(offset + 1, MemoryType(bUnknownByte));
            return true;
        case MemoryType::STRING:
            segment->SetType(offset + 1, MemoryType::STRING);
            return true;
        case MemoryType::CODE:
        case MemoryType::UNKNOWN:
            segment->SetType(offset, immType, 2);
            return true;
        default:
            return false;
        }
    }
    return false;
}
