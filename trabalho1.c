#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define maxLin 1000
#define maxCol 1000

typedef int tipoMatriz[maxLin][maxCol];

tipoMatriz matrizPrincipal;

int nLinhas, nColunas;

typedef struct {
    int linA, colB;
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

    return 0;
}





