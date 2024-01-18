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
int media(int vizAtras, int vizFrente, int vizCima, int vizBaixo, int vizEsqCima, int vizDirCima, int vizEsqBaixo, int vizDirBaixo, int quantidadeViz);

typedef struct {
    int linha, coluna, id;
} tipoValor;

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
    tipoValor *pack[maxLin];

    printf("\nDetermine um numero de linhas para matriz => ");
    scanf("%d", &nLinhas);

    printf("\nDetermine um numero de colunas para matriz => ");
    scanf("%d", &nColunas);

    printf("\nMatriz:\n");
    geraMatriz(matrizPrincipal, nLinhas, nColunas);

    for (i = 0; i < nLinhas; i++) {
        pack[i] = malloc(sizeof(tipoValor));
        pack[i]->linha = i;
        pack[i]->id = i+ 1;
        pthread_create(&(thread[i]), NULL, mediaAritmetica, (void *)pack[i]);
    }

    for (i = 0; i < nLinhas; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("\n\nMatriz Resultante:\n");

     for (i = 0; i < nLinhas; i++) {
        printf("| ");
        for (j = 0; j < nColunas; j++) {
            printf("%d | ", matrizPrincipal[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

void *mediaAritmetica(void *linhaMatriz){

    tipoValor *pacote;
    pacote = (tipoValor *)linhaMatriz;
    printf("\nLinha: %d ->", pacote->id);
    int i, j;
    for (i = 0; i < nColunas; i++) {    
       //printf(" %d ",matrizPrincipal[pacote->linha][i]);
       verificaVizinhos(pacote->linha, i);
    }
    printf("\n-----");
    
}


void *verificaVizinhos(int linhaElem, int colunaElem){
    // TEM QUE PENSAR EM UMA LOGICA PARA QUANDO O VALOR ANTERIOR NAO TIVER SIDO CALCULADO
    int vizAtras = 0;
    int vizFrente = 0;
    int vizCima = 0;
    int vizBaixo = 0;
    int vizEsqCima = 0;
    int vizDirCima = 0;
    int vizEsqBaixo = 0;
    int vizDirBaixo = 0;

    int resultadoMedia = 0;

    if (linhaElem == 0 && colunaElem == 0 ) {
        printf("\nPrimeiro Elemento posicao %d %d, ELEM: %d ", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
        //buscaVizinhos
        vizFrente = matrizPrincipal[linhaElem][colunaElem + 1];
        vizBaixo = matrizPrincipal[linhaElem + 1][colunaElem];
        vizDirBaixo = matrizPrincipal[linhaElem + 1][colunaElem + 1];

        resultadoMedia = media(0, vizFrente, 0, vizBaixo, 0, 0, 0, vizDirBaixo, 3);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;

    } else if(linhaElem == (nLinhas - 1) && colunaElem == (nColunas - 1) ){
        printf("\nUltimo Elemento posicao %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);

        vizAtras = matrizPrincipal[linhaElem][colunaElem - 1];
        vizCima = matrizPrincipal[linhaElem - 1][colunaElem];
        vizEsqCima = matrizPrincipal[linhaElem - 1][colunaElem - 1];

        resultadoMedia = media(vizAtras, 0, vizCima, 0, vizEsqCima, 0, 0, 0, 3);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;

    } else if(linhaElem == 0 && colunaElem == (nColunas - 1)) {
        //elemento na primeira linha e na ultima coluna
        printf("\nElemento na primeira linha e na ultima coluna %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
        vizAtras = matrizPrincipal[linhaElem][colunaElem - 1];
        vizBaixo = matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqBaixo = matrizPrincipal[linhaElem + 1][colunaElem - 1];

        resultadoMedia = media(vizAtras, 0, 0, vizBaixo, 0, 0, vizEsqBaixo, 0, 3);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;

    } else if(linhaElem == (nLinhas -1 ) && colunaElem == 0) {
        //elemento na ultima linha e na primeira coluna
        printf("\nElemento na ultima linha e na primeira coluna %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
        vizFrente = matrizPrincipal[linhaElem][colunaElem + 1];
        vizCima = matrizPrincipal[linhaElem - 1][colunaElem];
        vizDirCima = matrizPrincipal[linhaElem - 1][colunaElem + 1];

        resultadoMedia = media(0, vizFrente, vizCima, 0, 0, vizDirCima, 0, 0, 3);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;

    } else if (linhaElem == (nLinhas - 1) ) {
        printf("\nElemento na ultima linha %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
		//não calcula vizinhos abaixo
        vizAtras =  matrizPrincipal[linhaElem][colunaElem - 1];
        vizFrente =  matrizPrincipal[linhaElem][colunaElem + 1];
        vizCima =  matrizPrincipal[linhaElem - 1][colunaElem];
        vizEsqCima =  matrizPrincipal[linhaElem - 1][colunaElem - 1];
        vizDirCima =  matrizPrincipal[linhaElem - 1][colunaElem + 1];

        resultadoMedia =  media(vizAtras, vizFrente, vizCima, 0, vizEsqCima, vizDirCima, 0, 0, 5);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;

	} else if(colunaElem == (nColunas - 1)){
		//não calcula vizinhos a direita
        printf("\nElemento na ultima coluna %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
        vizAtras =  matrizPrincipal[linhaElem][colunaElem - 1];
        vizCima =  matrizPrincipal[linhaElem - 1][colunaElem];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqCima =  matrizPrincipal[linhaElem - 1][colunaElem - 1];
        vizEsqBaixo =  matrizPrincipal[linhaElem + 1][colunaElem - 1];

        resultadoMedia = media(vizAtras, 0, vizCima, vizBaixo, vizEsqCima, 0, vizEsqBaixo, 0, 5);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;
	} else if(linhaElem == 0){
        printf("\nElemento na primeira linha %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
        vizAtras =  matrizPrincipal[linhaElem][colunaElem - 1];
        vizFrente =  matrizPrincipal[linhaElem ][colunaElem + 1];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqBaixo =  matrizPrincipal[linhaElem + 1][colunaElem - 1];
        vizDirBaixo =  matrizPrincipal[linhaElem + 1][colunaElem + 1];

        resultadoMedia = media(vizAtras, vizFrente, 0, vizBaixo, 0, 0, vizEsqBaixo, vizDirBaixo, 5);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;
    } else if(colunaElem == 0){
        printf("\nElemento na primeira coluna %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]);
        vizFrente =  matrizPrincipal[linhaElem ][colunaElem + 1];
        vizCima =  matrizPrincipal[linhaElem - 1][colunaElem];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizDirCima =  matrizPrincipal[linhaElem - 1][colunaElem + 1];
        vizDirBaixo =  matrizPrincipal[linhaElem + 1][colunaElem + 1];


        resultadoMedia = media(0, vizFrente, vizCima, vizBaixo, 0, vizDirCima, 0, vizDirBaixo, 5);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;
	} else {
        printf("\nCentro,  %d %d, ELEM: %d", linhaElem, colunaElem, matrizPrincipal[linhaElem][colunaElem]); 
        vizAtras = matrizPrincipal[linhaElem ][colunaElem - 1];
        vizFrente =  matrizPrincipal[linhaElem ][colunaElem + 1];
        vizCima =  matrizPrincipal[linhaElem - 1][colunaElem];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqCima =  matrizPrincipal[linhaElem - 1][colunaElem - 1];
        vizEsqBaixo =  matrizPrincipal[linhaElem + 1][colunaElem - 1];
        vizDirCima =  matrizPrincipal[linhaElem - 1][colunaElem + 1];
        vizDirBaixo =  matrizPrincipal[linhaElem + 1][colunaElem + 1];
        
        resultadoMedia = media(vizAtras, vizFrente, vizCima, vizBaixo,vizEsqCima, vizDirCima, vizEsqBaixo, vizDirBaixo, 8);
        matrizPrincipal[linhaElem][colunaElem] = resultadoMedia;
    }
    
}


int media(int vizAtras, int vizFrente, int vizCima, int vizBaixo, int vizEsqCima, int vizDirCima, int vizEsqBaixo, int vizDirBaixo, int quantidadeViz){
    int resultado = (vizAtras + vizFrente + vizCima + vizBaixo + vizEsqCima + vizDirCima + vizEsqBaixo + vizDirBaixo) / quantidadeViz;
    return resultado;
}
