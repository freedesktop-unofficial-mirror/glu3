#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef HAVE_STRTOF
#define strtof(_a, _b) ((float) strtod(_a, _b))
#endif
