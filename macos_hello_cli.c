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
 */
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
 * # Minimal Mcos syscall interface
 * ############################################################################
 *
 * macOS syscall numbers differ from Linux; x86_64 uses syscall instruction
 * with number in RAX. Apple Silicon (aarch64) uses "svc 0".
 */
#if defined(__x86_64__)

static long sys_write(unsigned int fd, const char *buf, unsigned long len)
{
    long ret;
    __asm__ volatile(
        "movq $0x2000004, %%rax\n\t" /* SYS_write */
        "movq %1, %%rdi\n\t"
        "movq %2, %%rsi\n\t"
        "movq %3, %%rdx\n\t"
        "syscall"
        : "=a"(ret)
        : "r"((long)fd), "r"(buf), "r"(len)
        : "rcx", "r11", "memory");
    return ret;
}

static void sys_exit(int code)
{
    __asm__ volatile(
        "movq $0x2000001, %%rax\n\t" /* SYS_exit */
        "movq %0, %%rdi\n\t"
        "syscall"
        :
        : "r"((long)code)
        : "memory");
    for (;;)
        ;
}

#elif defined(__aarch64__)

static long sys_write(unsigned int fd, const char *buf, unsigned long len)
{
    register long x0 __asm__("x0") = fd;
    register const char *x1 __asm__("x1") = buf;
    register long x2 __asm__("x2") = (long)len;
    register long x16 __asm__("x16") = 0x2000004; /* SYS_write */
    __asm__ volatile(
        "svc 0"
        : "+r"(x0)
        : "r"(x1), "r"(x2), "r"(x16)
        : "memory");
    return x0;
}

static void sys_exit(int code)
{
    register long x0 __asm__("x0") = code;
    register long x16 __asm__("x16") = 0x2000001; /* SYS_exit */
    __asm__ volatile("svc 0" : : "r"(x0), "r"(x16) : "memory");
    for (;;)
        ;
}

#else
#error "Unsupported architecture: macOS x86_64 or arm64 only"
#endif

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
    char *prefix = "argv: ";

    for (i = 0; i < argc; ++i)
    {
        char *s;

        sys_write(1, prefix, 6);

        s = argv[i];

        while (*s)
        {
            sys_write(1, s, 1);
            s++;
        }

        sys_write(1, "\n", 1);
    }

    return 0;
}

/* ############################################################################
 * # Main entry point
 * ############################################################################
 */
#ifdef __clang__
__attribute__((visibility("default")))
#elif __GNUC__
__attribute((externally_visible))
#endif
#ifdef __i686__
__attribute((force_align_arg_pointer))
#endif
int nostdlib_main(void)
{
    int ret;

#if defined(__x86_64__)
    int argc;
    char **argv;
    __asm__ volatile(
        "mov (%%rsp), %0\n\t"
        "lea 8(%%rsp), %1\n\t"
        : "=r"(argc), "=r"(argv));
#elif defined(__aarch64__)
    long argc;
    char **argv;
    __asm__ volatile(
        "ldr %0, [sp]\n\t"
        "add %1, sp, #8\n\t"
        : "=r"(argc), "=r"(argv));
#endif

    ret = start((int)argc, argv);

    sys_exit(ret);
    return ret;
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
