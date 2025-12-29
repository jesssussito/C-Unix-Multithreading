#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>

int main() { 
    printf("====================================\n");
    printf("          EJERCICIO 1\n");
    printf("====================================\n");
    int numHijos = 4; 
    int num,ult,som;
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    if (semid == -1)
    {
        perror ("ERROR");
        exit(EXIT_FAILURE);
    }

    if (semctl(semid, 0, SETVAL, 2) == -1)
    {
        perror("ERROR"); 
        exit(EXIT_FAILURE);
    }

    struct sembuf semaforo;
    semaforo.sem_num=0;
    semaforo.sem_op=-1;
    semaforo.sem_flg=0;
    

    for (int i = 0; i < numHijos; i++) { 
        pid_t pid = fork(); 
        if (pid < 0) { 
            perror("ERROR en la creacion de procesos\n"); 
            exit(EXIT_FAILURE); 
        }    
        if (pid==0) { 
            for(int j=0;j<3;j++){
                printf ("Proceso con intenciones de entrar: %d\n", getpid());
                fflush(stdout);
                sleep(2);
                printf("\n");
                semaforo.sem_op=-1;
                if (semop(semid,&semaforo,1) == -1)
                {
                    perror("ERROR entrando\n"); 
                    exit(EXIT_FAILURE);
                }

                printf ("Proceso dentro de seccion %d\n", getpid());
                fflush(stdout);
                sleep(1);
                printf("\n");
                semaforo.sem_op=1;
                if (semop(semid,&semaforo,1) == -1)
                {
                    perror("ERROR saliendo de la seccion\n"); 
                    exit(EXIT_FAILURE);
                }
                printf ("Proceso que salió de la seccion: %d\n", getpid());
                fflush(stdout);
                sleep(2);
                printf("\n");
            }
            exit(0); // Termina el proceso hijo 
        }
    } 
 
// Código del padre: Espera a que terminen todos los hijos 
    for (int i = 0; i < numHijos; i++) { 
        wait(NULL); 
    } 
printf("Todos los procesos hijos han terminado. Padre finaliza.\n"); 
return 0; 
}
