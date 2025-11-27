#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


int main() {
    setlocale(LC_ALL, "Portuguese");

    char tab1[8][8], tab2[8][8]; // matrizes dos navios dos jogadores 1 e 2
    char ataque1[8][8], ataque2[8][8]; //matrizes dos ataques

    // inicializa tabuleiros com '~'
    inicializartab(tab1);
    inicializartab(tab2);
    inicializartab(ataque1);
    inicializartab(ataque2);

    int opcao; // variável usada para guardar a opção escolhida pelo usuário no menu

    do {
        printf("\n==============================\n");
        printf("    BATALHA NAVAL - MENU\n");
        printf("==============================\n");
        printf("1 - Novo Jogo\n");
        printf("2 - Continuar Jogo\n");
        printf("3 - Instruções\n");
        printf("4 - Sair do Jogo\n");
        printf("Escolha uma opção (1–4): ");

        scanf("%d", &opcao); // lê opção do jogador

        switch(opcao) { // estrutura switch para tratar cada opção do menu
            case 1:
                printf("\n Novo jogo \n");
                printf("\n");

                mostrartabuleiro(tab1); // exibe o tabuleiro
                posicionarNavio(tab1); // começa posicionamento dos navios
                break;

            case 2:
                  printf("\n Continuar jogo :( \n");
                // ainda tem que fazer aq tb kkkkkkkk
                break;

            case 3:
                instrucoes(); // mostra instruções
                break;

            case 4:
                printf("\n Saindo do jogo...\n"); // finaliza o programa
                return 0;
                break;

            default: // se o usuário digitar algo diferente de 1 a 4
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 4); // o menu continua repetindo até que a opção 4 seja escolhida

    return 0;
}
