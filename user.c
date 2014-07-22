/* 
Aplicação usuário para comunicação com um módulo de kernel que  cria um 
device em /dev/primo e uma entrada em /proc/primo. Recebe como entrada no
/dev/primo um valor i e encontra o i-esimo numero primo referente a este valor.
Imprimindo o i-ésimo primo no log do kernel e imprimindo uma lista de números
primos obtidos até o i-ésimo primo na entrada /proc/primo "
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]){

	/*Definição da permissão de escrita*/
    system("mknod /dev/primo c 60 0");
    system("chmod 666 /dev/primo");

    printf("Aplicação usuário de comunicação com módulo de Kernel\n");

    while(1){
        printf("Insira um valor i:\n");

        /* Lê um número */
        int numero = 0;
        scanf("%d", &numero);

        /* Comando a ser executado*/
	   	char command[50];

	   	/* Concatenação */
	   	sprintf(command, "echo %d > /dev/primo", numero); 
		
	   	/* Chamada de sistema para escrever um valor no /dev/primo */
		system(command);

		printf("Saída do /proc/primo: \n");

		/* Chamada de sistema para imprimir a saída do /proc/primo */
		system("cat /proc/primo");

		printf("\n");
    }
    return 0;
}
