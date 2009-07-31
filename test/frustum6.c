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
	GLUvec4 a = {{1.0, 2.0, 1.0, 1.0}};
	GLUvec4 b = {{-2.0, 3.0, 1.0, 1.0}};
	GLUvec4 result;
	GLUmat4 identity_frustum;

	(void)argc;
	(void)argv;

	gluFrustum6(&identity_frustum,
		    -1, 1,
		    -1, 1,
		    -1, 1);
	gluMult4m_4v(&result, &identity_frustum, &a);
	vec4_divide(&result);
	assert(vec4_equals(&result, &a));
	gluMult4m_4v(&result, &identity_frustum, &b);
	vec4_divide(&result);
	assert(vec4_equals(&result, &b));

	return 0;
}
