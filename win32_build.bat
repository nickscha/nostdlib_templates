@echo off
set DEF_COMPILER_FLAGS=-mconsole -march=native -mtune=native -std=c89 -pedantic -nodefaultlibs -nostdlib -mno-stack-arg-probe -Xlinker /STACK:0x100000,0x100000 ^
-fno-builtin -ffreestanding -fno-asynchronous-unwind-tables -fuse-ld=lld ^
-Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion -Wsign-conversion -Wmissing-field-initializers -Wuninitialized -Winit-self -Wunused -Wunused-macros -Wunused-local-typedefs

set DEF_FLAGS_LINKER=-lkernel32

cc -s -O2 %DEF_COMPILER_FLAGS% win32_hello_world.c -o win32_hello_world.exe %DEF_FLAGS_LINKER%
cc -s -O2 %DEF_COMPILER_FLAGS% win32_hello_cli.c -o win32_hello_cli.exe %DEF_FLAGS_LINKER%

win32_hello_world.exe
win32_hello_cli.exe test_argument
