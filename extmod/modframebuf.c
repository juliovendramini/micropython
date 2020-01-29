/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include "py/runtime.h"

#if MICROPY_PY_FRAMEBUF

static const uint8_t font_petme128_8x8[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32= 
    0x00,0x00,0x00,0x4f,0x4f,0x00,0x00,0x00, // 33=!
    0x00,0x07,0x07,0x00,0x00,0x07,0x07,0x00, // 34="
    0x14,0x7f,0x7f,0x14,0x14,0x7f,0x7f,0x14, // 35=#
    0x00,0x24,0x2e,0x6b,0x6b,0x3a,0x12,0x00, // 36=$
    0x00,0x63,0x33,0x18,0x0c,0x66,0x63,0x00, // 37=%
    0x00,0x32,0x7f,0x4d,0x4d,0x77,0x72,0x50, // 38=&
    0x00,0x00,0x00,0x04,0x06,0x03,0x01,0x00, // 39='
    0x00,0x00,0x1c,0x3e,0x63,0x41,0x00,0x00, // 40=(
    0x00,0x00,0x41,0x63,0x3e,0x1c,0x00,0x00, // 41=)
    0x08,0x2a,0x3e,0x1c,0x1c,0x3e,0x2a,0x08, // 42=*
    0x00,0x08,0x08,0x3e,0x3e,0x08,0x08,0x00, // 43=+
    0x00,0x00,0x80,0xe0,0x60,0x00,0x00,0x00, // 44=,
    0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00, // 45=-
    0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00, // 46=.
    0x00,0x40,0x60,0x30,0x18,0x0c,0x06,0x02, // 47=/
    0x00,0x3e,0x7f,0x49,0x45,0x7f,0x3e,0x00, // 48=0
    0x00,0x40,0x44,0x7f,0x7f,0x40,0x40,0x00, // 49=1
    0x00,0x62,0x73,0x51,0x49,0x4f,0x46,0x00, // 50=2
    0x00,0x22,0x63,0x49,0x49,0x7f,0x36,0x00, // 51=3
    0x00,0x18,0x18,0x14,0x16,0x7f,0x7f,0x10, // 52=4
    0x00,0x27,0x67,0x45,0x45,0x7d,0x39,0x00, // 53=5
    0x00,0x3e,0x7f,0x49,0x49,0x7b,0x32,0x00, // 54=6
    0x00,0x03,0x03,0x79,0x7d,0x07,0x03,0x00, // 55=7
    0x00,0x36,0x7f,0x49,0x49,0x7f,0x36,0x00, // 56=8
    0x00,0x26,0x6f,0x49,0x49,0x7f,0x3e,0x00, // 57=9
    0x00,0x00,0x00,0x24,0x24,0x00,0x00,0x00, // 58=:
    0x00,0x00,0x80,0xe4,0x64,0x00,0x00,0x00, // 59=;
    0x00,0x08,0x1c,0x36,0x63,0x41,0x41,0x00, // 60=<
    0x00,0x14,0x14,0x14,0x14,0x14,0x14,0x00, // 61==
    0x00,0x41,0x41,0x63,0x36,0x1c,0x08,0x00, // 62=>
    0x00,0x02,0x03,0x51,0x59,0x0f,0x06,0x00, // 63=?
    0x00,0x3e,0x7f,0x41,0x4d,0x4f,0x2e,0x00, // 64=@
    0x00,0x7c,0x7e,0x0b,0x0b,0x7e,0x7c,0x00, // 65=A
    0x00,0x7f,0x7f,0x49,0x49,0x7f,0x36,0x00, // 66=B
    0x00,0x3e,0x7f,0x41,0x41,0x63,0x22,0x00, // 67=C
    0x00,0x7f,0x7f,0x41,0x63,0x3e,0x1c,0x00, // 68=D
    0x00,0x7f,0x7f,0x49,0x49,0x41,0x41,0x00, // 69=E
    0x00,0x7f,0x7f,0x09,0x09,0x01,0x01,0x00, // 70=F
    0x00,0x3e,0x7f,0x41,0x49,0x7b,0x3a,0x00, // 71=G
    0x00,0x7f,0x7f,0x08,0x08,0x7f,0x7f,0x00, // 72=H
    0x00,0x00,0x41,0x7f,0x7f,0x41,0x00,0x00, // 73=I
    0x00,0x20,0x60,0x41,0x7f,0x3f,0x01,0x00, // 74=J
    0x00,0x7f,0x7f,0x1c,0x36,0x63,0x41,0x00, // 75=K
    0x00,0x7f,0x7f,0x40,0x40,0x40,0x40,0x00, // 76=L
    0x00,0x7f,0x7f,0x06,0x0c,0x06,0x7f,0x7f, // 77=M
    0x00,0x7f,0x7f,0x0e,0x1c,0x7f,0x7f,0x00, // 78=N
    0x00,0x3e,0x7f,0x41,0x41,0x7f,0x3e,0x00, // 79=O
    0x00,0x7f,0x7f,0x09,0x09,0x0f,0x06,0x00, // 80=P
    0x00,0x1e,0x3f,0x21,0x61,0x7f,0x5e,0x00, // 81=Q
    0x00,0x7f,0x7f,0x19,0x39,0x6f,0x46,0x00, // 82=R
    0x00,0x26,0x6f,0x49,0x49,0x7b,0x32,0x00, // 83=S
    0x00,0x01,0x01,0x7f,0x7f,0x01,0x01,0x00, // 84=T
    0x00,0x3f,0x7f,0x40,0x40,0x7f,0x3f,0x00, // 85=U
    0x00,0x1f,0x3f,0x60,0x60,0x3f,0x1f,0x00, // 86=V
    0x00,0x7f,0x7f,0x30,0x18,0x30,0x7f,0x7f, // 87=W
    0x00,0x63,0x77,0x1c,0x1c,0x77,0x63,0x00, // 88=X
    0x00,0x07,0x0f,0x78,0x78,0x0f,0x07,0x00, // 89=Y
    0x00,0x61,0x71,0x59,0x4d,0x47,0x43,0x00, // 90=Z
    0x00,0x00,0x7f,0x7f,0x41,0x41,0x00,0x00, // 91=[
    0x00,0x02,0x06,0x0c,0x18,0x30,0x60,0x40, // 92='\'
    0x00,0x00,0x41,0x41,0x7f,0x7f,0x00,0x00, // 93=]
    0x00,0x08,0x0c,0x06,0x06,0x0c,0x08,0x00, // 94=^
    0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0, // 95=_
    0x00,0x00,0x01,0x03,0x06,0x04,0x00,0x00, // 96=`
    0x00,0x20,0x74,0x54,0x54,0x7c,0x78,0x00, // 97=a
    0x00,0x7f,0x7f,0x44,0x44,0x7c,0x38,0x00, // 98=b
    0x00,0x38,0x7c,0x44,0x44,0x6c,0x28,0x00, // 99=c
    0x00,0x38,0x7c,0x44,0x44,0x7f,0x7f,0x00, // 100=d
    0x00,0x38,0x7c,0x54,0x54,0x5c,0x58,0x00, // 101=e
    0x00,0x08,0x7e,0x7f,0x09,0x03,0x02,0x00, // 102=f
    0x00,0x98,0xbc,0xa4,0xa4,0xfc,0x7c,0x00, // 103=g
    0x00,0x7f,0x7f,0x04,0x04,0x7c,0x78,0x00, // 104=h
    0x00,0x00,0x00,0x7d,0x7d,0x00,0x00,0x00, // 105=i
    0x00,0x40,0xc0,0x80,0x80,0xfd,0x7d,0x00, // 106=j
    0x00,0x7f,0x7f,0x30,0x38,0x6c,0x44,0x00, // 107=k
    0x00,0x00,0x41,0x7f,0x7f,0x40,0x00,0x00, // 108=l
    0x00,0x7c,0x7c,0x18,0x30,0x18,0x7c,0x7c, // 109=m
    0x00,0x7c,0x7c,0x04,0x04,0x7c,0x78,0x00, // 110=n
    0x00,0x38,0x7c,0x44,0x44,0x7c,0x38,0x00, // 111=o
    0x00,0xfc,0xfc,0x24,0x24,0x3c,0x18,0x00, // 112=p
    0x00,0x18,0x3c,0x24,0x24,0xfc,0xfc,0x00, // 113=q
    0x00,0x7c,0x7c,0x04,0x04,0x0c,0x08,0x00, // 114=r
    0x00,0x48,0x5c,0x54,0x54,0x74,0x20,0x00, // 115=s
    0x04,0x04,0x3f,0x7f,0x44,0x64,0x20,0x00, // 116=t
    0x00,0x3c,0x7c,0x40,0x40,0x7c,0x3c,0x00, // 117=u
    0x00,0x1c,0x3c,0x60,0x60,0x3c,0x1c,0x00, // 118=v
    0x00,0x1c,0x7c,0x30,0x18,0x30,0x7c,0x1c, // 119=w
    0x00,0x44,0x6c,0x38,0x38,0x6c,0x44,0x00, // 120=x
    0x00,0x9c,0xbc,0xa0,0xa0,0xfc,0x7c,0x00, // 121=y
    0x00,0x44,0x64,0x74,0x5c,0x4c,0x44,0x00, // 122=z
    0x00,0x08,0x08,0x3e,0x77,0x41,0x41,0x00, // 123={
    0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00, // 124=|
    0x00,0x41,0x41,0x77,0x3e,0x08,0x08,0x00, // 125=}
    0x00,0x02,0x03,0x01,0x03,0x02,0x03,0x01, // 126=~
    0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55, // 127
};

