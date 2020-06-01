#include "queue.h"
#include <stdio.h>

/*Função de inserção de um elemento 'elem' em uma lista 'queue'*/
void queue_append (queue_t **queue, queue_t *elem){

    /*Caso o elemento seja nulo, imprime uma mensagem de erro e não insere na lista*/
    if(!elem)
        printf("ERRO: elemento nao existe!\n");

    /*Caso o elemento pertença a outra fila, seus membros 'next' e 'prev' não serão nulos e estarão...
    ...apontando para outro elemento de outra lista, portanto o elemento não pode ser inserido nessa lista...
    ...Para tal é imprimido uma mensagem de erro*/
    else if(elem->next != NULL || elem->prev != NULL)
        printf("ERRO: elemento ja pertence a alguma fila!\n");

    /*Caso a lista esteja vazia, será a inserido seu primeiro elemento*/
    else if((*queue) == NULL){
        *queue = elem;
        (*queue)->next = elem;
        (*queue)->prev = elem;
    }

    /*Caso a lista já tenha ao menos um elemento, insere o novo elemento no final da lista*/
    else{
        queue_t *aux = (*queue)->prev;
        elem->next = (*queue);
        elem->prev = aux;
        (*queue)->prev = elem;
        aux->next = elem;
    }
}

/*Função de exclusão de um elemento 'elem' de uma lista 'queue'*/
queue_t *queue_remove (queue_t **queue, queue_t *elem){
    queue_t *aux = NULL;

    /*Caso a fila esteja vazia, imprime uma mensagem de erro*/
    if(!(*queue))
        printf("ERRO: Fila esta vazia!\n");

    /*Caso o elemento recebido não exista, imprime uma mensagem de erro*/
    else if (!elem)
        printf("ERRO: elemento nao existe!\n");

    /*Caso tudo esteja correto para exclusão*/
    else{
        aux = (*queue);

        /*Caso haja apenas um elemento na lista e seja igual ao elemento*/
        if(aux == elem){

            /*Caso haja apenas um elemento na lista, ou seja, o membro 'next' aponta para ele mesmo*/
            if(aux->next == aux)
                (*queue) = NULL;

            /*Caso haja dois ou mais itens na lista*/
            else{
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                (*queue) = aux->next;
            }
            aux->next = NULL;
            aux->prev = NULL;
            return aux;
        }

        /*Percorre os demais itens da lista procurando o correto para exclusão
        Essa parte do algoritmo apenas testa todos os elementos que não sejam o primeiro da lista, e nem...
        ...se o segundo elemento for o último, ou seja, o membro 'next' aponta para o inicio da lista...
        Para se descobrir o fim da lista, testa-se se o membro 'next' de um elemento aponta para o primeiro elemento...
        Como o teste do laço while compara o próximo elemento em relação a outro na lista, um deles não serão comparado...
        ...pois satisfará a comparação, saindo do laço. Nesse caso, por exemplo, o primeiro elemento não será comparado...
        ...no if dentro do laço, por isso é necessário criar o algoritmo acima.
        Se a linha: 'aux = aux->next' for colocada apos o if, o último item da lista não será comparado no if.*/
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

        /*Caso o elemento não seja encontrado nos dois algoritmos acima, ele não pertence a esta lista.
        Se isso ocorrer, uma mensagem de erro é mostrada.*/
        printf("ERRO: elemento nao pertence a lista em questao!\n");
        aux = NULL;
    }
    return aux;
}

/*Função que retorna a quantidade de itens em uma lista*/
int queue_size (queue_t *queue) {
    int cont = 0;

    /*Caso a lista esteja vazia o teste if será pulado, retornando zero.
    Caso contrário a lista será percorrida e um contador irá armazenar a quantidade de elementos*/
    if(queue){
        cont++;
        queue_t *aux = queue;

        /*O laço while a seguir conta um item a menos, pois quando o percorrimento da lista chega...
        ...no último elemento, percebe que o membro 'next' aponta para o inicio da lista e o laço termina, sem contá-lo.
        Por isso é necessário adicionar 1 ao contador antes de iniciar o laço.*/
        while(aux->next != queue){
            cont++;
            aux = aux->next;
        }
    }
    return cont;
}

/*Função que percorre a lista e imprime o conteudo 'id' de cada elemento, além do 'id' do elemento anterior e do posterior*/
void queue_print (char *name, queue_t *queue, void print_elem (void*) ) {
    printf("%s: [", name);

    /*Caso a lista exista.*/
    if(queue){
        queue_t *aux = queue;
        print_elem(aux);

        /*Percorre a lista e imprime o conteudo de cada elemento*/
        while(aux->next != queue){
            printf(" ");
            aux = aux->next;
            print_elem(aux);
        }
    }
    printf("]\n");
}















