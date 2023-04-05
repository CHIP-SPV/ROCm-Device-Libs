/*===--------------------------------------------------------------------------
 *                   ROCm Device Libraries
 *
 * This file is distributed under the University of Illinois Open Source
 * License. See LICENSE.TXT for details.
 *===------------------------------------------------------------------------*/

#ifndef IRIF_H
#define IRIF_H

#pragma OPENCL EXTENSION cl_khr_fp16 : enable

// Floating point patterns
#define SIGNBIT_SP32      (int)0x80000000
#define EXSIGNBIT_SP32    0x7fffffff
#define EXPBITS_SP32      0x7f800000
#define MANTBITS_SP32     0x007fffff
#define ONEEXPBITS_SP32   0x3f800000
#define TWOEXPBITS_SP32   0x40000000
#define HALFEXPBITS_SP32  0x3f000000
#define IMPBIT_SP32       0x00800000
#define QNANBITPATT_SP32  0x7fc00000
#define PINFBITPATT_SP32  0x7f800000
#define NINFBITPATT_SP32  (int)0xff800000
#define EXPBIAS_SP32      127
#define EXPSHIFTBITS_SP32 23
#define BIASEDEMIN_SP32   1
#define EMIN_SP32         -126
#define BIASEDEMAX_SP32   254
#define EMAX_SP32         127
#define MANTLENGTH_SP32   24
#define BASEDIGITS_SP32   7

// Bit patterns
#define SIGNBIT_DP64      0x8000000000000000L
#define EXSIGNBIT_DP64    0x7fffffffffffffffL
#define EXPBITS_DP64      0x7ff0000000000000L
#define MANTBITS_DP64     0x000fffffffffffffL
#define ONEEXPBITS_DP64   0x3ff0000000000000L
#define TWOEXPBITS_DP64   0x4000000000000000L
#define HALFEXPBITS_DP64  0x3fe0000000000000L
#define IMPBIT_DP64       0x0010000000000000L
#define QNANBITPATT_DP64  0x7ff8000000000000L
#define INDEFBITPATT_DP64 0xfff8000000000000L
#define PINFBITPATT_DP64  0x7ff0000000000000L
#define NINFBITPATT_DP64  0xfff0000000000000L
#define EXPBIAS_DP64      1023
#define EXPSHIFTBITS_DP64 52
#define BIASEDEMIN_DP64   1
#define EMIN_DP64         -1022
#define BIASEDEMAX_DP64   2046
#define EMAX_DP64         1023
#define LAMBDA_DP64       1.0e300
#define MANTLENGTH_DP64   53
#define BASEDIGITS_DP64   15

// Floating point patterns
#define SIGNBIT_HP16      0x8000
#define EXSIGNBIT_HP16    0x7fff
#define EXPBITS_HP16      0x7c00
#define MANTBITS_HP16     0x03ff
#define ONEEXPBITS_HP16   0x3c00
#define TWOEXPBITS_HP16   0x4000
#define HALFEXPBITS_HP16  0x3800
#define IMPBIT_HP16       0x0400
#define QNANBITPATT_HP16  0x7e00
#define PINFBITPATT_HP16  0x7c00
#define NINFBITPATT_HP16  0xfc00
#define EXPBIAS_HP16      15
#define EXPSHIFTBITS_HP16 10
#define BIASEDEMIN_HP16   1
#define EMIN_HP16         -14
#define BIASEDEMAX_HP16   30
#define EMAX_HP16         15
#define MANTLENGTH_HP16   11
#define BASEDIGITS_HP16   5

static const unsigned char PIBITS_TBL[] = {
    224, 241, 27, 193, 12, 88, 33, 116, 53, 126, 196, 126, 237, 175,
    169, 75, 74, 41, 222, 231, 28, 244, 236, 197, 151, 175, 31,
    235, 158, 212, 181, 168, 127, 121, 154, 253, 24, 61, 221, 38,
    44, 159, 60, 251, 217, 180, 125, 180, 41, 104, 45, 70, 188,
    188, 63, 96, 22, 120, 255, 95, 226, 127, 236, 160, 228, 247,
    46, 126, 17, 114, 210, 231, 76, 13, 230, 88, 71, 230, 4, 249,
    125, 209, 154, 192, 113, 166, 19, 18, 237, 186, 212, 215, 8,
    162, 251, 156, 166, 196, 114, 172, 119, 248, 115, 72, 70, 39,
    168, 187, 36, 25, 128, 75, 55, 9, 233, 184, 145, 220, 134, 21,
    239, 122, 175, 142, 69, 249, 7, 65, 14, 241, 100, 86, 138, 109,
    3, 119, 211, 212, 71, 95, 157, 240, 167, 84, 16, 57, 185, 13,
    230, 139, 2, 0, 0, 0, 0, 0, 0, 0
};

