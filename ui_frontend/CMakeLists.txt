cmake_minimum_required(VERSION 3.16)
project(dis6502_imgui CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SDL2 (expects the dev package installed system-wide, e.g.
#     `sudo apt install libsdl2-dev` on Debian/Ubuntu, or `brew install sdl2` on macOS) ---
find_package(SDL2 REQUIRED)

# --- Dear ImGui (fetched from source; there's no distro package for it) ---
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.0   # pin a known-good docking-enabled tag; adjust as needed
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC SDL2::SDL2)

# --- nativefiledialog-extended (native Open/Save dialogs) ---
# Target name "nfd" and the API used in NativeFileDialog.cpp are both
# confirmed directly against the project's own README/examples.
FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG        master   # pin a release tag once you've confirmed which one you're building against
)
FetchContent_MakeAvailable(nfd)

# --- The ported core engine (../src/, ../src/systems/) ---
file(GLOB CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/systems/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/systems/*/*.cpp
)
# MainTest.cpp's siblings and anything test-runner-specific stay out of
# the app binary; only the engine itself is needed here.
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*Test\\.cpp$")

add_library(dis6502_core STATIC ${CORE_SOURCES})
target_include_directories(dis6502_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/systems
)
target_include_directories(dis6502_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/GSL-main/include
)

# --- The ImGui front end itself ---
add_executable(dis6502_imgui main.cpp NativeFileDialog.cpp)
target_include_directories(dis6502_imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dis6502_imgui PRIVATE dis6502_core imgui SDL2::SDL2 nfd)

# The bundled bitmap fonts (systems/{Name}/{Name}.fon) are looked up at
# runtime relative to the executable's own directory (via
# Application::GetModuleFilePath, which resolves through /proc/self/exe)
# - so they need to sit alongside the built binary, not just in the
# source tree.
add_custom_command(TARGET dis6502_imgui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/systems
        $<TARGET_FILE_DIR:dis6502_imgui>/systems
)

# Same idea for assets/ - the About dialog's two bitmaps (dis6502-icon.bmp,
# alfred.bmp), looked up the same way via GetModuleFilePath.
add_custom_command(TARGET dis6502_imgui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:dis6502_imgui>/assets
)
