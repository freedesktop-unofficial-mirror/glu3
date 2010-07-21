#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(GLU3, 0.9-20100721, idr@freedesktop.org, GLU3)
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
AC_CHECK_HEADERS([fcntl.h unistd.h sys/stat.h sys/types.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS(fopen_s)
AC_CHECK_FUNCS(strtof)

AC_ARG_WITH([winsys],
            [AS_HELP_STRING([--with-winsys],
              [select windowing system to use between glx, egl, and wgl @<:@default=check@:>@])],
            [],
            [with_winsys=check])

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

# Determine which window system layer to use.  If none is selected or detected,
# see if the required functions are statically available (this maybe the case
# on OpenGL ES 2.0 or on OS X).

if test "x$with_winsys" = xcheck ; then
   AC_CHECK_HEADER("GL/glx.h", [have_glx=yes], [], [])
   AC_CHECK_HEADER("EGL/egl.h", [have_egl=yes], [], [])
   AC_CHECK_HEADER("GL/wgl.h", [have_wgl=yes], [], [])

   if test "x$have_glx" = xyes ; then
      if test "x$have_egl" = xyes -o "x$have_wgl" = xyes ; then
          AC_MSG_ERROR("cannot determine which window system to use")
      fi

      with_winsys=glx
   fi

   if test "x$have_egl" = xyes ; then
      if test "x$have_wgl" = xyes ; then
          AC_MSG_ERROR("cannot determine which window system to use")
      fi

      with_winsys=egl
   fi

   if test "x$have_wgl" = xyes ; then
      with_winsys=wgl
   fi
fi

if test "x$with_winsys" = xglx ; then
   AC_DEFINE([HAVE_GLX], [1], [Use GLX as the window system layer.])
elif test "x$with_winsys" = xegl ; then
   AC_DEFINE([HAVE_EGL], [1], [Use EGL as the window system layer.])
elif test "x$with_winsys" = xwgl ; then
   AC_DEFINE([HAVE_WGL], [1], [Use WGL as the window system layer.])
elif test "x$with_winsys" = xcheck -o "x$with_winsys" = xno; then
   # No window system.  Check that the OpenGL library makes OpenGL 2.0
   # symbols statically available.
   AC_CHECK_FUNCS(glCreateShader,
                  [],
                  [AC_MSG_ERROR("cannot find glCreateShader.  Update library path or select a window system via --with-winsys")]
		  )
else
   AC_MSG_ERROR("invalid window system selection")
fi



AC_OUTPUT([Makefile
	src/Makefile
	test/Makefile
	glu3.pc])

