#include "queue.h"
#include <stdio.h>

void queue_append (queue_t **queue, queue_t *elem){
    if(!elem)
        printf("\nERRO: elemento nao existe!\n");
    else if(elem->next != NULL || elem->prev != NULL)
        printf("\nERRO: elemento pertence a outra fila!\n");
    else if((*queue) == NULL){
        *queue = elem;
        (*queue)->next = elem;
        (*queue)->prev = elem;
    }
    else{
        queue_t *aux = (*queue)->prev;
        elem->next = (*queue);
        elem->prev = aux;
        (*queue)->prev = elem;
        aux->next = elem;
    }
}

queue_t *queue_remove (queue_t **queue, queue_t *elem){
    queue_t *aux = NULL;

    if(!(*queue))
        printf("\nERRO: Fila esta vazia!\n");
    else if (!elem)
        printf("\nERRO: elemento nao existe!\n");
    else{
        aux = (*queue);
        //Caso haja apenas um elemento na lista e seja igual ao elemento
        if(aux == elem){
            if(aux->next == aux)
                (*queue) = NULL;
            else{
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                (*queue) = aux->next;
            }
            aux->next = NULL;
            aux->prev = NULL;
            return aux;
        }
        while(aux->next != (*queue)){
            aux = aux->next;
            if(aux == elem){
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                aux->next = NULL;
                aux->prev = NULL;
                return aux;
            }
        }
        //Caso o elemento não pertenca à lista
        aux = NULL;
    }
    return aux;
}

int queue_size (queue_t *queue) {
    int cont = 0;

    if(queue != NULL){
        cont++;
        queue_t *aux = queue;
        while(aux->next != queue){
            cont++;
            aux = aux->next;
        }
    }
    return cont;
}

void queue_print (char *name, queue_t *queue, void print_elem (void*) ) {

}















