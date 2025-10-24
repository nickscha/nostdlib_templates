@echo off

REM ---------------------------------------------------------------------------
REM GCC/Clang Build
REM ---------------------------------------------------------------------------
REM
set DEF_COMPILER_FLAGS=-mconsole -march=native -mtune=native -std=c89 -pedantic -nodefaultlibs -nostdlib -mno-stack-arg-probe ^
-Xlinker /STACK:0x100000,0x100000 ^
-Xlinker /ENTRY:nostdlib_main ^
-fno-builtin -ffreestanding -fno-asynchronous-unwind-tables -fuse-ld=lld ^
-Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion -Wsign-conversion -Wmissing-field-initializers -Wuninitialized -Winit-self -Wunused -Wunused-macros -Wunused-local-typedefs

set DEF_FLAGS_LINKER=-lkernel32

REM ---------------------------------------------------------------------------
REM Build Targets
REM ---------------------------------------------------------------------------
cc -s -O2 %DEF_COMPILER_FLAGS% win32_hello_world.c -o win32_hello_world.exe %DEF_FLAGS_LINKER%
cc -s -O2 %DEF_COMPILER_FLAGS% win32_hello_cli.c -o win32_hello_cli.exe %DEF_FLAGS_LINKER%
cc -s -O2 -shared %DEF_COMPILER_FLAGS% win32_hello_dll.c -o win32_hello_dll.dll %DEF_FLAGS_LINKER%

REM ---------------------------------------------------------------------------
REM Run tests
REM ---------------------------------------------------------------------------
win32_hello_world.exe
win32_hello_cli.exe test_argument foo bar

REM ---------------------------------------------------------------------------
REM MSVC Build
REM ---------------------------------------------------------------------------
REM
REM If you don't have MSVC download the "VS Build Tools"
REM Then you have to install at least these components:
REM   - MSVC v143 - VS 2022 C++ x64/x86 build tools
REM   - C++ CMake tools for windows (contains vcvars64.bat)
REM   - Windows 10 SDK (contains the libs and include headers)
REM You might need to run:
REM   - "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
REM If you get messages like "LNK1181: cannot open input file 'kernel32.lib'" you need to set the win sdk lib path:
REM   - "LIB=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64"
set MSVC_COMPILER_FLAGS=-nologo -O2 -GS- -GR- -Gm- -EHa- -Gs9999999
set MSVC_LINKER_FLAGS=/ENTRY:nostdlib_main /SUBSYSTEM:CONSOLE /NODEFAULTLIB kernel32.lib /STACK:0x100000,0x100000 /RELEASE /OPT:REF /OPT:ICF /DEBUG:NONE

REM ---------------------------------------------------------------------------
REM Build Targets
REM ---------------------------------------------------------------------------
cl %MSVC_COMPILER_FLAGS% win32_hello_world.c /Fe:win32_hello_world_msvc.exe -link %MSVC_LINKER_FLAGS%
cl %MSVC_COMPILER_FLAGS% win32_hello_cli.c /Fe:win32_hello_cli_msvc.exe -link %MSVC_LINKER_FLAGS%

REM ---------------------------------------------------------------------------
REM Run tests
REM ---------------------------------------------------------------------------
win32_hello_world_msvc.exe
win32_hello_cli_msvc.exe test_argument foo bar
