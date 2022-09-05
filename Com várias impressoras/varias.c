#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define NUM 4

int main (int argc, char* argv[]){

    int s, n = 0, i, f, t, m, stat;
    int imp_usada = 0;
    int imp_comeca_procura = 0;
    int num_impressoras = argc - 1;
    char nome[5]; 
    char ficheiro[1024];
    int estado[num_impressoras]; //o estado para o pai
    
    if (argc != 4){
        printf("Número de argumentos inválido! \n");
        exit(1);
    }
	
    if (strlen(argv[1]) != 9 || strlen(argv[2]) != 9 || strlen(argv[3]) != 9){
        printf("\n O path e nome do terminal só pode ter 9 carateres! \n");
        exit(1);
    }

    s = open ("/tmp/impr", O_RDONLY);
    if (s == -1){
        perror("Erro na abertura do fifo!");
        exit(-1); //ou exit(erron)
    }

    //inicializar estado das impressoras a 0
    for (i=0; i<num_impressoras; i++)
        estado[i] = 0;
    
    memset(nome, 0, sizeof(nome));
    while(1){

        //escolher a proxima impressora a usar
        for (i=imp_comeca_procura; i<(num_impressoras + n); i++){
            if (estado[i] == 0){
                imp_usada = i; //a impressora livre  
                break; 
            }
            else{
                imp_usada = -1; 
            }
        }

        //cheguei ao fim do ciclo, então n aumenta 1 unidade
        if (n == 2){
            n = 0;
            imp_comeca_procura = 0; 
        }
        else{
            n++;
            imp_comeca_procura = imp_usada + 1;
        }
        
        //se não tenho impressoras livres
        if (imp_usada == -1){
            wait(&stat); //espera que uma impressora fique livre

            if (WIFEXITED(stat)){ //se uma impressora terminal
                printf("Impressora que terminou: %d\n", WEXITSTATUS(stat));
                estado[WEXITSTATUS(stat)] = 0; //agora o estado da impressora que terminou é 0
            }
            else if (WIFSIGNALED(stat)) //se os filhos terminarem incorretamente
                psignal(WTERMSIG(stat), "O filho não terminou bem :(");

        }
        //se há uma impressora livre
        else{
            memset(nome, 0, sizeof(nome));

            while ((read(s, nome, NUM)) > 0){

                printf("impressora a usar %d \n", imp_usada);
                printf("conteudo nome %s \n", nome);

                //agora a impressora a usar vai ficar ocupada
                estado[imp_usada] = 1;

                //os meus ajudantes imprimem os ficheiros
                if (! fork()){
                    f = open(nome, O_RDONLY);
                    t = open(argv[imp_usada + 1], O_WRONLY);
       
                    while((m = read(f, ficheiro, sizeof(ficheiro)))>0)
                        write(t, ficheiro, m);
                    exit(imp_usada); //envia o exit, a avisar que agora já está livre
                }

                //se a impressora terminou, meto o estado dela a 0
                if ((wait(&stat)) > 0){

                    if (WIFEXITED(stat)){ //se uma impressora terminal
                        printf("Impressora que terminou agoraa: %d\n", WEXITSTATUS(stat));
                        estado[WEXITSTATUS(stat)] = 0; //agora o estado da impressora que terminou é 0
                        printf("o meu estado agora é %d \n", estado[WEXITSTATUS(stat)]);
                    }
                    else if (WIFSIGNALED(stat)) //se os filhos terminarem incorretamente
                        psignal(WTERMSIG(stat), "O filho não terminou bem :(");
                } 

                memset(nome, 0, sizeof(nome));
            }   
        } 
    }
}   

