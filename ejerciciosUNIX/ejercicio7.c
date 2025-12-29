#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>
#define SHM_SIZE 100
int main(void){
    printf("====================================\n");
    printf("          EJERCICIO 7\n");
    printf("====================================\n");
    int shmid=shmget(IPC_PRIVATE,1024,IPC_CREAT | 0666);
    if(shmid ==-1){
        perror("Error\n");
        exit(EXIT_FAILURE);
    }
    char *shm_ptr = (char *)shmat(shmid,NULL,0);
        if(shm_ptr == (char *)-1){
            perror("Error en shmat (hijo)\n");
            exit(EXIT_FAILURE);
        }
    switch (fork())
    {
    case -1:
        perror("Error");
        exit(EXIT_FAILURE);
    case 0:
    //puntero a la memoria compartida
        char *shm_ptrH = (char *)shmat(shmid,NULL,0);
       
        snprintf(shm_ptrH,SHM_SIZE,"Te estoy mandando el mensaje");
        printf("[Hijo] Mensaje mandado\n");
        shmdt(shm_ptr);
        exit(0);
        
        
    
    default:
        sleep(5);
        char *shm_ptrP = (char *)shmat(shmid,NULL,0);
       
        printf("[Padre] Mensaje recibido :\n%s",shm_ptrP);
        exit(0);
    }
    if(shmctl(shmid,IPC_RMID,NULL)==-1){
        perror("Error en la liberacion de mem compartida\n");
        exit(EXIT_FAILURE);
    }
    
    wait(NULL);
    wait(NULL);
    exit(0);
}