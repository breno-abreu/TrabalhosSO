// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Estruturas de dados internas do sistema operacional

#ifndef __DATATYPES__
#define __DATATYPES__
#include <ucontext.h>
#define SUSPENSA 0
#define PRONTA 1
#define USUARIO 2
#define SISTEMA 3
#define ADORMECIDA 4
#define FINALIZADA 5

// Estrutura que define uma tarefa
typedef struct task_t
{
   struct task_t *prev, *next;      /*Ponteiros que permitem a integração da tarefa em uma lista do tipo queue_t*/
   int tid;                         /*Identificador da tarefa*/
   ucontext_t context;              /*Contexto da tarefa*/
   int prioridadeEstatica;          /*Prioridade estatica da tarefa, usada para o escalonamento*/
   int prioridadeDinamica;          /*Prioridade dinamica da tarefa, usada para o escalonamento*/
   int contadorQuantum;             /*Contador de quantum*/
   int quantumEstatico;             /*Valor estático de quantum*/
   int tipoTarefa;                  /*Define se é uma tarefa de sistema (3) ou tarefa de usuário (2)*/
   unsigned int tempoExecucao;      /*Contador do tempo de execução de uma tarefa, do tempo que começa a executar, até o tempo em que acaba*/
   unsigned int tempoProcessamento; /*Contador do tempo em que uma tarefa é processada*/
   int ativacoes;                   /*Quantidade de vezes que uma tarefa é ativada*/
   int estado;                      /*Estado da tarefa. Pode ser suspensa(0) ou pronta(1)*/
   struct task_t *suspendedQueue;   /*Fila de tarefas suspendidas por uma tarefa*/
   int exitCode;                    /*Código de finalização da tarefa recebido na função task_exit(exitCode)*/
   int sleepTime;                   /*Quantidade de tempo em segundos em que a tarefa ficara adormecida*/
} task_t ;

// estrutura que define um semáforo
typedef struct
{
    int value;                        /*Contador de tarefas na fila*/
    struct task_t *semQueue;          /*Fila de tarefas de um semáforo*/
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
    int maxThreads;                   /*Quantidade máxima de threads que uma barreira suporta*/
    int contThreads;                  /*Contador de threads da barreira*/
    struct task_t *barrierQueue;      /*Fila de tarefas da barreira*/
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif
