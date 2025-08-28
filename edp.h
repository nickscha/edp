/* edp.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Executable Data Packager (EDP).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef EDP_H
#define EDP_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define EDP_INLINE inline
#define EDP_API static
#elif defined(__GNUC__) || defined(__clang__)
#define EDP_INLINE __inline__
#define EDP_API static
#elif defined(_MSC_VER)
#define EDP_INLINE __inline
#define EDP_API static
#else
#define EDP_INLINE
#define EDP_API static
#endif

typedef struct edp_footer
{
  char magic[8];     /* "EDPv1   " */
  unsigned int size; /* payload size */

} edp_footer;

/* #############################################################################
 * # WIN32 Implementation
 * #############################################################################
 *
 * Because windows.h is massivly bloated and increases compilation time a lot
 * we declare only the function we really need
 */
#ifndef _WINDOWS_
#define EDP_WIN32_API(r) __declspec(dllimport) r __stdcall

#define INVALID_HANDLE_VALUE ((void *)-1)
#define GENERIC_READ (0x80000000L)
#define FILE_SHARE_READ 0x00000001
#define OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define INVALID_FILE_SIZE ((unsigned long)0xFFFFFFFF)
#define FILE_BEGIN 0
#define INVALID_SET_FILE_POINTER ((unsigned long)-1)
#define MEM_COMMIT 0x00001000
#define MEM_RELEASE 0x8000
#define MEM_RESERVE 0x00002000
#define PAGE_READWRITE 0x04

EDP_WIN32_API(int)
CloseHandle(void *hObject);

EDP_WIN32_API(unsigned long)
GetLastError(void);

EDP_WIN32_API(unsigned long)
GetModuleFileNameA(void *hModule, char *lpFilename, unsigned long nSize);

EDP_WIN32_API(void *)
CreateFileA(char *lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, void *lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void *hTemplateFile);

EDP_WIN32_API(unsigned long)
GetFileSize(void *hFile, unsigned long *lpFileSizeHigh);

EDP_WIN32_API(int)
ReadFile(void *hFile, void *lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long *lpNumberOfBytesRead, void *lpOverlapped);

EDP_WIN32_API(unsigned long)
SetFilePointer(void *hFile, long lDistanceToMove, long *lpDistanceToMoveHigh, unsigned long dwMoveMethod);

EDP_WIN32_API(void *)
VirtualAlloc(void *lpAddress, unsigned long dwSize, unsigned longflAllocationType, unsigned longflProtect);

EDP_WIN32_API(int)
VirtualFree(void *lpAddress, unsigned long dwSize, unsigned long dwFreeType);

#endif /* _WINDOWS_ */

EDP_API EDP_INLINE int edp_data_load(void **out_data, unsigned int *out_size)
{
  char path[512];

  unsigned long size_footer = (unsigned long)sizeof(edp_footer);
  unsigned long size_file = 0;
  unsigned long size_readed = 0;

  void *handle;

  edp_footer footer = {0};

  if (!out_data || !out_size || !GetModuleFileNameA(0, path, sizeof(path)))
  {
    return 0;
  }

  handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (handle == INVALID_HANDLE_VALUE)
  {
    return 0;
  }

  size_file = GetFileSize(handle, 0);

  if ((size_file == INVALID_FILE_SIZE) ||
      (size_file < size_footer) ||
      (SetFilePointer(handle, (long)(size_file - size_footer), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER && GetLastError() != 0l) ||
      (!ReadFile(handle, &footer, sizeof(footer), &size_readed, 0) || size_readed != sizeof(footer)) ||
      (footer.magic[0] != 'E' || footer.magic[1] != 'D' || footer.magic[2] != 'P' || footer.magic[3] != 'v' || footer.magic[4] != '1' || footer.magic[5] != ' ' || footer.magic[6] != ' ' || footer.magic[7] != ' ') ||
      (footer.size > size_file - size_footer))
  {
    CloseHandle(handle);

    return 0;
  }

  *out_size = footer.size;
  *out_data = (void *)VirtualAlloc(0, footer.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

  if (!*out_data)
  {
    CloseHandle(handle);

    return 0;
  }

  if ((SetFilePointer(handle, (long)(size_file - size_footer - footer.size), 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER && GetLastError() != 0l) ||
      (!ReadFile(handle, *out_data, footer.size, &size_readed, 0) || size_readed != footer.size))
  {
    VirtualFree(*out_data, 0, MEM_RELEASE);
    CloseHandle(handle);

    return 0;
  }

  CloseHandle(handle);

  return 1;
}

EDP_API EDP_INLINE void edp_data_free(void *p)
{
  if (p)
  {
    VirtualFree(p, 0, MEM_RELEASE);
  }
}

#endif /* EDP_H */

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
