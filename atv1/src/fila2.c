#include <malloc.h>
#include "../include/fila2.h"

int	CreateFila2(PFILA2 pFila) {
  pFila->it    = NULL;
  pFila->first = NULL;
  pFila->last  = NULL;
  return 0;
}

int	FirstFila2(PFILA2 pFila){
    if(pFila->first == NULL){
      return -1;
    } else {
      pFila->it = pFila->first;
      return 0;
    }
}

int	LastFila2(PFILA2 pFila){
  if(pFila->last == NULL){
    return -1;
  } else {
    pFila->it = pFila->last;
    return 0;
  }
}

int NextFila2(PFILA2 pFila){
  if(pFila->it == NULL || (pFila->it)->next == NULL){
    return -1;
  } else {
    pFila->it = (pFila->it)->next;
    return 0;
  }
}


/**
Retorna o ponteiro armazenado no conteúdo do item endereçado pelo iterador
da fila. É o elemento “nodo” da estrutura “sFilaNodo2”
 */
void *GetAtIteratorFila2(PFILA2 pFila){
    return (pFila->it)->node;
}

// Salva o ponteiro “content” em um novo item e coloca-o no final da fila
// “pFila”. Requer alocação dinâmica da estrutura “sFilaNodo2”.
int	AppendFila2(PFILA2 pFila, void *content){
  NODE2 *NN; //Novo Nodo
  NODE2 *last; //
  NN = (NODE2 *) malloc(sizeof(NODE2));
  NN->node = (void *) content;
  NN->next = NULL;
  if(pFila->first == NULL){
    pFila->first = NN;
    pFila->last = NN;
    pFila->it = NN; // Inicia o iterador???
    NN->ant = NULL;
  } else {
    last = pFila->last;
    pFila->last = NN;
    NN->ant = last;
    last->next = NN;
  }
  return 0;
}

// Coloca o ponteiro “content” em um novo item e coloca-o logo após o item
// apontado pelo iterador da fila “pFila”. Requer alocação dinâmica dessa
// estrutura “sFilaNodo2”.
int	InsertAfterIteratorFila2(PFILA2 pFila, void *content){
  NODE2 *NN; //Novo Nodo
  NODE2 *oldNext;
  //NODE2 *oldPrev;
  NN = (NODE2 *) malloc(sizeof(NODE2));
  NN->node = (void *) content;
  if(pFila->first == NULL){
    pFila->it = NN;
    pFila->first = NN;
    pFila->last = NN;
    NN->ant = NULL;
    NN->next = NULL;
  } else {
    oldNext = pFila->it->next;
    pFila->it->next = NN;
    NN->ant = pFila->it;
    NN->next = oldNext;

    if(oldNext == NULL){
      pFila->last = NN;
    } else {
      //oldPrev = oldNext->ant;
      oldNext->ant = NN;
    }

  }
  return 0;
}

// Remove da fila “pFila” o item indicado pelo iterador e libera a memória
// correspondente à estrutura “sFilaNodo2”. Ao final da função, o iterador
// deverá estar apontando para o elemento seguinte àquele que foi apagado. Vai
// receber NULL se a fila ficar vazia.
int	DeleteAtIteratorFila2(PFILA2 pFila){
    NODE2 *ant;
    NODE2 *nxt;
    if(pFila->first == NULL){
      return -1;
    }
    ant = pFila->it->ant;
    nxt = pFila->it->next;
    if(ant != NULL){
      ant->next = pFila->it->next;
    }
    if(nxt != NULL){
      nxt->ant = pFila->it->ant;
    }

    free(pFila->it);

    if(pFila->last == pFila->it){
        if(ant == NULL){
          pFila->last = NULL;
        } else {
          pFila->last = ant;
        }
    }
    if(pFila->first == pFila->it){
        if(nxt == NULL){
          pFila->first = NULL;
        } else {
          pFila->first = nxt;
        }
    }
    if(nxt == NULL){
      if(ant == NULL){
        pFila->it = NULL;
      } else {
        pFila->it = ant;
      }
    } else {
      pFila->it = nxt;
    }
    return 0;
}
