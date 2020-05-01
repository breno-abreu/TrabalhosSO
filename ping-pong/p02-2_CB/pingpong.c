#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 32768

task_t MainTask;
task_t *CurrentTask;
task_t *TaskQueue;
int contador = 0;

void pingpong_init()
{
    //desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf (stdout, 0, _IONBF, 0);
    CurrentTask = &MainTask;
}

int task_create( task_t *task,
                 void (*start_func)(void *),
                 void *arg)
{

    char *stack;
    contador++;

    if(getcontext(&task->context) < 0)
        return -1;

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

    makecontext(&task->context, start_func, 1, (char*) arg);
    task->tid = contador;

    return 0;
}

int task_switch (task_t *task)
{
    task_t *Aux = CurrentTask;
    CurrentTask = task;
    if(swapcontext(&Aux->context, &task->context) < 0)
        return -1;

    return 0;
}

void task_exit (int exitCode)
{
    task_switch(&MainTask);
}

int task_id ()
{
    return CurrentTask->tid;
}




















