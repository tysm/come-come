#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "entities.h"

typedef struct {
	int x;
	int y;
}edit_cursor;

void save_map(char b_map[23][80], int s_map, int n_food);
/*
*Funções úteis
*/
void m_update(char b_map[23][80], edit_cursor *cursor, int *g_out, int *n_food, char *m_block);
void m_render(char b_map[23][80], edit_cursor cursor, char *m_block);
void m_render_to_buffer(char screen[24][80], int px, int py, char c);
void m_sync(void);

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
*Mapa selecionado (s_map) define:
*criação de mapa se for 0
*edição se for diferente de 0
*/
void mk_edit_map(int s_map){
	int n_food, g_out;
	char b_map[23][80], m_block;
	edit_cursor cursor;
	
	cursor.x = 1;
	cursor.y = 1;
	
	g_out = 0;
	
	m_block = '\0';
	
	/*PS o spawn dos mobs deve ser pré-definido assim como as paredes*/
	c_map(b_map, s_map, &n_food);
	while(!g_out){
		cli_update_keys();
        m_update(b_map, &cursor, &g_out, &n_food, &m_block);
        m_render(b_map, cursor, &m_block);
        m_sync();
    }
	
	save_map(b_map, s_map, n_food);
}

/**
*Lê a quantidade
*de mapas no arquibo n_map.txt
*/
int read_n_map(void){
	FILE *file;
	int n_map;
	file = fopen("maps\\n_map.txt", "r");
	fscanf(file, "%d", &n_map);
	fclose(file);
	
	return n_map;
}

/**
*Lista numeração de mapas existentes
*/
void list_map(void){
	int i, n_map;
	
	n_map = read_n_map();
	
	if (n_map==1){/*passar para o render*/
		/*existe apenas o mapa base*/
		printf("\nNao ha mapas\n");
	}
	else{/*passar para o render*/
		printf("\nMapas:\n");
		for (i=1; i<n_map; i++){
			printf("%d. \n", i);
		}
	}
}

/**
*Função para salvar o mapa
*Tanto o novo/editado
*Parametros em ordem:
*mapa a ser salvo
*número do mapa (se for 0: vai criar um novo mapa no diretório já que sempre deve existir o mapa padrão/se não: vai sobrepor o mapa da posição como editado)
*número de comida do mapa atualizado
*/
void save_map(char b_map[23][80], int s_map, int n_food){
	FILE *file;
	char f_map[30];
	int i, j, n_map;
	
	if (s_map){
		sprintf(f_map, "maps\\map%d.txt", s_map);
	}
	
	else{
		n_map = read_n_map();
		
		file = fopen("maps\\n_map.txt", "w");
		fprintf(file, "%d", n_map+1);
		fclose(file);
		
		sprintf(f_map, "maps\\map%d.txt", n_map);
	}
	
	file = fopen(f_map, "w");
	fprintf(file, "%d %d %d\n", 23, 80, n_food);
	for (i=0; i<23; i++){
		for (j=0; j<80; j++){
			fprintf(file, "%c", b_map[i][j]);
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
*Funções úteis
*/
void m_update(char b_map[23][80], edit_cursor *cursor, int *g_out, int *n_food, char *m_block){
	if(keyhold(KEY_UP)){
		(*cursor).y--;
		if (!(*cursor).y||!(*cursor).x||(*cursor).x==78)
			(*cursor).y++; 
	}
	else if(keyhold(KEY_DOWN)){
		(*cursor).y++;
		if ((*cursor).y==22||!(*cursor).x||(*cursor).x==78)
			(*cursor).y--;
	}
	else if(keyhold(KEY_LEFT)){
		(*cursor).x--;
		if (!(*cursor).x&&(*cursor).y!=11)
			(*cursor).x++;
	}
	else if(keyhold(KEY_RIGHT)){
		(*cursor).x++;
		if ((*cursor).x==78&&(*cursor).y!=11)
			(*cursor).x--; 
	}
	
	if ((*cursor).y==11&&(*cursor).x==-1)
		(*cursor).x = 78;
	else if ((*cursor).y==11&&(*cursor).x==79)
		(*cursor).x = 0;
	
	if(keyhold(KEY_STAR)){
		*m_block = '*';
	}
	else if(keyhold(KEY_HASH)){
		*m_block = '#';
	}
	else if(keyhold(KEY_SPACE)){
		*m_block = ' ';
	}
	else if(keyhold(KEY_ESCAPE)){
		*m_block = '\0';
	}
	else if(keyhold(KEY_ENTER)){
		*g_out=1;
	}
	
	if (*m_block!='\0'){
		if (*m_block=='*'){
			if (b_map[(*cursor).y][(*cursor).x]!='*')
				(*n_food)++;
		}
		else if (b_map[(*cursor).y][(*cursor).x]=='*')
			(*n_food)--;
		
		b_map[(*cursor).y][(*cursor).x] = *m_block;
	}
}
void m_render(char b_map[23][80], edit_cursor cursor, char *m_block){
    int i, j;
	char screen[24][80];
	for (i=0; i<23; i++){
		for (j=0; j<80; j++){
			screen[i][j]=b_map[i][j];
		}
	}
	
	m_render_to_buffer(screen, cursor.y, cursor.x, '@');
	
	memset(screen[23], '\0', sizeof(char)*80);
	if (*m_block=='\0')
		sprintf(screen[23], "NO BLOCKS MARKED    PRESS ' ', '*', '#' TO MARK THE BLOCK   PRESS ENTER TO SAVE", *m_block);
	else
		sprintf(screen[23], "Marked Block: '%c'           PRESS ESCAPE TO UNMARK BLOCK    PRESS ENTER TO SAVE", *m_block);
	
	cli_render(screen);
}
void m_render_to_buffer(char screen[24][80], int px, int py, char c){
	if(px >= 0 && px < 24 && py >= 0 && py < 80)
		screen[px][py] = c;
}
void m_sync(void){
    cli_sync();
}
