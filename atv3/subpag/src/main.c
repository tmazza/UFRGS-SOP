#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define FIFO 0
#define LRU 1
#define SEGCHAN 2
#define SEGCHAN2 3


// Valores alterados por parâmetro
long int P = 8; // Quantidades de páginas do processo
long int lenSR = 20; // Seq. de pags usadas pelo processo
long int Q = 3; // Quantidade mem RAM

long int *SR; // Vetor com lenSR elementos
short int *operacaoSR; // Vetor com lenSR elementos
long int *M; // Representa RAM com Q quadros
short int *naMemoria; // Usando para ver se página esta em M
short int *bitRef; // ??
short int *bitMod; // ??

long int espacoLivre; // Conta a quantidade quadros disponíveis na memoria

int *fifo; // Vetor de tamanho Q com inclusão e remoção tratadas como um FIFO
int fifoStart = 0;
int fifoEnd = -1;

int *lru; // Vetor de tamanho Q com inclusão e remoção tratadas como um LRU
int lruStart = 0;
int lruEnd = -1;

int *segChan; // Vetor de tamanho Q com inclusão e remoção tratadas como um Segunda chance
int segChanStart = 0;
int segChanEnd = -1;

int pageIn[4] = { 0,0,0,0 };
int pageOut[4] = { 0,0,0,0 };
int miss[4] = { 0,0,0,0 };

void init();
void esvaziaTabelaMemoria();
void addPagFirstPosFree(long int pag, int algoritmo);
void aplicaTrocaFIFO(long int carregada);
void aplicaTrocaLRU(long int carregada);
void aplicaTrocaSegChan(long int carregada);
// Controle da FIFO
void addEmFifo(int indice);
int remDaFifo();
// Controle da LRU
void addEmLru(int indice);
int remDaLru();
void moveParaFinalFila(long int pagina);
// Controle de Segunda chance
void addEmSegChan(int indice);
void remDaSegChan(int indice);
int getProxSegChan();

void printM(){
	int i;
	for(i=0;i<Q;i++){ printf(" %li",M[i]); }
	//printf("\n");
}

int main(int argc, char* argv[]) {
	int i;
	// TODO: ler parametros... para P,lenSR e Q
	init();

	/** FIFO **/
	for(i=0;i<lenSR;i++){
		if(!naMemoria[SR[i]]){ // Se pag não está na memoria
			miss[FIFO]++;
			if(espacoLivre > 0){
				addPagFirstPosFree(SR[i],FIFO);
			} else {
				aplicaTrocaFIFO(SR[i]);	// Decidi qual a página vítima e troca página
			}
		}
		if(operacaoSR[i] > 12){ 	// Se operação é de escrita seta bit modificação
			bitMod[SR[i]] = 1;
		}
	}
	esvaziaTabelaMemoria();

	/** LRU **/
	for(i=0;i<lenSR;i++){
		if(!naMemoria[SR[i]]){ // Se pag não está na memoria
			miss[LRU]++;
			if(espacoLivre > 0){
				addPagFirstPosFree(SR[i],LRU);
			} else {
				aplicaTrocaLRU(SR[i]);	// Decidi qual a página vítima e faz a troca
			}
		}
		if(operacaoSR[i] > 12){ 	// Se operação é de escrita seta bit modificação
			bitMod[SR[i]] = 1;
		}
		moveParaFinalFila(SR[i]);
	}
	esvaziaTabelaMemoria();

	/** Algoritmo segunda chance **/
	for(i=0;i<lenSR;i++){
		printf("%li: ", SR[i]);
		printM();
		if(naMemoria[SR[i]]){ // Se pag não está na memoria
			bitRef[SR[i]]=1;
		} else {
		 	miss[SEGCHAN]++;
		 	if(espacoLivre > 0){
		 		addPagFirstPosFree(SR[i],SEGCHAN);
			} else {
				aplicaTrocaSegChan(SR[i]);	// Decidi qual a página vítima e faz a troca
			}
		}
		if(operacaoSR[i] > 12){ 	// Se operação é de escrita seta bit modificação
		 	bitMod[SR[i]] = 1;
		}
		printf(" - ");
		printM();
		printf(" | S:%d E:%d",segChanStart,segChanEnd);
		printf("\n");
	}

	printf("\t----------------------------\n");
	printf("\tpage-in\tpage-out\tmiss\n");
	printf("FIFO\t%d\t%d\t\t%d\n",pageIn[FIFO],pageOut[FIFO],miss[FIFO]);
	printf("LRU\t%d\t%d\t\t%d\n",pageIn[LRU],pageOut[LRU],miss[LRU]);
	printf("SC\t%d\t%d\t\t%d\n",pageIn[SEGCHAN],pageOut[SEGCHAN],miss[SEGCHAN]);
	printf("SC Mel\t%d\t%d\t\t%d\n",pageIn[SEGCHAN2],pageOut[SEGCHAN2],miss[SEGCHAN2]);

	return 0;
}

