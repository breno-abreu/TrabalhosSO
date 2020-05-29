//#define DEBUG
#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define STACKSIZE 32768


task_t MainTask;            /*Tarefa referente ao contexto da função main*/
task_t *CurrentTask;        /*Variável que aponta para a tarefa que está em andamento*/
int contador = 0;           /*Variável que será utilizada para inicializar o campo 'tid' da struct 'task_t'...
                            ...tornando possível que cada tarefa tenha um identificador diferente.*/
task_t dispatcher;          /*Tarefa referente ao dispatcher*/
task_t *readyQueue;         /*Fila de tarefas prontas para serem executadas*/

/*Retorna a próxima tarefa a ser executada*/
task_t* scheduler()
{
    /*Retorna a cabeça da lista de prontas, ou seja o primeiro elemento que entrou an lista*/
    return readyQueue;
}

/*Função recebida pelo dispatcher, irá executar as tarefas que estão na fila, enquanto houverem elementos*/
void dispatcher_body()
{
    task_t * next;

    /*Enquanto há elementos na fila de prontas continua executando as tarefas*/
    while(queue_size((queue_t*)readyQueue) > 0){
        next = scheduler();
        if(next){
            task_switch(next);
        }
    }

    /*Após executar todas as tarefas, retorna para o contexto da função main*/
    task_exit(0);
}

void pingpong_init()
{
    /*desativa o buffer da saida padrao (stdout), usado pela função printf*/
    setvbuf (stdout, 0, _IONBF, 0);

    /*Inicialmente a tarefa atuar será a relacionada a função main*/
    CurrentTask = &MainTask;
    MainTask.tid = 0;

    /*Cria a tarefa dispatcher*/
    task_create(&dispatcher, dispatcher_body, NULL);

    readyQueue = NULL;
}

int task_create( task_t *task,
                 void (*start_func)(void *),
                 void *arg)
{

    char *stack;
    contador++;

    /*Inicializa o contexto de uma tarefa*/
    if(getcontext(&task->context) < 0)
        return -1;

    /*Preenche os campos necessários do contexto antes de chamar a função 'makecontext()'*/
    stack = malloc(STACKSIZE);
    if(stack){
        task->context.uc_stack.ss_sp = stack;
        task->context.uc_stack.ss_size = STACKSIZE;
        task->context.uc_stack.ss_flags = 0;
        task->context.uc_link = 0;
    }
    else{
        perror ("Erro na criação da pilha: ");
        return -1;
    }

    /*Relaciona a função 'start_func' com o contexto em questão*/
    makecontext(&task->context, (void*)start_func, 1, (char*) arg);

    /*Dá a tarefa um identificador único*/
    task->tid = contador;

    #ifdef DEBUG
    printf("task_create: criou tarefa %d\n", task->tid);
    #endif

    /*Inclui tarefas novas na lista de prontas, a menos que a tarefa seja o dispatcher*/
    if(task != &dispatcher)
        queue_append((queue_t**) &readyQueue, (queue_t*) task);

    return task->tid;
}

int task_switch (task_t *task)
{
    /*O ponteiro 'CurrentTask' aponta para a tarefa recebida*/
    task_t *Aux = CurrentTask;
    CurrentTask = task;

    /*Salva o contexto da tarefa anterior e aciona o contexto da tarefa recebida*/
    if(swapcontext(&Aux->context, &task->context) < 0)
        return -1;

    #ifdef DEBUG
    printf("task_switch: trocando contexto %d -> %d\n", Aux->tid, task->tid);
    #endif

    return 0;
}

void task_exit (int exitCode)
{
    #ifdef DEBUG
    printf("task_exit: tarefa %d sendo encerrada\n", CurrentTask->tid);
    #endif

    /*Caso a tarefa atual não seja nem a main nem o dispatcher, remove da fila de prontas e...
    ...troca para a tarefa dispatcher*/
    if(CurrentTask != &dispatcher && CurrentTask != &MainTask){
        queue_remove((queue_t**) &readyQueue, (queue_t*) CurrentTask);
        task_switch(&dispatcher);
    }

    /*Caso a tarefa a ser finalizada seja o dispatcher, troca para a tarefa main*/
    else
        task_switch(&MainTask);
}

int task_id ()
{
    /*Retorna o identificador da tarefa que está em andamento*/
    return CurrentTask->tid;
}

void task_yield ()
{
    #ifdef DEBUG
    printf("task_yield: tarefa %d sendo colocada no final da lista de prontas e devolvendo o contexto para o dispatcher\n", CurrentTask->tid);
    #endif

    /*Caso a tarefa atual não seja nem a tarefa main nem o dispatcher, exclui o elemento da lista...
    ..e o reposiciona no final da lista*/
    if(CurrentTask != &MainTask && CurrentTask != &dispatcher){
        queue_remove((queue_t**) &readyQueue, (queue_t*) CurrentTask);
        queue_append((queue_t**) &readyQueue, (queue_t*) CurrentTask);
    }

    /*Troca o contexto para o dispatcher*/
    task_switch(&dispatcher);
}

void task_suspend (task_t *task, task_t **queue)
{
    if(queue && task){
        queue_t * aux = (queue_t*) task;

        /*Nó é retirado caso ja pertença a alguma fila**/
        if(aux->next != NULL && aux->prev != NULL){
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;
        }

        /*Adiciona o nó na fila de prontas*/
        queue_append((queue_t**) &queue, aux);
    }

}

void task_resume (task_t *task)
{
    queue_t * aux = (queue_t*) task;

    /*Nó é retirado caso ja pertença a alguma fila*/
    if(aux->next != NULL && aux->prev != NULL){
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;
    }

    /*Adiciona o nó na fila de prontas*/
    queue_append((queue_t**) &readyQueue, aux);
}


















