/*
 *	Implmentação de libcthread
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
#include <malloc.h>
#include <ucontext.h>

#define CRIADO 0
#define APTO 1
#define EXECUTANDO 2
#define BLOQUEADO 3
#define FINALIZADO 4

typedef struct s_TCB {
  int tid;
  int state;
  ucontext_t context;
} TCB_t;

int tid = 0;                    // Incrementado antes de executar ccreate | zero reservado para thread main
int inicializado = 0;
int done = 0;

TCB_t *tMain;  // thread principal
TCB_t *tExec;  // thread em execução

FILA2 fAptos; // fila de aptos

/****Funções auxiliares****/
void init();                          // Cria fila
void incluiNaFilaDeAptos(TCB_t *tcb); // Inclui tcb no final da fila de aptos

int ccreate (void* (*start)(void*), void *arg) {
  init();

  char t_stack[SIGSTKSZ];
  ucontext_t *t_context;
  TCB_t *tcb;

  // Criação de contexto definido a função que será executada pela thread
  t_context = (ucontext_t *) malloc(sizeof(ucontext_t));
  getcontext(t_context);
  t_context->uc_link          = &tMain->context;
  t_context->uc_stack.ss_sp   = t_stack;
  t_context->uc_stack.ss_size = sizeof(t_stack);
  makecontext(t_context, (void (*)(void))start, 1, arg);

  // Criação TCB
  tid++;
  tcb          = (TCB_t *) malloc(sizeof(TCB_t));
  tcb->tid     = tid;
  tcb->state   = CRIADO;
  tcb->context = *t_context;

  incluiNaFilaDeAptos(tcb);

  return tid;
}

int cyield(void) {
  incluiNaFilaDeAptos(tExec);
  swapcontext(&tExec->context,&tMain->context);
  return -1;
}

// Outras funcoes
void cexec(){
  setcontext(&tMain->context);
}

void loop(){

  while(fAptos.it != NULL){

    // tExec = (TCB_t *) (fAptos.it)->node;
    // printf("-->%p\n", tExec);

    tExec = (TCB_t *) (fAptos.it)->node;

    tExec->state = EXECUTANDO;
    swapcontext(&tMain->context,&tExec->context);


    printf("-->%p\n", fAptos.it);
    printf("-->%p\n", (fAptos.it)->next);

  }
  return;
}

void init(){
  if(!inicializado){
    inicializado = 1;
    CreateFila2(&fAptos);

    // Contexto para thread main
    char stack[SIGSTKSZ];
    ucontext_t *main_context;
    main_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext(main_context);
    main_context->uc_link           = NULL;
    main_context->uc_stack.ss_sp    = stack;
    main_context->uc_stack.ss_size  = sizeof(stack);
    makecontext(main_context,loop,0);

    // TCB para thread main
    tMain          = (TCB_t *) malloc(sizeof(TCB_t));
    tMain->tid     = 0;
    tMain->state   = CRIADO;
    tMain->context = *main_context;

  }
}

void incluiNaFilaDeAptos(TCB_t *tcb){
  tcb->state = APTO;
  AppendFila2(&fAptos,tcb);
}

/// TODO
int cjoin(int tid) { return -1;}
int csem_init(csem_t *sem, int count) { return -1; }
int cwait(csem_t *sem) { return -1; }
int csignal(csem_t *sem) { return -1; }