typedef struct _mp_obj_framebuf_t {
    mp_obj_base_t base;
    mp_obj_t buf_obj; // need to store this to prevent GC from reclaiming buf
    void *buf;
    uint16_t width, height, stride;
    uint8_t format;
} mp_obj_framebuf_t;

typedef void (*setpixel_t)(const mp_obj_framebuf_t*, int, int, uint32_t);
typedef uint32_t (*getpixel_t)(const mp_obj_framebuf_t*, int, int);
typedef void (*fill_rect_t)(const mp_obj_framebuf_t *, int, int, int, int, uint32_t);

typedef struct _mp_framebuf_p_t {
    setpixel_t setpixel;
    getpixel_t getpixel;
    fill_rect_t fill_rect;
} mp_framebuf_p_t;

// constants for formats
#define FRAMEBUF_MVLSB    (0)
#define FRAMEBUF_RGB565   (1)
#define FRAMEBUF_XRGB8888 (7)
#define FRAMEBUF_GS2_HMSB (5)
#define FRAMEBUF_GS4_HMSB (2)
#define FRAMEBUF_GS8      (6)
#define FRAMEBUF_MHLSB    (3)
#define FRAMEBUF_MHMSB    (4)

// Functions for MHLSB and MHMSB

STATIC void mono_horiz_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    size_t index = (x + y * fb->stride) >> 3;
    int offset = fb->format == FRAMEBUF_MHMSB ? x & 0x07 : 7 - (x & 0x07);
    ((uint8_t*)fb->buf)[index] = (((uint8_t*)fb->buf)[index] & ~(0x01 << offset)) | ((col != 0) << offset);
}

