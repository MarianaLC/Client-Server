#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define NUM 5

int main (int argc, char* argv[]){

    int s;
    char nome[5]; 
    char opcao[2]; 
    
    if (argc != 3){
        printf("Número de argumentos inválido! \n");
        exit(1);
    }
	
    if (strlen(argv[1]) != 9 || strlen(argv[2]) != 9){
        printf("\n O path e nome do terminal só pode ter 9 carateres! \n");
        exit(1);
    }

    memset(nome, 0, sizeof(nome));
    memset(opcao, 0, sizeof(opcao));

    s = open ("/tmp/impr", O_RDONLY);
    if (s == -1){
        perror("Erro na abertura do fifo!");
        exit(-1); //ou exit(erron)
    }

    while(1){
        
        while ((read(s, nome, NUM))>0 && (read(s, opcao, 1))>0){

            printf("conteudo nome %s \n", nome);
            printf("conteudo opcao %s \n", opcao);
            
            if (strcmp(opcao, "1") == 0) {
                if (! fork()){
                    printf("aqui argv[2] %s \n", argv[2]);
                    execlp("cp","cp", nome, argv[2], NULL);
                }
                wait(NULL);
                memset(nome, 0, sizeof(nome));
                memset(opcao, 0, sizeof(opcao));
            }

            else{
                if (! fork()){
                    printf("acola argv[1] %s \n", argv[1]);
                    execlp("cp","cp", nome, argv[1], NULL);
                }
                wait(NULL);
                memset(nome, 0, sizeof(nome));
                memset(opcao, 0, sizeof(opcao));
            }
        }
    }
}
    