static inline double __builtin_trig_preop_generic_f64(double input, int shift) {
  shift = shift * 53;
  int expon = 0;
  double mant = frexp(input, &expon);
  if (expon > 1077) {
    shift += expon - 1077;
  }
  const unsigned char *ptr = PIBITS_TBL + shift / 8;
  ulong tmp = *(ulong *)(ptr);
  ulong tmp2 = *(ulong *)(ptr+1);
  int rem = shift % 8;
  if (rem) {
    int mask = (1 << rem) - 1;
    
  }
  double result = as_double(tmp & 0x1fffffffffffffUL);
  int scale = (-53 - shift);
  if (expon >= 1968) {
    scale += 128;
  }
  return ldexp(result, scale);
}

static inline float __builtin_generic_frac_f32(float x) {
    float fract_x = x;
    float fract_r = fract_x - (int)fract_x;

    int x_as_int = *((int*)&fract_x);
    int x_is_inf = (x_as_int == PINFBITPATT_SP32) || (x_as_int == NINFBITPATT_SP32);

    if (x_is_inf) {
        fract_r = 0.0f;
    }

    return fract_r;
}

static inline double __builtin_generic_frac_f64(double x) {
    double fract_x = x;
    double fract_r = fract_x - (long)fract_x;

    unsigned long x_as_ulong = *((unsigned long*)&fract_x);
    int x_is_inf = (x_as_ulong == PINFBITPATT_DP64) || (x_as_ulong == NINFBITPATT_DP64);

    if (x_is_inf) {
        fract_r = 0.0;
    }

    return fract_r;
}

// Assuming 'half' is an available data type for 16-bit floating-point numbers.
static inline float __builtin_generic_frac_f16(half x) {
    half fract_x = x;
    half fract_r = fract_x - (short)fract_x;

    unsigned short x_as_ushort = *((unsigned short*)&fract_x);
    int x_is_inf = (x_as_ushort == PINFBITPATT_HP16) || (x_as_ushort == NINFBITPATT_HP16);

    if (x_is_inf) {
        fract_r = 0.0h;
    }

    return fract_r;
}

static inline float __builtin_generic_clamp_f32(float x, float l, float h) {
    return x < l ? l : (x > h ? h : x);
}

static inline double __builtin_generic_clamp_f64(double x, double l, double h) {
    return x < l ? l : (x > h ? h : x);
}

// Assuming half type is supported, otherwise you can use custom implementation for half type
static inline half __builtin_generic_clamp_f16(half x, half l, half h) {
    return x < l ? l : (x > h ? h : x);
}

inline int __builtin_generic_class_f32(float x, int klass) {
    unsigned long ix = *((unsigned long *)&x);

    if ((klass & CLASS_PINF) && (ix == PINFBITPATT_SP32)) {
        return -1;
    }
    if ((klass & CLASS_NINF) && (ix == NINFBITPATT_SP32)) {
        return -1;
    }
    if ((klass & (CLASS_QNAN | CLASS_SNAN)) && (ix & QNANBITPATT_SP32)) {
        return -1;
    }
    if ((klass & (CLASS_NZER | CLASS_PZER)) && ((ix & (~SIGNBIT_SP32)) == 0)) {
        return -1;
    }
    if ((klass & (CLASS_NSUB | CLASS_PSUB)) && (((ix & EXPBITS_SP32) == 0) && ((ix & MANTBITS_SP32) != 0))) {
        return -1;
    }

    return 0;
}

