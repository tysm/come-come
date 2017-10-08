#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "platform.h"
#include "powers.h"
#include "maps.h"

static char map[23][80];
int n_food;

entity_t* player;
entity_t* player_list = NULL;

entity_t* enemy_list = NULL;

/* provisorio para testes */
powers_queue_t *pfront = NULL, *pend = NULL;

void update(void);
void render(void);
void sync(void);

static void render_to_buffer(char screen[24][80], int px, int py, char c);

int main(int argc, char* argv[])
{
    int i, extra;
    entity_t* ent;
	
	power_up_id_t power = BRAKE_WALL;
	
    srand(time(0));
	
    player = entity_alloc(&player_list, ENTITY_PLAYER1);
    player->points = 0.0f;
	player->life=3;
	
	/* provisorio para testes */
	enqueue(&pend, &pfront, power);
	
    for(i = ENTITY_GHOST_START; i <= ENTITY_GHOST_END; ++i)
    {
        ent = entity_alloc(&enemy_list, i);
        ent->x = (1.0f + i * 4.0f);
        ent->y = (1.0f + i * 4.0f);
    }
	
	/*
	*criar função para o player decidir
	*se vai criar/editar/excluir/jogar
	*se for jogar devemos pedir o parametro s_map
	*para a função c_map carregar o mapa escolhido
	*função de criar entra logo abaixo
	*/
	do{/*provisório até arrumarem um menu*/
		system("cls");
		printf("1. Criar mapa;\n");
		printf("2. editar mapa;\n");
		printf("3. jogar;\n");
		printf("Ola, selecione o que deseja fazer: ");
		scanf("%d", &extra);
		switch(extra){
			case 1:
				list_map('m');
				break;
			case 2:
				list_map('e');
				break;
			default:
				break;
		}
	}while (extra!=3);
	/*
	*função para selecionar o mapa
	*e pegar quantidade de comida,
	*no momento esta selecionando o
	*mapa 0, padrão.
	*/
    c_map(map, 0, &n_food);
    
	while(1)
    {
		cli_update_keys();
        update();
        render();
        sync();
    }
    
    entity_free(&player_list, player);
    
    for(ent = enemy_list; ent; ent = ent->next)
        entity_free(&enemy_list, ent);
    
    return 0;
}


/**
 * Atualiza o estado lógico do jogo.
 */
void update(void)
{
    entity_t *pl, *ent;

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
	else if(keyhold(KEY_SPACE))
		power_caller(map, &player, &pfront);
	
	if (player->y==11&&player->x==0&&player->x_dir==-1)
		player->x = 78.0f;
	else if (player->y==11&&player->x==78&&player->x_dir==1)
		player->x = 0.0f;
	else {
		player->x += player->x_dir;
		player->y += player->y_dir;
	}
	if (map[(int)player->y][(int)player->x]==35||map[(int)player->y][(int)player->x]==45){
		player->x -= player->x_dir;
		player->y -= player->y_dir;
	} else {
		if (map[(int)player->y][(int)player->x]==42){
			player->points += 100.0f;
			map[(int)player->y][(int)player->x]=' ';
			n_food--;
			if (!n_food)
				/*TODO, mas isso é tipo um game over*/
				exit(0);
		}
	}
	
	for(pl = player_list; pl; pl = pl->next)
	{
        for(ent = enemy_list; ent; ent = ent->next)
        {
            if(entity_collides(pl, ent))
            {
				player->life-=1;
				player->points-=3*player->points/4;
				player->x = 38.0f;
				player->y = 13.0f;
				player->x_dir = 0.0f;
				player->y_dir = 0.0f;
				if (player->life==0){
					/* TODO game over para o player `pl` */
					exit(0);/*  Nunca façam isso em casa! Tá errado. */
				}
			}
        }
    }
}

/**
 * Renderiza o estado atual do jogo.
 */
void render(void)
{
    int i, j;
	char screen[24][80], life[10];
	entity_t* ent;

	for (i=0; i<23; i++){
		for (j=0; j<80; j++){
			screen[i][j]=map[i][j];
		}
	}
	
	for(ent = enemy_list; ent; ent = ent->next)
        render_to_buffer(screen, (int)ent->y, (int)ent->x, '&');
	
	for(ent = player_list; ent; ent = ent->next)
        render_to_buffer(screen, (int)ent->y, (int)ent->x, '@');
	
	memset(screen[23], '\0', sizeof(char)*80);
	for(i = 1; i <= 9; ++i)
        life[i-1] = (player->life >= i? '@' : ' ');
    life[9] = '\0';
    i = sprintf(&screen[23][0], "Life: %s  Points: %.2f", life, player->points);
    screen[23][0+i] = ' ';
	
    cli_render(screen);
}

void render_to_buffer(char screen[24][80], int px, int py, char c)
{
    if(px >= 0 && px < 24 && py >= 0 && py < 80)
        screen[px][py] = c;
}


/**
 * Aguarda um pouco antes do próximo ciclo.
 */
void sync(void)
{
    cli_sync();
}
