#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>

int tuberia[10][2];

int main(void){
    printf("====================================\n");
    printf("          EJERCICIO 4\n");
    printf("====================================\n");
    int hijos;
    for(hijos = 0; hijos < 10; hijos++)
    {
        if(pipe(tuberia[hijos])==-1){
            perror("ERROR en la creacion de procesos\n"); 
            exit(EXIT_FAILURE); 
        }
    
       switch(fork()) {
          case -1: perror("fork");
                   exit(EXIT_FAILURE);
          case 0: 
                    //hijo
                    close(tuberia[hijos][1]);
                    pid_t datoP;    
                    if(read(tuberia[hijos][0], &datoP, sizeof(pid_t)) == -1)
                    {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stdout, "Soy el hijo %d y mi padre es %d.\n", getpid(), datoP); 
                    close(tuberia[hijos][0]);
                    exit(EXIT_SUCCESS);
          default: 
                    
                    pid_t pidP = getpid();
                    //printf("Soy el padre con pid:%d\n",pidP);

                    close(tuberia[hijos][0]);
                    if(write(tuberia[hijos][1], &pidP, sizeof(pid_t)) == -1)
                    {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                    close(tuberia[hijos][1]);
                    for (int i = 0; i < 10; i++)
                    {
                        wait(NULL);
                    }
                    
          }
    }
    for(int i=0;i<10;i++){
        wait(NULL);
    }
    return 0;
}