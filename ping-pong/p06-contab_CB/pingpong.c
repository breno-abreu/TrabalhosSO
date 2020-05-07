//#define DEBUG
#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "queue.h"

#define STACKSIZE 32768
#define QUANTUM 20

task_t MainTask;                            //Tarefa referente ao contexto da função main
task_t *CurrentTask;                        //Variável que aponta para a tarefa que está em andamento
int contador;                               //Variável que será utilizada para inicializar o campo 'tid' da struct 'task_t'...
                                            //...tornando possível que cada tarefa tenha um identificador diferente.
task_t dispatcher;                          //Tarefa referente ao dispatcher
task_t *readyQueue;                         //Fila de tarefas prontas para serem executadas
int aging;                                  //Fator de envelhecimento
struct sigaction action;                    //Estrutura que define o tratador de sinal
struct itimerval timer;                     //Estrutura de inicialização do timer
int contadorTimer;                          //Contador referente ao Quantum de cada tarefa
unsigned int tempoAtual;                    //Conta o tempo de execução do programa em milisegundos

//Função que será ativada quando o temporizador chegar a um determinado tempo
void tratador()
{
    
    CurrentTask->tempoProcessamento++;      //Tempo de processamento da tarefa atual é atualizado

    //Caso seja uma tarefa de usuário
    if(CurrentTask->tarefaUsuario == 1){
        //Caso o quantum chegue a zero, seu quantum é atualizado para o valor de quantum predefinido, ...
        //...,a tarefa é adicionada ao final da fila de prontos e o contexto é mudado para o dispatcher
        if(CurrentTask->contadorQuantum <= 0){
            CurrentTask->contadorQuantum = QUANTUM;
            task_yield();
        }
        CurrentTask->contadorQuantum--;     //Decrementa o contador de quantum da tarefa atual
    }
    tempoAtual++;                           //Contador de tempo de execução é atualizado a cada 1 milisegundo
}

//Retorna a próxima tarefa a ser executada
task_t* scheduler()
{
    /*task_t* aux = readyQueue;
    task_t* tarefa = NULL;
    int minPrio = 21;                       //Variável que recebe o valor de prioridade da tarefa com maior prioridade
    int minIden = 10;                       //Variável que recebe o valor identificador da tarefa com maior prioridade
    int tamanhoFila = queue_size((queue_t*) readyQueue);//Recebe o tamanho da fila de prontos

    //Percorre a lista de prontos
    for(int i = 0; i < tamanhoFila; i++){
        //Se o valor da prioridade dinâmica for menor que o valor de 'minPrio', 'minPrio' é atualizada assim como a tarefa que será retornada.
        //Além disso, caso duas tarefas tenham o mesmo valor de prioridade dinâmica, o fator de desempate é qual delas possui o menor valor identificador 'tid'
        if(aux->prioridadeDinamica < minPrio || (aux->prioridadeDinamica == minPrio && aux->tid < minIden)){
            minPrio = aux->prioridadeDinamica;
            minIden = aux->tid;
            tarefa = aux;
        }
        aux = aux->next;
    }

    //Percorre a fila de prontos novamente
    for(int i = 0; i < tamanhoFila; i++){
        //Caso a tarefa não seja a escolhida para ser retornada, ou seja, não seja a de maior prioridade, ...
        //...ela é envelhecida somando sua prioridade dinâmica com o valor predefinido 'aging'
        if(aux != tarefa){
            aux->prioridadeDinamica += aging;
            if(aux->prioridadeDinamica < -20)
                aux->prioridadeDinamica = -20;
        }
        //Caso a tarefa seja a escolhida, ou seja, tem maior prioridade, seu valor de prioridade dinâmica é atualizada recebendo seu valor de prioridade estática
        else
            aux->prioridadeDinamica = aux->prioridadeEstatica;

        aux = aux->next;
    }
    return tarefa;                          //Retorna a tarefa de maior prioridade*/

    return readyQueue;                      //Retorna o primeiro elemento da fila de prontos
}

