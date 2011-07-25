
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if DEBUG
#include <std/error.h>
#else
#define panic(...) abort()
#define assert(x)
#endif

#ifndef public
# define public
#endif

#ifndef export
# define export
#endif

#ifndef private
# define private static inline
#endif

