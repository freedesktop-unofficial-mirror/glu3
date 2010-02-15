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
#include "revolve.h"
#include "mesh.h"

/**
 * \name Call-back functions for sphere generation routine
 */
/*@{*/
static void sphere_revolve_cb(void *data,
			      const GLUvec4 *position,
			      const GLUvec4 *normal,
			      const GLUvec4 *tangent,
			      const GLUvec4 *uv);

static void sphere_begin_cb(void *data, GLenum mode);

static void sphere_index_cb(void *data, unsigned index);

static void sphere_end_cb(void *data);
/*@}*/

/**
 * GLUsphere decorator to implement call-backs
 *
 * To generate the sphere data, the \c GLUsphere class interfaces with various
 * C functions that use a call-back mechanism.  These call-backs are analogous
 * the \c emit_vertex, \c emit_begin, \c emit_index, and \c emit_end methods
 * that \c GLUsphere subclasses will provide.
 *
 * However, these methods are all \c protected.  As a result the non-class
 * call-back functions cannot call these methods unless they are \c friend
 * functions.  It is undesireable to expose the implementation detail in the
 * application-facing header file.  This can be worked around by creating a
 * dummy subclass of \c GLUsphere that only contains the \c friend function
 * declarations.  Pointers to \c GLUsphere objects can be cast to pointers to
 * \c GLUsphereFriend objects without side-effect.
 *
 * This is arguably a mis-use of the "decorator" pattern, but it is the most
 * efficient way to do this.
 */
class GLUsphereFriend : public GLUsphere {
	friend void sphere_revolve_cb(void *data,
				      const GLUvec4 *position,
				      const GLUvec4 *normal,
				      const GLUvec4 *tangent,
				      const GLUvec4 *uv);

	friend void sphere_begin_cb(void *data, GLenum mode);

	friend void sphere_index_cb(void *data, unsigned index);

	friend void sphere_end_cb(void *data);
};


GLUsphere::GLUsphere(GLdouble radius, GLint slices, GLint stacks)
{
	this->radius = radius;
	this->slices = (slices < 4) ? 4 : (unsigned) slices;
	this->stacks = (stacks < 4) ? 4 : (unsigned) stacks;
}


unsigned
GLUsphere::vertex_count(void) const
{
	/* Each line of vertices from the north pole to the south pole consists
	 * of (stacks+1) vertices.  There are a total of (slices+1) of these
	 * lines of vertices.
	 */
	return (slices + 1) * (stacks + 1);
}


unsigned
GLUsphere::primitive_count(void) const
{
	/* For each slice there is a triangle strip from the north pole to the
	 * south pole.
	 */
	return slices;
}


unsigned
GLUsphere::element_count(void) const
{
	/* Each slice is a triangle strip represented by 2 elements plus
	 * 2 elements for each stack;
	 */
	return slices * (2 * (stacks + 1));
}


static void
sphere_revolve_cb(void *data, const GLUvec4 *position, const GLUvec4 *normal,
		  const GLUvec4 *tangent, const GLUvec4 *uv)
{
	GLUsphereFriend *s = (GLUsphereFriend *) data;

	s->emit_vertex(*position, *normal, *tangent, *uv);
}


static void
sphere_begin_cb(void *data, GLenum mode)
{
	GLUsphereFriend *s = (GLUsphereFriend *) data;

	s->emit_begin(mode);
}


static void
sphere_index_cb(void *data, unsigned index)
{
	GLUsphereFriend *s = (GLUsphereFriend *) data;

	s->emit_index(index);
}


static void
sphere_end_cb(void *data)
{
	GLUsphereFriend *s = (GLUsphereFriend *) data;

	s->emit_end();
}


void
GLUsphere::generate(void)
{
	generate_sphere(radius, slices, stacks,
			normals_point_out,
			sphere_revolve_cb, (void *) this);

	generate_triangle_mesh(slices, stacks + 1, stacks + 1,
			       sphere_begin_cb,
			       sphere_index_cb,
			       sphere_end_cb,
			       (void *) this);
}
