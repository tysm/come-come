#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "powers.h"

void dequeue(powers_queue_t** front){
	powers_queue_t* trash = *front;
	(*front)=(*front)->next;
	free(trash);
}
void enqueue(powers_queue_t** end, powers_queue_t** front, power_up_id_t power){
	powers_queue_t* new_power = malloc(sizeof(powers_queue_t));
	new_power->power = power;
	new_power->next=NULL;
	if (*end!=NULL)
		*end = (*end)->next = new_power;
	else
		*end = *front = new_power;
}
void brake_wall(char map[23][80], entity_t* player){
	int i=player->y, j=player->x;
	
	if (player->y_dir)
		for(; i>=0&&i<23; i+=player->y_dir){
			if (map[i][j]==35){
				map[i][j]=32;
				return;
			}
		}
	else
		for(; j>=0&&j<80; j+=player->x_dir){
			if (map[i][j]==35){
				map[i][j]=32;
				return;
			}
		}
}
void power_caller(char map[23][80], entity_t** player, powers_queue_t** front){
	switch((*front)->power){
		case BRAKE_WALL:
			brake_wall(map, *player);
			break;
	}
	dequeue(front);
}
