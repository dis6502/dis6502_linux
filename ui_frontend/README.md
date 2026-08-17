# dis6502 ImGui/SDL2 front end

## Status: written, NOT compile-tested here

Unlike the core engine in `../src/` (which was fully compiled, linked,
and verified against the project's real unit test suite in this
sandbox), the code in this folder could not be compile-tested here: the
sandbox has no network access, so `apt-get install libsdl2-dev` and
fetching Dear ImGui's source both fail (confirmed - both come back
403). Everything here is written as carefully as possible against the
well-established SDL2 and Dear ImGui APIs and against the engine calls
that *were* verified (`WorkspaceLogic::AddFile`, `Disassembly::
StartDisassembly`, `DisassemblyResult::SelectLine`, etc. - see
`../colorizer_verify.cpp` for a working, verified example of driving
the engine and consuming its output), but treat it as a solid first
draft rather than something guaranteed to compile as-is. Expect to fix
a handful of small things (an off-by-one in an API name, a missing
include) when you build it for real.

## What's here

- `main.cpp` - SDL2 window + ImGui context setup, loads a file from the
  command line, runs the real disassembly, and renders it in a
  scrollable, syntax-colored list (via `ImGuiListClipper`, so it stays
  fast even on huge disassemblies) plus a segment-list side panel.
- `DisassemblyLineColorizer.h` - the syntax-highlighting state machine
  ported from the original `DisassemblyControlImpl::PrintOneLineInColor`,
  as a pure function (text in, colored segments out) with no drawing
  dependency. This one **was** verified in the sandbox - see
  `../colorizer_verify.cpp`, which runs it against real disassembly
  output from the `Segments-RUNAD.xex` test fixture and prints the
  colored breakdown of each line.
- `CMakeLists.txt` - fetches Dear ImGui via `FetchContent`, finds SDL2
  via `find_package`, and builds the core engine + this front end.

## Building (on a machine with real network access)

```sh
sudo apt install libsdl2-dev libgtk-3-dev cmake g++    # or your distro's equivalent
cd ui_frontend
cmake -B build
cmake --build build -j
./build/dis6502_imgui /path/to/some/file.xex
```

(`libgtk-3-dev` is nativefiledialog-extended's Linux backend dependency;
pass `-DNFD_PORTAL=ON` to `cmake -B build` instead if you'd rather use
the xdg-desktop-portal backend and skip the GTK dependency.)

## What's NOT implemented yet (see also the earlier conversation for the
full list of controller/dialog files parked in `../ui_todo/`)

- **Deleting a single equate.** `EquateList`'s public API only offers
  `Clear()` (wipe everything) - there's no public "remove just this one"
  method to call, so the Equate editor below is add-only. (The original
  Win32 dialog's own Add-equate handler was actually commented out in
  the source too - a half-finished feature even there.)

## File dialogs

File > Open/Save and Equates > Open/Save/Export now use real native
dialogs via [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended)
(see `NativeFileDialog.h`/`.cpp`), filtered using the engine's own
`FileTypeFactory::GetInfo()` metadata rather than a hand-maintained
extension list. NFD's dialogs are synchronous/blocking calls, so
there's no popup/pending-action state machine needed - selecting a menu
item just calls straight into `NativeFileDialog::OpenFile`/`SaveFile`
and acts on the result.

## View menu

`Display as Screen Code` and `No Disassembly` are real Workspace
settings toggles. `Profile...` opens a straightforward field-by-field
editor over `Profile`'s ~30 public settings (grouped the same way as
the struct's own comments); `Default Folders...` opens an editor over
the per-computer-system folder paths, backed by the already-portable
`DefaultFoldersLogic`/`ApplicationSettingsSection` (the INI-file reader/
writer built during the core-engine porting work).

## Memory Inspector

`DrawMemoryInspectorPanel` is a real hex-dump view (address / hex bytes
/ per-byte coloring) over whichever segment is selected in the Segments
panel, built on the already-portable `MemoryInspectorSelection`/`Segment`
core classes - no placeholder here. Byte colors match the original
`MemoryInspectorControlImpl`'s `Colors[]` table exactly (same RGB values,
indexed by `MemoryType`). Click a byte to select it; shift-click extends
the selection - simpler than the original's full click-drag range
selection, but drives the same `MemoryInspectorSelection::SetSelection()`
underneath. Not ported: the sprite-bitmap rendering mode.

