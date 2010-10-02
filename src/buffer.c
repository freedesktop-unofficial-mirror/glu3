/*
 * Copyright © 2010 Ian D. Romanick
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>
#include "buffer.h"

void
cb_buffer_init(struct cb_buffer *buf, GLUvec4 *pos,
	       GLUvec4 *nrm, GLUvec4 *tng, GLUvec4 *uv,
	       unsigned size)
{
	memset(buf, 0, sizeof(*buf));

	buf->pos = pos;
	buf->nrm = nrm;
	buf->tng = tng;
	buf->uv = uv;
	buf->size = size;

	if (buf->pos == NULL) {
		buf->pos = malloc(sizeof(GLUvec4) * buf->size);
		buf->free_pos = true;
	}

	if (buf->nrm == NULL) {
		buf->nrm = malloc(sizeof(GLUvec4) * buf->size);
		buf->free_nrm = true;
	}

	if (buf->tng == NULL) {
		buf->tng = malloc(sizeof(GLUvec4) * buf->size);
		buf->free_tng = true;
	}

	if (buf->uv == NULL) {
		buf->uv = malloc(sizeof(GLUvec4) * buf->size);
		buf->free_uv = true;
	}
}

void
cb_buffer_destroy(struct cb_buffer *buf)
{
	if (buf->free_pos)
		free(buf->pos);

	if (buf->free_nrm)
		free(buf->nrm);

	if (buf->free_tng)
		free(buf->tng);

	if (buf->free_uv)
		free(buf->uv);

	memset(buf, 0, sizeof(*buf));
}
