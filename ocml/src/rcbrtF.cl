/*===--------------------------------------------------------------------------
 *                   ROCm Device Libraries
 *
 * This file is distributed under the University of Illinois Open Source
 * License. See LICENSE.TXT for details.
 *===------------------------------------------------------------------------*/

#include "mathF.h"

CONSTATTR float
MATH_MANGLE(rcbrt)(float x)
{
    if (DAZ_OPT()) {
        x = BUILTIN_CANONICALIZE_F32(x);
    }

    float ax = BUILTIN_ABS_F32(x);
    
    if (!DAZ_OPT()) {
        ax = BUILTIN_CLASS_F32(x, CLASS_NSUB|CLASS_PSUB) ?
             BUILTIN_FLDEXP_F32(ax, 24) : ax;
    }

    float z = BUILTIN_EXP2_F32(-0x1.555556p-2f * BUILTIN_LOG2_F32(ax));
    z = MATH_MAD(MATH_MAD(z*z, -z*ax, 1.0f), 0x1.555556p-2f*z, z);

    if (!DAZ_OPT()) {
        z = BUILTIN_CLASS_F32(x, CLASS_NSUB|CLASS_PSUB) ?
            BUILTIN_FLDEXP_F32(z, 8) : z;
    }

    float xi = MATH_FAST_RCP(x);
    z = (isnan(x) || x == 0.0f || isinf(x)) ? xi : z;

    return BUILTIN_COPYSIGN_F32(z, x);
}

