#include <stdio.h>
#include "navios.h" // inclui structs e funções relacionadas a navios

void posicaonavio(ponto inicial){
    do {
        printf("Linha (1–8): \n");
        scanf("%d", &inicial.x); // lê linha escolhida
         if(inicial.x >8 || inicial.x <1)
            printf("Coordenada inválida!\n");
    } while (inicial.x >8 || inicial.x <1); // repete até estar correto

    do {
        printf("Coluna (1–8): \n ");
        scanf("%d", &inicial.y); // lê coluna escolhida
        if (inicial.x >8 || inicial.x <1)
            printf("Coordenada inválida!\n");
    } while (inicial.x >8 || inicial.x <1); // repete até estar correto
}

void posicionarNavio(char tab[8][8]){
    portaAvioes pA1;
    navioTanque nT1;
    submarino sub1;
    bote bote1, bote2;

    printf("\n");
    printf("Vamos posicionar os navios! \n");
    printf("Vamos começar pelo Porta-Avião.\n");
    printf("Lembrando que ele ocupa 5 casas. \n");
    printf("Escolha a primeira posição: \n");
    posicaonavio(pA1.posicao[0]);


}
