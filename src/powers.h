#include "entities.h"

extern void enqueue(powers_queue_t** end, powers_queue_t** front, power_up_id_t power);

void power_caller(char map[23][80], entity_t** player, powers_queue_t** front);
