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

static GLboolean vec4_equals(GLUvec4 *a, GLUvec4 *b)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (a->values[i] != b->values[i])
			return GL_FALSE;
	}

	return GL_TRUE;
}

int main(int argc, char **argv)
{
	GLUvec4 a = {{1.0, 2.0, 3.0, 4.0}};
	GLUvec4 b = {{-2.0, 3.0, 5.0, -10.0}};
	GLUvec4 expected = {{-1.0, 5.0, 8.0, -6.0}};
	GLUvec4 result;

	(void)argc;
	(void)argv;

	gluAdd4v_4v(&result, &a, &b);

	assert(vec4_equals(&result, &expected));

	return 0;
}
