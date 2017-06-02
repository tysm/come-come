#include <stdlib.h>
#include <string.h>
#include "platform.h"

typedef struct player
{
    float x, y;
} player_t;

player_t* player;

void player_init(player_t* p)
{
    p->x = 0.0f;
    p->y = 0.0f;
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
}


/**
 * Renderiza o estado atual do jogo.
 */
void render(void)
{
    //char screen[24][80];
	
	//kimblee edit-lixo
    int i;
	char screen[24][80];
	memset(screen[0], '_', sizeof(char)*79);
	for (i=1; i<24; i++){
		screen[i][0]='|';
		screen[i][78]='|';
		memset(&screen[i][1], ' ', sizeof(char)*77);
	}
	memset(&screen[23][1], '_', sizeof(char)*77);
    //end edit
	
	//memset(screen, '.', sizeof(char)*24*80);
    memcpy(&screen[2][5], "Exemplo", 7);
    
    cli_render(screen);
}

/**
 * Aguarda um pouco antes do próximo ciclo.
 */
void sync(void)
{
    cli_sync();
}
