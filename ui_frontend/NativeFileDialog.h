#pragma once

#include <optional>
#include <string>

#include "FileType.h"

// Thin wrapper around nativefiledialog-extended (github.com/btzy/nativefiledialog-extended).
// NFD's dialogs are synchronous/blocking (the call doesn't return until the
// user picks a file or cancels), which is why there's no popup/pending-
// action machinery here, unlike the earlier "type a path" placeholder this
// replaces.
//
// NOTE: nfd-extd's exact type/function names have shifted a little across
// versions (e.g. whether the filter-count parameter is `nfdfiltersize_t` or
// plain `int`). This is written against the commonly-documented, stable
// core API (NFD_Init/NFD_Quit/NFD_OpenDialog/NFD_SaveDialog/NFD_FreePath/
// NFD_GetError, nfdfilteritem_t) - if CMake's FetchContent pulls a version
// with slightly different names, expect a small, mechanical fix here, the
// same kind we've hit with ImGui before.
namespace NativeFileDialog {

    // Call once at startup/shutdown (see main.cpp).
    void Init();
    void Shutdown();

    // Filters the dialog using the real extension list from
    // FileTypeFactory::GetInfo(fileType) - e.g. Executable File offers
    // .bin/.com/.exe/.sys/.xex, Disk Image offers .atr/.xfd, etc. - so the
    // filter list always matches what the engine actually understands for
    // that FileType, rather than a hand-maintained duplicate list.
    std::optional<std::string> OpenFile(FileType fileType);

    // defaultName pre-fills the save dialog's filename field.
    std::optional<std::string> SaveFile(FileType fileType, const std::string& defaultName = "");

}
