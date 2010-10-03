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
 * Call-back function for sphere generation routine
 */
static void sphere_revolve_cb(void *data,
			      const GLUvec4 *position,
			      const GLUvec4 *normal,
			      const GLUvec4 *tangent,
			      const GLUvec4 *uv,
			      unsigned count);

/**
 * GLUsphereProducer decorator to implement call-backs
 *
 * To generate the sphere data, the \c GLUsphereProducer class interfaces with
 * a C functions that uses a call-back mechanism.  This call-backs is
 * analogous to \c vertex_batch method that \c GLUshapeConsumer subclasses
 * will provide.
 *
 * However, this method is \c protected.  As a result the non-class call-back
 * functions cannot call this method unless it is a \c friend function.  It is
 * undesireable to expose this implementation detail in the application-facing
 * header file.  This can be worked around by creating a dummy subclass of \c
 * GLUshapeConsumer that only contains the \c friend function declaration.
 * Pointers to \c GLUshapeConsumer objects can be cast to pointers to
 * \c GLUconsumerFriend objects without side-effect.
 *
 * This is arguably a mis-use of the "decorator" pattern, but it is the most
 * efficient way to do this.
 */
class GLUconsumerFriend : public GLUshapeConsumer {
	friend void sphere_revolve_cb(void *data,
				      const GLUvec4 *position,
				      const GLUvec4 *normal,
				      const GLUvec4 *tangent,
				      const GLUvec4 *uv,
				      unsigned count);
};


GLUsphereProducer::GLUsphereProducer(GLdouble radius, GLint slices,
				     GLint stacks)
  : GLUmeshProducer(slices, stacks + 1, stacks + 1)
{
	this->radius = radius;
	this->rows = (slices < 4) ? 4 : (unsigned) slices;
	this->columns = (stacks < 3) ? 4 : (unsigned) (stacks + 1);
	this->width = this->columns;
}


unsigned
GLUsphereProducer::vertex_count(void) const
{
	/* Each line of vertices from the north pole to the south pole consists
	 * of (stacks+1) vertices.  There are a total of (slices+1) of these
	 * lines of vertices.
	 */
	
	return (this->rows + 1) * (this->columns);
}


static void
sphere_revolve_cb(void *data, const GLUvec4 *position, const GLUvec4 *normal,
		  const GLUvec4 *tangent, const GLUvec4 *uv, unsigned count)
{
	GLUconsumerFriend *c = (GLUconsumerFriend *) data;

	c->vertex_batch(position, normal, tangent, uv, count);
}


void
GLUsphereProducer::generate(GLUshapeConsumer *consumer) const
{
	GLUvec4 data[4 * 16];
	cb_buffer buf;

	if (consumer->vertex_count == 0) {
		cb_buffer_init(&buf, &data[0], &data[16], &data[32],
			       &data[48], 16);
	} else {
		cb_buffer_init(&buf, consumer->position, consumer->normal,
			       consumer->tangent, consumer->uv,
			       consumer->vertex_count);
	}

	generate_sphere(this->radius, this->rows, this->columns - 1,
			this->normals_point_out,
			sphere_revolve_cb, (void *) consumer, &buf);

	GLUmeshProducer::generate(consumer);
}
