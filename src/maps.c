#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "entities.h"
#include <windows.h>
#include <conio.h>
/*
*Funções uteis
*#denihelp
*/
void m_update(char b_map[23][80], entity_t **player, int *g_out, int *n_food);
void m_render(char b_map[23][80], entity_t* player);
void m_render_to_buffer(char screen[24][80], int px, int py, char c);
void m_sync(void);
void save_map(char b_map[23][80], int s_map, int n_food);

/**
*Carrega o mapa de jogo
*/
void c_map(char map[23][80], int s_map, int *n_food){
	FILE *file;
	int l, c, i, j;
	char f_map[30];
	
	sprintf(f_map, "maps\\map%d.txt", s_map);
	
	file = fopen(f_map, "r");
	fscanf(file, "%d%d%d", &l, &c, &*n_food);
	
	fseek(file, 2, SEEK_CUR);
	for (i = 0; i < l; i++){
		for (j = 0; j < c; j++){
			fscanf(file, "%c", &map[i][j]);
		}
	}
	fclose(file);
}

/**
*Cria/Edita mapas
*Fiz semelhante ao main
*Já que é um "outro modo"
*o mapa selecionado: s_map
*define:
*criação de mapa se for 0 ou
*edição de mapa se for diferente de 0
*editando o mapa selecionado
*/
void mk_edit_map(int s_map){
	int n_food, g_out;
	char b_map[23][80];
	entity_t* player;
	
	player->y=1.0f;
	player->x=1.0f;
	/*Passar que não é pra sair do while*/
	g_out=0;
	
	/*carregar o mapa base para edição*/
	c_map(b_map, s_map, &n_food);
	/*PS o spawn dos mobs deve ser pré-definido assim como as paredes*/
	
	while(!g_out){
		/* Se for usar aqui, tem que alterar cli_update_keys();*/
        m_update(b_map, &player, &g_out, &n_food);
        m_render(b_map, player);
        m_sync();
    }
	/*salvar mapa*/
	save_map(b_map, s_map, n_food);
}
/**
*Lista numeração de mapas existentes
*/
void list_map(void){
	FILE *file;
	int i, n_map;
	file = fopen("maps\\n_map.txt", "r");
	fscanf(file ,"%d", &n_map);
	if (n_map==1){
		/*existe apenas o mapa base*/
		printf("\nNão há mapas\n");
	}
	else{
		printf("\nMapas:\n");
		for (i=1; i<n_map; i++){
			printf("%d. \n", i);
			/*os mapas terão nomes?*/
		}
	}
	fclose(file);
}
/**
*Excluir mapas
*/
void del_map(s_map){
	/*FILE *file;
	preciso apagar o mapa selecionado e renomear todos os mapas a partir do s_map*/
}

/*
*funções úteis
*/
void m_update(char b_map[23][80], entity_t **player, int *g_out, int *n_food){
	char c;
	c = _getch();
	if(c == 224 && _kbhit()){
		switch(_getch()){
			case 72:
				(*player)->x_dir = 0.0f;
				(*player)->y_dir = -1.0f; 
				break;
			case 80:
				(*player)->x_dir = 0.0f;
				(*player)->y_dir = 1.0f;
				break;
			case 75:
				(*player)->x_dir = -1.0f;
				(*player)->y_dir = 0.0f;
				break;
			case 77:
				(*player)->x_dir = 1.0f;
				(*player)->y_dir = 0.0f; 
				break;
		}
	}
	else{
		switch(c){
			case 42:
				b_map[(int)(*player)->y][(int)(*player)->x]='*';
				(*n_food)++;
				break;
			case 35:
				if (b_map[(int)(*player)->y][(int)(*player)->x]=='*')
					(*n_food)--;
				b_map[(int)(*player)->y][(int)(*player)->x]='#';
				break;
			case 27:
				if (b_map[(int)(*player)->y][(int)(*player)->x]=='*')
					(*n_food)--;
				b_map[(int)(*player)->y][(int)(*player)->x]=' ';/*seta para espaço em branco*/
				break;
			case 13:
				*g_out=1;/*sair concluir a criação*/
				break;
		}
	}
	/*o player so podera ver a alteração que fez no mapa após sair de cima da posição alterada.*/
	
	if ((*player)->y==11&&(*player)->x==0&&(*player)->x_dir==-1)
		(*player)->x = 78.0f;
	else if ((*player)->y==11&&(*player)->x==78&&(*player)->x_dir==1)
		(*player)->x = 0.0f;
	else {
		(*player)->x += (*player)->x_dir;
		(*player)->y += (*player)->y_dir;
	}
	if (!(*player)->y||(*player)->y==22||(!(*player)->x&&(*player)->y!=11)||((*player)->x==78&&(*player)->y!=11)){
		(*player)->x -= (*player)->x_dir;
		(*player)->y -= (*player)->y_dir;
	}
}
void m_render(char b_map[23][80], entity_t* player){
    int i, j;
	char screen[24][80];

	for (i=0; i<23; i++){
		for (j=0; j<80; j++){
			screen[i][j]=b_map[i][j];
		}
	}
	
	m_render_to_buffer(screen, (int)player->y, (int)player->x, '@');
	
	memset(screen[23], '\0', sizeof(char)*80);
	
	cli_render(screen);
}
void m_render_to_buffer(char screen[24][80], int px, int py, char c){
	if(px >= 0 && px < 24 && py >= 0 && py < 80)
		screen[px][py] = c;
}
void m_sync(void){
    cli_sync();
}

/**
*Função para salvar
*Tanto o novo mapa
*Quanto o mapa editado
*Parametros em ordem:
*mapa a ser salvo
*número do mapa (se for 0: vai criar um novo mapa no diretório já que sempre deve existir o mapa padrão/se não: vai sobrepor o mapa da posição como editado)
*número de comida do mapa atualizado
*/
void save_map(char b_map[23][80], int s_map, int n_food){
	FILE *file;
	char f_map[30];
	int i, j, n_map;
	
	/*salvar edição*/
	if (s_map){
		sprintf(f_map, "maps\\map%d.txt", s_map);
	}
	
	/*salvar criação*/
	else{
		/*checar quantos mapas existem*/
		file = fopen("maps\\n_map.txt", "r");
		fscanf(file, "%d", &n_map);
		fprintf(file, "%d", n_map+1);
		fclose(file);
		
		sprintf(f_map, "maps\\map%d.txt", n_map);
	}
	
	/*upa o mapa*/
	file = fopen(f_map, "w");
	fprintf(file, "%d %d %d\n", 23, 80, n_food);
	for (i=0; i<23; i++){
		/*oq é mais ráido, printar a string linha direta ou cada char?*/
		for (j=0; j<80; j++){
			fprintf(file, "%c", b_map[i][j]);
		}
	}
	fclose(file);
}