/**
 * Gera SR com numeros entre 0 e P-1
 * Gera operacaoSR com numeros entre 0 e 16
 * Inicia todos as poscieos de PemM para 0;
 */
void init(){
	M = (long int *) malloc( sizeof(long int) * Q );
	SR = (long int *) malloc( sizeof(long int) * lenSR );
	operacaoSR = (short int *) malloc( sizeof(short int) * lenSR );
	naMemoria = (short int *) malloc( sizeof(short int) * P );
	bitMod = (short int *) malloc( sizeof(short int) * P );
	bitRef = (short int *) malloc( sizeof(short int) * P );
	fifo = (int *) malloc( sizeof(int) * Q );
	lru = (int *) malloc( sizeof(int) * Q );
	segChan = (int *) malloc( sizeof(int) * Q );
	if(M==NULL||SR==NULL||operacaoSR==NULL||naMemoria==NULL||bitMod==NULL||bitRef==NULL||fifo==NULL||segChan==NULL){
		printf("\tFalha ao alocar memoria\n"); exit(1);
	}

	srand(time(NULL));
	int i;
	for(i=0;i<lenSR;i++){
		//SR[i] = rand() % P;
		operacaoSR[i] = rand() % 16;
	}

	SR[0] = 7;
	SR[1] = 0;
	SR[2] = 1;
	SR[3] = 2;
	SR[4] = 0;
	SR[5] = 3;
	SR[6] = 0;
	SR[7] = 4;
	SR[8] = 2;
	SR[9] = 3;
	SR[10] = 0;
	SR[11] = 3;
	SR[12] = 2;
	SR[13] = 1;
	SR[14] = 2;
	SR[15] = 0;
	SR[16] = 1;
	SR[17] = 7;
	SR[18] = 0;
	SR[19] = 1;

	esvaziaTabelaMemoria();
}

/**
 * Esvazia memoria e bits de referencia e modificacao.
 * Aplicado ao iniciar o processo e a cada alteracao de algoritmo de substituicao de pag
 */
void esvaziaTabelaMemoria(){
  int i;
	for(i=0;i<P;i++){
		naMemoria[i]=0;
		bitMod[i]=0;
		bitRef[i]=0;
	}
	for(i=0;i<Q;i++) {  // Memória inicial vazia
		M[i]=-1;
		fifo[i]=-1;
		lru[i]=-1;
	}
	espacoLivre = Q;
}

/**
 * Carrega página na primeira posição livre
 */
void addPagFirstPosFree(long int pag,int algoritmo){
	int j;
	for(j=0;j<Q;j++){
		if(M[j]==-1){
			M[j]=pag; // inclui "pagina" na memoria
			naMemoria[pag]=1; // Marca página como na memoria
			if(algoritmo == FIFO) { addEmFifo(j); }
			if(algoritmo == LRU) { addEmLru(j); }
			if(algoritmo == SEGCHAN) { addEmSegChan(j); }
			break;
		}
	}
	// Zera bit de referencia ao carregar a página
	bitRef[pag]=0;
	espacoLivre--;
	pageIn[algoritmo]++;
}

// Controle FIFO
/**
 * Remove 1º pag adicionada da fila inclui nova no final da fila
 */
void aplicaTrocaFIFO(long int carregada){
	int indiceVitima = remDaFifo();
	//printf("POS: %d - SAI: %li ENTRA: %li\n",indiceVitima,M[indiceVitima],carregada);

	if(bitMod[M[indiceVitima]]) {
		// TODO: TOASK: Fazer algo se o bit de modificacao da pagina que esta saindo é 1?
		bitMod[M[indiceVitima]] = 0;
	}

	naMemoria[M[indiceVitima]] = 0;
	M[indiceVitima]=carregada;
	naMemoria[carregada]=1;
	addEmFifo(indiceVitima);

	pageIn[FIFO]++;
	pageOut[FIFO]++;
}

/**
 * Inclui no final da fila
 */
void addEmFifo(int indice){
	if(fifoEnd == Q-1) fifoEnd = 0;
	else fifoEnd++;
	if(fifo[fifoEnd] != -1){
		printf("***** FILA NAO ESTAVA VAZIA NA POSICAO %d VAL: %d SUBS POR: %d \n",fifoEnd,fifo[fifoEnd],indice);
	}
	fifo[fifoEnd] = indice;
}

/**
 * Remove 1º da fila
 * retorna valor removido
 */
int remDaFifo(){
	int indice = fifo[fifoStart];
	fifo[fifoStart]=-1;
	if(fifoStart == Q-1) fifoStart = 0;
	else fifoStart++;
	return indice;
}

