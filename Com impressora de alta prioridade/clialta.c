#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#define MAX 4

int main (int argc, char* argv[]){
    
    int s;

    //abro o pipeline para escrita
    s = open ("/tmp/impr", O_WRONLY);
    if (s == -1){
        perror("Erro na abertura do fifo!");
        exit(-1); //ou exit(erron)
    }

    if (argc != 2){
        printf("Número de argumentos inválido! \n");
        exit(1);
    }

    if (strlen(argv[1]) != MAX){
        printf("\n O nome do ficheiro só pode ter 4 carateres! \n");
        exit(1);
    }

    if (strlen(argv[2]) != 1){
        printf("\n O nome da opção só pode ter 1 carater! \n");
        exit(1);
    }

    write(s, argv[1], strlen(argv[1]));
    write(s, argv[2], strlen(argv[2]));

    return(0);
}