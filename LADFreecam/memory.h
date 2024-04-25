#include "pch.h"

namespace mem
{
    void Patch(BYTE* dst, BYTE* src, unsigned int size);
    void Nop(BYTE* dst, unsigned int size);

   // DWORD jmpBackAddy;
}