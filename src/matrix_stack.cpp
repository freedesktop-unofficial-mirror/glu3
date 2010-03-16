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

extern "C" {
void
gluPushMatrix4m(struct GLUmat4Stack *s)
{
	if (s->size >= GLU_MAX_STACK_DEPTH)
		return;

	memmove(& s->stack[1], &  s->stack[0], sizeof(s->stack[0]) * s->size);
	s->size++;
}


void
gluPopMatrix4m(struct GLUmat4Stack *s)
{
	if (s->size == 0)
		return;

	s->size--;
	memmove(& s->stack[0], &  s->stack[1], sizeof(s->stack[0]) * s->size);
}
};


void
GLUmat4Stack::push()
{
	gluPushMatrix4m(this);
}


void
GLUmat4Stack::pop()
{
	gluPopMatrix4m(this);
}
