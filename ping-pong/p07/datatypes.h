// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Estruturas de dados internas do sistema operacional

#ifndef __DATATYPES__
#define __DATATYPES__
#include <ucontext.h>

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
   int tarefaUsuario;               /*Define se é uma tarefa de sistema (0) ou tarefa de usuário (1)*/
   unsigned int tempoExecucao;      /*Contador do tempo de execução de uma tarefa, do tempo que começa a executar, até o tempo em que acaba*/
   unsigned int tempoProcessamento; /*Contador do tempo em que uma tarefa é processada*/
   int ativacoes;                   /*Quantidade de vezes que uma tarefa é ativada*/
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif
