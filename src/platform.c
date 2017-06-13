#include "platform.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(PLATFORM_WIN32)
#   include <windows.h>
#   include <conio.h>
#elif defined(PLATFORM_UNIX)
#   include <unistd.h>
#   include <time.h>
#   include <sys/select.h>
#   include <ncurses.h>
#else
#   error Unsupported platform
#endif

static char keys_now[KEY_MAX];
static char keys_before[KEY_MAX];

int keyhit(key_id_t k)
{
    return keys_now[k] && !keys_before[k];
}

int keyhold(key_id_t k)
{
    return keys_now[k];
}

void cli_init_keys(void)
{
#if defined(PLATFORM_UNIX)
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, 1);
#endif
}

void cli_update_keys(void)
{
    int c1, c2, c3, c4;

    memcpy(keys_before, keys_now, sizeof(keys_before));
    memset(keys_now, 0, sizeof(keys_now));

#if defined(PLATFORM_WIN32)
    if(_kbhit())
    {
        c1 = _getch();
        if(c1 == 224 && _kbhit())
        {
            switch(_getch())
            {
                case 72:
                    keys_now[KEY_UP] = 1;
                    break;
                case 80:
                    keys_now[KEY_DOWN] = 1;
                    break;
                case 75:
                    keys_now[KEY_LEFT] = 1;
                    break;
                case 77:
                    keys_now[KEY_RIGHT] = 1;
                    break;
            }
        }
        else
        {
            switch(c1)
            {
                case 27:
                    keys_now[KEY_ESCAPE] = 1;
                    break;
                case 13:
                    keys_now[KEY_ENTER] = 1;
                    break;
            }
        }
    }
#elif defined(PLATFORM_UNIX)
    c1 = getch();
    if(c1 != ERR)
    {
        c2 = getch();
        if(c2 != ERR)
        {
            c3 = getch();
            if(c3 != ERR)
            {
                if(c1 == 27 && c2 == 91)
                {
                    switch(c3)
                    {
                        case 65:
                            keys_now[KEY_UP] = 1;
                            break;
                        case 66:
                            keys_now[KEY_DOWN] = 1;
                            break;
                        case 68:
                            keys_now[KEY_LEFT] = 1;
                            break;
                        case 69:
                            keys_now[KEY_RIGHT] = 1;
                            break;
                    }
                }
            }
        }
        else
        {
            switch(c1)
            {
                case 27:
                    keys_now[KEY_ESCAPE] = 1;
                    break;
                case 10:
                    keys_now[KEY_ENTER] = 1;
                    break;
            }
        }
    }
#endif
}

void cli_render(char screen[24][80])
{
    int i, j;

#if defined(PLATFORM_WIN32)
    system("cls");

    for(i = 0; i < 24; ++i)
        screen[i][79] = '\n';
	
    fwrite(screen, sizeof(char), 24*80, stdout);
    fflush(stdout);

#elif defined(PLATFORM_UNIX)
    for(i = 0; i < 24; ++i)
    {
        for(j = 0; j < 79; ++j)
            mvwaddch(stdscr, i, j, screen[i][j]);
    }

    refresh();
#endif


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