// Controle LRU
/**
 * Pega página menos utilizada e substitui pela sendo incluida
 * Controle da fila de LRU é feito a cada solicitacao de páginas
 * pelo processo. A página solicitada é movida(caso já esteja na memoria)/incluida  no final da fila.
 */
void aplicaTrocaLRU(long int carregada){
	int indiceVitima = remDaLru();

	if(bitMod[M[indiceVitima]]) {
		bitMod[M[indiceVitima]] = 0;
	}

	naMemoria[M[indiceVitima]] = 0;
	M[indiceVitima]=carregada;
	naMemoria[carregada]=1;
	bitRef[carregada]=0;
	addEmLru(indiceVitima);

	pageIn[LRU]++;
	pageOut[LRU]++;
}

/**
 * Inclui no final da fila
 */
void addEmLru(int indice){
	if(lruEnd == Q-1) lruEnd = 0;
	else lruEnd++;
	if(lru[lruEnd] != -1){
		printf("***** FILA NAO ESTAVA VAZIA NA POSICAO %d VAL: %d SUBS POR: %d \n",lruEnd,lru[lruEnd],indice);
	}
	lru[lruEnd] = indice;
}

/**
 * Remove 1º da fila
 * retorna valor removido
 */
int remDaLru(){
	int indice = lru[lruStart];
	lru[lruStart]=-1;
	if(lruStart == Q-1) lruStart = 0;
	else lruStart++;
	return indice;
}

/**
 * Busca pagina nos Q quadros de M e move pagina para o final da fila LRU
 */
void moveParaFinalFila(long int pagina) {
	int i;
	if(lruStart != lruEnd && pagina != M[lru[lruEnd]]){
		// Busca posicao da pagina no lista de quadros
		int indicePagina = -1;
		i=0;
		do {
			if(pagina == M[lru[i]])
				indicePagina = i;
			i++;
		} while(i<Q && indicePagina==-1);
		// Desloca elemetos de i+1 até lruStart para esquerda e coloca indice da pagina no final
		i = indicePagina;
		int indicePaginaEmM = lru[indicePagina];
		int next = -1;
		do {
			if(i == Q-1){ // Final da lista?
				next = 0;
			} else {
				next = i + 1;
			}
			if(next != lruStart){
				lru[i] = lru[next];
				i = next;
			}
		} while(next != lruStart);
		lru[lruEnd]=indicePaginaEmM;
	}
}

// Controle Segunda chance
/**
 * A partid da primeira pagina da fila busca a 1º com bit zero, zerando aquelas com bit 1
 */
void aplicaTrocaSegChan(long int carregada){
	int indiceVitima = getProxSegChan();

	if(bitMod[M[indiceVitima]]) {
		bitMod[M[indiceVitima]] = 0;
	}

	naMemoria[M[indiceVitima]] = 0;
	M[indiceVitima]=carregada;
	naMemoria[carregada]=1;
	addEmSegChan(indiceVitima);

	pageIn[SEGCHAN]++;
	pageOut[SEGCHAN]++;
}

/**
 * Inclui no final da fila
 */
void addEmSegChan(int indice){
	if(segChanEnd == Q-1) segChanEnd = 0;
	else segChanEnd++;
	segChan[segChanEnd] = indice;
}

/**
 * Remove elemento e ajusa lista
 */
void remDaSegChan(int indice){

	if(indice == segChanStart){ // Remocao do inicio da lista
		segChan[segChanStart]=-1;
		if(segChanStart == Q-1) segChanStart = 0;	else segChanStart++;// incrementa inicio
	} else if(indice == segChanEnd){ // Remocao do fim da lista
		segChan[segChanEnd]=-1;
		if(segChanEnd == 0) segChanEnd = Q-1;	else segChanEnd--; // decrementa fim
	} else { // Remocao no meio da lista | desloca elemento de indice+1 até start para esquerda
		int indiceDaFrente = indice;
		while(indice < segChanEnd){
			segChan[indice] = indiceDaFrente;
			if(indiceDaFrente == Q-1) indiceDaFrente = indice+1; else indiceDaFrente++;
			indice = indiceDaFrente;
		}
		segChan[segChanEnd] = -1;
		if(segChanEnd == 0) segChanEnd = Q-1;	else segChanEnd--; // decrementa fim
	}
}

/**
 * Remove 1º elemento da lista. Se elemento tem bit de referencia == 0 este é removido (lista é 'ajustada') e retornado
 * se elemento tem bit de referencia == 1 bit de referencia e alterado para 0
 */
int getProxSegChan(){
	int indice = segChanStart;
	int encontrado = 0;
	do {
		if(bitRef[M[segChan[indice]]] == 0){
			encontrado = 1;
		} else {
			bitRef[M[segChan[indice]]] = 0;
			if(indice == Q-1) indice = 0;
			else indice++;
		}
	} while(!encontrado);

	remDaSegChan(indice);

	//printf("Prox: %d ", indice);
	return indice;
}