STATIC uint32_t mono_horiz_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    size_t index = (x + y * fb->stride) >> 3;
    int offset = fb->format == FRAMEBUF_MHMSB ? x & 0x07 : 7 - (x & 0x07);
    return (((uint8_t*)fb->buf)[index] >> (offset)) & 0x01;
}

STATIC void mono_horiz_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    int reverse = fb->format == FRAMEBUF_MHMSB;
    int advance = fb->stride >> 3;
    while (w--) {
        uint8_t *b = &((uint8_t*)fb->buf)[(x >> 3) + y * advance];
        int offset = reverse ?  x & 7 : 7 - (x & 7);
        for (int hh = h; hh; --hh) {
            *b = (*b & ~(0x01 << offset)) | ((col != 0) << offset);
            b += advance;
        }
        ++x;
    }
}

// Functions for MVLSB format

STATIC void mvlsb_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    size_t index = (y >> 3) * fb->stride + x;
    uint8_t offset = y & 0x07;
    ((uint8_t*)fb->buf)[index] = (((uint8_t*)fb->buf)[index] & ~(0x01 << offset)) | ((col != 0) << offset);
}

STATIC uint32_t mvlsb_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    return (((uint8_t*)fb->buf)[(y >> 3) * fb->stride + x] >> (y & 0x07)) & 0x01;
}

