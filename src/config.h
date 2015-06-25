#pragma once

/// Max recursion level for raytracing
#define MAX_TRACE_LEVELS 5

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

/// Whether to use anti aliasing
#define USE_ANTIALIASING 1
// The anti aliasing factor
//#define ANTIALIASING_LEVEL 4
#define ANTIALIASING_LEVEL 16
#define MAX_KD_DEPTH 20
#define MIN_KD_TRIANGLES 100



// DO NOT EDIT

#ifndef MAX_TRACE_LEVELS
# define MAX_TRACE_LEVELS 1
#endif

#if USE_ANTIALIASING
# ifndef ANTIALIASING_LEVEL
# define ANTIALIASING_LEVEL 4
# endif

# if ANTIALIASING_LEVEL == 4
#  define ANTIALIASING_COEF (0.25f)
# elif ANTIALIASING_LEVEL == 16
#  define ANTIALIASING_COEF (0.25f * 0.25f)
# else
#  error Invalid AntiAliasing level
# endif
#else
# define ANTIALIASING_COEF (1.f)
#endif