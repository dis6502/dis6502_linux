# dis6502 Linux port - working notes and artifacts

This is the output of porting dis6502 (a Win32 6502 disassembler) toward
Linux + ImGui/SDL2. Two different confidence levels apply here - read
this before diving into the code.

## `src/` - the core disassembly engine: COMPILED, LINKED, AND TESTED HERE

This is the real payoff. All of `src/*.cpp` and `src/systems/**/*.cpp`
(85 files) compile cleanly on Linux/g++ with `-std=c++20`, link into a
working binary, and **pass the project's own real unit test suite**
(`test_runner_bin`, built from `test_runner.cpp`) against its actual
fixture files in `tst/suite/` - real Atari `.atr`/`.xex` files, a real
C64 `.prg`, real XML reference-output comparisons. Zero compile errors,
zero test failures.

Getting here involved:
- A `PlatformCompat.h` shim (~200 lines) replacing every genuine
  Windows-CRT-only call the engine used (`_stricmp`, `_wfopen`,
  `strncpy_s`, wide-char file I/O, etc.)
- Discovering that several classes named like plain data/logic
  (`FileSystemLogic`, `MemoryInspector`, `Main*.cpp`, `EquateListLogic`,
  `ProfileLogic`, `ComputerFont`, `utils.cpp`) actually directly drive
  Win32 dialogs/GDI - these got moved to `ui_todo/` since they're
  properly UI-layer code, not engine code
- Fixing a few genuine pre-existing bugs found along the way (see
  `PORTING_NOTES.md` for details): a broken exception message path, an
  extern global that was declared but never defined anywhere (would
  never have linked on Windows either if that code path was hit), a
  hardcoded Windows-only path separator, one use of a non-portable MSVC
  STL internal method

Try it yourself:
```sh
make test        # builds everything and runs the real unit test suite
```
(GSL and the tst/ fixtures are bundled directly in this zip under
`lib/` and `tst/`, so this works standalone - no other repo needed)

## `ui_frontend/` - the ImGui/SDL2 shell: WRITTEN, NOT COMPILE-TESTED HERE

This sandbox has no network access (confirmed: `apt-get install
libsdl2-dev` and fetching ImGui's source both fail with 403), so unlike
the core engine, this part could not be built and verified here. It's
written carefully against the real, verified engine API (see
`colorizer_verify.cpp`, which *was* built and run here, proving the
syntax-highlighter and the engine calls it depends on both work
correctly against real disassembly output) and against well-established
SDL2/ImGui APIs, but treat it as a strong first draft, not a guarantee.
See `ui_frontend/README.md` for build instructions and a list of what's
implemented vs. still needed.

## `ui_todo/` - original UI/controller code, kept for reference

Everything that got moved out of `src/` because it's genuinely
Win32-UI-coupled (dialogs, GDI drawing, the Win32 message-loop
controllers). Keep these around as the reference implementation when
rebuilding each piece as an ImGui panel - the underlying logic (what
each dialog is supposed to do) is still valid, only the drawing/event
mechanics need replacing.

## Suggested order for continuing this

1. Build `ui_frontend/` for real, fix whatever small issues come up
2. Wire up a real file-open dialog (see `ui_frontend/README.md`)
3. Rebuild the Memory Inspector panel using `ui_todo/MemoryInspector.cpp`
   as the reference for behavior
4. Work through the remaining dialogs (equates, comments, find, etc.)

## Missing <cstdint> includes fixed (real build failures reported)

Several core headers used fixed-width integer types (`uintmax_t`,
`uint32_t`, `int32_t`, `uint16_t`) without including `<cstdint>`
directly - they'd been compiling by luck via transitive inclusion
through some other header, which isn't a portable guarantee across
different g++/libstdc++ versions. Confirmed as a real, reported build
failure (repeated `uintmax_t` errors from `String.h`) in an environment
where that transitive inclusion doesn't happen. Fixed directly in
`String.h`, and proactively found and fixed the same latent issue in
three other files before they became the next reported error:
`FileIO.h`, `Workspace1X.h`, `systems/atari800/AtariDOS.h`. (Checked
`tinyxml2.h`/`.cpp` too - those already have an unconditional
`#include <stdint.h>`, so no fix needed there; that was a false
positive from the initial grep, which only checked for the C++-style
`<cstdint>` spelling.)

Full clean rebuild + the real test suite (`make test`) reconfirmed all
8 suites still pass after these fixes.
