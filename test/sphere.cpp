#include <cassert>
#include <cstdio>
#include "glu3.h"

class check_sphere : public GLUshapeConsumer {
public:
	check_sphere(const GLUshapeProducer &shape, double r) :
		pass(true), vert(0), prim(0), elts(0), r(r), mode(0)
	{
		primitive_count = shape.primitive_count();
		vertex_count = shape.vertex_count();
	}

	bool pass;
	unsigned vert;
	unsigned prim;
	unsigned elts;
	double r;
	GLenum mode;

	unsigned vertex_count;
	unsigned primitive_count;

	virtual void vertex(const GLUvec4 &position,
				 const GLUvec4 &normal,
				 const GLUvec4 &tangent,
				 const GLUvec4 &tex_coord)
	{
		printf("vert %u: {%.3f %.3f %.3f %.3f} ",
		       vert,
		       position.values[0],
		       position.values[1],
		       position.values[2],
		       position.values[3]);
		printf("{%.3f %.3f %.3f %.3f} ",
		       normal.values[0],
		       normal.values[1],
		       normal.values[2],
		       normal.values[3]);
		printf("{%.3f %.3f %.3f %.3f} ",
		       tangent.values[0],
		       tangent.values[1],
		       tangent.values[2],
		       tangent.values[3]);
		printf("{%.3f %.3f %.3f %.3f}\n",
		       tex_coord.values[0],
		       tex_coord.values[1],
		       tex_coord.values[2],
		       tex_coord.values[3]);

		if (fabs(gluLength(normal) - 1.0) > 0.00001) {
			printf("vert %u: bad normal |n| = %f\n",
			       vert,
			       gluLength(normal));
			pass = false;
		}

		if (fabs(gluLength(tangent) - 1.0) > 0.00001) {
			printf("vert %u: bad tangent |t| = %f\n",
			       vert,
			       gluLength(tangent));
			pass = false;
		}

		GLUvec4 p_dir(position);
		p_dir.values[3] = 0.0;

		if (fabs(gluLength(p_dir) - r) > 0.00001) {
			printf("vert %u: bad position |p| = %f vs. %f expected\n",
			       vert,
			       gluLength(p_dir),
			       r);
			pass = false;
		}

		if (position.values[3] != 1.0) {
			printf("vert %u: bad position W = %f\n",
			       vert,
			       position.values[3]);
			pass = false;
		}

		const GLUvec4 p_norm(gluNormalize(p_dir));
		if (fabs(gluDot3(p_norm, normal) - 1.0) > 0.00001) {
			printf("vert %u: bad position or normal direction\n",
			       vert);
			pass = false;
		}

		/* The tangent and the normal must always be orthogonal to
		 * each other, so the dot-product must be zero.
		 */
		if (fabs(gluDot3(tangent, normal)) > 0.000001) {
			printf("vert %u: bad tangent or normal direction\n",
			       vert);
			pass = false;
		}

		vert++;
	}

	virtual void begin_primitive(GLenum mode)
	{
		printf("prim %u: begin\n", prim);

		if (this->mode != 0) {
			printf("prim %u: bad primitive begin inside another "
			       "begin\n",
			       prim);
			pass = false;
		}

		if (prim == primitive_count) {
			printf("prim %u: too many primitives\n",
			       prim);
			pass = false;
		}

		switch (mode) {
		case GL_TRIANGLES:
		case GL_TRIANGLE_STRIP:
		case GL_TRIANGLE_FAN:
			break;

		default:
			printf("prim %u: invalid primitive mode 0x%04x\n",
			       prim, mode);
			pass = false;
			break;
		}

		this->mode = mode;
	}

	virtual void index(unsigned idx)
	{
		printf("prim %u, elt %u: %u\n", prim, elts, idx);

		if (idx >= vertex_count) {
			printf("prim %u, elt %u: bad elt %u > %u\n",
			       prim, elts, idx, vertex_count - 1);
			pass = false;
		}

		elts++;
	}

	virtual void end_primitive(void)
	{
		printf("prim %u: end\n", prim);

		if (this->mode == 0) {
			printf("prim %u: bad primitive end outside begin\n",
			       prim);
			pass = false;
		}

		mode = 0;
		prim++;
	}
};


int
main(int argc, char **argv)
{
	GLUsphereProducer s(4.0, 5, 5);
	check_sphere c(s, 4.0);

	s.generate(& c);
	assert(c.pass);
	assert(c.vert == s.vertex_count());
	assert(c.prim == s.primitive_count());
	assert(c.elts == s.element_count());
	return 0;
}
