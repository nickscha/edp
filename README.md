# edp
A C89 standard compliant, single header, nostdlib (no C Standard Library) Executable Data Packager (EDP).

<p align="center">
<a href="https://github.com/nickscha/edp"><img src="assets/edp.png"></a>
</p>

For more information please look at the "edp.h" file or take a look at the "examples" or "tests" folder.

> [!WARNING]
> THIS PROJECT IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS PROJECT AT YOUR OWN RISK!

## Quick Start

Download or clone edp.h && edp.c and include it in your project.

The project contains two parts:
- **edp.c**: This is the nostdlib standalone program that allow you to append data to an executable via **edp.exe myexecutable.exe my_data_file**
- **edp.h**: This lets you load appended executable data during runtime via **edp_data_load**


### Using the edp.c appender:

This standlone nostdlib c89 program allows you to append data to an executable.

#### Compile the edp.c or download from github actions or run the "build.bat" in the tests folder.

In this compilation we are extra strict about warning and treat everything as an error.
Also we set here the required nostdlib compilation flags.

Clang/GCC:

```bat
set DEF_COMPILER_FLAGS=-mconsole -march=native -mtune=native -std=c89 -pedantic -nodefaultlibs -nostdlib -mno-stack-arg-probe -Xlinker /STACK:0x100000,0x100000 ^
-fno-builtin -ffreestanding -fno-asynchronous-unwind-tables -fuse-ld=lld ^
-Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion -Wsign-conversion -Wuninitialized -Winit-self -Wunused -Wunused-function -Wunused-macros -Wunused-parameter -Wunused-value -Wunused-variable -Wunused-local-typedefs

set DEF_FLAGS_LINKER=-lkernel32
cc -s -Os %DEF_COMPILER_FLAGS% edp.c -o edp.exe %DEF_FLAGS_LINKER%
```

#### Run the edp.exe:

This will append the specified data file into the executable.

```sh
edp.exe myexecutable.exe my_data_file
```

### Using the edp.h loader:

This header allows you to load the appended executable data.

```C
#include "edp.h" /* Executable Data Packager */

int main() {

    void *payload = (void *)0;
    unsigned int payload_size = 0;

    if (!edp_data_load(&payload, &payload_size))
    {   
        /* No data has been appended to the executable */
        return 1;
    }

    /* Now you can use the payload and payload_size */

    /* Free the executable payload */
    edp_data_free(payload);

    return 0;
}
```

## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/edp_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.

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
