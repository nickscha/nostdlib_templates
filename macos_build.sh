#!/usr/bin/env bash
set -e

# -----------------------------------------------------------------------------
# Compiler and linker configuration (Macos nostdlib build)
# -----------------------------------------------------------------------------
DEF_COMPILER_FLAGS="-march=native -mtune=native -std=c89 -pedantic \
-nostdlib \
-fno-builtin -ffreestanding -fno-asynchronous-unwind-tables \
-fuse-ld=lld \
-Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion \
-Wsign-conversion -Wmissing-field-initializers -Wuninitialized -Winit-self \
-Wunused -Wunused-macros -Wunused-local-typedefs"

DEF_LINKER_FLAGS="-Wl,-dead_strip"

# -----------------------------------------------------------------------------
# Entry point function for nostdlib binaries
# -----------------------------------------------------------------------------
ENTRY_POINT="-Wl,-e,nostdlib_main"

# -----------------------------------------------------------------------------
# Build Targets
# -----------------------------------------------------------------------------
cc -O2 $DEF_COMPILER_FLAGS $ENTRY_POINT macos_hello_world.c -o macos_hello_world $DEF_LINKER_FLAGS
cc -O2 $DEF_COMPILER_FLAGS $ENTRY_POINT macos_hello_cli.c -o macos_hello_cli $DEF_LINKER_FLAGS

# -----------------------------------------------------------------------------
# Run tests
# -----------------------------------------------------------------------------
./macos_hello_world
./macos_hello_cli test_argument foo bar
