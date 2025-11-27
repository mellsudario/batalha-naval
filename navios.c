#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// guarda as coordenadas de um navio
typedef struct {
    int x;
    int y;
} ponto;

// porta-aviões ocupa 5 espaços
typedef struct {
    ponto posicao[5];
} portaAvioes;

// navio-tanque ocupa 4 espaços
typedef struct {
    ponto posicao[4];
} navioTanque;

// submarino ocupa 3 espaços
typedef struct {
    ponto posicao[3];
} submarino;

// bote ocupa 2 espaços
typedef struct {
    ponto posicao[2];
} bote;


// função que posiciona um navio no tabuleiro
void posicaonavio (char tab[8][8], ponto posicoes[], int tamanho) {
    int linha, coluna, direcao;

    while (1) { // repete até colocar o navio corretamente

        printf("Escolha a linha (1-8): ");
        scanf("%d", &linha);
        printf("Escolha a coluna (1-8): ");
        scanf("%d", &coluna);

        // converte para índice começando em 0
        linha--;
        coluna--;

        // escolhe direção
        printf("Escolha a direção:\n");
        printf("1 - Norte\n");
        printf("2 - Sul\n");
        printf("3 - Leste\n");
        printf("4 - Oeste\n");
        scanf("%d", &direcao);

        int pode = 1; // assume que pode colocar

        // verifica cada parte do navio
        for (int i = 0; i < tamanho; i++) {
            int x = linha;
            int y = coluna;

            if (direcao == 1) x -= i; // norte
            if (direcao == 2) x += i; // sul
            if (direcao == 3) y += i; // leste
            if (direcao == 4) y -= i; // oeste

            // verifica bordas
            if (x < 0 || x > 7 || y < 0 || y > 7) {
                pode = 0;
                break;
            }

            // verifica colisão com outro navio
            if (tab[x][y] != '~') {
                pode = 0;
                break;
            }
        }

        if (pode == 0) {
            printf("\n Posição inválida! O navio não cabe ou colide com outro.\n\n");
            continue; // repete desde o início
        }

            posicoes[i].x = x;   // salva linha
            posicoes[i].y = y;   // salva coluna

            tab[x][y] = 'N'; // marca o navio no tabuleiro
        }

        printf("\nNavios colocados com sucesso!\n\n");
        break; // sai do while, navio posicionado
    }
}



// função que posiciona todos os navios
void posicionarNavio(char tab[8][8]){

    // cria as estruturas
    portaAvioes pA1;
    navioTanque nT1;
    submarino sub1;
    bote bote1, bote2;

    printf("Vamos posicionar os navios! \n");

    printf("\n=== Porta-Aviões (5 espaços) ===\n");
    posicaonavio(tab, pA1.posicao, 5);

    printf("\n=== Navio-Tanque (4 espaços) ===\n");
    posicaonavio(tab, nT1.posicao, 4);

    printf("\n=== Submarino (3 espaços) ===\n");
    posicaonavio(tab, sub1.posicao, 3);

    printf("\n=== Bote 1 (2 espaços) ===\n");
    posicaonavio(tab, bote1.posicao, 2);

    printf("\n=== Bote 2 (2 espaços) ===\n");
    posicaonavio(tab, bote2.posicao, 2);
}
