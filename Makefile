# Makefile for the ported dis6502 core engine.
#
# This builds the same thing that was compiled, linked, and run against
# the real unit test suite in the sandbox: every file under src/ and
# src/systems/, minus the test-runner's own driver.
#
# Usage:
#   make                # builds obj/*.o, test_runner_bin, colorizer_verify_bin, assembler_verify_bin
#   make test            # builds and runs the real unit test suite
#   make clean
#
# GSL_INC below points at the bundled header-only Guidelines Support
# Library, shipped inside this zip at lib/GSL-main/include.

CXX      := g++
CXXSTD   := -std=c++20
SRC_DIR  := src
OBJ_DIR  := obj
GSL_INC  ?= lib/GSL-main/include

INCLUDES := -I$(SRC_DIR) -I$(SRC_DIR)/systems -I$(SRC_DIR)/systems/atari5200 -I$(SRC_DIR)/systems/atari800 -I$(SRC_DIR)/systems/c64 -I$(SRC_DIR)/systems/oric -I$(SRC_DIR)/systems/unknown -I$(GSL_INC) -I.
CXXFLAGS := $(CXXSTD) $(INCLUDES) -Wall -Wextra -Wno-unused-parameter

# Every core engine source file (src/*.cpp and src/systems/**/*.cpp).
CORE_SOURCES := $(wildcard $(SRC_DIR)/*.cpp) \
                 $(wildcard $(SRC_DIR)/systems/*.cpp) \
                 $(wildcard $(SRC_DIR)/systems/*/*.cpp)
CORE_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CORE_SOURCES))

.PHONY: all test clean

all: test_runner_bin colorizer_verify_bin smoke_test_bin assembler_verify_bin fnt_verify_bin sprite_verify_bin

# --- pattern rule: mirror src/'s subdirectory structure under obj/ ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- the three verification programs from the porting session ---
$(OBJ_DIR)/test_runner.o: test_runner.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/colorizer_verify.o: colorizer_verify.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/assembler_verify.o: assembler_verify.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# fnt_verify has no core-engine dependency (FntFont.h is pure byte
# parsing) so it builds and links directly, unlike the other verify
# programs above.
fnt_verify_bin: fnt_verify.cpp
	$(CXX) $(CXXSTD) -I. $< -o $@

sprite_verify_bin: sprite_verify.cpp
	$(CXX) $(CXXSTD) -I. $< -o $@

$(OBJ_DIR)/smoke_test.o: smoke_test.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_runner_bin: $(OBJ_DIR)/test_runner.o $(CORE_OBJECTS)
	$(CXX) $(CXXSTD) $^ -o $@

colorizer_verify_bin: $(OBJ_DIR)/colorizer_verify.o $(CORE_OBJECTS)
	$(CXX) $(CXXSTD) $^ -o $@

assembler_verify_bin: $(OBJ_DIR)/assembler_verify.o $(CORE_OBJECTS)
	$(CXX) $(CXXSTD) $^ -o $@

smoke_test_bin: $(OBJ_DIR)/smoke_test.o $(CORE_OBJECTS)
	$(CXX) $(CXXSTD) $^ -o $@

# --- run the real unit test suite against tst/suite/ ---
# TestPaths::GetSuitePath() resolves "../../../tst/suite" relative to
# the *binary's own* directory (via /proc/self/exe), so the runner
# needs to sit exactly 3 directories below wherever tst/ lives (tst/ is
# bundled directly in this zip, alongside src/). This recreates that
# layout with a symlink rather than moving tst/ itself.
test: test_runner_bin
	@mkdir -p run/a/b/c
	@cp test_runner_bin run/a/b/c/
	@ln -sfn "$(abspath tst)" run/tst
	@cd run/a/b/c && ./test_runner_bin

clean:
	rm -rf $(OBJ_DIR) run test_runner_bin colorizer_verify_bin smoke_test_bin assembler_verify_bin fnt_verify_bin sprite_verify_bin
