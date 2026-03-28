/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*
* Description:
*/
#ifndef _AML_VCODEC_DPB_WAIT_H_
#define _AML_VCODEC_DPB_WAIT_H_

#include <linux/compiler.h>
#include <linux/delay.h>
#include <media/v4l2-mem2mem.h>

#include "aml_vcodec_drv.h"

#define AML_VCODEC_WAIT_SLICE_US 1000
#define AML_VCODEC_WAIT_SLICE_US_MAX 2000
#define AML_VCODEC_WAIT_TIMEOUT_MS 1000

static inline bool aml_vcodec_wait_dpb_ready(struct aml_vcodec_ctx *ctx)
{
	ulong expires = jiffies +
		msecs_to_jiffies(AML_VCODEC_WAIT_TIMEOUT_MS);

	while (!READ_ONCE(ctx->v4l_codec_dpb_ready)) {
		u32 ready_num;

		if (time_after(jiffies, expires))
			return false;

		ready_num = v4l2_m2m_num_dst_bufs_ready(ctx->m2m_ctx);
		if ((ready_num + ctx->buf_used_count) >= ctx->dpb_size)
			WRITE_ONCE(ctx->v4l_codec_dpb_ready, true);
		else
			usleep_range(AML_VCODEC_WAIT_SLICE_US,
				AML_VCODEC_WAIT_SLICE_US_MAX);
	}

	return true;
}

#endif