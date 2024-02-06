#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define maxLin 1000
#define maxCol 1000

typedef int tipoMatriz[maxLin][maxCol];

tipoMatriz matrizPrincipal, matrizResultado;

int nLinhas, nColunas;

int vetor;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int linha, coluna, id;
    int flagCalculado;
} tipoValor;

tipoValor *pack;

void *verificaColunaUnica(int linhaElem, int colunaElem);
void *verificaLinhaUnica(int linhaElem, int colunaElem);
void *mediaAritmetica(void *linhaMatriz);
void *verificaVizinhos(int linhaElem, int colunaElem);
int media(int vizAtras, int vizFrente, int vizCima, int vizBaixo, int vizEsqCima, int vizDirCima, int vizEsqBaixo, int vizDirBaixo, int quantidadeViz);


void geraMatriz(tipoMatriz m, int nLin, int nCol) {

    int i, j;
    int x = 1;

    for (i = 0; i < nLin; i++) {
        printf("| ");
        for (j = 0; j < nCol; j++) {
//             int val = 0;
//             if (j < 3) {
//                 val = x;
//                 x += 1;
//             }
//             if (i == 0 && j == 3) {
//                 val = 1;
//             }
//             if (i == 1 && j == 3) {
//                 val = 2;
//             }
//             if (i == 2 && j == 3) {
//                 val = 3;
//             }
// 
//             m[i][j] = val;
            m[i][j] = rand() % 10;
            printf("%d ", m[i][j]);
        }
        printf("|");
        printf("\n");
    }
}

