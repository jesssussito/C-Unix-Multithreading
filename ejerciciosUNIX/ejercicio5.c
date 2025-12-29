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
struct mensaje{
    long tipo;
    char txt[100];
};
struct mensaje1{
    long tipo;
    int num;
};

int main(void){
    printf("====================================\n");
    printf("          EJERCICIO 5\n");
    printf("====================================\n");
    int msgid;
    struct mensaje msg;
    struct mensaje1 msg1;
    if(msgid=msgget(IPC_PRIVATE,0666|IPC_CREAT)==-1){
        perror("ERROR en la creacion del buzon\n"); 
        exit(EXIT_FAILURE);
    }
    int hijos;
    for(hijos = 1; hijos <= 2; hijos++)
    {
        switch (fork())
        {
        case -1:
            perror("ERROR en el fork\n"); 
            exit(EXIT_FAILURE);
            break;
        case 0:
            if(hijos == 1){
                if(msgrcv(msgid,&msg,sizeof(msg)-sizeof(long),1,0)!=-1){
                    printf("El hijo %d recibio el mensaje %s \n",getpid(),msg.txt);
                    exit(EXIT_SUCCESS);
                }
            }
            
            if(msgrcv(msgid,&msg1,sizeof(msg1)-sizeof(long),2,0)!=-1){
                printf("El hijo %d recibio el mensaje %d \n",getpid(),msg1.num);
                exit(EXIT_SUCCESS);
             }
            perror("ERROR en el recibimiento de mensajes\n"); 
            exit(EXIT_FAILURE);
            break;
        
        default:
            msg.tipo=1;
            strcpy(msg.txt,"Luke, yo soy tu padre");
            if(msgsnd(msgid,&msg,sizeof(msg)-sizeof(long),0)==-1){
                perror("ERROR en el envio del mensaje\n"); 
                exit(EXIT_FAILURE);
            }
            msg1.tipo=2;
            msg1.num=getpid();
            if(msgsnd(msgid,&msg1,sizeof(msg1)-sizeof(long),0)==-1){
                perror("ERROR en el envio del mensaje\n"); 
                exit(EXIT_FAILURE);
            }
            for(int i=0;i<2;i++){
                wait(NULL);
            }
            break;
        }
    }

}