# nostdlib_templates
C89 standard compliant, nostdlib/nolibc templates (NOSTDLIB_TEMPLATES).

> [!WARNING]
> THIS PROJECT IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS PROJECT AT YOUR OWN RISK!

<p align="center">
  <a href="https://github.com/nickscha/nostdlib_templates/releases">
    <img src="https://img.shields.io/github/v/release/nickscha/nostdlib_templates?style=flat-square&color=blue" alt="Latest Release">
  </a>
  <a href="https://github.com/nickscha/nostdlib_templates/releases">
    <img src="https://img.shields.io/github/downloads/nickscha/nostdlib_templates/total?style=flat-square&color=brightgreen" alt="Downloads">
  </a>
  <a href="https://opensource.org/licenses/MIT">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square" alt="License">
  </a>
  <img src="https://img.shields.io/badge/Standard-C89-orange?style=flat-square" alt="C Standard">
  <img src="https://img.shields.io/badge/nolib-nostdlib-lightgrey?style=flat-square" alt="nostdlib">
</p>

## Example Programs

| Platform             | Example                                      | Description                                                      | Entry Point                                       | Notes                                                 |
| -------------------- | -------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------- |
| **Linux**         | [`linux_hello_world.c`](linux_hello_world.c) | Minimal “Hello World” using raw Linux syscalls (`write`, `exit`) | `_start` → `nostdlib_main`                        | Works on `x86_64`, `i386`, `ARM`, `AArch64`, etc.     |
| **Linux (CLI)**   | [`linux_hello_cli.c`](linux_hello_cli.c)     | Prints all command-line arguments using direct syscalls          | `_start` → `nostdlib_main(int argc, char **argv)` | Demonstrates manual `argc`/`argv` parsing from stack. |
| **Windows**       | [`win32_hello_world.c`](win32_hello_world.c) | Minimal Win32 console “Hello World” without CRT or libc          | `nostdlib_main`                                   | Uses only `WriteConsoleA` + `ExitProcess`.            |
| **Windows (CLI)** | [`win32_hello_cli.c`](win32_hello_cli.c)     | Prints command-line arguments via raw Win32 API                  | `nostdlib_main`                                   | Uses `GetCommandLineA` and manual parsing.            |

## Build & Run

On Linux use:

```bash
chmod +x linux_build.sh
./linux_build.sh
```

On Windows use:

```bat
win32_build.bat
```

## "nostdlib" Motivation & Purpose

nostdlib is a lightweight, minimalistic approach to C development that removes dependencies on the standard library. The motivation behind this project is to provide developers with greater control over their code by eliminating unnecessary overhead, reducing binary size, and enabling deployment in resource-constrained environments.

Many modern development environments rely heavily on the standard library, which, while convenient, introduces unnecessary bloat, security risks, and unpredictable dependencies. nostdlib aims to give developers fine-grained control over memory management, execution flow, and system calls by working directly with the underlying platform.

### Benefits

#### Minimal overhead
By removing the standard library, nostdlib significantly reduces runtime overhead, allowing for faster execution and smaller binary sizes.

#### Increased security
Standard libraries often include unnecessary functions that increase the attack surface of an application. nostdlib mitigates security risks by removing unused and potentially vulnerable components.

#### Reduced binary size
Without linking to the standard library, binaries are smaller, making them ideal for embedded systems, bootloaders, and operating systems where storage is limited.

#### Enhanced performance
Direct control over system calls and memory management leads to performance gains by eliminating abstraction layers imposed by standard libraries.

#### Better portability
By relying only on fundamental system interfaces, nostdlib allows for easier porting across different platforms without worrying about standard library availability.
