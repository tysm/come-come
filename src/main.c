#include <stdlib.h>
#include <string.h>
#include "platform.h"

typedef struct player
{
    float x, y, x_dir, y_dir;
} player_t;

player_t* player;

void player_init(player_t* p)
{
    p->x = 0.0f;
    p->y = 0.0f;
	p->x_dir = 0.0f;
	p->y_dir = 0.0f;
}

void update(void);
void render(void);
void sync(void);

int main(int argc, char* argv[])
{
    player_t p1;
    player_init(&p1);
    player = &p1;
    
    while(1)
    {
		cli_update_keys();
        update();
        render();
        sync();
    }
    
    return 0;
}


/**
 * Atualiza o estado lógico do jogo.
 */
void update(void)
{
	if(keyhold(KEY_UP))
	{
		player->x_dir = 0.0f;
		player->y_dir = -1.0f; 
	}
	else if(keyhold(KEY_DOWN))
	{
		player->x_dir = 0.0f; 
		player->y_dir = 1.0f; 
	}
	else if(keyhold(KEY_LEFT))
	{
		player->x_dir = -1.0f;
		player->y_dir = 0.0f; 
	}
	else if(keyhold(KEY_RIGHT))
	{
		player->x_dir = 1.0f;
		player->y_dir = 0.0f; 
	}
	/*modifiquei aqui*/
	if ((player->x_dir<0&&player->x>1)||(player->x_dir>0&&player->x<77)) player->x += player->x_dir;
	if ((player->y_dir<0&&player->y>1)||(player->y_dir>0&&player->y<22)) player->y += player->y_dir;
}


/**
 * Renderiza o estado atual do jogo.
 */
void render(void)
{
    int i;
	char screen[24][80];
	
	int px = (int)player->y;
	int py = (int)player->x;

	memset(screen[0], '#', sizeof(char)*79);
	for (i=1; i<24; i++){
		screen[i][0]='#';
		screen[i][78]='#';
		memset(&screen[i][1], ' ', sizeof(char)*77);
	}
	memset(&screen[23][1], '#', sizeof(char)*77);

	/*modifiquei aqui 'if(px >= 0 && px < 24 && py >= 0 && py < 80)'*/
	screen[px][py] = '@';
	
    cli_render(screen);
}


/**
 * Aguarda um pouco antes do próximo ciclo.
 */
void sync(void)
{
    cli_sync();
}