inline int __builtin_generic_class_f64(double x, int klass) {
    unsigned long long ix = *((unsigned long long *)&x);

    if ((klass & CLASS_PINF) && (ix == PINFBITPATT_DP64)) {
        return -1;
    }
    if ((klass & CLASS_NINF) && (ix == NINFBITPATT_DP64)) {
        return -1;
    }
    if ((klass & (CLASS_QNAN | CLASS_SNAN)) && (ix & QNANBITPATT_DP64)) {
        return -1;
    }
    if ((klass & (CLASS_NZER | CLASS_PZER)) && ((ix & (~SIGNBIT_DP64)) == 0)) {
        return -1;
    }
    if ((klass & (CLASS_NSUB | CLASS_PSUB)) && (((ix & EXPBITS_DP64) == 0) && ((ix & MANTBITS_DP64) != 0))) {
        return -1;
    }

    return 0;
}

inline int __builtin_generic_class_f16(unsigned short x, int klass) {
    if ((klass & CLASS_PINF) && (x == PINFBITPATT_HP16)) {
        return -1;
    }
    if ((klass & CLASS_NINF) && (x == NINFBITPATT_HP16)) {
        return -1;
    }
    if ((klass & (CLASS_QNAN | CLASS_SNAN)) && (x & QNANBITPATT_HP16)) {
        return -1;
    }
    if ((klass & (CLASS_NZER | CLASS_PZER)) && ((x & (~SIGNBIT_HP16)) == 0)) {
        return -1;
    }
    if ((klass & (CLASS_NSUB | CLASS_PSUB)) && (((x & EXPBITS_HP16) == 0) && ((x & MANTBITS_HP16) != 0))) {
        return -1;
    }

    return 0;
}

#define REQUIRES_16BIT_INSTS __attribute__((target("16-bit-insts")))
#define REQUIRES_GFX9_INSTS __attribute__((target("gfx9-insts")))

// Generic intrinsics
extern __attribute__((const)) float2 __llvm_fma_2f32(float2, float2, float2) __asm("llvm.fma.v2f32");

extern __attribute__((const)) half2 __llvm_fma_2f16(half2, half2, half2) __asm("llvm.fma.v2f16");
extern __attribute__((const)) half2 __llvm_fabs_2f16(half2) __asm("llvm.fabs.v2f16");
extern __attribute__((const)) half2 __llvm_minnum_2f16(half2, half2) __asm("llvm.minnum.v2f16");
extern __attribute__((const)) half2 __llvm_maxnum_2f16(half2, half2) __asm("llvm.maxnum.v2f16");
extern __attribute__((const)) half2 __llvm_copysign_2f16(half2, half2) __asm("llvm.copysign.v2f16");
extern __attribute__((const)) half2 __llvm_floor_2f16(half2) __asm("llvm.floor.v2f16");
extern __attribute__((const)) half2 __llvm_ceil_2f16(half2) __asm("llvm.ceil.v2f16");
extern __attribute__((const)) half2 __llvm_trunc_2f16(half2) __asm("llvm.trunc.v2f16");
extern __attribute__((const)) half2 __llvm_round_2f16(half2) __asm("llvm.round.v2f16");
extern __attribute__((const)) half2 __llvm_rint_2f16(half2) __asm("llvm.rint.v2f16");
extern __attribute__((const)) half2 __llvm_canonicalize_2f16(half2) __asm("llvm.canonicalize.v2f16");

// Intrinsics requiring wrapping
extern __attribute__((const)) uchar __llvm_ctlz_i8(uchar);
extern __attribute__((const)) ushort __llvm_ctlz_i16(ushort);
extern __attribute__((const)) uint __llvm_ctlz_i32(uint);
extern __attribute__((const)) ulong __llvm_ctlz_i64(ulong);

extern __attribute__((const)) uchar __llvm_cttz_i8(uchar);
extern __attribute__((const)) ushort __llvm_cttz_i16(ushort);
extern __attribute__((const)) uint __llvm_cttz_i32(uint);
extern __attribute__((const)) ulong __llvm_cttz_i64(ulong);

// Atomics
extern uint __llvm_ld_atomic_a1_x_dev_i32(__global uint *);
extern ulong __llvm_ld_atomic_a1_x_dev_i64(__global ulong *);
extern uint __llvm_ld_atomic_a3_x_wg_i32(__local uint *);
extern ulong __llvm_ld_atomic_a3_x_wg_i64(__local ulong *);

extern void __llvm_st_atomic_a1_x_dev_i32(__global uint *, uint);
extern void __llvm_st_atomic_a1_x_dev_i64(__global ulong *, ulong);
extern void __llvm_st_atomic_a3_x_wg_i32(__local uint *, uint);
extern void __llvm_st_atomic_a3_x_wg_i64(__local ulong *, ulong);

