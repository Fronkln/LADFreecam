#include "pch.h"
#include "memory.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
    DWORD oldProtect;

    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(dst, src, size);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
    DWORD oldProtect;

    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(dst, 0x90, size);
}



DWORD jmpBackAddy;