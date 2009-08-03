/*
 * Copyright © 2009 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#include <glu3.h>
#include <assert.h>

static GLboolean vec4_equals(const GLUvec4 *a, const GLUvec4 *b)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (a->values[i] != b->values[i])
			return GL_FALSE;
	}

	return GL_TRUE;
}

static void vec4_divide(GLUvec4 *a)
{
	a->values[0] /= a->values[3];
	a->values[1] /= a->values[3];
	a->values[2] /= a->values[3];
	a->values[3] = 1.0;
}

int main(int argc, char **argv)
{
	GLUvec4 eye = {{0.0, 0.0, 0.0, 0.0}};
	GLUvec4 center = {{0.0, 0.0, -1.0, 0.0}};
	GLUvec4 up = {{0.0, 1.0, 0.0, 0.0}};
	GLUvec4 result;
	GLUmat4 identity_lookat;
	GLUvec4 a = {{1.0, 2.0, 3.0, 1.0}};
	GLUvec4 b = {{-2.0, 3.0, 5.0, 1.0}};

	(void)argc;
	(void)argv;

	gluLookAt4v(&identity_lookat,
		    &eye,
		    &center,
		    &up);
	gluMult4m_4v(&result, &identity_lookat, &a);
	vec4_divide(&result);
	assert(vec4_equals(&result, &a));
	gluMult4m_4v(&result, &identity_lookat, &b);
	vec4_divide(&result);
	assert(vec4_equals(&result, &b));

	/* Make sure that LookAt doesn't take the 4th component
	 * eye/center/up into account.
	 */
	eye.values[3] = 1.0;
	center.values[3] = 1.0;
	up.values[3] = 1.0;

	gluLookAt4v(&identity_lookat,
		    &eye,
		    &center,
		    &up);
	gluMult4m_4v(&result, &identity_lookat, &a);
	vec4_divide(&result);
	assert(vec4_equals(&result, &a));
	gluMult4m_4v(&result, &identity_lookat, &b);
	vec4_divide(&result);
	assert(vec4_equals(&result, &b));

	return 0;
}
