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
#include "buffer.h"

GLUcubeProducer::GLUcubeProducer(double radius) :
	radius(radius)
{
}

unsigned
GLUcubeProducer::vertex_count(void) const
{
	/* Due to differeing normals and texture coordinates, each face of the
	 * cube has four unique vertices.  This means that each unique vertex
	 * position will appear in the output three times.  Note: 8*3 = 6*4.
	 */
	return 6 * 4;
}

unsigned
GLUcubeProducer::element_count(void) const
{
	/* Each face is made up of two triangles.  All data is sent in a single
	 * triangle list.
	 */
	return 6 * 2 * 3;
}

unsigned
GLUcubeProducer::primitive_count(void) const
{
	/* Each face is made up of two triangles.  All data is sent in a single
	 * triangle list.
	 */
	return 1;
}

#define Elements(a)  (sizeof(a) / sizeof(a[0]))

void
GLUcubeProducer::generate(GLUshapeConsumer *consumer) const
{
	static const GLUvec4 p[] = {
		GLUvec4(+1.0,  1.0,  1.0, 1.0),
		GLUvec4(-1.0,  1.0,  1.0, 1.0),
		GLUvec4(+1.0, -1.0,  1.0, 1.0),
		GLUvec4(-1.0, -1.0,  1.0, 1.0),
		GLUvec4(+1.0,  1.0, -1.0, 1.0),
		GLUvec4(-1.0,  1.0, -1.0, 1.0),
		GLUvec4(+1.0, -1.0, -1.0, 1.0),
		GLUvec4(-1.0, -1.0, -1.0, 1.0),
	};

#define X 0
#define Y 1
#define Z 2
#define P 0
#define N 1
#define V(x, y, z, n) (((((z) * 4) + (y * 2) + x) * 3) + (n))
	

	static const unsigned char elts[] = {
		/* +X face */
		V(P, P, P, X), V(P, N, N, X), V(P, P, N, X),
		V(P, P, P, X), V(P, N, P, X), V(P, N, N, X),

		/* +Y face */
		V(P, P, P, Y), V(P, P, N, Y), V(N, P, N, Y),
		V(P, P, P, Y), V(N, P, N, Y), V(N, P, P, Y),

		/* +Z face */
		V(N, P, P, Z), V(P, N, P, Z), V(P, P, P, Z),
		V(N, P, P, Z), V(N, N, P, Z), V(P, N, P, Z),

		/* -X face */
		V(N, P, P, X), V(N, P, N, X), V(N, N, N, X),
		V(N, P, P, X), V(N, N, N, X), V(N, N, P, X),

		/* -Y face */
		V(P, N, P, Y), V(N, N, N, Y), V(P, N, N, Y),
		V(P, N, P, Y), V(N, N, P, Y), V(N, N, N, Y),

		/* -Z face */
		V(N, P, N, Z), V(P, P, N, Z), V(P, N, N, Z),
		V(N, P, N, Z), V(P, N, N, Z), V(N, N, N, Z),
	};

#undef X
#undef Y
#undef Z
#undef N
#undef P
#undef V


	GLUvec4 pos[3 * Elements(p)];
	GLUvec4 nrm[3 * Elements(p)];
	GLUvec4 tng[3 * Elements(p)];
	GLUvec4 uv[3 * Elements(p)];
	cb_buffer buf;

	if (consumer->vertex_count > 0) {
		cb_buffer_init(&buf,
			       consumer->position,
			       consumer->normal,
			       consumer->tangent,
			       consumer->uv,
			       consumer->vertex_count);
	} else {
		cb_buffer_init(&buf, pos, nrm, tng, uv, 3 * Elements(p));
	}

	for (unsigned i = 0; i < Elements(p); i++) {
		CB_BUFFER_APPEND(buf,
				 p[i],
				 GLUvec4(p[i].values[0], 0.0, 0.0, 0.0),
				 GLUvec4(0.0, 0.0, p[i].values[0], 0.0),
				 GLUvec4((p[i].values[2] + 1.0) * 0.5,
					 (p[i].values[1] + 1.0) * 0.5,
					 0.0,
					 0.0));
		if (CB_BUFFER_IS_FULL(buf)) {
			consumer->vertex_batch(buf.pos, buf.nrm, buf.tng,
					       buf.uv, buf.used);
			CB_BUFFER_MAKE_EMPTY(buf);
		}


		CB_BUFFER_APPEND(buf,
				 p[i],
				 GLUvec4(0.0, p[i].values[1], 0.0, 0.0),
				 GLUvec4(p[i].values[1], 0.0, 0.0, 0.0),
				 GLUvec4((p[i].values[0] + 1.0) * 0.5,
					 (p[i].values[2] + 1.0) * 0.5,
					 0.0,
					 0.0));
		if (CB_BUFFER_IS_FULL(buf)) {
			consumer->vertex_batch(buf.pos, buf.nrm, buf.tng,
					       buf.uv, buf.used);
			CB_BUFFER_MAKE_EMPTY(buf);
		}


		CB_BUFFER_APPEND(buf,
				 p[i],
				 GLUvec4(0.0, 0.0, p[i].values[2], 0.0),
				 GLUvec4(p[i].values[2], 0.0, 0.0, 0.0),
				 GLUvec4((p[i].values[0] + 1.0) * 0.5,
					 (p[i].values[1] + 1.0) * 0.5,
					 0.0,
					 0.0));
		if (CB_BUFFER_IS_FULL(buf)) {
			consumer->vertex_batch(buf.pos, buf.nrm, buf.tng,
					       buf.uv, buf.used);
			CB_BUFFER_MAKE_EMPTY(buf);
		}
	}

	if (!CB_BUFFER_IS_EMPTY(buf)) {
		consumer->vertex_batch(buf.pos, buf.nrm, buf.tng, buf.uv,
				       buf.used);
		CB_BUFFER_MAKE_EMPTY(buf);
	}

	consumer->begin_primitive(GL_TRIANGLES, Elements(elts));

	for (unsigned i = 0; i < Elements(elts); i++)
		consumer->index(elts[i]);
	
	consumer->end_primitive();
}
