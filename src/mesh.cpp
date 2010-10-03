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

#include "mesh.h"

#define Elements(a)  (sizeof(a) / sizeof(a[0]))

#define EMIT_ELT(e)					\
	do {						\
		elts[count] = (e);			\
		count++;				\
		if (count >= Elements(elts)) {		\
			(*index_cb)(data, elts, count);	\
			count = 0;			\
		}					\
	} while (0)

void
generate_triangle_mesh(unsigned rows, unsigned cols, unsigned width, 
		       mesh_begin_cb *begin_cb, 
		       mesh_index_cb *index_cb, 
		       mesh_end_cb *end_cb, 
		       void *data)
{
	unsigned elts[64];
	unsigned count = 0;
	unsigned i;
	int j;

	(*begin_cb)(data, GL_TRIANGLE_STRIP, rows * ((2 * cols) + 1));
	for (i = 0; i < rows; i++) {
		if ((i & 1) == 0) {
			for (j = 0; j < (int)cols; j++) {
				const unsigned e0 = ((i + 0) * width) + j;
				const unsigned e1 = ((i + 1) * width) + j;

				EMIT_ELT(e0);
				EMIT_ELT(e1);
			}

			EMIT_ELT((width - 1) + ((i + 0) * width));
		} else {
			for (j = cols - 1; j >= 0; j--) {
				const unsigned e0 = ((i + 0) * width) + j;
				const unsigned e1 = ((i + 1) * width) + j;

				EMIT_ELT(e0);
				EMIT_ELT(e1);
			}

			EMIT_ELT((i + 0) * width);
		}
	}

	if (count != 0)
		(*index_cb)(data, elts, count);

	(*end_cb)(data);
}