## Cross References

`DrawXRefPanel` is a working search, but a deliberately simplified one.
The original marks `DIS_LINE::xrefLineNumber` during Pass1/label-reference
resolution and lists exactly those pre-marked lines (see
`ui_todo/MainDisassembly.cpp`'s `RefreshXRef`); this instead does a plain
case-insensitive substring search across every disassembled line's text
on demand. It's less precise (a line whose comment happens to contain
your search text will show up too, not just true label references), but
needed no changes to core engine code to implement. Selecting a result
jumps the disassembly view and Memory Inspector to that line/byte range,
mirroring the original's `XRefSelected()`.

Note: the Memory Inspector panel forces a white background + black
default text (`ImGuiCol_ChildBg`/`ImGuiCol_Text`), since the original's
`Colors[]` table (including pure black for `MemoryType::UNKNOWN`) was
designed against a white Win32 control background, not ImGui's dark
default theme - without this override, unanalyzed (UNKNOWN-type) bytes
rendered as invisible black-on-black text.

## Equate editor, Address Range, and Comments

`Equates > Edit User Equates...` is a real editor: a text field parses
and adds new equates via the engine's own `EquateList::AddEquate(line)`
(same parser used when loading an equate file), and lists all current
user equates below. It's add-only - see the "not implemented" note
above on why deletion isn't offered.

`Equates > Define Address Range...` is a real implementation of
`EquateList::SetRange()` - pick a base equate label and a start/end
address, and it splits that range into per-offset labels.

The Disassembly panel has a `Comment...` button that opens an editor for
the currently selected line's user comment, via
`SegmentList::GetUserComment()`/`SetUserComment()`. Both this and the
two Equate panels above call `RunDisassembly()` afterward (a small
refactor of the same disassembly-trigger code `LoadFile` already used)
since equate/comment changes need a fresh disassembly pass to show up in
the listing.

## Assemble

`Assemble...` in the Memory Inspector (enabled once you've selected a
byte) is a real, working port of `ui_todo/AssembleDialog.cpp`'s
interactive mini-assembler - type one line of 6502 assembly (e.g.
`LDA #$05`, `STA $0600`, `BEQ LABEL`), and it parses the operand
expression, figures out the addressing mode, finds the matching
`Instruction` in the engine's own `InstructionSet`, and writes the
resulting opcode+operand bytes directly into the segment. On success it
advances the selection past the new bytes and clears the input, so
typing several instructions in a row works like the original.

The expression parser and addressing-mode logic (`MiniAssembler.h`) are
a faithful, mechanical port of the original's `GetExpressionValue()` and
`ProcessDialogMessage()` - same recursive-descent parser (`<`/`>` for
low/high byte, labels via the engine's own equate lookup, `$hex`,
decimal, `+`/`-`/`&`/`|` operators - note `|` performs bitwise AND in
the original source, not OR; that's preserved here rather than "fixed",
since changing behavior wasn't the goal), same addressing-mode detection
switch. Being pure logic with no UI dependency, it could be pulled out
and unit-tested directly against the real engine - which is exactly
what `../assembler_verify.cpp` does: assembles `LDA #$05`, `STA $0600`,
and `NOP` against a real `Segment`/`Workspace` and checks the exact
opcode/operand bytes written match the real 6502 instruction set, plus
confirms an unknown instruction fails cleanly rather than crashing. All
checks pass. (While writing that verification program, it also caught a
real formatting bug - a `%s` where `%ls` was needed for a wide string in
`swprintf`, which was silently truncating the result message to one
character - fixed before this shipped.)

## Find (byte-pattern search)

`Find...` in the Memory Inspector is a real port of
`ui_todo/MemoryInspector.cpp`'s `FindString`/`SearchString`/
`FindNextString` - searches for an ASCII text pattern in raw memory
bytes (as opposed to the Cross References panel, which searches
disassembly *text*), optionally across all segments rather than just
the selected one, with "Find Next" continuing from just past the
previous match - same resumable-search state (`finder.segmentIndex`/
`offset`) as the original. On a match, it jumps the segment selection
and selects the matched byte range in the hex dump.

Simplified vs. the original: `ui_todo/FindStringDialog.cpp` synced an
ASCII field with a hex-bytes field (so you could search for
non-printable byte sequences by typing hex directly); this only takes
the ASCII text as the pattern. Good enough for finding text strings in
memory (the common case), not arbitrary byte sequences.

## Byte-type editing (and LowHighByteDialog)

The Memory Inspector now has a "Set Type" combo + button, listing the
same 13 `MemoryType` options in the same order as the original's
"Change type of selected bytes to" submenu (Code, Code with Low/High
Byte, Byte, Word, Label, SpartaDos X Label/Fix-Up, String, Screen Byte,
Display List, Data Store, Unknown) - a real port of
`ui_todo/MemoryInspector.cpp`'s `SetType`, including its guard checks
(no selection, non-binary segment, low/high-byte-specific checks) and
its CODE-boundary cleanup when a LOBYTE/HIBYTE byte gets overwritten by
another type.

Choosing "Code with Low Byte" or "Code with High Byte" opens a real port
of `ui_todo/LowHighByteDialog.cpp` - the original's small popup for
supplying the paired byte's value (which can't be inferred automatically,
since it usually lives at a different, non-adjacent address - e.g.
separate low-byte and high-byte tables). This was the blocker mentioned
in previous notes: LowHighByteDialog only makes sense as part of the
larger set-type feature, which didn't exist until now.

