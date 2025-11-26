#include <stdio.h>
#include "tabuleiro.h" // inclui funções do tabuleiro

void mostrartabuleiro(char tab[8][8]) {
    printf("  "); // recurso estético
    for(int p=1; p<9; p++) //numeração das colunas
        printf("%d ", p);
    printf("\n");

    for(int i=0; i<8; i++) {
        printf("%d ", i+1); // numeração das linhas
        for(int k=0; k<8; k++) {
            printf("%c ", tab[i][k]); // composicao da matriz
        }
        printf("\n");
    }
}

void inicializartab(char tab[8][8]) {
    for(int i=0; i<8; i++) {
        for(int k=0; k<8; k++) {
            tab[i][k] = '~'; // '~' representa água
        }
    }
}
