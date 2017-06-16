#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "entities.h"
#include "maps.h"

typedef struct {
	int x;
	int y;
}m_cursor;

void save_map(char b_map[23][80], int s_map, int n_food);
/*
*Funções úteis
*/
void map_update(char b_map[23][80], m_cursor *cursor, int *g_out, int *n_food, char *m_block);
void map_render(char b_map[23][80], m_cursor cursor, char *m_block);
void menu_map_update(m_cursor *cursor, int *s_option, int n_option, int *g_out, char function);
void menu_map_render(char background[24][80], m_cursor cursor);
void m_render_to_buffer(char screen[24][80], int px, int py, char c);
void m_sync(void);

/**
*Carrega o mapa de jogo
*/
void c_map(char map[23][80], int s_map, int *n_food){
	FILE *file;
	int l, c, i, j;
	char f_map[30];
	
	j = sprintf(f_map, "maps\\map%d.txt", s_map);
	
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
void mk_edit_map(int s_map, char function){
	int n_food, g_out;
	char b_map[23][80], m_block;
	m_cursor cursor;
	
	cursor.x = 38;
	cursor.y = 13;
	
	g_out = 0;
	
	m_block = '\0';
	
	/*PS o spawn dos mobs deve ser pré-definido assim como as paredes*/
	c_map(b_map, s_map, &n_food);
	while(!g_out){
		cli_update_keys();
        map_update(b_map, &cursor, &g_out, &n_food, &m_block);
        map_render(b_map, cursor, &m_block);
        m_sync();
    }
	if (function=='e')
		save_map(b_map, s_map, n_food);
	else
		save_map(b_map, 0, n_food);
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
void list_map(char function){
	FILE *file;
	int i, j, n_map, g_out, s_option;
	char background[24][80];
	m_cursor cursor;
	
	file = fopen("map_menu\\background.txt", "r");
	for (i=0; i<23; i++){
		for(j=0; j<80; j++){
			fscanf(file, "%c", &background[i][j]);
		}
	}
	fclose(file);
	j = sprintf(&background[20][63], ".VOLTAR");
	j = sprintf(&background[23][0], "PRESS UP/DOWN TO MOVE CURSOR                              PRESS ENTER TO SELECT\n");
	
	n_map = read_n_map();
	g_out = 0;
	s_option = 1;
	
	cursor.x = 12;
	cursor.y = 4;
	
	if (function=='e'){
		j = sprintf(&background[2][10], ".MAPAS EDITAVEIS");
		
		if (n_map==1){
			j = sprintf(&background[4][13], ".SEM MAPAS EDITAVEIS");
			s_option = 0;/*lembrar*/
			cursor.x = 62;
			cursor.y = 20;
		}
		else{
			for (i=1; i<n_map; i++){
				j = sprintf(&background[3+i][13], "%d.", i);
			}
		}
	}
	else if (function=='m'){
		j = sprintf(&background[2][10], ".MAPAS BASE");
		if (n_map==15){
			j = sprintf(&background[4][13], ".SEM ESPACO PARA NOVOS MAPAS");
			s_option = 0;
			cursor.x = 62;
			cursor.y = 20;
		}
		else {
			j = sprintf(&background[4][13], "0. PADRAO");
			for (i=1; i<n_map; i++){
				j = sprintf(&background[4+i][13], "%d.", i);
			}
			n_map++;
		}
	}
	
	while(!g_out){
		cli_update_keys();
		menu_map_update(&cursor, &s_option, n_map, &g_out, function);
		menu_map_render(background, cursor);
		m_sync();
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
		j = sprintf(f_map, "maps\\map%d.txt", s_map);
	}
	
	else{
		n_map = read_n_map();
		
		file = fopen("maps\\n_map.txt", "w");
		fprintf(file, "%d", n_map+1);
		fclose(file);
		
		j = sprintf(f_map, "maps\\map%d.txt", n_map);
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
void map_update(char b_map[23][80], m_cursor *cursor, int *g_out, int *n_food, char *m_block){
	if(keyhold(KEY_UP)){
		(*cursor).y--;
		if (!(*cursor).y||((((*cursor).x>=0&&(*cursor).x<=5)||((*cursor).x<=78&&(*cursor).x>=73))&&((*cursor).y==14||(*cursor).y==10))||((*cursor).y==12&&(*cursor).x<=41&&(*cursor).x>=35))
			(*cursor).y++; 
	}
	else if(keyhold(KEY_DOWN)){
		(*cursor).y++;
		if ((*cursor).y==22||((((*cursor).x>=0&&(*cursor).x<=5)||((*cursor).x<=78&&(*cursor).x>=73))&&((*cursor).y==12||(*cursor).y==8))||((*cursor).y==10&&(*cursor).x<=41&&(*cursor).x>=35))
			(*cursor).y--;
	}
	else if(keyhold(KEY_LEFT)){
		(*cursor).x--;
		if ((!(*cursor).x&&(*cursor).y!=11)||((*cursor).x==5&&(((*cursor).y<=10&&(*cursor).y>=8)||((*cursor).y>=12&&(*cursor).y<=14)))||((*cursor).y>=10&&(*cursor).y<=12&&(*cursor).x==41))
			(*cursor).x++;
	}
	else if(keyhold(KEY_RIGHT)){
		(*cursor).x++;
		if (((*cursor).x==78&&(*cursor).y!=11)||((*cursor).x==73&&(((*cursor).y<=10&&(*cursor).y>=8)||((*cursor).y>=12&&(*cursor).y<=14)))||((*cursor).y>=10&&(*cursor).y<=12&&(*cursor).x==35))
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
void map_render(char b_map[23][80], m_cursor cursor, char *m_block){
    int i, j;
	char screen[24][80];
	for (i=0; i<23; i++){
		for (j=0; j<80; j++){
			screen[i][j]=b_map[i][j];
		}
	}
	
	m_render_to_buffer(screen, cursor.y, cursor.x, '@');
	
	if (*m_block=='\0')
		j = sprintf(screen[23], "NO BLOCKS MARKED    PRESS ' ', '*', '#' TO MARK THE BLOCK   PRESS ENTER TO SAVE");
	else
		j = sprintf(screen[23], "Marked Block: '%c'               PRESS ESC TO UNMARK BLOCK   PRESS ENTER TO SAVE", *m_block);
	
	cli_render(screen);
}
void menu_map_update(m_cursor *cursor, int *s_option, int n_option, int *g_out, char function){
	if (!((function=='e'&&n_option==1)||(function=='m'&&n_option==15))){
		if (keyhold(KEY_UP)){
			(*cursor).y--;
			(*s_option)--;
			if ((*cursor).y==19){
				(*cursor).y = 2+n_option;
				(*cursor).x = 12;
				*s_option = n_option-1;
			}
			else if ((*cursor).y==3){
				(*cursor).y++;
				(*s_option)++;
			}
		}
		else if (keyhold(KEY_DOWN)){
			(*cursor).y++;
			(*s_option)++;
			if ((*cursor).y==19||*s_option==n_option){
				(*cursor).y = 20;
				(*cursor).x = 62;
				*s_option = 16;
			}
			else if ((*cursor).y==21){
				(*cursor).y--;
				(*s_option)--;
			}
		}
	}
	if (keyhold(KEY_ENTER)){
		/*função a ser realizada*/
		if (*s_option==16)
			*s_option = 0;
		if (!(*s_option))
			*g_out = 1;
		else {
			/*funcões criar e editar mapas*/
			if (function=='m'){
				mk_edit_map((*s_option)-1, function);
			}
			else if (function=='e'){
				mk_edit_map(*s_option, function);
			}
			/*TODO*/
			else *g_out = 1;
		}
	}
}
void menu_map_render(char background[24][80], m_cursor cursor){
	int i, j;
	char screen[24][80];
	
	for (i=0; i<24; i++){
		for(j=0; j<80; j++){
			screen[i][j] = background[i][j];
		}
	}
	
	m_render_to_buffer(screen, cursor.y, cursor.x, '>');
	
	cli_render(screen);
}
void m_render_to_buffer(char screen[24][80], int px, int py, char c){
	if(px >= 0 && px < 24 && py >= 0 && py < 80)
		screen[px][py] = c;
}
void m_sync(void){
    cli_sync();
}
