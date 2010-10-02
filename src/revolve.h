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

#include "system.h"
#include "glu3.h"
#include "buffer.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (revolve_cb)(void *data, const GLUvec4 *position,
			  const GLUvec4 *normal, const GLUvec4 *tangent,
			  const GLUvec4 *uv, unsigned count);

/**
 * Generate a surface of revolution
 *
 * \param points      Points that define the outline of the shape to be
 *                    revolved.
 * \param normals     Initial per-vertex normals.
 * \param u           Per-vertex position in parameter-space along the u-axis.
 * \param num_points  Number of points in \c points, \c normals, and \c u.
 * \param axis        Axis around which to rotate.
 * \param steps       Total number of steps to rotate through.  The angle of
 *                    rotation at the first step will be \c start_angle, and
 *                    the angle of rotation at the last step will be
 *                    \c end_angle.
 * \param start_angle Initial angle of rotation.
 * \param end_angle   Final angle of rotation.
 * \param cb          Per-vertex call-back function.
 * \param data        User-supplied data pointer passed to the per-vertex
 *                    call-back function.
 *
 * \note
 * Per-vertex tangents are generate based on the assumption that all of the
 * initial vertices lie within the X-Y plane and that the axis of revolution
 * also lies within the X-Y plane.
 */
extern void
revolve(const GLUvec4 *points, const GLUvec4 *normals, const float *u,
	unsigned num_points, 
	const GLUvec4 *axis,
	unsigned steps, float start_angle, float end_angle,
	revolve_cb *cb, void *data, struct cb_buffer *buf);

extern void
generate_sphere(double radius, unsigned slices, unsigned stacks,
		bool normals_point_out, revolve_cb *cb, void *data,
		struct cb_buffer *buf);

#ifdef __cplusplus
};
#endif
