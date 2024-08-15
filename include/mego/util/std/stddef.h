
#ifndef MEGO_UTIL_STD_STDDEF_H_INCLUDED
#define MEGO_UTIL_STD_STDDEF_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

#ifndef RSIZE_MAX
#define RSIZE_MAX (SIZE_MAX >> 1)
typedef size_t rsize_t;
#endif

#endif // !MEGO_UTIL_STD_STDDEF_H_INCLUDED
