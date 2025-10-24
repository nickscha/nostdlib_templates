/* nostdlib_templates - v0.4 - public domain data structures - nickscha 2025

C89 standard compliant, nostdlib/nolibc templates (NOSTDLIB_TEMPLATES).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/

/* ############################################################################
 * # COMPILER Settings
 * ############################################################################
 *
 * Some versions of GCC/Clang still tries to call memset/memcpy even with
 * having "-fno-builtin" set.
 *
 * Additionally for MSVC (cl.exe) the "_fltused" has to be specified for a
 * working nostdlib project.
 */
#ifdef _MSC_VER
int _fltused = 0;
#endif

#ifdef _MSC_VER
#pragma function(memset)
#endif
void *memset(void *dest, int c, unsigned int count)
{
  char *bytes = (char *)dest;
  while (count--)
  {
    *bytes++ = (char)c;
  }
  return dest;
}

#ifdef _MSC_VER
#pragma function(memcpy)
#endif
void *memcpy(void *dest, void *src, unsigned int count)
{
  char *dest8 = (char *)dest;
  char *src8 = (char *)src;
  while (count--)
  {
    *dest8++ = *src8++;
  }
  return dest;
}

/* ############################################################################
 * # Manually define windows.h function we actually need
 * ############################################################################
 *
 * "windows.h" is huge (even with #NO_XXX #LEAN_.. defines).
 * This reduces compilation speed (especially with GCC/Clang) a lot since they,
 * compared to MSVC, do not precompile the windows header.
 *
 * Since most programs only use a small amount of win32 functions we can just
 * manually define them. This also lets us replace the DWORD, PTR, ... types
 * with standard C types.
 */
#ifndef _WINDOWS_
#define WIN32_API(r) __declspec(dllimport) r __stdcall

#define STD_OUTPUT_HANDLE ((unsigned long)-11)

WIN32_API(void *)
GetStdHandle(unsigned long nStdHandle);
WIN32_API(int)
WriteConsoleA(void *hConsoleOutput, void *lpBuffer, unsigned long nNumberOfCharsToWrite, unsigned long *lpNumberOfCharsWritten, void *lpReserved);
WIN32_API(char *)
GetCommandLineA(void);

#endif /* _WINDOWS_ */

static unsigned int my_strlen(char *str)
{
  char *s = str;

  while (*s)
  {
    s++;
  }
  return (unsigned int)(s - str);
}

/* ############################################################################
 * # C-Like main function
 * ############################################################################
 *
 * The "mainCRTStartup" will read the command line arguments parsed and call
 * this function.
 */
int start(int argc, char **argv)
{
  int i;

  unsigned long written;
  void *hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  WriteConsoleA(hConsole, "cli arguments:\n", 15, &written, 0);

  for (i = 0; i < argc; ++i)
  {
    WriteConsoleA(hConsole, argv[i], (unsigned long)my_strlen(argv[i]), &written, 0);
    WriteConsoleA(hConsole, "\n", 1, &written, 0);
  }

  return 0;
}

/* ############################################################################
 * # Command line parsing
 * ############################################################################
 *
 * Basic (non-quoted) command line parser
 * Converts "program.exe arg1 arg2" -> argc=3, argv={"program.exe","arg1","arg2",NULL}
 * In-place: modifies the command line buffer.
 */
static int win32_parse_command_line(char *cmdline, char ***argv_out)
{
  static char *argv_local[64]; /* up to 63 args */
  int argc = 0;

  while (*cmdline)
  {
    /* skip whitespace */
    while (*cmdline == ' ' || *cmdline == '\t')
    {
      cmdline++;
    }

    if (!*cmdline)
    {
      break;
    }

    if (argc < 63)
    {
      argv_local[argc++] = cmdline;
    }

    /* parse token (basic, no quote handling) */
    while (*cmdline && *cmdline != ' ' && *cmdline != '\t')
    {
      cmdline++;
    }

    if (*cmdline)
    {
      *cmdline++ = '\0';
    }
  }

  argv_local[argc] = (char *)0;
  *argv_out = argv_local;

  return argc;
}

/* ############################################################################
 * # Main entry point
 * ############################################################################
 */
#ifdef __clang__
#elif __GNUC__
__attribute((externally_visible))
#endif
#ifdef __i686__
__attribute((force_align_arg_pointer))
#endif
int nostdlib_main(void)
{
  char *cmdline = GetCommandLineA();

  char **argv;
  int argc = win32_parse_command_line(cmdline, &argv);

  return start(argc, argv);
}

/*
   ------------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2025 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
