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
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include "revolve.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void
revolve(const GLUvec4 *points, const GLUvec4 *normals, const float *u,
	unsigned num_points, 
	const GLUvec4 *axis,
	unsigned steps, float start_angle, float end_angle,
	revolve_cb *cb, void *data, struct cb_buffer *buf)
{
	const float angle_step = (end_angle - start_angle) / (float) (steps - 1);
	const GLUvec4 tangent = {{ 0.0, 0.0, 1.0, 0.0 }};
	unsigned i;
	unsigned j;


	for (i = 0; i < steps; i++) {
		const float a = start_angle + (angle_step * i);
		const float v = (float) i / (float) (steps - 1);
		GLUmat4 r;

		/* Generate a rotation matrix to rotate the position and the
		 * normal around the axis by the current angular step.  This
		 * generates the new position and normal.
		 */
		gluRotate4v(& r, axis, a);

		for (j = 0; j < num_points; j++) {
			gluMult4m_4v(& buf->pos[buf->used], & r, & points[j]);
			gluMult4m_4v(& buf->nrm[buf->used], & r, & normals[j]);
			gluMult4m_4v(& buf->tng[buf->used], & r, & tangent);

			buf->uv[buf->used].values[0] = u[j];
			buf->uv[buf->used].values[1] = v;
			buf->uv[buf->used].values[2] = 0.0;
			buf->uv[buf->used].values[3] = 0.0;

			buf->used++;

			if (CB_BUFFER_IS_FULL(*buf)) {
				(*cb)(data, buf->pos, buf->nrm, buf->tng,
				      buf->uv, buf->used);
				CB_BUFFER_MAKE_EMPTY(*buf);
			}
		}
	}

	if (!CB_BUFFER_IS_EMPTY(*buf)) {
		(*cb)(data, buf->pos, buf->nrm, buf->tng, buf->uv, buf->used);
		CB_BUFFER_MAKE_EMPTY(*buf);
	}
}


void
generate_sphere(double radius, unsigned slices, unsigned stacks,
		_Bool normals_point_out,
		revolve_cb *cb, void *data, struct cb_buffer *buf)
{
	const GLUvec4 y_axis = {{ 0.0, 1.0, 0.0, 0.0 }};
	GLUvec4 *positions;
	GLUvec4 *normals;
	float *u;
	double latitude_step;
	unsigned i;

	positions = (GLUvec4 *) malloc((2 * sizeof(GLUvec4) + sizeof(float))
				       * (stacks + 1));
	normals = positions + (stacks + 1);
	u = (float *)(normals + (stacks + 1));

	latitude_step = M_PI / (double)(stacks);
	for (i = 0; i < (stacks + 1); i++) {
		const double latitude = latitude_step * i;

		normals[i].values[0] = sin(latitude);
		normals[i].values[1] = cos(latitude);
		normals[i].values[2] = 0.0;
		normals[i].values[3] = 0.0;

		gluMult4v_f(& positions[i],
			    & normals[i],
			    radius);
		positions[i].values[3] = 1.0;

		u[i] = (float) i / (float) stacks;

		if (!normals_point_out) {
			normals[i].values[0] = -normals[i].values[0];
			normals[i].values[1] = -normals[i].values[1];
		}
	}

	/* Note the rotation is *from* 2pi *to* 0.  The counter-clockwise
	 * rotation ensures that the vertices are generated in the correct
	 * order to be drawn using triangle strips with back-face culling.
	 */
	revolve(positions, normals, u, stacks + 1, & y_axis,
		slices + 1, 2.0 * M_PI, 0.0, cb, data, buf);
}


void
generate_torus(double tube_radius, double path_radius,
	       unsigned sides, unsigned rings, _Bool normals_point_out,
	       revolve_cb *cb, void *data, struct cb_buffer *buf)
{
	const GLUvec4 y_axis = {{ 0.0, 1.0, 0.0, 0.0 }};
	GLUvec4 *positions;
	GLUvec4 *normals;
	float *u;
	double angle_step;
	unsigned i;

	positions = (GLUvec4 *) malloc((2 * sizeof(GLUvec4) + sizeof(float))
				       * (sides + 1));
	normals = positions + (sides + 1);
	u = (float *)(normals + (sides + 1));

	angle_step = 2.0 * M_PI / (double)(sides);
	for (i = 0; i < (sides + 1); i++) {
		const double angle = angle_step * i;

		normals[i].values[0] = sin(angle);
		normals[i].values[1] = cos(angle);
		normals[i].values[2] = 0.0;
		normals[i].values[3] = 0.0;

		gluMult4v_f(& positions[i],
			    & normals[i],
			    tube_radius);
		positions[i].values[3] = 1.0;
		positions[i].values[0] += path_radius;

		u[i] = (float) i / (float) sides;

		if (!normals_point_out) {
			normals[i].values[0] = -normals[i].values[0];
			normals[i].values[1] = -normals[i].values[1];
		}
	}

	/* Note the rotation is *from* 2pi *to* 0.  The counter-clockwise
	 * rotation ensures that the vertices are generated in the correct
	 * order to be drawn using triangle strips with back-face culling.
	 */
	revolve(positions, normals, u, sides + 1, & y_axis,
		rings + 1, 2.0 * M_PI, 0.0, cb, data, buf);
}
