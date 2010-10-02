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

#include "glu3.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (mesh_begin_cb)(void *data, GLenum mode, unsigned count);
typedef void (mesh_index_cb)(void *data, unsigned index);
typedef void (mesh_end_cb)(void *data);

/**
 * Generate indexed drawing primitives for a rectangular mesh.
 *
 * Generates a sequence of \c GL_TRANGLE_STRIP primitives to draw a rectangular
 * mesh with the specified parameters.  Each triangle strip generated will be
 * of the form:
 *
 * (i*width)+0, ((i+1) *width)+0, (i*width)+1, ((i+1) *width)+1, ...
 * (i*width)+(cols-1), ((i+1) *width)+(cols-1)
 *
 * where \c i is the index of the row being generated, starting from 0.
 *
 * The triangle strips are generated in row-major order.
 *
 * \param rows      Number of rows of primitives in the mesh.  This value must
 *                  be at least 1.  Note that if there are \c rows of
 *                  primitives, there must be at least \c rows + 1 of vertices.
 * \param cols      Number of columns of primitives in the mesh.  This value
 *                  must be at least 1.  Note that if there are \c cols of
 *                  primitives, there must be at least \c cols + 1 of vertices.
 * \param width     Width, in vertices, of the underlying mesh data.  If
 *                  \c width is greater than \c cols, data for a submesh within
 *                  a larger primitive can be generated.  If \c width is less
 *                  than \c cols, the results are undefined (and probably
 *                  wrong).
 * \param begin_cb  Call-back used to begin a new primitive.
 * \param index_cb  Call-back used for each index within a primitive.  This
 *                  will be called 2 * \c cols times between each call to
 *                  \c begin_cb and \c end_cb.
 * \param end_cb    Call-back used to end a primitive.
 * \param data      User-supplied data pointer passed to the various call-back
 *                  functions.
 *
 * \sa glDrawMeshArraysSUN
 */
extern void
generate_triangle_mesh(unsigned rows, unsigned cols, unsigned width, 
		       mesh_begin_cb *begin_cb, 
		       mesh_index_cb *index_cb, 
		       mesh_end_cb *end_cb, 
		       void *data);

#ifdef __cplusplus
};
#endif
