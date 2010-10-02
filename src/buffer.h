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

#ifndef GLU3_BUFFER_H
#define GLU3_BUFFER_H

#include "system.h"
#include "glu3.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct cb_buffer {
	GLUvec4 *pos;
	GLUvec4 *nrm;
	GLUvec4 *tng;
	GLUvec4 *uv;
	unsigned size;
	unsigned used;

	unsigned free_pos:1;
	unsigned free_nrm:1;
	unsigned free_tng:1;
	unsigned free_uv:1;
};

extern void cb_buffer_init(struct cb_buffer *buf, GLUvec4 *pos,
			   GLUvec4 *nrm, GLUvec4 *tng, GLUvec4 *uv,
			   unsigned size);

extern void cb_buffer_destroy(struct cb_buffer *buf);

#define CB_BUFFER_APPEND(cb, p, n, t, u)		\
	do {						\
		(cb).pos[(cb).used] = (p);		\
		(cb).nrm[(cb).used] = (n);		\
		(cb).tng[(cb).used] = (t);		\
		(cb).uv[(cb).used] = (u);		\
		(cb).used += 1;				\
	} while (0)

#define CB_BUFFER_IS_FULL(cb)    ((cb).used >= (cb).size)
#define CB_BUFFER_IS_EMPTY(cb)   ((cb).used == 0)
#define CB_BUFFER_MAKE_EMPTY(cb) ((cb).used = 0)

#ifdef __cplusplus
};
#endif

#endif /* GLU3_BUFFER_H */
