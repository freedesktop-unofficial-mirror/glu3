AM_CFLAGS = -I$(srcdir)/../include
AM_CPPFLAGS = -I$(srcdir)/../include
LDADD = ../src/libGLU3.a -lm

sphere_SOURCES = sphere.cpp

TESTS = \
	add4v_4v \
	lookat4v \
	det \
	sphere

EXTRA_PROGRAMS = $(TESTS)
CLEANFILES = $(EXTRA_PROGRAMS)
