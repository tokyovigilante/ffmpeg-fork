/*
 * DSP utils
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef DSPUTIL_H
#define DSPUTIL_H

#include "common.h"
#include "avcodec.h"

//#define DEBUG
/* dct code */
typedef short DCTELEM;

void fdct_ifast (DCTELEM *data);

void j_rev_dct (DCTELEM *data);

void fdct_mmx(DCTELEM *block);

extern void (*av_fdct)(DCTELEM *block);

/* encoding scans */
extern UINT8 ff_alternate_horizontal_scan[64];
extern UINT8 ff_alternate_vertical_scan[64];
extern UINT8 zigzag_direct[64];

/* permutation table */
extern UINT8 permutation[64];

/* pixel operations */
#define MAX_NEG_CROP 384

/* temporary */
extern UINT32 squareTbl[512];
extern UINT8 cropTbl[256 + 2 * MAX_NEG_CROP];

void dsputil_init(void);

/* pixel ops : interface with DCT */

extern void (*ff_idct)(DCTELEM *block);
extern void (*ff_idct_put)(UINT8 *dest, int line_size, DCTELEM *block);
extern void (*ff_idct_add)(UINT8 *dest, int line_size, DCTELEM *block);
extern void (*get_pixels)(DCTELEM *block, const UINT8 *pixels, int line_size);
extern void (*diff_pixels)(DCTELEM *block, const UINT8 *s1, const UINT8 *s2, int stride);
extern void (*put_pixels_clamped)(const DCTELEM *block, UINT8 *pixels, int line_size);
extern void (*add_pixels_clamped)(const DCTELEM *block, UINT8 *pixels, int line_size);
extern void (*gmc1)(UINT8 *dst, UINT8 *src, int srcStride, int h, int x16, int y16, int rounder);
extern void (*clear_blocks)(DCTELEM *blocks);
extern int (*pix_sum)(UINT8 * pix, int line_size);
extern int (*pix_norm1)(UINT8 * pix, int line_size);



void get_pixels_c(DCTELEM *block, const UINT8 *pixels, int line_size);
void diff_pixels_c(DCTELEM *block, const UINT8 *s1, const UINT8 *s2, int stride);
void put_pixels_clamped_c(const DCTELEM *block, UINT8 *pixels, int line_size);
void add_pixels_clamped_c(const DCTELEM *block, UINT8 *pixels, int line_size);
void clear_blocks_c(DCTELEM *blocks);

/* add and put pixel (decoding) */
typedef void (*op_pixels_func)(UINT8 *block, const UINT8 *pixels, int line_size, int h);
typedef void (*qpel_mc_func)(UINT8 *dst, UINT8 *src, int dstStride, int srcStride, int mx, int my);

extern op_pixels_func put_pixels_tab[4];
extern op_pixels_func avg_pixels_tab[4];
extern op_pixels_func put_no_rnd_pixels_tab[4];
extern op_pixels_func avg_no_rnd_pixels_tab[4];
extern qpel_mc_func qpel_mc_rnd_tab[16];
extern qpel_mc_func qpel_mc_no_rnd_tab[16];

/* motion estimation */

typedef int (*op_pixels_abs_func)(UINT8 *blk1, UINT8 *blk2, int line_size);

extern op_pixels_abs_func pix_abs16x16;
extern op_pixels_abs_func pix_abs16x16_x2;
extern op_pixels_abs_func pix_abs16x16_y2;
extern op_pixels_abs_func pix_abs16x16_xy2;
extern op_pixels_abs_func pix_abs8x8;
extern op_pixels_abs_func pix_abs8x8_x2;
extern op_pixels_abs_func pix_abs8x8_y2;
extern op_pixels_abs_func pix_abs8x8_xy2;

int pix_abs16x16_c(UINT8 *blk1, UINT8 *blk2, int lx);
int pix_abs16x16_x2_c(UINT8 *blk1, UINT8 *blk2, int lx);
int pix_abs16x16_y2_c(UINT8 *blk1, UINT8 *blk2, int lx);
int pix_abs16x16_xy2_c(UINT8 *blk1, UINT8 *blk2, int lx);

static inline int block_permute_op(int j)
{
	return permutation[j];
}

void block_permute(INT16 *block);

#if defined(HAVE_MMX)

#define MM_MMX    0x0001 /* standard MMX */
#define MM_3DNOW  0x0004 /* AMD 3DNOW */
#define MM_MMXEXT 0x0002 /* SSE integer functions or AMD MMX ext */
#define MM_SSE    0x0008 /* SSE functions */
#define MM_SSE2   0x0010 /* PIV SSE2 functions */

extern int mm_flags;

int mm_support(void);

static inline void emms(void)
{
    __asm __volatile ("emms;":::"memory");
}

#define emms_c() \
{\
    if (mm_flags & MM_MMX)\
        emms();\
}

#define __align8 __attribute__ ((aligned (8)))

void dsputil_init_mmx(void);
void dsputil_set_bit_exact_mmx(void);

#elif defined(ARCH_ARMV4L)

#define emms_c()

/* This is to use 4 bytes read to the IDCT pointers for some 'zero'
   line ptimizations */
#define __align8 __attribute__ ((aligned (4)))

void dsputil_init_armv4l(void);   

#elif defined(HAVE_MLIB)
 
#define emms_c()

/* SPARC/VIS IDCT needs 8-byte aligned DCT blocks */
#define __align8 __attribute__ ((aligned (8)))

void dsputil_init_mlib(void);   

#elif defined(ARCH_ALPHA)

#define emms_c()
#define __align8 __attribute__ ((aligned (8)))

void dsputil_init_alpha(void);

#elif defined(ARCH_POWERPC)

#define emms_c()
#define __align8 __attribute__ ((aligned (16)))

void dsputil_init_altivec(void);

#else

#define emms_c()

#define __align8

#endif

#ifdef __GNUC__

struct unaligned_64 { uint64_t l; } __attribute__((packed));
struct unaligned_32 { uint32_t l; } __attribute__((packed));

#define LD32(a) (((const struct unaligned_32 *) (a))->l)
#define LD64(a) (((const struct unaligned_64 *) (a))->l)

#define ST32(a, b) (((struct unaligned_32 *) (a))->l) = (b)

#else /* __GNUC__ */

#define LD32(a) (*((uint32_t*)(a)))
#define LD64(a) (*((uint64_t*)(a)))

#define ST32(a, b) *((uint32_t*)(a)) = (b)

#endif /* !__GNUC__ */

/* PSNR */
void get_psnr(UINT8 *orig_image[3], UINT8 *coded_image[3],
              int orig_linesize[3], int coded_linesize,
              AVCodecContext *avctx);
              
#endif
