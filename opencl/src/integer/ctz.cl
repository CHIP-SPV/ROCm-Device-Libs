/*===--------------------------------------------------------------------------
 *                   ROCm Device Libraries
 *
 * This file is distributed under the University of Illinois Open Source
 * License. See LICENSE.TXT for details.
 *===------------------------------------------------------------------------*/

#include "int.h"

#define UEXPATTR __attribute__((always_inline, overloadable, const))
UEXP(char,ctz)
UEXP(uchar,ctz)
UEXP(short,ctz)
UEXP(ushort,ctz)
UEXP(int,ctz)
UEXP(uint,ctz)
UEXP(long,ctz)
UEXP(ulong,ctz)

UEXPATTR char
ctz(char x)
{
    uint y = (uint)(uchar)x;
    return (char)min(__ockl_ctz_u32(y), 8u);
}

UEXPATTR uchar
ctz(uchar x)
{
    uint y = (uint)x;
    return (uchar)min(__ockl_ctz_u32(y), 8u);
}

UEXPATTR short
ctz(short x)
{
    uint y = (uint)(ushort)x;
    return (short)min(__ockl_ctz_u32(y), 16u);
}

UEXPATTR ushort
ctz(ushort x)
{
    uint y = (uint)x;
    return (ushort)min(__ockl_ctz_u32(y), 16u);
}

UEXPATTR int
ctz(int x)
{
    return (int)__ockl_ctz_u32((uint)x);
}

UEXPATTR uint
ctz(uint x)
{
    return __ockl_ctz_u32(x);
}

UEXPATTR long
ctz(long x)
{
    return (long)__ockl_ctz_u64((ulong)x);
}

UEXPATTR ulong
ctz(ulong x)
{
    return __ockl_ctz_u64(x);
}

