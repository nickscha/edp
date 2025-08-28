/* edp.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Executable Data Packager (EDP).

This is the standlone edp.exe tool that allows you to append data to an executable via command line.

USAGE
  edp.exe your_executable.exe your_data_file

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#include "edp.h"

/* #############################################################################
 * # WIN32 Implementation
 * #############################################################################
 *
 * Because windows.h is massivly bloated and increases compilation time a lot
 * we declare only the function we really need
 */
#ifndef _WINDOWS_
EDP_WIN32_API(void *)
GetStdHandle(unsigned long nStdHandle);
EDP_WIN32_API(void)
ExitProcess(unsigned int uExitCode);
EDP_WIN32_API(int)
WriteFile(void *hFile, void *lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long *lpNumberOfBytesWritten, void *lpOverlapped);
EDP_WIN32_API(char *)
GetCommandLineA(void);

#define STD_OUTPUT_HANDLE ((unsigned long)-11)
#define STD_ERROR_HANDLE ((unsigned long)-12)
#define GENERIC_WRITE (0x40000000L)
#define FILE_END 2
#endif /* _WINDOWS_ */

EDP_API EDP_INLINE void edp_write_str(void *h, char *s)
{
    unsigned long len = 0, wr = 0;
    while (s[len])
    {
        len++;
    }
    WriteFile(h, s, len, &wr, 0);
}

EDP_API EDP_INLINE void edp_parse_cli_arguments(char **a1, char **a2)
{
    char *cmd = GetCommandLineA();
    char *buf;

    unsigned long i;
    unsigned long len = 0;

    int argc = 0;
    char *args[3] = {0};
    char *p;

    while (cmd[len])
    {
        len++;
    }
    len++;

    buf = (char *)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!buf)
    {
        ExitProcess(1);
    }

    for (i = 0; i < len; ++i)
    {
        buf[i] = cmd[i];
    }

    /* Simple argv split */
    p = buf;

    while (*p && argc < 3)
    {
        while (*p == ' ' || *p == '\t')
        {
            p++;
        }

        if (!*p)
        {
            break;
        }

        args[argc++] = p;

        if (*p == '"')
        {
            p++;

            while (*p && *p != '"')
            {
                p++;
            }

            if (*p == '"')
            {
                p++;
            }
        }
        else
        {
            while (*p && *p != ' ' && *p != '\t')
            {
                p++;
            }
        }
        if (*p)
        {
            *p++ = 0;
        }
    }
    *a1 = args[1];
    *a2 = args[2];
}

void mainCRTStartup(void)
{
    char *a1;
    char *a2;

    void *executable;
    void *data;
    unsigned long data_size;

    edp_footer foot = {0};

    char *chunk;
    unsigned long rd = 0, wr = 0;

    edp_parse_cli_arguments(&a1, &a2);

    if (!a1 || !a2)
    {
        edp_write_str(GetStdHandle(STD_OUTPUT_HANDLE), "[edp] usage: edp.exe <execubale> <data>\r\n");

        ExitProcess(1);
    }

    executable = CreateFileA(a1, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (executable == INVALID_HANDLE_VALUE)
    {
        edp_write_str(GetStdHandle(STD_ERROR_HANDLE), "[edp] error: open target\r\n");

        ExitProcess(2);
    }

    data = CreateFileA(a2, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (data == INVALID_HANDLE_VALUE)
    {
        edp_write_str(GetStdHandle(STD_ERROR_HANDLE), "[edp] error: open data\r\n");

        CloseHandle(executable);
        ExitProcess(3);
    }

    data_size = GetFileSize(data, 0);

    SetFilePointer(executable, 0, 0, FILE_END);

    chunk = (char *)VirtualAlloc(0, 65536, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    for (;;)
    {
        if (!ReadFile(data, chunk, 65536, &rd, 0) || rd == 0)
        {
            break;
        }

        if (!WriteFile(executable, chunk, rd, &wr, 0) || wr != rd)
        {
            VirtualFree(chunk, 0, MEM_RELEASE);
            ExitProcess(4);
        }
    }

    VirtualFree(chunk, 0, MEM_RELEASE);

    foot.magic[0] = 'E';
    foot.magic[1] = 'D';
    foot.magic[2] = 'P';
    foot.magic[3] = 'v';
    foot.magic[4] = '1';
    foot.magic[5] = ' ';
    foot.magic[6] = ' ';
    foot.magic[7] = ' ';
    foot.size = data_size;

    WriteFile(executable, &foot, sizeof(foot), &wr, 0);

    CloseHandle(data);
    CloseHandle(executable);

    edp_write_str(GetStdHandle(STD_OUTPUT_HANDLE), "[edp] succes: wrote data to executable.\r\n");

    ExitProcess(0);
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