//Função recebida pelo dispatcher, irá executar as tarefas que estão na fila, enquanto houverem elementos
void dispatcher_body()
{
    task_t * next;
    //Enquanto há elementos na fila de prontos continua executando as tarefas
    while(queue_size((queue_t*)readyQueue) > 0){
        next = scheduler();                 //next recebe a próxima tarefa a ser executada
        next->ativacoes++;                  //Atualiza a quantidade de ativações da tarefa a ser executada
        if(next){
            task_switch(next);
        }
    }
    task_exit(0);                           //Após executar todas as tarefas, retorna para o contexto da função main
}

//Inicializa algumas variáveis e desativa o buffer da saída padrão
void pingpong_init()
{

    setvbuf (stdout, 0, _IONBF, 0);         //desativa o buffer da saida padrao (stdout), usado pela função printf
    CurrentTask = &MainTask;                //Inicialmente a tarefa atual será a relacionada a função main
    contador = 0;
    MainTask.tid = 0;
    task_create(&dispatcher, dispatcher_body, NULL);    //Cria a tarefa dispatcher
    dispatcher.tarefaUsuario = 0;
    readyQueue = NULL;
    aging = -1;
    contadorTimer = QUANTUM;
    tempoAtual = 0;

    //Registra a ação para o sinal de timer SIGALRM
    action.sa_handler = tratador;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction (SIGALRM, &action, 0) < 0){
        perror ("Erro em sigaction: ");
        exit (1);
    }
    //Ajusta valores  do temporizador, que irá gerar um pulso a cada 1 milisegundo
    timer.it_value.tv_usec = 1;             // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec  = 0;             // primeiro disparo, em segundos
    timer.it_interval.tv_usec = 1000;       // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec  = 0;          // disparos subsequentes, em segundos

    //Arma o temporizador ITIMER_REAL
    if (setitimer (ITIMER_REAL, &timer, 0) < 0){
        perror ("Erro em setitimer: ");
        exit (1);
    }
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

    //Preenche os campos necessários do contexto antes de chamar a função 'makecontext()'
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

    makecontext(&task->context, (void*)start_func, 1, (char*) arg); //Relaciona a função 'start_func' com o contexto em questão
    task->tid = contador;                   //Dá a tarefa um identificador único

    //Código para depuração caso DEBUG esteja definido
    #ifdef DEBUG
    printf("task_create: criou tarefa %d\n", task->tid);
    #endif

    //Inclui tarefas novas na lista de prontos, a menos que a tarefa seja o dispatcher
    if(task != &dispatcher)
        queue_append((queue_t**) &readyQueue, (queue_t*) task);

    //Inicializa os valores de prioridade estática e dinâmica usadas no escalonamento
    task->prioridadeEstatica = 0;
    task->prioridadeDinamica = 0;

    task->tarefaUsuario = 1;                //Com valor 1 significa que a tarefa é de usuário
    task->contadorQuantum = QUANTUM;        //Inicializado o contador de quantum com o quantum predefinido
    task->tempoExecucao = systime();
    task->tempoProcessamento = 0;
    task->ativacoes = 0;

    return task->tid;
}

int task_switch (task_t *task)
{

    task_t *Aux = CurrentTask;              //O ponteiro 'CurrentTask' aponta para a tarefa recebida
    CurrentTask = task;

    //Código para depuração caso DEBUG esteja definido
    #ifdef DEBUG
    printf("task_switch: trocando contexto %d -> %d\n", Aux->tid, task->tid);
    #endif

    //Salva o contexto da tarefa anterior e aciona o contexto da tarefa recebida
    if(swapcontext(&Aux->context, &task->context) < 0)
        return -1;

    return 0;
}

