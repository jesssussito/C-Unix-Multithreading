#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>

int sem1id,sem2id = -1;
int pidP;
int numHijos = 4; 

void manejar_sigint (int sig);
int main() { 
    printf("====================================\n");
    printf("          EJERCICIO 2\n");
    printf("====================================\n");
    signal(SIGINT, manejar_sigint);
    int num,ult,som;
    pidP = getpid();
    sem1id = semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    sem2id = semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    if (sem1id == -1 || sem2id == -1)
    {
        perror ("ERROR");
        exit(EXIT_FAILURE);
    }

    if (semctl(sem1id, 0, SETVAL, 2) == -1 || semctl(sem2id, 0, SETVAL, 1) == -1)
    {
        perror("ERROR"); 
        exit(EXIT_FAILURE);
    }

    struct sembuf semaforo;
    semaforo.sem_num=0;
    semaforo.sem_op=-1;
    semaforo.sem_flg=0;
    struct sembuf pantalla;
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
            system("clear");
            for(int j=0;j<3;j++){
                printf ("Proceso con intenciones de entrar: %d\n", getpid());
                sleep(1);
                //num=getpid();
                //ult=num%10;
                //som = (ult/2)+1;
                //sleep(som);
                //fflush(stdout);
                semaforo.sem_op=-1;
                if (semop(sem1id,&semaforo,1) == -1)
                {
                    perror("ERROR entrando\n"); 
                    exit(EXIT_FAILURE);
                }

                printf ("Proceso dentro de seccion %d\n", getpid());
                fflush(stdout);
                //Dentro de la seccion
                pantalla.sem_op=-1;
                if (semop(sem2id,&pantalla,1) == -1)
                {
                    perror("ERROR entrando a pantalla\n"); 
                    exit(EXIT_FAILURE);
                }
                printf ("Proceso dentro de pantalla %d\n", getpid());
                sleep(1);
                fflush(stdout);
                //num=getpid();
                //ult=num%10;
                //system("tput cup %d 3",ult);
                char command[50];
                char blanco[70];
                //sprintf(command, "tput cup %d 3", ult);
                //system(command);
                //som = (ult/2)+1;
                //sleep(som);
                //sprintf(blanco, "tput cup %d 3", ult);
                //system(blanco);
                pantalla.sem_op=1;
                if (semop(sem2id,&pantalla,1) == -1)
                {
                    perror("ERROR saliendo de la pantañña\n"); 
                    exit(EXIT_FAILURE);
                }
                semaforo.sem_op=1;
                if (semop(sem1id,&semaforo,1) == -1)
                {
                    perror("ERROR saliendo de la seccion\n"); 
                    exit(EXIT_FAILURE);
                }
                //num=getpid();
                //ult=num%10;
                //som = (ult/2)+1;
                //sleep(som);
                printf ("Proceso que salió de la seccion: %d\n", getpid());
                sleep(2);
                fflush(stdout);
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

void manejar_sigint (int sig)
{
    if(getpid()==pidP){
        for (int i = 0; i < numHijos; i++) { 
            wait(NULL); 
        } 
        printf ("Saliendo programa\n");
        if (sem1id != -1 && sem2id !=-1)
        {   
            if (semctl(sem1id, 0, IPC_RMID) == -1 || semctl(sem2id, 0, IPC_RMID) == -1)
            {
                perror ("ERROR en la liberacion de semaforos\n");
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
    }
    
    
    exit(0);
}