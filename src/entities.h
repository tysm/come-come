#pragma once

/**
 * Tipos de poderes (habilidades) no jogo.
 */
typedef enum power_up_id{
	BRAKE_WALL/*,
	FREEZE,
	FUCK_YOU,
	KAMEHAMEHA,
	SPEED_UP,
	TELEPORT*/
} power_up_id_t;

/**
 * Estrutura de queue de poderes de cada personagem (NPC).
 */
typedef struct powers_queue{
	power_up_id_t power;
	struct powers_queue* next;
}powers_queue_t;

/**
 * Tipos de entidades (NPCs) no jogo.
 */
typedef enum entity_type
{
    ENTITY_PLAYER1,        /* aka Pac-Man */
    ENTITY_GHOST_RED,      /* aka Blinky */
    ENTITY_GHOST_BLUE,     /* aka Inky */
    ENTITY_GHOST_PINK,     /* aka Pinky */
    ENTITY_GHOST_ORANGE,   /* aka Clyde */
    
    ENTITY_GHOST_START = ENTITY_GHOST_RED,
    ENTITY_GHOST_END   = ENTITY_GHOST_ORANGE
    
} entity_type_t;

/**
 * Estrutura de uma entidade (NPC).
 *
 * As entidades são também nós de uma lista encadeada de entidades,
 * portanto contem ponteiros para as próximas entidades da lista.
 *
 */
typedef struct entity
{
    
    struct entity *next, *prev;
    
    entity_type_t type;
	/* powers_queue_t *front, *end; */
	int life;
    float x, y, x_dir, y_dir, points;

} entity_t;

/**
 * Aloca uma entidade na lista encabeçada por `head` e retorna seu ponteiro.
 */
extern entity_t* entity_alloc(entity_t** head, entity_type_t type);

/**
 * Libera uma entidade `P` alocada na lista encabeçada por `head`.
 */
extern void entity_free(entity_t** head, entity_t* p);

/**
 * Testa se a entidade `a` está colidindo com a entidade `b`.
 */
extern int entity_collides(entity_t* a, entity_t* b);

