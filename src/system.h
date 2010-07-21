#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef _WIN32
#define HAVE_FOPEN_S
#define HAVE_WGL
#endif

#ifndef HAVE_STRTOF
#define strtof(_a, _b) ((float) strtod(_a, _b))
#endif