One deliberate deviation from the original, for safety rather than
fidelity: the original's boundary-cleanup logic has a comment
`/* && not at the end of buffer */` next to a check that was never
actually implemented, meaning it could write one byte past the end of a
segment's buffer in an edge case. Unlike the `MiniAssembler`'s
preserved `|`-means-AND quirk (a behavior quirk, not a safety issue),
this one is an actual out-of-bounds write risk, so the missing bounds
check was added here rather than faithfully reproduced.

## Clipboard, drag-and-drop, and confirmation prompts

- **Clipboard**: `Application::SetClipboardText` (core) now calls a
  `std::function` hook (`Application::clipboardTextSetter`) instead of
  being a no-op - kept as a hook rather than a virtual method/subclass
  so core still doesn't depend on any UI toolkit. `main.cpp` wires this
  hook to `SDL_SetClipboardText` at startup.
- **Drag-and-drop**: dropping a file on the window now loads it, via a
  new `SDL_DROPFILE` case in the event loop (file type guessed from
  extension, same as passing a path on the command line).
- **Confirmation prompt**: `Equates > Clear System/User Equates` now go
  through a real Yes/No popup (`AppState::RequestConfirm`, a small
  generic confirmation mechanism) before actually clearing, matching the
  original's MessageBox confirmation.

## Real font rendering

The disassembly listing and Memory Inspector hex dump now render using
the *actual* bundled 8x8 bitmap font from the original
(`systems/Atari800/Atari800.fon`, copied here from the original repo),
not ImGui's default font - genuinely the hardest of the four items in
this batch, and worth documenting in detail:

**The parser (`FntFont.h`)**: the original's `.fon` files are legacy
Windows 3.x NE-executable files with an embedded FNT bitmap-font
resource. Rather than fully parse the NE resource table to locate it,
`FntFont::Load` scans for a position where a plausible FNT version word
(0x0200/0x0300) is followed by a declared size that, added to that
position, exactly equals the file's total size - a strong, effectively
unique condition for a file known to contain exactly one FNT resource.
Header field offsets and the char-table structure were derived from the
public Windows FNT format documentation and cross-checked by hand
against a hex dump of the real file (matching copyright string, matching
file-size arithmetic, matching header-size boundaries) before being
written as code.

