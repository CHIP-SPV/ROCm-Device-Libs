/*===--------------------------------------------------------------------------
 *                   ROCm Device Libraries
 *
 * This file is distributed under the University of Illinois Open Source
 * License. See LICENSE.TXT for details.
 *===------------------------------------------------------------------------*/

#include "mathF.h"
#include "trigpiredF.h"

CONSTATTR float
MATH_MANGLE(cospi)(float x)
{
    float ax = BUILTIN_ABS_F32(x);
    struct redret r = MATH_PRIVATE(trigpired)(ax);
    struct scret sc = MATH_PRIVATE(sincospired)(r.hi);
    sc.s = -sc.s;

    float c =  (r.i & 1) != 0 ? sc.s : sc.c;
    c = AS_FLOAT(AS_INT(c) ^ (r.i > 1 ? 0x80000000 : 0));

    if (!FINITE_ONLY_OPT()) {
        c = BUILTIN_ISFINITE_F32(ax) ? c : QNAN_F32;
    }

    return c;
}

