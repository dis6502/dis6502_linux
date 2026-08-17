#include <cstdio>
#include "ui_frontend/DisassemblyLineColorizer.h"

#include "Application.h"
#include "TestPaths.h"
#include "DisassemblyLine.h"
#include "Workspace.h"
#include "WorkspaceLogic.h"
#include "Disassembly.h"
#include "DisassemblyProgressMonitor.h"
#include "DisassemblyResult.h"
#include "FileType.h"
#include "systems/ComputerSystemFactory.h"
#include "String.h"

const char* ClassName(DisLineSyntaxClass c) {
    switch (c) {
        case DisLineSyntaxClass::Normal: return "Normal";
        case DisLineSyntaxClass::Comment: return "Comment";
        case DisLineSyntaxClass::Number: return "Number";
        case DisLineSyntaxClass::String: return "String";
        case DisLineSyntaxClass::Instruction: return "Instr";
        case DisLineSyntaxClass::Unreferenced: return "Unref";
    }
    return "?";
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    try {

    ComputerSystemFactory computerSystemFactory;
    std::printf("Step: ComputerSystemFactory constructed\n");
    Workspace workspace(computerSystemFactory);
    std::printf("Step: Workspace constructed\n");
    WorkspaceLogic workspaceLogic;
    workspace.SetComputerSystemType(ComputerSystemType::ATARI800);
    std::printf("Step: computer system type set\n");

    // Load one of the real test fixtures we already validated the engine against.
    wstring filePath = TestPaths::GetSuitePath() + L"/system/atari800/in/Segments-RUNAD.xex";
    std::printf("Step: about to AddFile\n");
    bool loaded = workspaceLogic.AddFile(workspace, FileType::EXECUTABLE_FILE, filePath);
    std::printf("Load result: %s\n", loaded ? "OK" : "FAILED");

    Disassembly disassembly;
    std::printf("Step: Disassembly constructed\n");
    DisassemblyProgressMonitor progressMonitor;
    disassembly.SetProgressMonitor(progressMonitor);
    disassembly.SetWorkspace(workspace);
    std::printf("Step: about to StartDisassembly\n");
    disassembly.StartDisassembly();
    std::printf("Step: StartDisassembly done\n");

    auto* result = workspace.GetDisassemblyResult();
    auto lineCount = result->GetLineCount();
    std::printf("Disassembly produced %zu lines. Showing first 15, colorized:\n\n", (size_t)lineCount);

    for (DisassemblyResult::LineNumber i = 1; i <= lineCount && i <= 15; i++) {
        DIS_LINE* line = result->SelectLine(i);
        if (!line) continue;
        const wchar_t* text = line->GetLine();
        // Mirror main.cpp's actual referenced computation (see its
        // comment for why): DIS_LINE::referenced is only ever computed
        // for SYSTEM_EQUATES lines, so treat every other section as
        // fully colored regardless of the raw (default-false) field.
        bool referenced = line->referenced ||
            (line->GetSection()->GetType() != DisassemblySectionType::SYSTEM_EQUATES);
        auto segments = ColorizeDisassemblyLine(text, referenced);

        std::printf("Line %2d: \"%s\"\n", (int)i, String::wstring_to_utf8(text).c_str());
        for (auto& seg : segments) {
            std::printf("    [%-6s] \"%s\"\n", ClassName(seg.syntaxClass), String::wstring_to_utf8(seg.text).c_str());
        }
    }

    } catch (const std::exception& ex) {
        std::printf("EXCEPTION: %s\n", ex.what());
        return 1;
    }

    return 0;
}
