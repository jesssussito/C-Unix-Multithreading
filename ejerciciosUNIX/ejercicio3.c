#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
//ejemplo tuberias
int descript[2];
//pipe da => O_RONLY  y O_WONLY
int main(void){
    printf("====================================\n");
    printf("          EJERCICIO 3\n");
    printf("====================================\n");
        if(pipe(descript)==-1){
            perror("ERROR en la creacion de la tuberia\n"); 
            exit(EXIT_FAILURE); 
        }
        pid_t pid = fork(); 
        if (pid < 0) { 
            perror("ERROR en la creacion de procesos\n"); 
            exit(EXIT_FAILURE); 
        }   
        if (pid==0) { 
            //hijo
            close(descript[1]);
            pid_t msg;
            read(descript[0],&msg,sizeof(pid_t));
            printf("He recibido: %d \n",msg);
            close(descript[0]);
            exit(EXIT_SUCCESS);

        }
        else{
            //padre
            close(descript[0]);
            pid_t pidP = getpid();
            printf("Mi pid es: %d\n",pidP);
            write(descript[1],&pidP,sizeof(pid_t));
            close(descript[1]);
            wait(NULL);
        } 
        

}