STATIC void mvlsb_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    while (h--) {
        uint8_t *b = &((uint8_t*)fb->buf)[(y >> 3) * fb->stride + x];
        uint8_t offset = y & 0x07;
        for (int ww = w; ww; --ww) {
            *b = (*b & ~(0x01 << offset)) | ((col != 0) << offset);
            ++b;
        }
        ++y;
    }
}

// Functions for RGB565 format

STATIC void rgb565_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    ((uint16_t*)fb->buf)[x + y * fb->stride] = col;
}

STATIC uint32_t rgb565_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    return ((uint16_t*)fb->buf)[x + y * fb->stride];
}

STATIC void rgb565_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    uint16_t *b = &((uint16_t*)fb->buf)[x + y * fb->stride];
    while (h--) {
        for (int ww = w; ww; --ww) {
            *b++ = col;
        }
        b += fb->stride - w;
    }
}

// Functions for XRGB8888 format

STATIC void xrgb8888_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    ((uint32_t*)fb->buf)[x + y * fb->stride] = col;
}

STATIC uint32_t xrgb8888_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    return ((uint32_t*)fb->buf)[x + y * fb->stride];
}

STATIC void xrgb8888_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    uint32_t *b = &((uint32_t*)fb->buf)[x + y * fb->stride];
    while (h--) {
        for (int ww = w; ww; --ww) {
            *b++ = col;
        }
        b += fb->stride - w;
    }
}

// Functions for GS2_HMSB format

STATIC void gs2_hmsb_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    uint8_t *pixel = &((uint8_t*)fb->buf)[(x + y * fb->stride) >> 2];
    uint8_t shift = (x & 0x3) << 1;
    uint8_t mask = 0x3 << shift;
    uint8_t color = (col & 0x3) << shift;
    *pixel = color | (*pixel & (~mask));
}

STATIC uint32_t gs2_hmsb_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    uint8_t pixel = ((uint8_t*)fb->buf)[(x + y * fb->stride) >> 2];
    uint8_t shift = (x & 0x3) << 1;
    return (pixel >> shift) & 0x3;
}

STATIC void gs2_hmsb_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    for (int xx=x; xx < x+w; xx++) {
        for (int yy=y; yy < y+h; yy++) {
            gs2_hmsb_setpixel(fb, xx, yy, col);
        }
    }
}

// Functions for GS4_HMSB format

STATIC void gs4_hmsb_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    uint8_t *pixel = &((uint8_t*)fb->buf)[(x + y * fb->stride) >> 1];

    if (x % 2) {
        *pixel = ((uint8_t)col & 0x0f) | (*pixel & 0xf0);
    } else {
        *pixel = ((uint8_t)col << 4) | (*pixel & 0x0f);
    }
}

STATIC uint32_t gs4_hmsb_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    if (x % 2) {
        return ((uint8_t*)fb->buf)[(x + y * fb->stride) >> 1] & 0x0f;
    }

    return ((uint8_t*)fb->buf)[(x + y * fb->stride) >> 1] >> 4;
}

STATIC void gs4_hmsb_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    col &= 0x0f;
    uint8_t *pixel_pair = &((uint8_t*)fb->buf)[(x + y * fb->stride) >> 1];
    uint8_t col_shifted_left = col << 4;
    uint8_t col_pixel_pair = col_shifted_left | col;
    int pixel_count_till_next_line = (fb->stride - w) >> 1;
    bool odd_x = (x % 2 == 1);

    while (h--) {
        int ww = w;

        if (odd_x && ww > 0) {
            *pixel_pair = (*pixel_pair & 0xf0) | col;
            pixel_pair++;
            ww--;
        }

        memset(pixel_pair, col_pixel_pair, ww >> 1);
        pixel_pair += ww >> 1;

        if (ww % 2) {
            *pixel_pair = col_shifted_left | (*pixel_pair & 0x0f);
            if (!odd_x) {
                pixel_pair++;
            }
        }

        pixel_pair += pixel_count_till_next_line;
    }
}

// Functions for GS8 format

STATIC void gs8_setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    uint8_t *pixel = &((uint8_t*)fb->buf)[(x + y * fb->stride)];
    *pixel = col & 0xff;
}