void task_exit (int exitCode)
{
    //Código para depuração caso DEBUG esteja definido
    #ifdef DEBUG
    printf("task_exit: tarefa %d sendo encerrada\n", CurrentTask->tid);
    #endif

    //Caso a tarefa atual não seja nem a main nem o dispatcher, remove da fila de prontos e...
    //...troca para a tarefa dispatcher
    if(CurrentTask != &dispatcher && CurrentTask != &MainTask){
        queue_remove((queue_t**) &readyQueue, (queue_t*) CurrentTask);
        CurrentTask->tempoExecucao = systime() - CurrentTask->tempoExecucao;
        printf("Task: %d exit: running time %d ms, cpu time %d ms, %d activations\n",
                CurrentTask->tid, CurrentTask->tempoExecucao, CurrentTask->tempoProcessamento, CurrentTask->ativacoes);
        task_switch(&dispatcher);
    }
    //Caso a tarefa a ser finalizada seja o dispatcher, troca para a tarefa main
    else{
        dispatcher.tempoExecucao = systime() - dispatcher.tempoExecucao;
        printf("Task: %d exit: running time %d ms, cpu time %d ms, %d activations\n",
                 dispatcher.tid, dispatcher.tempoExecucao,  dispatcher.tempoProcessamento, dispatcher.ativacoes);
        task_switch(&MainTask);
    }
}

int task_id ()
{
    return CurrentTask->tid;                //Retorna o identificador da tarefa que está em andamento
}

void task_yield ()
{
    #ifdef DEBUG
    printf("task_yield. Tarefa atual: %d. Trocando para dispatcher\n", CurrentTask->tid);
    #endif

    //Caso a tarefa atual não seja nem a tarefa main nem o dispatcher, exclui o elemento da lista...
    //..e o reposiciona no final da lista
    if(CurrentTask != &MainTask && CurrentTask != &dispatcher){
        queue_remove((queue_t**) &readyQueue, (queue_t*) CurrentTask);
        queue_append((queue_t**) &readyQueue, (queue_t*) CurrentTask);
    }
    dispatcher.ativacoes++;                 //Atualiza a quantidade de ativações do dispatcher
    task_switch(&dispatcher);               //Troca para o dispatcher
}

void task_suspend (task_t *task, task_t **queue)
{
    if(queue && task){
        queue_t * aux = (queue_t*) task;
        //Nó é retirado caso ja pertença a alguma fila
        if(aux->next != NULL && aux->prev != NULL){
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;
        }
        //Adiciona o nó na fila de prontos
        queue_append((queue_t**) &queue, aux);
    }
}

void task_resume (task_t *task)
{
    queue_t * aux = (queue_t*) task;
    //Nó é retirado caso ja pertença a alguma fila
    if(aux->next != NULL && aux->prev != NULL){
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;
    }
    //Adiciona o nó na fila de prontos
    queue_append((queue_t**) &readyQueue, aux);
}

void task_setprio (task_t *task, int prio)
{
    #ifdef DEBUG
    printf("task_setprio. Escrevendo o valor de prioridade da tarefa: %d, para %d\n", task->tid, prio);
    #endif

    //Caso o valor 'prio' recebido esteja entre -20 e 20, ou seja cumpre com os valores de prioridade padrões no estilo UNIX
    if(prio >= -20 && prio <= 20){
        //Caso a tarefa recebida seja nula, a tarefa atual recebe os valores de prioridade
        if(!task){
            CurrentTask->prioridadeEstatica = prio;
            CurrentTask->prioridadeDinamica = prio;
        }
        //Caso contrário, a tarefa recebida recebe os valores de prioridade
        else{
            task->prioridadeEstatica = prio;
            task->prioridadeDinamica = prio;
        }
    }
    //Se o valor de 'prio' esteja além dos limites dos valores de prioridade do padrão UNIX, mostra a seguinte mensagem de erro
    else
        printf("ERRO: prioridade está fora do limite");
}

int task_getprio (task_t *task)
{
    //Caso a tarefa seja nula, retorna o valor da prioridade estática da tarefa atual
    if(!task)
        return CurrentTask->prioridadeEstatica;
    //Caso contrário, retrona o valor da prioridade estática da tarefa recebida
    return task->prioridadeEstatica;
}

unsigned int systime ()
{
    return tempoAtual;                      //Retorna o tempo atual de execução do programa
}


















