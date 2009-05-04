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

#include <string.h>
#include "glu3.h"

#define DEG2RAD(d) ((d) * M_PI / 180.0)

const GLUmat4 gluIdentityMatrix = {
	{
		{ { 1.0f, 0.0f,  0.0f,  0.0f } },
		{ { 0.0f, 1.0f,  0.0f,  0.0f } },
		{ { 0.0f, 0.0f,  1.0f,  0.0f } },
		{ { 0.0f, 0.0f,  0.0f,  1.0f } }
	}
};


GLUmat4 gluTranslate4v(const GLUvec4 *t)
{
	GLUmat4 result;

	memcpy(& result, & gluIdentityMatrix, sizeof(gluIdentityMatrix));
	result.col[3] = *t;
	result.col[3].values[3] = 1.0f;

	return result;
}


GLUmat4 gluScale4v(const GLUvec4 *t)
{
	GLUmat4 result;

	memcpy(& result, & gluIdentityMatrix, sizeof(gluIdentityMatrix));
	result.col[0].values[0] = t->values[0];
	result.col[1].values[1] = t->values[1];
	result.col[2].values[2] = t->values[2];

	return result;
}


GLUmat4 gluLookAt4v(const GLUvec4 *eye,
		    const GLUvec4 *center,
		    const GLUvec4 *up)
{
	static const GLUvec4 col3 = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	const GLUvec4 e = { 
		{ -eye->values[0], -eye->values[1], -eye->values[2], 0.0f }
	};
	const GLUmat4 translate = gluTranslate4v(& e);
	GLUmat4  rotate;
	GLUvec4  f;
	GLUvec4  s;
	GLUvec4  u;


	f = gluSub4v_4v(center, eye);
	f = gluNormalize4v(& f);

	u = gluNormalize4v(up);

	s =  gluCross4v(& f, & u);
	u =  gluCross4v(& s, & f);

	rotate.col[0] = s;
	rotate.col[1] = u;
	rotate.col[2].values[0] = -f.values[0];
	rotate.col[2].values[1] = -f.values[1];
	rotate.col[2].values[2] = -f.values[2];
	rotate.col[2].values[3] = 0.0f;
	rotate.col[3] = col3;
	rotate = gluTranspose4(& rotate);

	return gluMult4m_4m(& rotate, & translate);
}


GLUmat4 gluRotate4v(const GLUvec4 *_axis, GLfloat angle)
{
#if 0
	GLUvec4 axis = gluNormalize4v(_axis);
	GLUmat4 E =  {
		{
			{ { 0.0f, -axis.values[2], axis.values[1], 0.0f } },
			{ { axis.values[3], 0.0f, -axis.values[0], 0.0f } },
			{ { -axis.values[1], axis.values[0], 0.0f, 0.0f } },
			{ { 0.0f, 0.0f,             0.0f,            1.0f } }
		}
	};

	const GLfloat cos_a = cos(angle);
	const GLfloat sin_a = sin(angle);

	GLUmat4 O = gluOuter4v(&axis, &axis);
	const GLUmat4 I = gluMult4m_f(& gluIdentityMatrix, cos_a);
	GLUmat4 temp;


	E = gluMult4m_f(& E, -sin_a);
	O = gluMult4m_f(& O, 1.0f - cos_a);

	temp = gluAdd4m_4m(& I, & O);
	return gluAdd4m_4m(& temp, & E);
#else
	GLUvec4 axis = gluNormalize4v(_axis);
	GLUmat4 m;
	const float c = cos(angle);
	const float s = sin(angle);
	const float one_c = 1.0 - c;

	const float xx = axis.values[0] * axis.values[0];
	const float yy = axis.values[1] * axis.values[1];
	const float zz = axis.values[2] * axis.values[2];

	const float xs = axis.values[0] * s;
	const float ys = axis.values[1] * s;
	const float zs = axis.values[2] * s;

	const float xy = axis.values[0] * axis.values[1];
	const float xz = axis.values[0] * axis.values[2];
	const float yz = axis.values[1] * axis.values[2];


	m.col[0].values[0] = (one_c * xx) + c;
	m.col[0].values[1] = (one_c * xy) + zs;
	m.col[0].values[2] = (one_c * xz) - ys;
	m.col[0].values[3] = 0.0;

	m.col[1].values[0] = (one_c * xy) - zs;
	m.col[1].values[1] = (one_c * yy) + c;
	m.col[1].values[2] = (one_c * yz) + xs;
	m.col[1].values[3] = 0.0;


	m.col[2].values[0] = (one_c * xz) + ys;
	m.col[2].values[1] = (one_c * yz) - xs;
	m.col[2].values[2] = (one_c * zz) + c;
	m.col[2].values[3] = 0.0;

	m.col[3].values[0] = 0.0;
	m.col[3].values[1] = 0.0;
	m.col[3].values[2] = 0.0;
	m.col[3].values[3] = 1.0;

	return m;
#endif
}


GLUmat4
gluPerspective4(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far)
{
	GLUmat4 result;
	const double sine = sin(DEG2RAD(fovy / 2.0));
	const double cosine = cos(DEG2RAD(fovy / 2.0));
	const double sine_aspect = sine * aspect;
	const double dz = far - near;


	memcpy(&result, &gluIdentityMatrix, sizeof(result));
	if ((sine == 0.0) || (dz == 0.0) || (sine_aspect == 0.0)) {
		return result;
	}

	result.col[0].values[0] = cosine / sine_aspect;
	result.col[1].values[1] = cosine / sine;
	result.col[2].values[2] = -(far + near) / dz;
	result.col[2].values[3] = -1.0;
	result.col[3].values[2] = -2.0 * near * far / dz;
	result.col[3].values[3] =  0.0;

	return result;
}
