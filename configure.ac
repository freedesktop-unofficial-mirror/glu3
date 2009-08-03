#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(GLU3, 20090417, idr@freedesktop.org, GLU3)
AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_HEADER_STDC

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [use debug compiler flags and macros @<:@default=disabled@:>@])],
    [enable_debug="$enableval"],
    [enable_debug=no]
)
if test "x$enable_debug" = xyes; then
    DEFINES="$DEFINES -DDEBUG"
    if test "x$GCC" = xyes; then
        CFLAGS="$CFLAGS -ggdb3 -fstack-protector -D_FORTIFY_SOURCE=2"
    fi
    if test "x$GXX" = xyes; then
        CXXFLAGS="$CXXFLAGS -ggdb3 -fstack-protector -D_FORTIFY_SOURCE=2"
    fi
fi


if test "x$GCC" = xyes ; then
	WARN="-Wall -Wextra -Wunsafe-loop-optimizations -Wstack-protector"
else
	WARN=""
fi
CFLAGS="$CFLAGS $WARN"

if test "x$GXX" = xyes ; then
	WARN="-Wall -Wextra -Wunsafe-loop-optimizations -Wstack-protector"
else
	WARN=""
fi
CXXFLAGS="$CXXFLAGS $WARN"

AC_OUTPUT([Makefile
	src/Makefile
	test/Makefile
	glu3.pc])

