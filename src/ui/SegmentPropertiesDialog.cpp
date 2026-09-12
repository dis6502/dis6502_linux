#include "Application.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "Dialog.h"
#include "EditControl.h"
#include "InstructionSet.h"
#include "Memory.h"
#include "MessageBoxDialog.h"
#include "ProcessorType.h"
#include "Segment.h"
#include "Text.h"
#include "Workspace.h"
#include <memory>

#include "SegmentPropertiesDialog.h"

extern std::unique_ptr <Workspace> g_Workspace;


SegmentPropertiesDialog::SegmentPropertiesDialog(Window& parentWindow) : Dialog(parentWindow, L"SEGMENTPROPSBOX") {
    segment = nullptr;
}

bool SegmentPropertiesDialog::Show(Segment& segment) {
    this->segment = &segment;

    return ShowDialogBox();
}

bool SegmentPropertiesDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {

    case WM_INITDIALOG:
        CreateControls();
        GetEditControl(IDC_ADDRSEGMENT).SetAddress(segment->wBegin);
        GetCheckBox(IDC_CHECKBINARY).SetChecked(segment->bBinary);
        GetEditControl(IDC_LABELPREFIX).SetText(segment->szLabelPrefix);
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case IDOK: {

            const auto wBegin = GetEditControl(IDC_ADDRSEGMENT).GetAddress();
            const Memory::address wEnd = wBegin + segment->GetSize() - 1;

            if (wBegin > wEnd) { // In case of 64K overflow // TODO Will not work with >64K
                MessageBoxDialog::ShowAlert(this, L"Segment Properties", Text::Get(IDS_ERR_SEGMENT_OVERLAP));
                return true;
            }

            segment->wBegin = wBegin;
            segment->wEnd = wEnd;

            segment->bBinary = GetCheckBox(IDC_CHECKBINARY).IsChecked();
            segment->szLabelPrefix = GetEditControl(IDC_LABELPREFIX).GetText();

            auto index = GetComboBox(IDC_PROCESSORCOMBO).GetSelectedIndex();

            segment->processorType = processorTypes.at(index);

            return EndDialogBox(true);
        }

        case IDCANCEL:
            return EndDialogBox(false);

        default:
            return false;
        }

    default:
        return false;
    }

}

void SegmentPropertiesDialog::CreateControls() {
    auto comboBox = &GetComboBox(IDC_PROCESSORCOMBO);
    comboBox->Clear();
    for (auto processorType : processorTypes) {
        comboBox->AddString(::g_Workspace->GetInstructionSet(processorType)->GetName(), (void*)processorType);
    }

    comboBox->SetSelectedIndex(ProcessorTypeFactory::GetIndex(segment->processorType, processorTypes, 0));

}
