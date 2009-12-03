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

#include <glu3.h>
#include <math.h>
#include <assert.h>

struct foo {
	GLUmat4 matrix;
	GLfloat det;
};

static const struct foo test_cases[] = {
	{
		{{
			{{1.0, 0.0, 0.0, 0.0}},
			{{0.0, 1.0, 0.0, 0.0}},
			{{0.0, 0.0, 1.0, 0.0}},
			{{0.0, 0.0, 0.0, 1.0}}
		}},
		1.0
	},
	{
		{{
			{{1.0, 0.0, 0.0, 0.0}},
			{{0.0, 1.0, 0.0, 0.0}},
			{{0.0, 0.0, 1.0, 0.0}},
			{{0.0, 0.0, 0.0, 1.0}}
		}},
		1.0
	},
};

#define ELEMENTS_OF(a)  (sizeof(a) / sizeof(a[0]))

int main(int argc, char **argv)
{
	unsigned i;

	(void)argc;
	(void)argv;

	for (i = 0; i < ELEMENTS_OF(test_cases); i++) {
		const GLfloat det = gluDeterminant4_4m(& test_cases[i].matrix);

		assert(fabs(det - test_cases[i].det) < 0.0000001);
	}

	return 0;
}
