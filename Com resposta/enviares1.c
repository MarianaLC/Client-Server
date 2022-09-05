#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string.h>
#define MAX 4

int main (int argc, char* argv[]){

	char buf[5];
    char resposta[25];
    int s, n, r;

    if (argc != 2){
        printf("Número de argumentos inválido! \n");
        exit(1);
    }
	
	s = open ("/tmp/impr", O_RDONLY);
    if (s == -1){
        perror("Erro na abertura do fifo!");
        exit(-1); //ou exit(erron)
    }

    r = open ("/tmp/resp", O_WRONLY);
    if (r == -1){
        perror("Erro na abertura do fifo de resposta ser!");
        exit(-1); //ou exit(erron)
    }

    if (strlen(argv[1]) != 9){
        printf("\n O path e nome do terminal só pode ter 9 carateres! \n");
        exit(1);
    }

    memset(buf, 0, sizeof(buf));
    memset(resposta, 0, sizeof(resposta));
    while(1){
        while ((n=read(s, buf, MAX))>0){
          
            if(!fork()){
                execlp("cp","cp", buf, argv[1], NULL);
            }
            wait(NULL); 
            strcpy(resposta, "oiiiii tudo beem");
            write(r, resposta, strlen(resposta));
            memset(buf, 0, sizeof(buf));
            memset(resposta, 0, sizeof(resposta));
        }
    }
}

