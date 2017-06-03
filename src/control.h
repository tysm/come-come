#include <stdint.h>

#if defined(_WIN32)
#   define PLATFORM_WIN32
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#   define PLATFORM_UNIX
#else
#   error Unsupported platform
#endif

extern void player_control(float *x, float *y, char screen[24][80]);