**This was actually verified, not just written and hoped for** -
`fnt_verify.cpp` parses the real bundled `Atari800.fon` and prints
decoded glyphs as ASCII art. The first version had a real bug (glyph
bitmap bytes were being read column-major; the correct layout for these
8-pixel-wide fonts is row-major, one byte per row) - caught because
letters like 'A'/'B'/'S' rendered as plausible-looking but wrong blobs
while digits/space/punctuation happened to still look right, which was
a real, if narrow, false-positive risk. Manually decoding the raw bytes
for the letter 'A' by hand (`0x00 0x18 0x3C 0x66 0x66 0x7E 0x66 0x00`)
confirmed which layout was actually correct - row-major produces a
perfect, unambiguous letter A; column-major doesn't. All five bundled
fonts (Atari800, Atari5200, C64, Oric, Unknown) were re-verified after
the fix and all decode cleanly. Every bundled font happens to be 8
pixels wide (fits in one byte per row); wider fonts would need the
`bytesPerRow > 1` case, which is implemented but untested since nothing
here exercises it.

**The ImGui integration (`ComputerFontAtlas.h`)**: uses
`ImFontAtlas::AddCustomRectFontGlyph`, the standard mechanism
`imgui_demo.cpp` itself demonstrates for injecting custom bitmap glyphs.
Unlike the parser, this part could not be run against a real ImGui build
in this sandbox (no network access to fetch ImGui/SDL2) - treat it with
more caution than the parser itself. There's also a real ordering
requirement documented in the header: the glyphs must be registered
*before* `ImGui_ImplSDLRenderer2_Init()` runs, since that call uploads
whatever's in the atlas at that moment as a GPU texture.

Currently only the Atari800 font loads (matching `AppState`'s hardcoded
`ComputerSystemType::ATARI800` default) - swapping fonts when the
computer system becomes selectable is a small follow-up, not a
structural change.

**Bug fixed after real-app testing**: the first version of this
registered the 8x8 bitmap glyphs onto `io.Fonts->AddFontDefault()`
directly - but since that was the *only* font ever added to the atlas,
ImGui made it the app's global default font, applied everywhere (menu
bar, Segments list, Log, etc.), not just wherever `PushFont`/`PopFont`
was explicitly used around the Disassembly/Memory Inspector text. Every
pane ended up rendering with the 8-pixel bitmap font, which (among other
symptoms) made text look too wide throughout the whole app. Fixed by
registering a separate, untouched `AddFontDefault()` for general UI use
*before* `ComputerFontAtlas::RegisterGlyphs()` runs, so the bitmap
glyphs land on their own distinct `ImFont` instance instead.

## Display System Equates, and the rest of the Profile editor

