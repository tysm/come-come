//Player 01 movement control version Beta 0.0.1 by Kimblee
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//definindo teclas de controle
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

void player_control(int *x, int *y){
	int tecla;
	setbuf(stdin, NULL);
	tecla=getch();
	if (tecla==UP) *y+=1;
	else if (tecla==DOWN) *y-=1;
	else if (tecla==RIGHT) *x+=1;
	else if (tecla==LEFT) *x-=1;
}
