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

static const GLUmat4 identity = {
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

	memcpy(& result, & identity, sizeof(identity));
	result.col[3] = *t;
	result.col[3].values[3] = 1.0f;

	return result;
}


GLUmat4 gluScale4v(const GLUvec4 *t)
{
	GLUmat4 result;

	memcpy(& result, & identity, sizeof(identity));
	result.col[0].values[0] = t->values[0];
	result.col[1].values[1] = t->values[1];
	result.col[2].values[2] = t->values[2];

	return result;
}


GLUmat4 gluLookAt(const GLUvec4 *eye,
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
	rotate.col[2] = s;
	rotate.col[3] = col3;
	rotate = gluTranspose4(& rotate);

	return gluMult4m_4m(& rotate, & translate);
}


GLUmat4 gluRotate(const GLUvec4 *axis, GLfloat angle)
{
	GLUmat4 E =  {
		{
			{ { 0.0f, -axis->values[2], axis->values[1], 0.0f } },
			{ { axis->values[3], 0.0f, -axis->values[0], 0.0f } },
			{ { -axis->values[1], axis->values[0], 0.0f, 0.0f } },
			{ { 0.0f, 0.0f,             0.0f,            1.0f } }
		}
	};

	const GLfloat cos_a = cos(angle);
	const GLfloat sin_a = sin(angle);

	GLUmat4 O = gluOuter4v(axis, axis);
	const GLUmat4 I = gluMult4m_f(& identity, cos_a);
	GLUmat4 temp;


	E = gluMult4m_f(& E, -sin_a);
	O = gluMult4m_f(& O, 1.0f - cos_a);

	temp = gluAdd4m_4m(& I, & O);
	return gluAdd4m_4m(& temp, & E);
}