extern uint __llvm_atomic_add_a1_x_dev_i32(__global uint *, uint);
extern ulong __llvm_atomic_add_a1_x_dev_i64(__global ulong *, ulong);
extern uint __llvm_atomic_add_a3_x_wg_i32(__local uint *, uint);
extern ulong __llvm_atomic_add_a3_x_wg_i64(__local ulong *, ulong);

extern uint __llvm_atomic_and_a1_x_dev_i32(__global uint *, uint);
extern ulong __llvm_atomic_and_a1_x_dev_i64(__global ulong *, ulong);
extern uint __llvm_atomic_and_a3_x_wg_i32(__local uint *, uint);
extern ulong __llvm_atomic_and_a3_x_wg_i64(__local ulong *, ulong);

extern uint __llvm_atomic_or_a1_x_dev_i32(__global uint *, uint);
extern ulong __llvm_atomic_or_a1_x_dev_i64(__global ulong *, ulong);
extern uint __llvm_atomic_or_a3_x_wg_i32(__local uint *, uint);
extern ulong __llvm_atomic_or_a3_x_wg_i64(__local ulong *, ulong);

extern uint __llvm_atomic_max_a1_x_dev_i32(__global int *, int);
extern uint __llvm_atomic_umax_a1_x_dev_i32(__global uint *, uint);
extern ulong __llvm_atomic_max_a1_x_dev_i64(__global long *, long);
extern ulong __llvm_atomic_umax_a1_x_dev_i64(__global ulong *, ulong);
extern uint __llvm_atomic_max_a3_x_wg_i32(__local int *, int);
extern uint __llvm_atomic_umax_a3_x_wg_i32(__local uint *, uint);
extern ulong __llvm_atomic_max_a3_x_wg_i64(__local long *, long);
extern ulong __llvm_atomic_umax_a3_x_wg_i64(__local ulong *, ulong);

extern uint __llvm_atomic_min_a1_x_dev_i32(__global int *, int);
extern uint __llvm_atomic_umin_a1_x_dev_i32(__global uint *, uint);
extern ulong __llvm_atomic_min_a1_x_dev_i64(__global long *, long);
extern ulong __llvm_atomic_umin_a1_x_dev_i64(__global ulong *, ulong);
extern uint __llvm_atomic_min_a3_x_wg_i32(__local int *, int);
extern uint __llvm_atomic_umin_a3_x_wg_i32(__local uint *, uint);
extern ulong __llvm_atomic_min_a3_x_wg_i64(__local long *, long);
extern ulong __llvm_atomic_umin_a3_x_wg_i64(__local ulong *, ulong);

extern uint __llvm_cmpxchg_a1_x_x_dev_i32(__global uint *, uint, uint);
extern ulong __llvm_cmpxchg_a1_x_x_dev_i64(__global ulong *, ulong, ulong);
extern uint __llvm_cmpxchg_a3_x_x_wg_i32(__local uint *, uint, uint);
extern ulong __llvm_cmpxchg_a3_x_x_wg_i64(__local ulong *, ulong, ulong);

// AMDGPU intrinsics

// llvm.amdgcn.mov.dpp.i32 <src> <dpp_ctrl> <row_mask> <bank_mask> <bound_ctrl>

// llvm.amdgcn.update.dpp.i32 <old> <src> <dpp_ctrl> <row_mask> <bank_mask> <bound_ctrl>
extern uint __llvm_amdgcn_update_dpp_i32(uint, uint, uint, uint, uint, bool) __asm("llvm.amdgcn.update.dpp.i32");

// llvm.amdgcn.mov.dpp8.i32 <src> <sel>
extern uint __llvm_amdgcn_dpp8_i32(uint, uint) __asm("llvm.amdgcn.dpp8.i32");

// llvm.amdgcn.permlane16 <old> <src0> <src1> <src2> <fi> <bound_control>
extern uint __llvm_amdgcn_permlane16(uint, uint, uint, uint, bool, bool) __asm("llvm.amdgcn.permlane16");

// llvm.amdgcn.permlanex16 <old> <src0> <src1> <src2> <fi> <bound_control>
extern uint __llvm_amdgcn_permlanex16(uint, uint, uint, uint, bool, bool) __asm("llvm.amdgcn.permlanex16");

