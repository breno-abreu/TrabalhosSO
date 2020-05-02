#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 32768

//Tarefa referente ao contexto da função main
task_t MainTask;
//Variável que aponta para a tarefa que está em andamento
task_t *CurrentTask;
//Variável que será utilizada para inicializar o campo 'tid' da struct 'task_t'...
//...tornando possível que cada tarefa tenha um identificador diferente.
int contador = 0;

//Inicializa algumas variáveis e desativa o buffer da saída padrão
void pingpong_init()
{
    //desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf (stdout, 0, _IONBF, 0);
    //Inicialmente a tarefa atuar será a relacionada a função main
    CurrentTask = &MainTask;
}

int task_create( task_t *task,
                 void (*start_func)(void *),
                 void *arg)
{

    char *stack;
    contador++;
    //Inicializa o contexto de uma tarefa
    if(getcontext(&task->context) < 0)
        return -1;

    //Preenche os campos necessários do contexto antes de utilizar a função 'makecontext()'
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
    //Relaciona a função 'start_func' com o contexto em questão
    makecontext(&task->context, start_func, 1, (char*) arg);
    //Dá a tarefa um identificador único
    task->tid = contador;

    return 0;
}

int task_switch (task_t *task)
{
    //O ponteiro 'CurrentTask' aponta para a tarefa recebida
    task_t *Aux = CurrentTask;
    CurrentTask = task;
    //Salva o contexto da tarefa anterior e aciona o contexto da tarefa recebida
    if(swapcontext(&Aux->context, &task->context) < 0)
        return -1;

    return 0;
}

void task_exit (int exitCode)
{
    //Finaliza a tarefa atual retornando para a função main
    task_switch(&MainTask);
}

int task_id ()
{
    //Retorna o identificador da tarefa que está em andamento
    return CurrentTask->tid;
}




















