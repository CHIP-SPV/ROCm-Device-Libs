/*===--------------------------------------------------------------------------
 *                   ROCm Device Libraries
 *
 * This file is distributed under the University of Illinois Open Source
 * License. See LICENSE.TXT for details.
 *===------------------------------------------------------------------------*/

#include "mathD.h"

double
MATH_MANGLE(fract)(double x, __private double *ip)
{
    double i = BUILTIN_FLOOR_F64(x);

    double f;
    if (__oclc_ISA_version < 8000) {
        f = BUILTIN_MIN_F64(x - i, 0x1.fffffffffffffp-1);
        if (!FINITE_ONLY_OPT()) {
            f = BUILTIN_ISNAN_F64(x) ? x : f;
            f = BUILTIN_ISINF_F64(x) ? 0.0 : f;
        }
    } else {
        f = BUILTIN_FRACTION_F64(x);
    }

    *ip = i;
    return f;
}

