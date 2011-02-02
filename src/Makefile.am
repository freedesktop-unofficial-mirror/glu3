# Copyright © 2009 Ian D. Romanick
# All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# on the rights to use, copy, modify, merge, publish, distribute, sub
# license, and/or sell copies of the Software, and to permit persons to whom
# the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
# AUTHORS, COPYRIGHT HOLDERS, AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
# USE OR OTHER DEALINGS IN THE SOFTWARE.

AM_CXXFLAGS=-I../include
AM_CFLAGS=-I../include

lib_LIBRARIES = libGLU3.a
libGLU3_a_SOURCES = matrix.c load_text.c arcball.c revolve.c mesh.cpp \
	sphere.cpp torus.cpp cube.cpp shader.c mesh.h revolve.h system.h \
	buffer.c buffer.h shape.cpp

libGLU3includedir = ${includedir}
libGLU3include_HEADERS = ../include/glu3.h ../include/glu3_scalar.h

noinst_HEADERS = ../include/GL/glext.h ../include/GL/wglext.h
