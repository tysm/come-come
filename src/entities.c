#include "entities.h"
#include <assert.h>
#include <stdlib.h>

/**
 * Manipula a lista encadeada de entidades, adicionando o elemento `other`
 * da lista encabeçada por `head`.
 */
static void entity_add(entity_t** head, entity_t* other);

/**
 * Manipula a lista encadeada de entidades, removendo o elemento `other`
 * da lista encabeçada por `head`.
 */
static void entity_remove(entity_t** head, entity_t* other);


entity_t* entity_alloc(entity_t** head, entity_type_t type)
{
    entity_t* p = malloc(sizeof(entity_t));
    
    p->next = p->prev = NULL;
    p->type = type;
	p->x = 38.0f;
	p->y = 13.0f;
    p->x_dir = p->y_dir = 0.0f;

    entity_add(head, p);
    
    return p;
}

void entity_free(entity_t** head, entity_t* p)
{
    entity_remove(head, p);
    free(p);
}


int entity_collides(entity_t* a, entity_t* b)
{
    /* TODO colisão por interseção de cubos. */
    if((int)a->x == (int)b->x && (int)a->y == (int)b->y)
        return 1;
    return 0;
}

void entity_add(entity_t** head, entity_t* other)
{
    /* A entidade sendo adicionada não pode estar encadeada há uma lista. */
    assert(other->next == NULL && other->prev == NULL);
    
    /* A cabeça não pode ter entidade anterior a ela. */
    assert(*head == NULL || (*head)->prev == NULL);
    
    /* Transforma essa entidade na cabeça da lista. */
    other->prev = NULL;
    other->next = *head;
    
    /* Atualiza a cabeça antiga. */
    if(*head != NULL)
        (*head)->prev = other;
    
    *head = other;
}

void entity_remove(entity_t** head, entity_t* other)
{
    /* Como há um elemento sendo removido, a lista não é vazia, 
     * e portanto a cabeça não é nula. */
    assert(*head != NULL);

    /* Se o item a ser removido é a cabeça da lista, atualiza a cabeça. */
    if(other == *head)
    {
        assert(other->prev == NULL);
        *head = other->next;
    }

    /* Atualiza a entidade seguinte a esta. */
    if(other->next != NULL)
        other->next->prev = other->prev;

    /* Atualiza a entidade anterior a esta. */
    if(other->prev != NULL)
        other->prev->next = other->next;

    other->prev = NULL;
    other->next = NULL;
}
