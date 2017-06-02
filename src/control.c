#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//definindo teclas de controle
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

void player_control(float *x, float *y, char screen[24][80]){
	int tecla, intx=*x, inty=*y;
	//talvez seja bom colocar um fflush ou __fpurge aqui
	tecla=getch();
	if (tecla==UP) intx--;
	else if (tecla==DOWN) intx++;
	else if (tecla==RIGHT) inty++;
	else if (tecla==LEFT) inty--;
	screen[intx][inty]='@';
	
	//retornando valor para a struct que esta em float
	*x=intx;
	*y=inty;
}