`Equates > Display System Equates` is a straightforward read-only list
(same rendering as the Equate editor's list, just no add/edit controls)
over `Workspace::GetSystemEquateList()` - the CPU/OS-defined equates,
distinct from the user equates the editor covers.

The Profile editor now covers every field: `outputEncoding` is a real
combo box (Unknown/ASCII/ATASCII/Binary/UTF8), and the four `word`
count fields (`directiveBYTENumberOfBytesPerLine`,
`directiveBYTENumberOfCharactersPerString`,
`directiveWORDNumberOfWordsPerLine`,
`directiveINCLUDEMaximumNumberOfLinesPerFile` - there turned out to be
four of these, not two as an earlier pass here assumed) are real
integer inputs.

## Sprite view

`Sprite View...` in the Memory Inspector (enabled once you've selected
a byte range) renders those bytes as a bitmap, in one of the 8 Atari
graphics modes the original supported - a real, verified port of the
*data model* (mode tables and bit-to-pixel extraction), but a
from-scratch ImGui renderer rather than a port of the original's actual
drawing code.

**What's ported and verified**: `SpriteMode.h` carries over the
original's `wSpriteNbColors`/`wSpriteNbBytesPerLine`/
`wSpriteNbPixelsPerByte`/`wSpritePixelWidth`/`wSpritePixelHeight` mode
tables and its `SPRITE_GET_PIXEL_1`/`SPRITE_GET_PIXEL_2` macros (1-bit
vs. 2-bit-per-pixel extraction) as plain functions. `sprite_verify.cpp`
checks these against hand-computed values (e.g. byte `0xE4` in 2-bit
mode should decode to palette indices 3,2,1,0) and passes.

**What's a fresh implementation, not a port**: the original's actual
drawing code (`SpriteControlImpl::Paint`) manually blits into a
fixed-stride Win32 DIB buffer using hardcoded byte offsets (things like
`*(lpScreen + 960) = ... = cPixel` to replicate a row across a specific
pre-allocated bitmap's scanlines) - there's nothing meaningful to "port"
from that into ImGui's retained-draw-list model. Instead,
`DrawSpriteViewPanel` draws each logical pixel as a scaled filled rect
via `ImDrawList::AddRectFilled`, using a simple 4-shade grayscale palette
rather than attempting accurate GTIA color reproduction.

## Layout overhaul (matching the original's actual window arrangement)

Earlier rounds gave each panel its own independent, freely-movable
floating ImGui window. Based on a screenshot of the real application,
the original doesn't work that way at all - it's one fixed-layout main
window with a specific split-pane arrangement: Segments (top-left) and
Memory Inspector (bottom-left) stacked in a narrow left column,
Disassembly (top-right) and Cross References (bottom-right) filling the
rest, and a full-width Log panel along the bottom.

`DrawMainLayoutAndLog` now reproduces that: one non-movable, non-
resizable container window with four fixed `BeginChild` regions plus the
Log strip. `DrawSegmentList`/`DrawDisassemblyView`/
`DrawMemoryInspectorPanel`/`DrawXRefPanel` were each split into a
`*Content` function (just the widgets, no window of its own) that this
container calls into. The other panels (Profile, Default Folders, Equate
editor, Assemble, Find, Sprite View, etc.) are still independent
floating windows/popups, matching how those work as real dialogs in the
original too.

**The Log panel is new** - previously there was only a single
"latest status message" shown in a small floating box. `LoggedString` (a
tiny wrapper type) intercepts every existing `statusMessage = "..."`
assignment throughout the file and also appends it to a scrolling
`logMessages` history, without needing to touch each of the ~25 call
sites individually - the Log panel just displays that history,
auto-scrolled to the latest entry.

**Color matches** to the screenshot: switched from ImGui's default dark
theme to `ImGui::StyleColorsLight()` for the white/light chrome; the
Segments list uses a cyan/teal selected-row highlight and the
Disassembly view a yellow one (both via `ImGuiCol_Header*` style
pushes), and the Cross References panel has a light red/pink
background tint - all matching the reference screenshot. These are
approximate matches (picked to look similar), not measured/exact color
values from the original's source.

Not attempted: resizable splitters between the panes (currently fixed
proportions - left column 300px wide, top row 65% of the remaining
height), and an exact pixel-for-pixel match of the original's fonts/
spacing/borders.

## Layout fixes after real-app testing

Three real bugs found once this was actually running:

1. **"Disassembly frame too wide, vertical scrollbar off screen"**: the
   pane-size math used the raw window size (`io.DisplaySize`) instead of
   the window's actual usable content region, which is smaller once its
   own padding/border is accounted for. That made the sum of the pane
   widths slightly exceed what really fit, pushing the right column (and
   its scrollbar) past the visible edge. Fixed by computing sizes from
   `ImGui::GetContentRegionAvail()` instead.
2. **"Memory Inspector window too narrow"**: the left column was a fixed
   300px, not enough for the Memory Inspector's 16-bytes-per-row hex
   grid (~450px minimum). Widened to 480px.
3. **"Top info bar cut off and not accessible"**: each pane's header
   text (e.g. "Segments", "Log") was being drawn *inside* the same
   scrolling region as its content - so once there was enough content to
   need scrolling, the header scrolled away with it and became
   unreachable. Fixed by disabling scrolling on the four outer pane
   containers (`ImGuiWindowFlags_NoScrollbar | NoScrollWithMouse`) and
   giving each pane's actual content its own nested scrolling child
   underneath the now-fixed header - applied to Segments and the Log
   panel, which had this bug; Disassembly/Memory Inspector/Cross
   References already had their own nested scrolling children so weren't
   affected.

## Two more bugs found after further testing

1. **"Memory Inspector changing 2 bytes when only 1 is selected"**: a
   real, systemic off-by-one. `MemoryInspectorSelection::GetSize()` is
   defined as `GetEnd() - GetBegin() + 1` - an **inclusive** end offset
   (both endpoints included) - but every call site here (byte-click
   selection, shift-click extend, the selection highlight check, Find's
   match-selection, Cross References' "jump to this reference", and the
   Sprite View's selection-size calculation) was written assuming a
   standard exclusive `[begin, end)` range instead, universally passing
   `offset + 1`/`+ size` where it should have been `offset`/`+ size - 1`.
   Every one of those call sites is now fixed to match the engine's real
   (inclusive) convention.
2. **"Menu bar cut off on the right side"**: `DrawMainLayoutAndLog`'s
   container window was positioned/sized by manually computing
   `(0, GetFrameHeight())` and `(io.DisplaySize.x, io.DisplaySize.y -
   GetFrameHeight())` - a hand-rolled approximation of "the area below
   the main menu bar" that could drift from the menu bar's actual
   reserved space. Replaced with `ImGui::GetMainViewport()->WorkPos`/
   `WorkSize`, which is what `BeginMainMenuBar()` itself automatically
   keeps up to date to exclude its own space - the correct, idiomatic
   way to position content below the menu bar rather than recomputing
   it independently.

## Memory Inspector: right-click context menu

Based on a screenshot of the real app's right-click menu, the toolbar
buttons (Assemble/Find/Set Type/Sprite View) that used to sit above the
Memory Inspector's hex grid are gone - that's not how the original
works. They're now items in a right-click context menu on the hex grid
itself (`ImGui::BeginPopupContextWindow`), matching the original's exact
menu structure, order, and keyboard-shortcut labels:

- **Working, relocated from the old toolbar**: Change type of selected
  bytes to (submenu, with a checkmark on the selection's current type),
  Assemble at selection..., Find..., Select Sprites... (opens the Sprite
  View panel).
- **New and working**: Add/Edit comment... (the same comment editor the
  Disassembly panel already had, generalized via a new
  `AppState::OpenCommentPopup()` so both call sites share it), Find next
  (reuses whatever text is in the Find panel's search box), Save
  selection without header... / with header... (writes the selection's
  raw bytes to a file, optionally preceded by a 6-byte Atari
  binary-load header - `0xFFFF` + little-endian start/end address -
  which is the same format `Segment::IsHeader(FileHeader::ATARI_BINARY)`
  elsewhere in the engine already recognizes when loading files back
  in).
- **Grayed-out stubs, confirmed not needed**: Start code trace at
  selection, Set current block of Unknown type to Byte, Edit bytes at
  selection, Cut/Copy/Paste (before/after)/Delete/Split at selection,
  Select next block of Unknown type, Select all. (Worth noting: engine
  support does exist for some of these if they're ever wanted later -
  `Segment::SplitAt`/`CanSplitAt` for Split, `Segment::SetData()` for
  byte editing - but there's no byte-level insert/delete/resize API on
  `Segment` for Cut/Paste/Delete specifically, so those would need
  engine changes, not just UI work.)

## Two more fixes from a reference photo of the real app

1. **"Memory Inspector font too wide"**: not actually a font-size issue
   - it was dead space. Each byte cell reserved a fixed 24px plus the
   theme's default ~8px `SameLine` spacing, well more than the ~18px the
   8-pixel bitmap font's two hex digits actually need, making the whole
   hex dump look spread out compared to the original's tightly-packed
   layout. Tightened to an explicit 4px `SameLine` spacing and a 20px
   cell width, and switched from 16 to 8 bytes per row to match the
   reference photo's layout.
2. **"Disassembly font not dark enough"**: same root cause as an earlier
   Memory Inspector fix - `ImGui::StyleColorsLight()`'s default
   background isn't pure white, which reduces contrast for the syntax
   highlighter's already-dark colors. Gave the Disassembly view the same
   forced-white-background treatment the Memory Inspector already had.

**Still to do**: the reference photo also shows a whole column this port
doesn't have yet - a vertical divider after the hex bytes, followed by
each byte's Atari/ATASCII character-glyph representation (using the same
bitmap font, but indexed by the raw byte value directly rather than
through disassembly text - the "Display as Screen Code" feature this
already has a toggle for, per the View menu). Waiting on a follow-up
photo focused on that panel before implementing it.

## Character-glyph column, and darker disassembly text

Based on the follow-up reference photo, the Memory Inspector's hex grid
now has the column it was missing: a vertical divider followed by each
byte's raw value rendered directly as a glyph in the same bitmap font -
not through disassembly text, just the byte value itself used as a
character-set index, matching the reference photo's hearts/card-suit/
reversed-character look for arbitrary byte values.

This needed a different rendering path than everywhere else this font
is used: byte values 0x80-0xFF can't be passed through ImGui's normal
(UTF-8-decoding) text functions as a 1-byte string - they'd be
misinterpreted as invalid/continuation UTF-8 bytes. Instead this uses
`ImFont::RenderChar(draw_list, size, pos, col, ImWchar c)`, which draws
a single glyph by its raw codepoint directly, bypassing string encoding
entirely. Confirmed against the real Dear ImGui documentation before
using it (a real API, but a less commonly-used one than most of what
this front end otherwise calls, so it got the same "verify before
trusting" treatment as anything else uncertain).

Also darkened the Disassembly syntax-highlighter colors (Comment/
Number/String/Instruction went from ~128/255 to ~90/255 intensity) after
the white-background fix alone wasn't enough - worth noting these were
always an approximation, not a byte-exact extraction from the original's
source the way the Memory Inspector's `ColorForMemoryType` table is.

## Click-and-drag multi-byte selection in the Memory Inspector

Previously a single click always selected exactly one byte (with
shift-click to extend, from an earlier round) - no way to click and drag
across a range in one motion. Added real drag-select using ImGui's
standard idiom for this: `IsItemActivated()` fires once when the mouse
button is first pressed on a byte cell (recording that offset as a drag
anchor and starting a fresh single-byte selection there, unless shift is
held, which extends the existing selection instead); then every
subsequent frame the button stays held, `IsItemHovered() &&
IsMouseDown(ImGuiMouseButton_Left)` on whichever cell the mouse is
currently over extends the selection from the anchor to there. Once the
button is released, dragging stops affecting the selection until the
next press sets a new anchor.

## Fixed: drag-select wasn't extending past the first cell

Root cause, confirmed against the real ImGui source/docs: `IsItemHovered()`
returns `false` by default for any item *other* than whichever one is
currently "active" (mouse-button-down-captured) - specifically to avoid
accidental interactions while dragging elsewhere. That's exactly the
drag-select scenario: after pressing on the first byte, every *other*
cell you drag over is hovered while a *different* item (the first one)
is still active, so the plain `IsItemHovered()` check always returned
false for them - the selection never extended past the first byte.
Shift-click worked because it's a different code path
(`IsItemActivated()` on the newly-clicked item itself, not a hover check
on some other active item). Fixed by passing
`ImGuiHoveredFlags_AllowWhenBlockedByActiveItem`, which is the flag's
documented purpose ("useful for Drag and Drop patterns").

## Memory Inspector selection now syncs the Disassembly view

Selecting bytes in the Memory Inspector (click or drag) now jumps the
Disassembly view to the matching line and scrolls it into view - the
reverse direction of what Cross References already did (selecting a
reference there jumps the Memory Inspector to match). Uses
`DisassemblyResult::SelectLine(segmentIndex, offset)`, which the engine
already provides specifically for mapping a memory location back to its
disassembly line. The scroll itself computes the target line's Y
position directly (row height x line index, since every rendered row
uses a uniform height) and calls `ImGui::SetScrollY()` to center it,
rather than needing to actually render up to that line first.

## Sync now works both directions

Clicking a line in the Disassembly view now also jumps the Memory
Inspector to that line's segment/byte range and scrolls it into view -
previously only Memory Inspector -> Disassembly worked (and Cross
References -> Memory Inspector, from an earlier round). Factored the
jump-to-memory logic (already duplicated between an earlier version of
this and the Cross References panel) into one shared
`AppState::SyncMemoryInspectorToDisassemblyLine()` method, now used by
both. The Memory Inspector's scroll-to-selection uses the same approach
as the Disassembly view's scroll-to-line: uniform row height x target
row index (here, `offset / bytesPerRow`) gives the Y position directly.

## Colored pane title bars

Each pane's header now has a full-width colored bar matching the
original screenshots: Segments (yellow), Disassembly (green), Memory
Inspector (cyan), Cross References (pink/red), Log (lavender) - via a
small `DrawColoredHeaderBar()` helper (fills a rect the width of the
pane and one text-line tall via `ImDrawList::AddRectFilled`, then draws
the label on top). As with other color choices in this port, these are
picked by eye from photos of the real app, not measured/exact values.
