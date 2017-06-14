#include <stdio.h>
#include <stdlib.h>
/*Caso os mapas sejam escolhidos aleatoreamente
#include <time.h>*/
#include <string.h>

/**
*Carrega o mapa de jogo
*/
void c_map(char map[23][80], int s_map){
	FILE *file;
	int l, c, i, j;
	char f_map[30];
	
	/*caso os mapas sejam escolhidos aleatoreamente
	srand(time(NULL));
	sprintf(f_mapa, "mapa%d.txt", rand()%10);*/
	
	/*caso os mapas sejam aleatoriamente escolhidos remover esta parte*/
	sprintf(f_map, "maps\\map%d.txt", s_map);
	
	file = fopen(f_map, "r");
	fscanf(file, "%d%d", &l, &c);
	
	fseek(file, 2, SEEK_CUR);
	for (i = 0; i < l; i++){
		for (j = 0; j < c; j++){
			fscanf(file, "%c", &map[i][j]);
		}
	}
}

/**
*Edita o mapa de jogo
*/
/*Criar editor Aqui*/