#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mostrartabuleiro(char tab[8][8]) { // exibe o tabuleiro
    printf("  "); // recurso estético
    for(int p=0; p<8; p++) //numeração das colunas
        printf("%d ", p + 1);
    printf("\n");

    for(int i=0; i<8; i++) {
        printf("%d ", i+1); // numeração das linhas
        for(int k=0; k<8; k++) {
            printf("%c ", tab[i][k]); // composicao da matriz
        }
        printf("\n");
    }
}

void inicializartab(char tab[8][8]) { // preenche com '~'
    for(int i=0; i<8; i++) {
        for(int k=0; k<8; k++) {
            tab[i][k] = '~'; // '~' representa água
        }
    }
}
