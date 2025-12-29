#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
//punto de sincronizacion : 
//buzon de mensaje y otro lee y se obtiene
// con semaforos: P1 y P2 S1=2
//  wait()  wait() | lurgo wait(0)
//inicializas semaforo con num procesos cada proceso hace wait y luego wait(0) y se sincronizan
int sem1id = -1;
int pidP;
int main(void){
    printf("====================================\n");
    printf("          EJERCICIO 6\n");
    printf("====================================\n");
    pidP = getpid();
    sem1id = semget(IPC_PRIVATE, 2, IPC_CREAT|0600);
    if (sem1id == -1 )
    {
        perror ("ERROR");
        exit(EXIT_FAILURE);
    }

    if (semctl(sem1id, 0, SETVAL, 0) == -1 || semctl(sem1id, 1, SETVAL, 1) == -1)//aqui hago 2 semaforos en un array y sincornizo con sus valores
    {
        perror("ERROR"); 
        exit(EXIT_FAILURE);
    }

    struct sembuf w0;
    w0.sem_num=0;
    w0.sem_op=-1;
    w0.sem_flg=0;
    struct sembuf s0;
    s0.sem_num=0;
    s0.sem_op=1;
    s0.sem_flg=0;
    //con sem num le digo semaforo 2
    struct sembuf w1;
    w1.sem_num=1;
    w1.sem_op=-1;
    w1.sem_flg=0;
    struct sembuf s1;
    s1.sem_num=1;
    s1.sem_op=1;
    s1.sem_flg=0;
    switch (fork())
    {
    case -1:
        perror("Error");
        exit(EXIT_FAILURE);
    case 0:
        for (int i = 0; i < 100; i++)
        {
            if (semop(sem1id,&w0,1) == -1)
                {
                    perror("ERROR caso 1\n"); 
                    exit(EXIT_FAILURE);
                }
            printf("0");
            fflush(stdout);
            if (semop(sem1id,&s1,1) == -1)
            {
                perror("ERROR caso 2\n"); 
                exit(EXIT_FAILURE);
            }

        }
        exit(0);
        
        
    
    default:
        for (int i = 0; i < 100; i++)
        {
            if (semop(sem1id,&w1,1) == -1)
                {
                    perror("ERROR caso 3\n"); 
                    exit(EXIT_FAILURE);
                }
            printf("1");
            fflush(stdout);
            if (semop(sem1id,&s0,1) == -1)
                {
                    perror("ERROR caso 1\n"); 
                    exit(EXIT_FAILURE);
                }
        }
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    if (semctl(sem1id, 0, IPC_RMID) == -1 )
    {
        perror ("ERROR en la liberacion de semaforos\n");
        exit(EXIT_FAILURE);
    }
    exit(0);
}