extern __attribute__((const, convergent)) ulong __llvm_amdgcn_icmp_i64_i32(uint, uint, uint) __asm("llvm.amdgcn.icmp.i64.i32");
extern __attribute__((const, convergent)) ulong __llvm_amdgcn_icmp_i64_i64(ulong, ulong, uint) __asm("llvm.amdgcn.icmp.i64.i64");
extern __attribute__((const, convergent)) ulong __llvm_amdgcn_fcmp_i64_f32(float, float, uint) __asm("llvm.amdgcn.fcmp.i64.f32");
extern __attribute__((const, convergent)) ulong __llvm_amdgcn_fcmp_i64_f64(double, double, uint) __asm("llvm.amdgcn.fcmp.i64.f64");
extern __attribute__((const, convergent)) uint __llvm_amdgcn_icmp_i32_i32(uint, uint, uint) __asm("llvm.amdgcn.icmp.i32.i32");
extern __attribute__((const, convergent)) uint __llvm_amdgcn_icmp_i32_i64(ulong, ulong, uint) __asm("llvm.amdgcn.icmp.i32.i64");
extern __attribute__((const, convergent)) uint __llvm_amdgcn_fcmp_i32_f32(float, float, uint) __asm("llvm.amdgcn.fcmp.i32.f32");
extern __attribute__((const, convergent)) uint __llvm_amdgcn_fcmp_i32_f64(double, double, uint) __asm("llvm.amdgcn.fcmp.i32.f64");

// Buffer Load/Store
extern __attribute__((pure)) float4 __llvm_amdgcn_struct_buffer_load_format_v4f32(uint4 rsrc, uint vindex, uint voffset, uint soffset, uint cachepolicy) __asm("llvm.amdgcn.struct.buffer.load.format.v4f32");
extern __attribute__((pure)) half4 __llvm_amdgcn_struct_buffer_load_format_v4f16(uint4 rsrc, uint vindex, uint voffset, uint soffset, uint cachepolicy) __asm("llvm.amdgcn.struct.buffer.load.format.v4f16");
extern void __llvm_amdgcn_struct_buffer_store_format_v4f32(float4 vdata, uint4 rsrc, uint vindex, uint voffset, uint soffset, uint cachepolicy) __asm("llvm.amdgcn.struct.buffer.store.format.v4f32");
extern void __llvm_amdgcn_struct_buffer_store_format_v4f16( half4 vdata, uint4 rsrc, uint vindex, uint voffset, uint soffset, uint cachepolicy) __asm("llvm.amdgcn.struct.buffer.store.format.v4f16");