STATIC uint32_t gs8_getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    return ((uint8_t*)fb->buf)[(x + y * fb->stride)];
}

STATIC void gs8_fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    uint8_t *pixel = &((uint8_t*)fb->buf)[(x + y * fb->stride)];
    while (h--) {
        memset(pixel, col, w);
        pixel += fb->stride;
    }
}

STATIC mp_framebuf_p_t formats[] = {
    [FRAMEBUF_MVLSB] = {mvlsb_setpixel, mvlsb_getpixel, mvlsb_fill_rect},
    [FRAMEBUF_RGB565] = {rgb565_setpixel, rgb565_getpixel, rgb565_fill_rect},
    [FRAMEBUF_XRGB8888] = {xrgb8888_setpixel, xrgb8888_getpixel, xrgb8888_fill_rect},
    [FRAMEBUF_GS2_HMSB] = {gs2_hmsb_setpixel, gs2_hmsb_getpixel, gs2_hmsb_fill_rect},
    [FRAMEBUF_GS4_HMSB] = {gs4_hmsb_setpixel, gs4_hmsb_getpixel, gs4_hmsb_fill_rect},
    [FRAMEBUF_GS8] = {gs8_setpixel, gs8_getpixel, gs8_fill_rect},
    [FRAMEBUF_MHLSB] = {mono_horiz_setpixel, mono_horiz_getpixel, mono_horiz_fill_rect},
    [FRAMEBUF_MHMSB] = {mono_horiz_setpixel, mono_horiz_getpixel, mono_horiz_fill_rect},
};

static inline void setpixel(const mp_obj_framebuf_t *fb, int x, int y, uint32_t col) {
    formats[fb->format].setpixel(fb, x, y, col);
}

static inline uint32_t getpixel(const mp_obj_framebuf_t *fb, int x, int y) {
    return formats[fb->format].getpixel(fb, x, y);
}

STATIC void fill_rect(const mp_obj_framebuf_t *fb, int x, int y, int w, int h, uint32_t col) {
    if (h < 1 || w < 1 || x + w <= 0 || y + h <= 0 || y >= fb->height || x >= fb->width) {
        // No operation needed.
        return;
    }

    // clip to the framebuffer
    int xend = MIN(fb->width, x + w);
    int yend = MIN(fb->height, y + h);
    x = MAX(x, 0);
    y = MAX(y, 0);

    formats[fb->format].fill_rect(fb, x, y, xend - x, yend - y, col);
}

STATIC mp_obj_t framebuf_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 4, 5, false);

    mp_obj_framebuf_t *o = m_new_obj(mp_obj_framebuf_t);
    o->base.type = type;
    o->buf_obj = args[0];

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[0], &bufinfo, MP_BUFFER_WRITE);
    o->buf = bufinfo.buf;

    o->width = mp_obj_get_int(args[1]);
    o->height = mp_obj_get_int(args[2]);
    o->format = mp_obj_get_int(args[3]);
    if (n_args >= 5) {
        o->stride = mp_obj_get_int(args[4]);
    } else {
        o->stride = o->width;
    }

    switch (o->format) {
        case FRAMEBUF_MVLSB:
        case FRAMEBUF_RGB565:
        case FRAMEBUF_XRGB8888:
            break;
        case FRAMEBUF_MHLSB:
        case FRAMEBUF_MHMSB:
            o->stride = (o->stride + 7) & ~7;
            break;
        case FRAMEBUF_GS2_HMSB:
            o->stride = (o->stride + 3) & ~3;
            break;
        case FRAMEBUF_GS4_HMSB:
            o->stride = (o->stride + 1) & ~1;
            break;
        case FRAMEBUF_GS8:
            break;
        default:
            mp_raise_ValueError("invalid format");
    }

    return MP_OBJ_FROM_PTR(o);
}

STATIC mp_int_t framebuf_get_buffer(mp_obj_t self_in, mp_buffer_info_t *bufinfo, mp_uint_t flags) {
    (void)flags;
    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(self_in);
    bufinfo->buf = self->buf;
    bufinfo->len = self->stride * self->height * (self->format == FRAMEBUF_RGB565 ? 2 : 1);
    bufinfo->typecode = 'B'; // view framebuf as bytes
    return 0;
}

