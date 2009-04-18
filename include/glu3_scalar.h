#include <math.h>

extern inline GLUvec4 gluMult4v_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] * v2->values[0];
	result.values[1] = v1->values[1] * v2->values[1];
	result.values[2] = v1->values[2] * v2->values[2];
	result.values[3] = v1->values[3] * v2->values[3];

	return result;
}


extern inline GLUvec4 gluDiv4v_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] / v2->values[0];
	result.values[1] = v1->values[1] / v2->values[1];
	result.values[2] = v1->values[2] / v2->values[2];
	result.values[3] = v1->values[3] / v2->values[3];

	return result;
}


extern inline GLUvec4 gluAdd4v_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] + v2->values[0];
	result.values[1] = v1->values[1] + v2->values[1];
	result.values[2] = v1->values[2] + v2->values[2];
	result.values[3] = v1->values[3] + v2->values[3];

	return result;
}


extern inline GLUvec4 gluSub4v_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] - v2->values[0];
	result.values[1] = v1->values[1] - v2->values[1];
	result.values[2] = v1->values[2] - v2->values[2];
	result.values[3] = v1->values[3] - v2->values[3];

	return result;
}


extern inline GLUvec4 gluMult4v_f(const GLUvec4 *v1, GLfloat f)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] * f;
	result.values[1] = v1->values[1] * f;
	result.values[2] = v1->values[2] * f;
	result.values[3] = v1->values[3] * f;

	return result;
}


extern inline GLUvec4 gluDiv4v_f(const GLUvec4 *v1, GLfloat f)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] / f;
	result.values[1] = v1->values[1] / f;
	result.values[2] = v1->values[2] / f;
	result.values[3] = v1->values[3] / f;

	return result;
}


extern inline GLUvec4 gluAdd4v_f(const GLUvec4 *v1, GLfloat f)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] + f;
	result.values[1] = v1->values[1] + f;
	result.values[2] = v1->values[2] + f;
	result.values[3] = v1->values[3] + f;

	return result;
}


extern inline GLUvec4 gluSub4v_f(const GLUvec4 *v1, GLfloat f)
{
	GLUvec4 result;

	result.values[0] = v1->values[0] - f;
	result.values[1] = v1->values[1] - f;
	result.values[2] = v1->values[2] - f;
	result.values[3] = v1->values[3] - f;

	return result;
}


extern inline GLfloat gluDot4_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	return v1->values[0] * v2->values[0]
		+ v1->values[1] * v2->values[1]
		+ v1->values[2] * v2->values[2]
		+ v1->values[3] * v2->values[3];
}


extern inline GLfloat gluDot3_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	return v1->values[0] * v2->values[0]
		+ v1->values[1] * v2->values[1]
		+ v1->values[2] * v2->values[2];
}


extern inline GLfloat gluDot2_4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	return v1->values[0] * v2->values[0]
		+ v1->values[1] * v2->values[1];
}


extern inline GLUvec4 gluCross4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	GLUvec4 result;

	result.values[0] = (v1->values[1] * v2->values[2]) 
		- (v1->values[2] * v2->values[1]);
	result.values[1] = (v1->values[2] * v2->values[0]) 
		- (v1->values[0] * v2->values[2]);
	result.values[2] = (v1->values[0] * v2->values[1]) 
		- (v1->values[1] * v2->values[0]);
	result.values[3] = 0.0;

	return result;
}


extern inline GLUmat4 gluOuter4v(const GLUvec4 *v1, const GLUvec4 *v2)
{
	GLUmat4 result;

	result.col[0] = gluMult4v_f(v1, v2->values[0]);
	result.col[1] = gluMult4v_f(v1, v2->values[1]);
	result.col[2] = gluMult4v_f(v1, v2->values[2]);
	result.col[3] = gluMult4v_f(v1, v2->values[3]);

	return result;
}


extern inline GLfloat gluLengthSqr4v(const GLUvec4 *v)
{
	return gluDot4_4v(v, v);
}


extern inline GLfloat gluLength4v(const GLUvec4 *v)
{
	return sqrt(gluLengthSqr4v(v));
}


extern inline GLUvec4 gluNormalize4v(const GLUvec4 *v)
{
	return gluDiv4v_f(v, gluLength4v(v));
}



extern inline GLUmat4 gluTranspose4(const GLUmat4 *m)
{
	unsigned i;
	unsigned j;
	GLUmat4 result;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			result.col[i].values[j] = m->col[j].values[i];
		}
	}

	return result;
}


#ifdef __cplusplus
extern inline GLfloat gluDot4(const GLUvec4 &v1, const GLUvec4 &v2)
{
	return v1.values[0] * v2.values[0]
		+ v1.values[1] * v2.values[1]
		+ v1.values[2] * v2.values[2]
		+ v1.values[3] * v2.values[3];
}


extern inline GLfloat gluDot3(const GLUvec4 &v1, const GLUvec4 &v2)
{
	return v1.values[0] * v2.values[0]
		+ v1.values[1] * v2.values[1]
		+ v1.values[2] * v2.values[2];
}


extern inline GLfloat gluDot2(const GLUvec4 &v1, const GLUvec4 &v2)
{
	return v1.values[0] * v2.values[0]
		+ v1.values[1] * v2.values[1];
}


inline GLUvec4 GLUvec4::operator+(const GLUvec4 &v) const
{
	return GLUvec4(values[0] + v.values[0],
		       values[1] + v.values[1],
		       values[2] + v.values[2],
		       values[3] + v.values[3]);
}


inline GLUvec4 GLUvec4::operator-(const GLUvec4 &v) const
{
	return GLUvec4(values[0] - v.values[0],
		       values[1] - v.values[1],
		       values[2] - v.values[2],
		       values[3] - v.values[3]);
}


inline GLUvec4 GLUvec4::operator*(const GLUvec4 &v) const
{
	return GLUvec4(values[0] * v.values[0],
		       values[1] * v.values[1],
		       values[2] * v.values[2],
		       values[3] * v.values[3]);
}


inline GLUvec4 GLUvec4::operator*(GLfloat f) const
{
	return GLUvec4(values[0] * f,
		       values[1] * f,
		       values[2] * f,
		       values[3] * f);
}


inline GLUvec4 GLUvec4::operator*(const GLUmat4 &m) const
{
	return GLUvev4(gluDot4(*this, m.col[0]),
		       gluDot4(*this, m.col[1]),
		       gluDot4(*this, m.col[2]),
		       gluDot4(*this, m.col[3]));
}


inline GLUmat4 GLUmat4::operator+(const GLUmat4 &m) const
{
	return GLUmat4(col[0] + m.col[0],
		       col[1] + m.col[1],
		       col[2] + m.col[2],
		       col[3] + m.col[3]);
}


inline GLUmat4 GLUmat4::operator-(const GLUmat4 &m) const
{
	return GLUmat4(col[0] - m.col[0],
		       col[1] - m.col[1],
		       col[2] - m.col[2],
		       col[3] - m.col[3]);
}


inline GLUmat4 GLUmat4::operator*(GLfloat f) const
{
	return GLUmat4(col[0] * f, col[1] * f, col[2] * f, col[3] * f);
}


inline GLUvec4 GLUmat4::operator*(const GLUvec4 &v) const
{
	return (col[0] * v.value[0])
		+ (col[1] * v.value[1])
		+ (col[2] * v.value[2])
		+ (col[3] * v.value[3]);
}


#endif /* __cplusplus */
