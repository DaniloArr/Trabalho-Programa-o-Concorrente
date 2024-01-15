#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define maxLin 1000
#define maxCol 1000

typedef int tipoMatriz[maxLin][maxCol];

tipoMatriz matrizPrincipal;

int nLinhas, nColunas;

void *mediaAritmetica(void *linhaMatriz);
void *verificaVizinhos(int linhaElem, int colunaElem);
void *media(int vizAtras, int vizFrente, int vizCima, int vizBaixo, int vizEsqCima, int vizDirCima, int vizEsqBaixo, int vizDirBaixo, int quantidadeViz);

typedef struct {
    int linha, coluna;
    int id;
} tipoPack;

void geraMatriz(tipoMatriz m, int nLin, int nCol) {
    int i, j;

    for (i = 0; i < nLin; i++) {
        printf("| ");
        for (j = 0; j < nCol; j++) {
            m[i][j] = rand() % 10;
            printf("%d | ", m[i][j]);
        }
        printf("\n");
    }
}

int main(void){

    pthread_t thread[maxLin];
    int i, j;
    tipoPack *pack[maxLin];

    printf("\nDetermine um numero de linhas para matriz => ");
    scanf("%d", &nLinhas);

    printf("\nDetermine um numero de colunas para matriz => ");
    scanf("%d", &nColunas);

    printf("\nMatriz A:\n");
    geraMatriz(matrizPrincipal, nLinhas, nColunas);

    for (i = 0; i < nLinhas; i++) {
        pack[i] = malloc(sizeof(tipoPack));
        pack[i]->linha = i;
        pack[i]->id = i+ 1;
        pthread_create(&(thread[i]), NULL, mediaAritmetica, (void *)pack[i]);
    }

    for (i = 0; i < nLinhas; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}

void *mediaAritmetica(void *linhaMatriz){

    tipoPack *pacote;
    pacote = (tipoPack *)linhaMatriz;
    printf("\nLinha: %d ->", pacote->id);
    int i, j;
    for (i = 0; i < nColunas; i++) {    
       printf(" %d ",matrizPrincipal[pacote->linha][i]);
       verificaVizinhos(pacote->linha, i);
    }
    printf("\n-----");
    
}


void *verificaVizinhos(int linhaElem, int colunaElem){
    // posicoes que estao com 99 é porque possuem um valor no vizinho
    if (linhaElem == 0 && colunaElem == 0 ) {
        printf("Primeiro Elemento posicao 0 0");
        //buscaVizinhos
        media(0, 99, 0, 99, 0, 0, 0, 99, 3);
    } else if(linhaElem == (nLinhas - 1) && colunaElem == (nColunas - 1) ){
        printf("Ultimo Elemento posicao %d %d", nLinhas, nColunas);
        //buscaVizinhos
        media(99, 0, 99, 0, 99, 0, 0, 0, 3);
    } else if (linhaElem == (nLinhas - 1) ) {
        
        printf("Elemento na ultima linha");
		//não calcula vizinhos abaixo
        media(99, 99, 99, 0, 99, 99, 0, 0, 5);
	} else if(colunaElem == (nColunas - 1)){

		//não calcula vizinhos a direita
        printf("Elemento na ultima coluna");
        media(99, 0, 99, 99, 99, 0, 99, 0, 5);
	} else if(linhaElem == 0){

        printf("Elemento na primeira linha");
        media(99, 99, 0, 99, 0, 0, 99, 99, 5);
    } else if(colunaElem == 0){
        printf("Elemento na primeira coluna");

        media(0, 99, 99, 99, 0, 99, 0, 99, 5);
		//não calcula vizinhos a esquerda
	} 
    
}


void *media(int vizAtras, int vizFrente, int vizCima, int vizBaixo, int vizEsqCima, int vizDirCima, int vizEsqBaixo, int vizDirBaixo, int quantidadeViz){
    int resultado = (vizAtras + vizFrente + vizCima + vizBaixo + vizEsqCima + vizDirCima + vizEsqBaixo + vizDirBaixo) / quantidadeViz;
}


/*	if (linhaElemento== 0) {
		//não calcula vizinhos acima
	}
	if(colunaElemento == 0){
		//não calcula vizinhos a esquerda
	}
	
	if (linhaElemento== linhaUsuario - 1 ) {
		//não calcula vizinhos abaixo
	}
	if(colunaElemento== colunaUsuario - 1){
		//não calcula vizinhos a direita
	}*/