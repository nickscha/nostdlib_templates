#!/usr/bin/env bash
set -e

# -----------------------------------------------------------------------------
# Compiler and linker configuration (Linux nostdlib build)
# -----------------------------------------------------------------------------
DEF_COMPILER_FLAGS="-march=native -mtune=native -std=c89 -pedantic \
-nodefaultlibs -nostdlib \
-fno-builtin -ffreestanding -fno-asynchronous-unwind-tables \
-fuse-ld=lld \
-Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion \
-Wsign-conversion -Wmissing-field-initializers -Wuninitialized -Winit-self \
-Wunused -Wunused-macros -Wunused-local-typedefs"

DEF_LINKER_FLAGS="-static -s"

# -----------------------------------------------------------------------------
# Entry point function for nostdlib binaries
# -----------------------------------------------------------------------------
ENTRY_POINT="-Wl,-e,nostdlib_main"

# -----------------------------------------------------------------------------
# Build Targets
# -----------------------------------------------------------------------------
cc -O2 $DEF_COMPILER_FLAGS $ENTRY_POINT linux_hello_world.c -o linux_hello_world $DEF_LINKER_FLAGS
cc -O2 $DEF_COMPILER_FLAGS $ENTRY_POINT linux_hello_cli.c -o linux_hello_cli $DEF_LINKER_FLAGS

# -----------------------------------------------------------------------------
# Run tests
# -----------------------------------------------------------------------------
./linux_hello_world
./linux_hello_cli test_argument foo bar
