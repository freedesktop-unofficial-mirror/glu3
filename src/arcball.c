/*
 * Copyright © 2009 Ian D. Romanick
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
#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif

#include <math.h>
#include "glu3.h"


static void
map_to_sphere(float x, float y, GLUvec4 *v)
{
	const float length_squared = (x * x) + (y * y);


	if (length_squared > 1.0f) {
		const float n = 1.0f / sqrt(length_squared);

		v->values[0] = x * n;
		v->values[1] = y * n;
		v->values[2] = 0.0f;
	} else {
		v->values[0] = x;
		v->values[1] = y;
		v->values[2] = sqrt(1.0 - length_squared);
	}

	v->values[3] = 0.0;
}


static float
remap_x(float w_factor, unsigned x)
{
	return (x * w_factor) - 1.0f;
}


static float
remap_y(float h_factor, unsigned y)
{
	return 1.0f - (y * h_factor);
}


static void
get_transformation(GLUmat4 *mat, const GLUvec4 *quat)
{
	const float xx = quat->values[0] * quat->values[0];
	const float xy = quat->values[0] * quat->values[1];
	const float xz = quat->values[0] * quat->values[2];
	const float xw = quat->values[0] * quat->values[3];

	const float yy = quat->values[1] * quat->values[1];
	const float yz = quat->values[1] * quat->values[2];
	const float yw = quat->values[1] * quat->values[3];

	const float zz = quat->values[2] * quat->values[2];
	const float zw = quat->values[2] * quat->values[3];


	mat->col[0].values[0] = 1.0f - 2.0f * (yy + zz);
	mat->col[0].values[1] =        2.0f * (xy + zw);
	mat->col[0].values[2] =        2.0f * (xz - yw);
	mat->col[0].values[3] = 0.0f;

	mat->col[1].values[0] =        2.0f * (xy - zw);
	mat->col[1].values[1] = 1.0f - 2.0f * (xx + zz);
	mat->col[1].values[2] =        2.0f * (yz + xw);
	mat->col[1].values[3] = 0.0f;

	mat->col[2].values[0] =        2.0f * (xz + yw);
	mat->col[2].values[1] =        2.0f * (yz - xw);
	mat->col[2].values[2] = 1.0f - 2.0f * (xx + yy);
	mat->col[2].values[3] = 0.0f;

	mat->col[3].values[0] = 0.0f;
	mat->col[3].values[1] = 0.0f;
	mat->col[3].values[2] = 0.0f;
	mat->col[3].values[3] = 1.0f;
}


void
gluArcballClick(GLUarcball *ball, unsigned x, unsigned y)
{
	ball->click_x = x;
	ball->click_y = y;
}


void
gluArcballDrag(GLUarcball *ball, GLUmat4 *result, unsigned x, unsigned y)
{
	GLUvec4 end;
	GLUvec4 pick;
	GLUvec4 axis;
	float w_factor;
	float h_factor;
	float length_squared;

	if ((ball->viewport_width == 0.0) || (ball->viewport_height == 0.0)) {
		memcpy(result, & gluIdentityMatrix, sizeof(gluIdentityMatrix));
		return;
	}

	w_factor = 2.0f / (float)(ball->viewport_width  - 1);
	h_factor = 2.0f / (float)(ball->viewport_height - 1);

	map_to_sphere(remap_x(w_factor, ball->click_x - ball->viewport_x),
		      remap_y(h_factor, ball->click_y - ball->viewport_y),
		      & pick);
	map_to_sphere(remap_x(w_factor, x - ball->viewport_x),
		      remap_y(h_factor, y - ball->viewport_y),
		      & end);


	gluCross4v(& axis, & pick, & end);

	length_squared = gluLengthSqr4v(& axis);
	if (length_squared > 0.0000001) {
		axis.values[3] = gluDot3_4v(& pick, & end);
	} else {
		axis.values[0] = 0.0f;
		axis.values[1] = 0.0f;
		axis.values[2] = 0.0f;
		axis.values[3] = 0.0f;
	}

	get_transformation(result, & axis);
}
