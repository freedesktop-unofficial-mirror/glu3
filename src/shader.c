/*
 * Copyright © 2009, 2010 Ian D. Romanick
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
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#if defined(HAVE_GLX)
#include <GL/glx.h>
#define GetProcAddress(x) glXGetProcAddress(#x)
#elif defined(HAVE_EGL)
#include <EGL/egl.h>
#define GetProcAddress(x) eglGetProcAddress(#x)
#elif defined(HAVE_WGL)
#include <GL/wglext.h>
#define GetProcAddress(x) wglGetProcAddress(#x)
#else
#define GetProcAddress(x) (x)
#endif

static PFNGLSHADERSOURCEPROC ShaderSource = NULL;
static PFNGLCREATESHADERPROC CreateShader = NULL;
static PFNGLCOMPILESHADERPROC CompileShader = NULL;
static PFNGLGETSHADERIVPROC GetShaderiv = NULL;
static PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog = NULL;
static PFNGLDELETESHADERPROC DeleteShader = NULL;

static PFNGLLINKPROGRAMPROC LinkProgram = NULL;
static PFNGLATTACHSHADERPROC AttachShader = NULL;
static PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation = NULL;
static PFNGLGETPROGRAMIVPROC GetProgramiv = NULL;
static PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog = NULL;

bool
gluInitializeCompiler(void)
{
	/* Determine which set of interfaces are available for accessing GLSL.
	 * This will either be from OpenGL 2.0+ (or OpenGL ES 2.0) or from
	 * GL_ARB_shader_objects. 
	 */
	const char *const version_string = glGetString(GL_VERSION);
	float version = strtof(version_string, NULL);

	if (version >= 2.0f) {
		ShaderSource = (PFNGLSHADERSOURCEPROC)
			GetProcAddress(glShaderSource);
		CreateShader = (PFNGLCREATESHADERPROC)
			GetProcAddress(glCreateShader);
		CompileShader = (PFNGLCOMPILESHADERPROC)
			GetProcAddress(glCompileShader);
		GetShaderiv = (PFNGLGETSHADERIVPROC)
			GetProcAddress(glGetShaderiv);
		GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
			GetProcAddress(glGetShaderInfoLog);
		DeleteShader = (PFNGLDELETESHADERPROC)
			GetProcAddress(glDeleteShader);

		LinkProgram = (PFNGLLINKPROGRAMPROC)
			GetProcAddress(glLinkProgram);
		AttachShader = (PFNGLATTACHSHADERPROC)
			GetProcAddress(glAttachShader);
		BindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)
			GetProcAddress(glBindAttribLocation);
		GetProgramiv = (PFNGLGETPROGRAMIVPROC)
			GetProcAddress(glGetProgramiv);
		GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)
			GetProcAddress(glGetProgramInfoLog);
	} else {
		const char *const extension_string = glGetString(GL_EXTENSIONS);
		const size_t len = strlen("GL_ARB_shader_objects");
		const char *x;

		x = extension_string;
		while ((x = strstr(x, "GL_ARB_shader_objects")) != NULL) {
			if (x[len] == ' ' || x[len] == '\0')
				break;

			x += len;
		}

		if (strcmp(x, "GL_ARB_shader_objects") != 0)
			return false;

		ShaderSource = (PFNGLSHADERSOURCEPROC)
			GetProcAddress(glShaderSourceARB);
		CreateShader = (PFNGLCREATESHADERPROC)
			GetProcAddress(glCreateShaderARB);
		CompileShader = (PFNGLCOMPILESHADERPROC)
			GetProcAddress(glCompileShaderARB);
		DeleteShader = (PFNGLDELETESHADERPROC)
			GetProcAddress(glDeleteShaderARB);

		LinkProgram = (PFNGLLINKPROGRAMPROC)
			GetProcAddress(glLinkProgramARB);
		AttachShader = (PFNGLATTACHSHADERPROC)
			GetProcAddress(glAttachShaderARB);
		BindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)
			GetProcAddress(glBindAttribLocationARB);


		/* These functions break the mold a little bit.  Here the API
		 * changed between the extension and core GL.  For the uses
		 * internal to this library, we can work around the change by
		 * using glGetObjectParameterivARB for both glGetShaderiv and
		 * glGetProgramiv.  Likewise glGetInfoLogARB can be used for
		 * both glGetShaderInfoLog and glGetProgramInfoLog.
		 */
		GetShaderiv = (PFNGLGETSHADERIVPROC)
			GetProcAddress(glGetObjectParameterivARB);
		GetProgramiv = (PFNGLGETPROGRAMIVPROC) GetShaderiv;

		GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
			GetProcAddress(glGetInfoLogARB);
		GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)
			GetShaderInfoLog;
	}

	return true;
}


void
gluReleaseInfoLog(char *log)
{
	free(log);
}


GLint
gluCompileShader(GLenum target, const char *code, char **log_ptr)
{
	GLint status;
	GLuint shad = CreateShader(target);

	ShaderSource(shad, 1, &code, NULL);
	CompileShader(shad);

	if (log_ptr != NULL) {
		GLint len;

		/* The spec says that 0 is returned when no log is available.
		 * However, Nvidia's drivers like to return a log length of 1,
		 * and return a log that is just a NUL terminator.
		 */
		GetShaderiv(shad, GL_INFO_LOG_LENGTH, &len);
		if (len > 1) {
			char *log = (char *) malloc(len + 1);

			if (log != NULL) {
				GetShaderInfoLog(shad, len + 1, NULL, log);
				*log_ptr = log;
			}
		}
 	}

	GetShaderiv(shad, GL_COMPILE_STATUS, &status);
	if (!status) {
		DeleteShader(shad);
		return 0;
	}

	return shad;
}


bool
gluLinkProgram(GLuint prog, char **log_ptr)
{
	GLint status;

	LinkProgram(prog);

	if (log_ptr != NULL) {
		GLint len;

		/* The spec says that 0 is returned when no log is available.
		 * However, Nvidia's drivers like to return a log length of 1,
		 * and return a log that is just a NUL terminator.
		 */
		GetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		if (len > 1) {
			char *log = (char *) malloc(len + 1);

			if (log != NULL) {
				GetProgramInfoLog(prog, len + 1, NULL, log);
				*log_ptr = log;
			}
		}
	}

	GetProgramiv(prog, GL_LINK_STATUS, &status);
	return status == GL_TRUE;
}


void
gluAttachShaders(GLuint prog, GLuint shader, ...)
{
	va_list args;

	AttachShader(prog, shader);

	va_start(args, prog);

	while (true) {
		GLuint s = va_arg(args, GLuint);

		if (s == 0)
			break;

		AttachShader(prog, s);
	}

	va_end(args);
}


void
gluBindAttributes(GLuint prog, const char *name, GLuint location, ...)
{
	va_list args;

	BindAttribLocation(prog, location, name);

	va_start(args, prog);

	while (true) {
		const char *name = va_arg(args, char *);
		GLuint idx;
	
		if (name == NULL)
			break;

		idx = va_arg(args, unsigned);
		BindAttribLocation(prog, idx, name);
	}

	va_end(args);
}
