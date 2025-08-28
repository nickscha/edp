# edp
A C89 standard compliant, single header, nostdlib (no C Standard Library) Executable Data Packager (EDP).

<p align="center">
<a href="https://github.com/nickscha/edp"><img src="assets/edp.png"></a>
</p>

For more information please look at the "edp.h" file or take a look at the "examples" or "tests" folder.

> [!WARNING]
> THIS PROJECT IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS PROJECT AT YOUR OWN RISK!

<p align="center">
  <a href="https://github.com/nickscha/edp/releases">
    <img src="https://img.shields.io/github/v/release/nickscha/edp?style=flat-square&color=blue" alt="Latest Release">
  </a>
  <a href="https://github.com/nickscha/edp/releases">
    <img src="https://img.shields.io/github/downloads/nickscha/edp/total?style=flat-square&color=brightgreen" alt="Downloads">
  </a>
  <a href="https://opensource.org/licenses/MIT">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square" alt="License">
  </a>
  <img src="https://img.shields.io/badge/Standard-C89-orange?style=flat-square" alt="C Standard">
  <img src="https://img.shields.io/badge/nolib-nostdlib-lightgrey?style=flat-square" alt="nostdlib">
</p>

<p align="center">
  <b>EDP</b> lets you <b>append arbitrary data to an executable</b> and <b>access it at runtime</b>, 
  all without relying on the C Standard Library.
</p>

## **Features**
- **C89 compliant** — portable and legacy-friendly  
- **Single-header API** — just include `edp.h`  
- **nostdlib** — no dependency on the C Standard Library  
- **Minimal binary size** — optimized for small executables  
- **Cross-platform** — Windows, Linux, and more (WIP)  
- **Strict compilation** — built with aggressive warnings & safety checks  

### **1. Get EDP**
Clone the repository or download the standalone files:

```bash
git clone https://github.com/nickscha/edp.git
```

The project consists of two main components:

| File       | Purpose |
|-----------|----------------------|
| **edp.c** | Standalone nostdlib appender — append arbitrary data to an executable |
| **edp.h** | Loader API — read the appended data directly at runtime |

---

### **2. Building the Appender (`edp.c`)**

The appender is **freestanding** and **nostdlib**. Compile using **Clang** or **GCC** with strict flags:

```bat
set DEF_COMPILER_FLAGS=-mconsole -march=native -mtune=native -std=c89 -pedantic -nodefaultlibs -nostdlib -mno-stack-arg-probe -Xlinker /STACK:0x100000,0x100000 ^
-fno-builtin -ffreestanding -fno-asynchronous-unwind-tables -fuse-ld=lld ^
-Wall -Wextra -Werror -Wvla -Wconversion -Wdouble-promotion -Wsign-conversion -Wuninitialized -Winit-self ^
-Wunused -Wunused-function -Wunused-macros -Wunused-parameter -Wunused-value -Wunused-variable -Wunused-local-typedefs

set DEF_FLAGS_LINKER=-lkernel32
cc -s -Os %DEF_COMPILER_FLAGS% edp.c -o edp.exe %DEF_FLAGS_LINKER%
```

Or download prebuilt binaries from  
**[GitHub Actions → Latest Artifacts](https://github.com/nickscha/edp/actions)**.

---

### **3. Appending Data to an Executable**

Once compiled, append arbitrary data:

```bash
edp.exe my_program.exe my_payload.bin
```

This appends `my_payload.bin` to the end of `my_program.exe`.  
The data can then be accessed at runtime using `edp.h`.

---

### **4. Loading Appended Data (`edp.h`)**

Here’s how you load the embedded payload:

```c
#include "edp.h"  /* Executable Data Packager */

int main(void) {
    void *payload = 0;
    unsigned int payload_size = 0;

    if (!edp_data_load(&payload, &payload_size)) {
        /* No data was appended */
        return 1;
    }

    /* Use payload & payload_size here */

    /* Free memory after use */
    edp_data_free(payload);

    return 0;
}
```

---

## **Motivation**

**EDP** exists to give developers a **minimalistic, reliable, and portable** solution for:
- Embedding configuration, assets, or metadata inside executables  
- Accessing that data without external files  
- Operating **without the C Standard Library**  

This approach is especially useful for:
- Self-contained tools  
- Licensing & authentication systems  
- Bootloaders & embedded systems  
- Portable cross-platform executables  
- Assets

---

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
