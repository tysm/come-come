#include "platform.h"
#include <stdio.h>

#if defined(PLATFORM_WIN32)
#   include <windows.h>
#elif defined(PLATFORM_UNIX)
#   include <unistd.h>
#   include <time.h>
#else
#   error Unsupported platform
#endif

void cli_render(char screen[24][80])
{
    int i;

#if defined(PLATFORM_WIN32)
    system("cls");
#elif defined(PLATFORM_UNIX)
    system("clear && printf '\e[3J'");
#endif

    for(i = 0; i < 24; ++i)
        screen[i][79] = '\n';
	
    fwrite(screen, sizeof(char), 24*80, stdout);
    fflush(stdout);
}

void cli_sync(void)
{
    const uint32_t sync_ms = 100;
#if defined(PLATFORM_WIN32)
    Sleep(sync_ms);
#elif defined(PLATFORM_UNIX)
    struct timeval tv;
    tv.tv_sec = sync_ms / 1000;
    tv.tv_usec = (sync_ms % 1000) * 1000000;
    select(0, NULL, NULL, NULL, &tv);
#endif
}