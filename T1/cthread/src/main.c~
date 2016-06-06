
/*
 *	Teste da implementação de libcthread
 *
 *	Versão 1.0 - 26/04/2016
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 *  Aluno: Tiago Mazzarollo
 */

#include "../include/fila2.h"
#include "../include/cthread.h"
#include <stdio.h>

// void* func0(void *arg) {
// 	int i=0;
// 	while(1){
// 		printf("F0: %d\n", i);
// 		i++;
// 		sleep(1);
// 	}
// 	return;
// }
// 
// void* func1(void *arg) {
// 	int i=0;
// 	while(1){
// 		printf("--->F1: %d\n", i);
// 		i++;
// 		sleep(1);
// 	}
// }

void* even(void *arg) {
  int i;
  for (i =0; i <=10; i=i+2 ) {
    printf("%3d\n", i);
	  sleep(1);
		cyield();
   }
}

void* odd(void *arg) {
  int i;
  for (i =1; i <=10; i=i+2 ) {
  	printf("%3d\n", i);
    sleep(1);
		cyield();
  }
}

int main(int argc, char *argv[]) {
	int	id0, id1;
	int i;

	id0 = ccreate(even, (void *)&i);
	id1 = ccreate( odd, (void *)&i);

	printf("Eu sou a main apos a criacao de ID0-%d e ID1-%d\n",id0,id1);

	cexec();
	// cexec(id1);

//	cjoin(id0);
//	cjoin(id1);

	printf("Eu sou a main voltando para terminar o programa\n");
}
