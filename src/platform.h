#pragma once

#if defined(_WIN32)
# include <windows.h>
#endif

#if defined(__APPLE__) // OSX
# include "TargetConditionals.h"
# if TARGET_OS_MAC
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
# else
#  error Unsupported platform
# endif
#else // any UNIX or Windows
# include <GL/glut.h>
#endif