STATIC mp_obj_t framebuf_fill(mp_obj_t self_in, mp_obj_t col_in) {
    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t col = mp_obj_get_int(col_in);
    formats[self->format].fill_rect(self, 0, 0, self->width, self->height, col);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(framebuf_fill_obj, framebuf_fill);

STATIC mp_obj_t framebuf_fill_rect(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t x = mp_obj_get_int(args[1]);
    mp_int_t y = mp_obj_get_int(args[2]);
    mp_int_t width = mp_obj_get_int(args[3]);
    mp_int_t height = mp_obj_get_int(args[4]);
    mp_int_t col = mp_obj_get_int(args[5]);

    fill_rect(self, x, y, width, height, col);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_fill_rect_obj, 6, 6, framebuf_fill_rect);

STATIC mp_obj_t framebuf_pixel(size_t n_args, const mp_obj_t *args) {
    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t x = mp_obj_get_int(args[1]);
    mp_int_t y = mp_obj_get_int(args[2]);
    if (0 <= x && x < self->width && 0 <= y && y < self->height) {
        if (n_args == 3) {
            // get
            return MP_OBJ_NEW_SMALL_INT(getpixel(self, x, y));
        } else {
            // set
            setpixel(self, x, y, mp_obj_get_int(args[3]));
        }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_pixel_obj, 3, 4, framebuf_pixel);

STATIC mp_obj_t framebuf_hline(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t x = mp_obj_get_int(args[1]);
    mp_int_t y = mp_obj_get_int(args[2]);
    mp_int_t w = mp_obj_get_int(args[3]);
    mp_int_t col = mp_obj_get_int(args[4]);

    fill_rect(self, x, y, w, 1, col);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_hline_obj, 5, 5, framebuf_hline);

STATIC mp_obj_t framebuf_vline(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t x = mp_obj_get_int(args[1]);
    mp_int_t y = mp_obj_get_int(args[2]);
    mp_int_t h = mp_obj_get_int(args[3]);
    mp_int_t col = mp_obj_get_int(args[4]);

    fill_rect(self, x, y, 1, h, col);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_vline_obj, 5, 5, framebuf_vline);

STATIC mp_obj_t framebuf_rect(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t x = mp_obj_get_int(args[1]);
    mp_int_t y = mp_obj_get_int(args[2]);
    mp_int_t w = mp_obj_get_int(args[3]);
    mp_int_t h = mp_obj_get_int(args[4]);
    mp_int_t col = mp_obj_get_int(args[5]);

    fill_rect(self, x, y, w, 1, col);
    fill_rect(self, x, y + h- 1, w, 1, col);
    fill_rect(self, x, y, 1, h, col);
    fill_rect(self, x + w- 1, y, 1, h, col);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_rect_obj, 6, 6, framebuf_rect);

STATIC mp_obj_t framebuf_line(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t x1 = mp_obj_get_int(args[1]);
    mp_int_t y1 = mp_obj_get_int(args[2]);
    mp_int_t x2 = mp_obj_get_int(args[3]);
    mp_int_t y2 = mp_obj_get_int(args[4]);
    mp_int_t col = mp_obj_get_int(args[5]);

    mp_int_t dx = x2 - x1;
    mp_int_t sx;
    if (dx > 0) {
        sx = 1;
    } else {
        dx = -dx;
        sx = -1;
    }

    mp_int_t dy = y2 - y1;
    mp_int_t sy;
    if (dy > 0) {
        sy = 1;
    } else {
        dy = -dy;
        sy = -1;
    }

    bool steep;
    if (dy > dx) {
        mp_int_t temp;
        temp = x1; x1 = y1; y1 = temp;
        temp = dx; dx = dy; dy = temp;
        temp = sx; sx = sy; sy = temp;
        steep = true;
    } else {
        steep = false;
    }

    mp_int_t e = 2 * dy - dx;
    for (mp_int_t i = 0; i < dx; ++i) {
        if (steep) {
            if (0 <= y1 && y1 < self->width && 0 <= x1 && x1 < self->height) {
                setpixel(self, y1, x1, col);
            }
        } else {
            if (0 <= x1 && x1 < self->width && 0 <= y1 && y1 < self->height) {
                setpixel(self, x1, y1, col);
            }
        }
        while (e >= 0) {
            y1 += sy;
            e -= 2 * dx;
        }
        x1 += sx;
        e += 2 * dy;
    }

    if (0 <= x2 && x2 < self->width && 0 <= y2 && y2 < self->height) {
        setpixel(self, x2, y2, col);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_line_obj, 6, 6, framebuf_line);

STATIC mp_obj_t framebuf_blit(size_t n_args, const mp_obj_t *args) {
    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_obj_framebuf_t *source = MP_OBJ_TO_PTR(args[1]);
    mp_int_t x = mp_obj_get_int(args[2]);
    mp_int_t y = mp_obj_get_int(args[3]);
    mp_int_t key = -1;
    if (n_args > 4) {
        key = mp_obj_get_int(args[4]);
    }

    if (
        (x >= self->width) ||
        (y >= self->height) ||
        (-x >= source->width) ||
        (-y >= source->height)
    ) {
        // Out of bounds, no-op.
        return mp_const_none;
    }

    // Clip.
    int x0 = MAX(0, x);
    int y0 = MAX(0, y);
    int x1 = MAX(0, -x);
    int y1 = MAX(0, -y);
    int x0end = MIN(self->width, x + source->width);
    int y0end = MIN(self->height, y + source->height);

    for (; y0 < y0end; ++y0) {
        int cx1 = x1;
        for (int cx0 = x0; cx0 < x0end; ++cx0) {
            uint32_t col = getpixel(source, cx1, y1);
            if (col != (uint32_t)key) {
                setpixel(self, cx0, y0, col);
            }
            ++cx1;
        }
        ++y1;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_blit_obj, 4, 5, framebuf_blit);

STATIC mp_obj_t framebuf_scroll(mp_obj_t self_in, mp_obj_t xstep_in, mp_obj_t ystep_in) {
    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t xstep = mp_obj_get_int(xstep_in);
    mp_int_t ystep = mp_obj_get_int(ystep_in);
    int sx, y, xend, yend, dx, dy;
    if (xstep < 0) {
        sx = 0;
        xend = self->width + xstep;
        dx = 1;
    } else {
        sx = self->width - 1;
        xend = xstep - 1;
        dx = -1;
    }
    if (ystep < 0) {
        y = 0;
        yend = self->height + ystep;
        dy = 1;
    } else {
        y = self->height - 1;
        yend = ystep - 1;
        dy = -1;
    }
    for (; y != yend; y += dy) {
        for (int x = sx; x != xend; x += dx) {
            setpixel(self, x, y, getpixel(self, x - xstep, y - ystep));
        }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(framebuf_scroll_obj, framebuf_scroll);

STATIC mp_obj_t framebuf_text(size_t n_args, const mp_obj_t *args) {
    // extract arguments
    mp_obj_framebuf_t *self = MP_OBJ_TO_PTR(args[0]);
    const char *str = mp_obj_str_get_str(args[1]);
    mp_int_t x0 = mp_obj_get_int(args[2]);
    mp_int_t y0 = mp_obj_get_int(args[3]);
    mp_int_t col = 1;
    if (n_args >= 5) {
        col = mp_obj_get_int(args[4]);
    }

    // loop over chars
    for (; *str; ++str) {
        // get char and make sure its in range of font
        int chr = *(uint8_t*)str;
        if (chr < 32 || chr > 127) {
            chr = 127;
        }
        // get char data
        const uint8_t *chr_data = &font_petme128_8x8[(chr - 32) * 8];
        // loop over char data
        for (int j = 0; j < 8; j++, x0++) {
            if (0 <= x0 && x0 < self->width) { // clip x
                uint vline_data = chr_data[j]; // each byte is a column of 8 pixels, LSB at top
                for (int y = y0; vline_data; vline_data >>= 1, y++) { // scan over vertical column
                    if (vline_data & 1) { // only draw if pixel set
                        if (0 <= y && y < self->height) { // clip y
                            setpixel(self, x0, y, col);
                        }
                    }
                }
            }
        }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(framebuf_text_obj, 4, 5, framebuf_text);

STATIC const mp_rom_map_elem_t framebuf_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_fill), MP_ROM_PTR(&framebuf_fill_obj) },
    { MP_ROM_QSTR(MP_QSTR_fill_rect), MP_ROM_PTR(&framebuf_fill_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_pixel), MP_ROM_PTR(&framebuf_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_hline), MP_ROM_PTR(&framebuf_hline_obj) },
    { MP_ROM_QSTR(MP_QSTR_vline), MP_ROM_PTR(&framebuf_vline_obj) },
    { MP_ROM_QSTR(MP_QSTR_rect), MP_ROM_PTR(&framebuf_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_line), MP_ROM_PTR(&framebuf_line_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit), MP_ROM_PTR(&framebuf_blit_obj) },
    { MP_ROM_QSTR(MP_QSTR_scroll), MP_ROM_PTR(&framebuf_scroll_obj) },
    { MP_ROM_QSTR(MP_QSTR_text), MP_ROM_PTR(&framebuf_text_obj) },
};
STATIC MP_DEFINE_CONST_DICT(framebuf_locals_dict, framebuf_locals_dict_table);

STATIC const mp_obj_type_t mp_type_framebuf = {
    { &mp_type_type },
    .name = MP_QSTR_FrameBuffer,
    .make_new = framebuf_make_new,
    .buffer_p = { .get_buffer = framebuf_get_buffer },
    .locals_dict = (mp_obj_dict_t*)&framebuf_locals_dict,
};

// this factory function is provided for backwards compatibility with old FrameBuffer1 class
STATIC mp_obj_t legacy_framebuffer1(size_t n_args, const mp_obj_t *args) {
    mp_obj_framebuf_t *o = m_new_obj(mp_obj_framebuf_t);
    o->base.type = &mp_type_framebuf;

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[0], &bufinfo, MP_BUFFER_WRITE);
    o->buf = bufinfo.buf;

    o->width = mp_obj_get_int(args[1]);
    o->height = mp_obj_get_int(args[2]);
    o->format = FRAMEBUF_MVLSB;
    if (n_args >= 4) {
        o->stride = mp_obj_get_int(args[3]);
    } else {
        o->stride = o->width;
    }

    return MP_OBJ_FROM_PTR(o);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(legacy_framebuffer1_obj, 3, 4, legacy_framebuffer1);

STATIC const mp_rom_map_elem_t framebuf_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_framebuf) },
    { MP_ROM_QSTR(MP_QSTR_FrameBuffer), MP_ROM_PTR(&mp_type_framebuf) },
    { MP_ROM_QSTR(MP_QSTR_FrameBuffer1), MP_ROM_PTR(&legacy_framebuffer1_obj) },
    { MP_ROM_QSTR(MP_QSTR_MVLSB), MP_ROM_INT(FRAMEBUF_MVLSB) },
    { MP_ROM_QSTR(MP_QSTR_MONO_VLSB), MP_ROM_INT(FRAMEBUF_MVLSB) },
    { MP_ROM_QSTR(MP_QSTR_RGB565), MP_ROM_INT(FRAMEBUF_RGB565) },
    { MP_ROM_QSTR(MP_QSTR_XRGB8888), MP_ROM_INT(FRAMEBUF_XRGB8888) },
    { MP_ROM_QSTR(MP_QSTR_GS2_HMSB), MP_ROM_INT(FRAMEBUF_GS2_HMSB) },
    { MP_ROM_QSTR(MP_QSTR_GS4_HMSB), MP_ROM_INT(FRAMEBUF_GS4_HMSB) },
    { MP_ROM_QSTR(MP_QSTR_GS8), MP_ROM_INT(FRAMEBUF_GS8) },
    { MP_ROM_QSTR(MP_QSTR_MONO_HLSB), MP_ROM_INT(FRAMEBUF_MHLSB) },
    { MP_ROM_QSTR(MP_QSTR_MONO_HMSB), MP_ROM_INT(FRAMEBUF_MHMSB) },
};

STATIC MP_DEFINE_CONST_DICT(framebuf_module_globals, framebuf_module_globals_table);

const mp_obj_module_t mp_module_framebuf = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&framebuf_module_globals,
};

#endif // MICROPY_PY_FRAMEBUF
