#include "platform.h"
#include <stdio.h>

#if defined(PLATFORM_WIN32)
#   include <windows.h>
#   include <conio.h>
#elif defined(PLATFORM_UNIX)
#   include <unistd.h>
#   include <time.h>
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

void cli_update_keys(void)
{
    memcpy(keys_before, keys_now, sizeof(keys_before));
    memset(keys_now, 0, sizeof(keys_now));

#if defined(PLATFORM_WIN32)
    if(_kbhit())
    {
        int c1;
        
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
				case 42:
					keys_now[KEY_STAR] = 1;
					break;
				case 35:
					keys_now[KEY_HASH] = 1;
					break;
				case 32:
					keys_now[KEY_SPACE] = 1;
					break;
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
#   error cli_update_keys for Unix Systems not implemented
#endif
}

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
