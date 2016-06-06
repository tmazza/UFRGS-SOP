#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *montaLista(char *file);
int buscaMaior(int *lista,int n,int i, int j,char op);
int getMaior(int *lista,int i,int j);
int getMenor(int *lista,int i,int j);
int maiorDosFilhos(int *lista,int n,int i, int j, char op);

int level;
int tamLista = 0;

int main(int argc, char* argv[]) {
    int *lista;
    char op = 'M';
    int n = 1;
    char *file;

    int i = 0;
    char *numLevels;
    while(i < argc){
      if(strcmp(argv[i],"-f")==0){
        file = argv[i+1];
      }
      if(strcmp(argv[i],"-l")==0){
        numLevels = argv[i+1];
        n = atoi(numLevels);
      }
      if(strcmp(argv[i],"-m")==0){
        op = 'm';
      }
      if(strcmp(argv[i],"-M")==0){
        op = 'M';
      }
      i++;
    }

    level = n+1;
    lista = montaLista(file);

    int valor = buscaMaior(lista,n,0,tamLista,op);
    if(op == 'm'){
      printf("MENOR %d\n", valor);
    } else {
      printf("MAIOR %d\n", valor);
    }

    return 0;
}

int *montaLista(char *file){
  int i=0;
  int num;
  int *lista;

  FILE *fp = fopen(file,"r");
  // Conta total de elementos
  while(!feof(fp)){ char ch = fgetc(fp); if(ch == '\n'){ tamLista++; } }
  // Cria lista com tamanho necessario
  lista = (int *) malloc(tamLista * sizeof(int));
  rewind(fp);
  // Cria lista de inteiros
  while(fscanf(fp, "%d", &num) > 0) { lista[i] = num; i++; }
  fclose(fp);

  return lista;
}

int buscaMaior(int *lista,int n,int i, int j, char op){
  if(n==1){
    if(op == 'm'){
      return getMenor(lista,i,j);
    } else if(op == 'M'){
      return getMaior(lista,i,j);
    }
  } else {
    n = n-1;
    return maiorDosFilhos(lista,n,i,j,op);
  }
  return -1;
}

int getMenor(int *lista,int i,int j){
  int k;
  int menor = 128;
  for(k = i; k < j;k++){
    if(lista[k] < menor){
      menor = lista[k];
    }
  }
  return menor;
}

int getMaior(int *lista,int i,int j){
  int k;
  int maior = 0;
  for(k = i; k < j;k++){
    if(lista[k] > maior){
      maior = lista[k];
    }
  }
  return maior;
}

int maiorDosFilhos(int *lista,int n,int i, int j,char op){
  int retorno;
  pid_t pid1;
  pid_t pid2;

  // fork lista esquerda
  pid1 = fork();
  if (pid1 == 0) {
    printf("PROC %d LEVEL %d\n",getpid(),level-n);
    j = j/2;
    exit(buscaMaior(lista,n,i,j,op));
  } else {
    waitpid(pid1,&retorno,0);
    int a = WEXITSTATUS(retorno);
    // fork lista direita
    pid2 = fork();
    if (pid2 == 0) {
      printf("PROC %d LEVEL %d\n",getpid(),level-n);
      i = j/2;
      exit(buscaMaior(lista,n,i,j,op));
    } else {
      waitpid(pid2,&retorno,0);
      int b = WEXITSTATUS(retorno);
      if(op == 'm'){
        if(a < b){
          return a;
        } else {
          return b;
        }
      } else if(op == 'M'){
        if(a > b){
          return a;
        } else {
          return b;
        }
      }
    }
  }
  return -1;
}
