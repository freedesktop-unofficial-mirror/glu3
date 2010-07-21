#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef HAVE_STRTOF
#define strtof(_a, _b) ((float) strtod(_a, _b))
#endif

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else
# ifndef HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
#   define _Bool signed char
#  endif
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif
