#pragma once

#ifdef __GNUC__
# define likely(cond) (__builtin_expect(!!(cond), 1))
# define unlikely(cond) (__builtin_expect(!!(cond), 0))
#else
# define likely(cond) cond
# define unlikely(cond) cond
#endif
