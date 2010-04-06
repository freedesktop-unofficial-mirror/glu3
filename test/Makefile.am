AM_CFLAGS = -I$(srcdir)/../include
LDADD = ../src/libGLU3.a -lm

mat4stack_01_SOURCES = mat4stack-01.cpp

TESTS = \
	add4v_4v \
	lookat4v \
	det \
	mat4stack_01

EXTRA_PROGRAMS = $(TESTS)
CLEANFILES = $(EXTRA_PROGRAMS)