int main(void){
    srand(time(NULL));
    int i, j;
  
    printf("\nDetermine um numero de linhas para matriz => ");
    scanf("%d", &nLinhas);

    pthread_t thread[nLinhas];
    
    printf("\nDetermine um numero de colunas para matriz => ");
    scanf("%d", &nColunas);

    if (nLinhas > maxLin || nColunas > maxCol || nLinhas == 0 || nColunas == 0) {
        printf("Erro: O numero de linhas ou colunas deve ser maior que 0  e ate 1000. Programa encerrado.\n");
        exit(EXIT_FAILURE); // Encerra o programa
    }

    printf("\nMatriz:\n");
    geraMatriz(matrizPrincipal, nLinhas, nColunas);

    tipoValor *vetor =  malloc(nLinhas * sizeof(tipoValor));
    for (int i = 0; i < nLinhas; i++) {
        tipoValor linhaThread;
        linhaThread.linha = i;
        linhaThread.coluna = 0;
        linhaThread.id = i;
        vetor[i] = linhaThread;
    }

    pack = vetor;
    
    for (i = 0; i < nLinhas; i++) {
        pthread_create(&(thread[i]), NULL, mediaAritmetica, (void *)&pack[i]);
    }
    
    for (i = 0; i < nLinhas; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("\n\nMatriz Resultante:\n");

     for (i = 0; i < nLinhas; i++) {
        printf("| ");
        for (j = 0; j < nColunas; j++) {
            printf("%d ", matrizResultado[i][j]);
        }
        printf("|\n");
    }
    printf("\n");

    return 0;
}

void *mediaAritmetica(void *linhaMatriz){

    tipoValor *pacote;
    pacote = (tipoValor *)linhaMatriz;
    int i, j;

    if(pacote->linha != 0){
        while ((pack[pacote->linha - 1].coluna  <= (nColunas - 1))  && ((pack[pacote->linha - 1].coluna - pacote->coluna) <= 2)){
            printf("\nAguardando Thread %d, Thread Atual: %d ", pacote->linha - 1 ,pacote->id);
        }
    }
        
    for (i = 0; i < nColunas; i++) {
        printf("\nLinha Atual: %d Coluna Atual: %d Coluna da Thread anterior: %d",pacote->linha, i, pack[pacote->linha - 1].coluna);

        if(nLinhas == 1 && nColunas == 1){
             matrizResultado[pacote->linha][i] =  matrizPrincipal[pacote->linha][i];
        }
        else if(nLinhas == 1){
            verificaLinhaUnica(pacote->linha, i);
        }else if(nColunas == 1){
            verificaColunaUnica(pacote->linha, i);
        } else {
            verificaVizinhos(pacote->linha, i);
        }
        pacote->coluna += 1;

    }
    

}

void *verificaLinhaUnica(int linhaElem, int colunaElem){
    int vizAtras = 0;
    int vizFrente = 0;

    int resultadoMedia = 0;

    if(colunaElem == 0){
        matrizResultado[linhaElem][colunaElem] = matrizPrincipal[linhaElem][colunaElem + 1];
    } else if(colunaElem == (nColunas - 1)){
        matrizResultado[linhaElem][colunaElem] =  matrizResultado[linhaElem][colunaElem - 1];
    } else {
        vizAtras = matrizResultado[linhaElem][colunaElem - 1];
        vizFrente = matrizPrincipal[linhaElem][colunaElem + 1];
        resultadoMedia = media(vizAtras, vizFrente, 0, 0, 0, 0, 0, 0, 2);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;
    }

}

void *verificaColunaUnica(int linhaElem, int colunaElem){
    int vizCima = 0;
    int vizBaixo = 0;

    int resultadoMedia = 0;

    if(linhaElem == 0){
        matrizResultado[linhaElem][colunaElem] = matrizPrincipal[linhaElem + 1][colunaElem];
    } else if(linhaElem == (nLinhas - 1)){
        matrizResultado[linhaElem][colunaElem] =  matrizResultado[linhaElem - 1][colunaElem];
    } else {
        vizCima = matrizResultado[linhaElem - 1][colunaElem];
        vizBaixo = matrizPrincipal[linhaElem + 1][colunaElem];
        resultadoMedia = media(0, 0, vizCima, vizBaixo, 0, 0, 0, 0, 2);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;
    }

}

void *verificaVizinhos(int linhaElem, int colunaElem){
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
        vizFrente = matrizPrincipal[linhaElem][colunaElem + 1];
        vizBaixo = matrizPrincipal[linhaElem + 1][colunaElem];
        vizDirBaixo = matrizPrincipal[linhaElem + 1][colunaElem + 1];

        resultadoMedia = media(0, vizFrente, 0, vizBaixo, 0, 0, 0, vizDirBaixo, 3);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;

    } else if(linhaElem == (nLinhas - 1) && colunaElem == (nColunas - 1) ){
        vizAtras = matrizResultado[linhaElem][colunaElem - 1];
        vizCima = matrizResultado[linhaElem - 1][colunaElem];
        vizEsqCima = matrizResultado[linhaElem - 1][colunaElem - 1];

        resultadoMedia = media(vizAtras, 0, vizCima, 0, vizEsqCima, 0, 0, 0, 3);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;

    } else if(linhaElem == 0 && colunaElem == (nColunas - 1)) {
        vizAtras = matrizResultado[linhaElem][colunaElem - 1];
        vizBaixo = matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqBaixo = matrizPrincipal[linhaElem + 1][colunaElem - 1];

        resultadoMedia = media(vizAtras, 0, 0, vizBaixo, 0, 0, vizEsqBaixo, 0, 3);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;

    } else if(linhaElem == (nLinhas -1 ) && colunaElem == 0) {
        vizFrente = matrizPrincipal[linhaElem][colunaElem + 1];
        vizCima = matrizResultado[linhaElem - 1][colunaElem];
        vizDirCima = matrizResultado[linhaElem - 1][colunaElem + 1];

        resultadoMedia = media(0, vizFrente, vizCima, 0, 0, vizDirCima, 0, 0, 3);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;

    } else if (linhaElem == (nLinhas - 1) ) {
        vizAtras =  matrizResultado[linhaElem][colunaElem - 1];
        vizFrente =  matrizPrincipal[linhaElem][colunaElem + 1];
        vizCima =  matrizResultado[linhaElem - 1][colunaElem];
        vizEsqCima =  matrizResultado[linhaElem - 1][colunaElem - 1];
        vizDirCima =  matrizResultado[linhaElem - 1][colunaElem + 1];

        resultadoMedia =  media(vizAtras, vizFrente, vizCima, 0, vizEsqCima, vizDirCima, 0, 0, 5);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;

	} else if(colunaElem == (nColunas - 1)){
        vizAtras =  matrizResultado[linhaElem][colunaElem - 1];
        vizCima =  matrizResultado[linhaElem - 1][colunaElem];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqCima =  matrizResultado[linhaElem - 1][colunaElem - 1];
        vizEsqBaixo =  matrizPrincipal[linhaElem + 1][colunaElem - 1];

        resultadoMedia = media(vizAtras, 0, vizCima, vizBaixo, vizEsqCima, 0, vizEsqBaixo, 0, 5);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;
	} else if(linhaElem == 0){
        vizAtras =  matrizResultado[linhaElem][colunaElem - 1];
        vizFrente =  matrizPrincipal[linhaElem ][colunaElem + 1];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqBaixo =  matrizPrincipal[linhaElem + 1][colunaElem - 1];
        vizDirBaixo =  matrizPrincipal[linhaElem + 1][colunaElem + 1];

        resultadoMedia = media(vizAtras, vizFrente, 0, vizBaixo, 0, 0, vizEsqBaixo, vizDirBaixo, 5);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;
    } else if(colunaElem == 0){
        vizFrente =  matrizPrincipal[linhaElem ][colunaElem + 1];
        vizCima =  matrizResultado[linhaElem - 1][colunaElem];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizDirCima =  matrizResultado[linhaElem - 1][colunaElem + 1];
        vizDirBaixo =  matrizPrincipal[linhaElem + 1][colunaElem + 1];

        resultadoMedia = media(0, vizFrente, vizCima, vizBaixo, 0, vizDirCima, 0, vizDirBaixo, 5);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;
	} else {
        vizAtras = matrizResultado[linhaElem ][colunaElem - 1];
        vizFrente =  matrizPrincipal[linhaElem ][colunaElem + 1];
        vizCima =  matrizResultado[linhaElem - 1][colunaElem];
        vizBaixo =  matrizPrincipal[linhaElem + 1][colunaElem];
        vizEsqCima =  matrizResultado[linhaElem - 1][colunaElem - 1];
        vizEsqBaixo =  matrizPrincipal[linhaElem + 1][colunaElem - 1];
        vizDirCima =  matrizResultado[linhaElem - 1][colunaElem + 1];
        vizDirBaixo =  matrizPrincipal[linhaElem + 1][colunaElem + 1];
        
        resultadoMedia = media(vizAtras, vizFrente, vizCima, vizBaixo,vizEsqCima, vizDirCima, vizEsqBaixo, vizDirBaixo, 8);
        matrizResultado[linhaElem][colunaElem] = resultadoMedia;
    }
    
}


int media(int vizAtras, int vizFrente, int vizCima, int vizBaixo, int vizEsqCima, int vizDirCima, int vizEsqBaixo, int vizDirBaixo, int quantidadeViz){
    int resultado = (vizAtras + vizFrente + vizCima + vizBaixo + vizEsqCima + vizDirCima + vizEsqBaixo + vizDirBaixo) / quantidadeViz;
    return resultado;
}
