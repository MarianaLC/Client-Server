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
    char ficheiro[1024];
    int s, n, f, printer;

    if (argc != 2){
        printf("Número de argumentos inválido! \n");
        exit(1);
    }
	
	s = open ("/tmp/bio", O_RDONLY);
    if (s == -1){
        perror("Erro na abertura do fifo!");
        exit(-1); //ou exit(erron)
    }

    printer = open(argv[1], O_RDWR);
    if (printer == -1){
        perror("Erro na abertura do terminal!");
        exit(-1); //ou exit(erron)
    }

    memset(ficheiro,0,sizeof(ficheiro));
    memset(buf,0,sizeof(buf));
    while(1){
        while((read(s, buf, MAX)) > 0){
            printf("conteudo buffer %s \n", buf);
            f = open(buf, O_RDONLY);
            if (f == -1){
                perror("Erro na abertura do ficheiro para imprimir!");
                exit(-1); //ou exit(erron)
            }

            while ((n = read(f, ficheiro, sizeof(ficheiro)))>0)
                write(printer, ficheiro, n);

            memset(ficheiro,0,sizeof(ficheiro));
            memset(buf,0,sizeof(buf));
        }
    }
    return(0);  
}