// Image load, store, sample, gather
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_1d_v4f32_i32(uint ix, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_2d_v4f32_i32(uint ix, uint iy, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_3d_v4f32_i32(uint ix, uint iy, uint iz, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_cube_v4f32_i32(uint ix, uint iy, uint iface, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_1darray_v4f32_i32(uint ix, uint islice, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_2darray_v4f32_i32(uint ix, uint iy, uint islice, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_mip_1d_v4f32_i32(uint ix, uint imip, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_mip_2d_v4f32_i32(uint ix, uint iy, uint imip, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_mip_3d_v4f32_i32(uint ix, uint iy, uint iz, uint imip, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_mip_cube_v4f32_i32(uint ix, uint iy, uint iface, uint imip, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_mip_1darray_v4f32_i32(uint ix, uint islice, uint imip, uint8 t);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_load_mip_2darray_v4f32_i32(uint ix, uint iy, uint islice, uint imip, uint8 t);

extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_1d_v4f16_i32(uint ix, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_2d_v4f16_i32(uint ix, uint iy, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_3d_v4f16_i32(uint ix, uint iy, uint iz, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_cube_v4f16_i32(uint ix, uint iy, uint iface, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_1darray_v4f16_i32(uint ix, uint islice, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_2darray_v4f16_i32(uint ix, uint iy, uint islice, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_mip_1d_v4f16_i32(uint ix, uint imip, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_mip_2d_v4f16_i32(uint ix, uint iy, uint imip, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_mip_3d_v4f16_i32(uint ix, uint iy, uint iz, uint imip, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_mip_cube_v4f16_i32(uint ix, uint iy, uint iface, uint imip, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_mip_1darray_v4f16_i32(uint ix, uint islice, uint imip, uint8 t);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_load_mip_2darray_v4f16_i32(uint ix, uint iy, uint islice, uint imip, uint8 t);

extern __attribute__((pure)) float __llvm_amdgcn_image_load_2d_f32_i32(uint ix, uint iy, uint8 t);
extern __attribute__((pure)) float __llvm_amdgcn_image_load_2darray_f32_i32(uint ix, uint iy, uint islice, uint8 t);
extern __attribute__((pure)) float __llvm_amdgcn_image_load_mip_2d_f32_i32(uint ix, uint iy, uint imip, uint8 t);
extern __attribute__((pure)) float __llvm_amdgcn_image_load_mip_2darray_f32_i32(uint ix, uint iy, uint islice, uint imip, uint8 t);

extern void __llvm_amdgcn_image_store_1d_v4f32_i32(float4 pix, uint ix, uint8 t);
extern void __llvm_amdgcn_image_store_2d_v4f32_i32(float4 pix, uint ix, uint iy, uint8 t);
extern void __llvm_amdgcn_image_store_3d_v4f32_i32(float4 pix, uint ix, uint iy, uint iz, uint8 t);
extern void __llvm_amdgcn_image_store_cube_v4f32_i32(float4 pix, uint ix, uint iy, uint iface, uint8 t);
extern void __llvm_amdgcn_image_store_1darray_v4f32_i32(float4 pix, uint ix, uint islice, uint8 t);
extern void __llvm_amdgcn_image_store_2darray_v4f32_i32(float4 pix, uint ix, uint iy, uint islice, uint8 t);
extern void __llvm_amdgcn_image_store_mip_1d_v4f32_i32(float4 pix, uint ix, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_2d_v4f32_i32(float4 pix, uint ix, uint iy, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_3d_v4f32_i32(float4 pix, uint ix, uint iy, uint iz, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_cube_v4f32_i32(float4 pix, uint ix, uint iy, uint iface, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_1darray_v4f32_i32(float4 pix, uint ix, uint islice, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_2darray_v4f32_i32(float4 pix, uint ix, uint iy, uint islice, uint imip, uint8 t);

extern void __llvm_amdgcn_image_store_1d_v4f16_i32(half4 pix, uint ix, uint8 t);
extern void __llvm_amdgcn_image_store_2d_v4f16_i32(half4 pix, uint ix, uint iy, uint8 t);
extern void __llvm_amdgcn_image_store_3d_v4f16_i32(half4 pix, uint ix, uint iy, uint iz, uint8 t);
extern void __llvm_amdgcn_image_store_cube_v4f16_i32(half4 pix, uint ix, uint iy, uint iface, uint8 t);
extern void __llvm_amdgcn_image_store_1darray_v4f16_i32(half4 pix, uint ix, uint islice, uint8 t);
extern void __llvm_amdgcn_image_store_2darray_v4f16_i32(half4 pix, uint ix, uint iy, uint islice, uint8 t);
extern void __llvm_amdgcn_image_store_mip_1d_v4f16_i32(half4 pix, uint ix, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_2d_v4f16_i32(half4 pix, uint ix, uint iy, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_3d_v4f16_i32(half4 pix, uint ix, uint iy, uint iz, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_cube_v4f16_i32(half4 pix, uint ix, uint iy, uint iface, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_1darray_v4f16_i32(half4 pix, uint ix, uint islice, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_2darray_v4f16_i32(half4 pix, uint ix, uint iy, uint islice, uint imip, uint8 t);

extern void __llvm_amdgcn_image_store_2d_f32_i32(float pix, uint ix, uint iy, uint8 t);
extern void __llvm_amdgcn_image_store_2darray_f32_i32(float pix, uint ix, uint iy, uint islice, uint8 t);
extern void __llvm_amdgcn_image_store_mip_2d_f32_i32(float pix, uint ix, uint iy, uint imip, uint8 t);
extern void __llvm_amdgcn_image_store_mip_2darray_f32_i32(float pix, uint ix, uint iy, uint islice, uint imip, uint8 t);

extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_1d_v4f32_f32(float x, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_lz_1d_v4f32_f32(float x, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_l_1d_v4f32_f32(float x, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_d_1d_v4f32_f32_f32(float dxdh, float dxdv, float x, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_2d_v4f32_f32(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_lz_2d_v4f32_f32(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_l_2d_v4f32_f32(float x, float y, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_d_2d_v4f32_f32_f32(float dxdh, float dydh, float dxdv, float dydv, float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_3d_v4f32_f32(float x, float y, float z, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_lz_3d_v4f32_f32(float x, float y, float z, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_l_3d_v4f32_f32(float x, float y, float z, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_d_3d_v4f32_f32_f32(float dxdh, float dydh, float dzdh, float dxdv, float dydv, float dzdv, float x, float y, float z, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_cube_v4f32_f32(float x, float y, float face, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_lz_cube_v4f32_f32(float x, float y, float face, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_l_cube_v4f32_f32(float x, float y, float face, float lod, uint8 t, uint4 s);

extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_1darray_v4f32_f32(float x, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_lz_1darray_v4f32_f32(float x, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_l_1darray_v4f32_f32(float x, float slice, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_d_1darray_v4f32_f32_f32(float dxdh, float dxdv, float x, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_2darray_v4f32_f32(float x, float y, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_lz_2darray_v4f32_f32(float x, float y, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_l_2darray_v4f32_f32(float x, float y, float slice, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_sample_d_2darray_v4f32_f32_f32(float dxdh, float dydh, float dxdv, float dydv, float x, float y, float slice, uint8 t, uint4 s);

extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_1d_v4f16_f32(float x, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_lz_1d_v4f16_f32(float x, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_l_1d_v4f16_f32(float x, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_d_1d_v4f16_f32_f32(float dxdh, float dxdv, float x, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_2d_v4f16_f32(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_lz_2d_v4f16_f32(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_l_2d_v4f16_f32(float x, float y, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_d_2d_v4f16_f32_f32(float dxdh, float dydh, float dxdv, float dydv, float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_3d_v4f16_f32(float x, float y, float z, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_lz_3d_v4f16_f32(float x, float y, float z, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_l_3d_v4f16_f32(float x, float y, float z, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_d_3d_v4f16_f32_f32(float dxdh, float dydh, float dzdh, float dxdv, float dydv, float dzdv, float x, float y, float z, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_cube_v4f16_f32(float x, float y, float face, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_lz_cube_v4f16_f32(float x, float y, float face, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_l_cube_v4f16_f32(float x, float y, float face, float lod, uint8 t, uint4 s);

extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_1darray_v4f16_f32(float x, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_lz_1darray_v4f16_f32(float x, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_l_1darray_v4f16_f32(float x, float slice, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_d_1darray_v4f16_f32_f32(float dxdh, float dxdv, float x, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_2darray_v4f16_f32(float x, float y, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_lz_2darray_v4f16_f32(float x, float y, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_l_2darray_v4f16_f32(float x, float y, float slice, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) half4 __llvm_amdgcn_image_sample_d_2darray_v4f16_f32_f32(float dxdh, float dydh, float dxdv, float dydv, float x, float y, float slice, uint8 t, uint4 s);

extern __attribute__((pure)) float __llvm_amdgcn_image_sample_2d_f32_f32(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_lz_2d_f32_f32(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_l_2d_f32_f32(float x, float y, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_d_2d_f32_f32_f32(float dxdh, float dydh, float dxdv, float dydv, float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_2darray_f32_f32(float x, float y, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_lz_2darray_f32_f32(float x, float y, float slice, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_l_2darray_f32_f32(float x, float y, float slice, float lod, uint8 t, uint4 s);
extern __attribute__((pure)) float __llvm_amdgcn_image_sample_d_2darray_f32_f32_f32(float dxdh, float dydh, float dxdv, float dydv, float x, float y, float slice, uint8 t, uint4 s);

extern __attribute__((pure)) float4 __llvm_amdgcn_image_gather4_lz_2d_v4f32_f32_r(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_gather4_lz_2d_v4f32_f32_g(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_gather4_lz_2d_v4f32_f32_b(float x, float y, uint8 t, uint4 s);
extern __attribute__((pure)) float4 __llvm_amdgcn_image_gather4_lz_2d_v4f32_f32_a(float x, float y, uint8 t, uint4 s);


#pragma OPENCL EXTENSION cl_khr_fp16 : disable
#endif // IRIF_H
