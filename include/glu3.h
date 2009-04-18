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

#ifndef __glu3_h__
#define __glu3_h__

#include <GL/gl.h>

#define GLU_VERSION_3_0

struct GLUvec4 {
	GLfloat values[4];

#ifdef __cplusplus
	inline GLUvec4(GLfloat x , GLfloat y, GLfloat z, GLfloat w)
	{
		values[0] = x;
		values[1] = y;
		values[2] = z;
		values[3] = w;
	}

	GLUvec4 operator *(const GLUmat4 &) const;
	GLUvec4 operator *(const GLUvec4 &) const;
	GLUvec4 operator *(GLfloat) const;

	GLUvec4 operator +(const GLUvec4 &) const;
	GLUvec4 operator -(const GLUvec4 &) const;
#endif /* __cplusplus */
};


struct GLUmat4 {
	struct GLUvec4 col[4];

#ifdef __cplusplus
	inline GLUmat4(const GLUvec4 & c0, const GLUvec4 & c1,
		       const GLUvec4 & c2, const GLUvec4 & c3)
	{
		col[0] = c0;
		col[1] = c1;
		col[2] = c2;
		col[3] = c3;
	}

	GLUvec4 operator *(const GLUvec4 &) const;
	GLUmat4 operator *(const GLUmat4 &) const;
	GLUmat4 operator *(GLfloat) const;

	GLUmat4 operator *(const GLUmat4 &) const;
	GLUmat4 operator -(const GLUmat4 &) const;
#endif	/* __cplusplus */
};

#define GLU_MAX_STACK_DEPTH 32

struct GLUmat4Stack {
	struct GLUmat4 stack[GLU_MAX_STACK_DEPTH];
	unsigned top;

#ifdef __cplusplus
	GLUmat4Stack() : top(0)
	{
		/* empty */
	}
#endif	/* __cplusplus */
};

#ifndef __cplusplus
typedef struct GLUvec4 GLUvec4;
typedef struct GLUmat4 GLUmat4;
typedef struct GLUmat4Stack GLUmat4Stack;
#endif /*  __cplusplus */


GLfloat gluDot4_4v(const GLUvec4 *, const GLUvec4 *);
GLfloat gluDot3_4v(const GLUvec4 *, const GLUvec4 *);
GLfloat gluDot2_4v(const GLUvec4 *, const GLUvec4 *);

#ifdef __cplusplus
GLfloat gluDot4(const GLUvec4 &, const GLUvec4 &);
GLfloat gluDot3(const GLUvec4 &, const GLUvec4 &);
GLfloat gluDot2(const GLUvec4 &, const GLUvec4 &);
#endif

GLUvec4 gluCross4v(const GLUvec4 *, const GLUvec4 *);
GLUvec4 gluNormalize4v(const GLUvec4 *);
GLfloat gluLength4v(const GLUvec4 *);
GLfloat gluLengthSqr4v(const GLUvec4 *);
GLUmat4 gluOuter4v(const GLUvec4 *, const GLUvec4 *);

#ifdef __cplusplus
GLUvec4 gluCross(const GLUvec4 &, const GLUvec4 &);
GLUvec4 gluNormalize(const GLUvec4 &);
GLfloat gluLength(const GLUvec4 &);
GLfloat gluLengthSqr(const GLUvec4 &);
#endif /* __cplusplus */

GLUvec4 gluMult4v_4v(const GLUvec4 *, const GLUvec4 *);
GLUvec4 gluDiv4v_4v(const GLUvec4 *, const GLUvec4 *);
GLUvec4 gluAdd4v_4v(const GLUvec4 *, const GLUvec4 *);
GLUvec4 gluSub4v_4v(const GLUvec4 *, const GLUvec4 *);

GLUvec4 gluMult4v_f(const GLUvec4 *, GLfloat);
GLUvec4 gluDiv4v_f(const GLUvec4 *, GLfloat);
GLUvec4 gluAdd4v_f(const GLUvec4 *, GLfloat);
GLUvec4 gluSub4v_f(const GLUvec4 *, GLfloat);

GLUmat4 gluMult4m_4m(const GLUmat4 *, const GLUmat4 *);
GLUmat4 gluAdd4m_4m(const GLUmat4 *, const GLUmat4 *);
GLUmat4 gluSub4m_4m(const GLUmat4 *, const GLUmat4 *);

GLUmat4 gluMult4m_f(const GLUmat4 *, GLfloat);

GLUmat4 gluScale4v(const GLUvec4 *);
GLUmat4 gluTranslate4v(const GLUvec4 *);
GLUmat4 gluLookAt(const GLUvec4 *, const GLUvec4 *, const GLUvec4 *);

#include "glu3_scalar.h"

#endif /* __glu3_h__ */
