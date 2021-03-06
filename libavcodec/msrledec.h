/*
 * Micrsoft RLE Decoder
 * Copyright (C) 2008 Konstantin Shishkov
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_MSRLEDEC_H
#define AVCODEC_MSRLEDEC_H

#include "avcodec.h"

/**
 * decode stream in MS RLE format into frame
 *
 * @param avctx     codec context
 * @param pic       destination frame
 * @param depth     bit depth
 * @param data      input stream
 * @param data_size input size
 */
int ff_msrle_decode(AVCodecContext *avctx, AVPicture *pic, int depth,
                    const uint8_t* data, int data_size);

#endif /* AVCODEC_MSRLEDEC_H */

