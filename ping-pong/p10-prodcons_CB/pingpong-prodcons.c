#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pingpong.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

task_t prod1, prod2, prod3, cons1, cons2;
semaphore_t s_buffer, s_item, s_vaga;
int item;
int buffer[5] = {-1, -1, -1, -1, -1};

void produtor(void *arg)
{
    while(1){
        task_sleep(1);
        item = rand() % 100;

        sem_down(&s_vaga);
        sem_down(&s_buffer);

        int cont = 0;
        while(buffer[cont] != -1 && cont < 5){
            cont++;
        }

        if(cont < 5){
            buffer[cont] = item;

        }

        sem_up(&s_buffer);
        sem_up(&s_item);
        printf("%s produziu %d\n", (char*)arg, item);
    }
    task_exit(0);
}

void consumidor (void *arg)
{
    while(1){
        sem_down(&s_item);

        sem_down(&s_buffer);
        item = buffer[0];

        for(int i = 0; i < 4; i++){
            buffer[i] = buffer[i + 1];
        }
        buffer[4] = -1;

        sem_up(&s_buffer);
        sem_up(&s_vaga);
        printf("%s consumiu %d\n", (char*)arg, item);
        task_sleep(1);
    }
    task_exit(0);
}

int main (int argc, char *argv[])
{
    printf("Main INICIO\n");
    srand(time(0));
    pingpong_init();

    sem_create(&s_buffer, 1);
    sem_create(&s_item, 0);
    sem_create(&s_vaga, 5);

    task_create(&prod1, produtor, "p1");
    task_create(&prod2, produtor, "p2");
    task_create(&prod3, produtor, "p3");

    task_create(&cons1, consumidor, "                    c1");
    task_create(&cons2, consumidor, "                    c2");


    printf("Main FIM\n");
    task_exit(0);

    exit(0);
}


















