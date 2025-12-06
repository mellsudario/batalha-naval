#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

void instrucoes() { // exibe explicação das regras
    printf("\n===== INSTRUÇÕES =====\n");
    printf("1. O jogo segue o modelo tradicional de Batalha Naval.\n");
    printf("2. Dois jogadores posicionam seus navios no tabuleiro.\n");
    printf("3. Os jogadores alternam turnos tentando acertar os navios adversários.\n");
    printf("4. Vence quem destruir todos os navios inimigos.\n\n");
}

typedef struct{
    int x;
    int y;
}ponto;

typedef struct{
    ponto posicao [5];
}portaAvioes;

typedef struct{
    ponto posicao [4];
}navioTanque;

typedef struct{
    ponto posicao [3];
}submarino;

typedef struct{
    ponto posicao[2];
}bote;


void mostrartabuleiro(char tab1[8][8]){ // funçao para a posiçao dos navios do jogador
    printf("  "); // recurso estético
    for(int p=1;p<9;p++)//numeração das colunas
        printf("%d ",p);
    printf("\n");
    for(int i=0; i<8;i++){
        printf("%d ",i+1); // numeração das linhas
        for(int k=0;k<8;k++){
        printf("%c ",tab1[i][k]); // composicao da matriz
        }
        printf("\n");
        }
    }


void inicializartabuleiro(char tab1[8][8]){  // funçao para inicializar o tabuleiro
    for(int i=0; i<8;i++){
        for(int k=0;k<8;k++){
            tab1[i][k] = '~';
        }
    }
}

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
        do {
          printf("1 - Norte\n");
          printf("2 - Sul\n");
          printf("3 - Leste\n");
          printf("4 - Oeste\n");
          printf("Digite a direção (1-4): ");
          scanf("%d", &direcao);
       if (direcao < 1 || direcao > 4) {
        printf("\nDireção inválida! Escolha um valor entre 1 e 4.\n\n");
    }
} while (direcao < 1 || direcao > 4);

        int pode = 1; // assume que pode colocar

        // verifica cada parte do navio (sem gravar)
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


          // se chegou aqui,pode==1 e agora grava posições
        for (int i = 0; i < tamanho; i++) {
            int x = linha;
            int y = coluna;

            if (direcao == 1) x -= i;
            if (direcao == 2) x += i;
            if (direcao == 3) y += i;
            if (direcao == 4) y -= i;

            posicoes[i].x = x;
            posicoes[i].y = y;

            tab[x][y] = 'N'; // marca o navio no tabuleiro
        }

        printf("\nNavio colocado com sucesso!\n\n");
        break; // sai do while, navio posicionado
    }
}

void posicionarNavio(char tab1[8][8]){

    // cria as estruturas
    bote bote1, bote2;
    portaAvioes pA1;
    navioTanque nT1;
    submarino sub1;

    printf("\n");
    printf("Vamos posicionar os navios! \n");
    printf("Vamos começar pelo Porta-Avião.\n");

    printf("\n=== Porta-Aviões (5 espaços) ===\n");
    posicaonavio(tab1, pA1.posicao, 5);

    printf("\n=== Navio-Tanque (4 espaços) ===\n");
    posicaonavio(tab1, nT1.posicao, 4);

    printf("\n=== Submarino (3 espaços) ===\n");
    posicaonavio(tab1, sub1.posicao, 3);

    printf("\n=== Bote 1 (2 espaços) ===\n");
    posicaonavio(tab1, bote1.posicao, 2);

    printf("\n=== Bote 2 (2 espaços) ===\n");
    posicaonavio(tab1, bote2.posicao, 2);
}


int acertosJog1 = 0;
int errosJog1 = 0;
int acertosJog2 = 0;
int errosJog2 = 0;
int contador_rodadas = 0;


void rodadas(char tabDefesa[8][8], char tabAtaque[8][8], int jogador) {

    int linha, coluna;
    int x, y;


    printf("\nPosiscione seu ataque: \n");
    printf("Escolha a linha (1-8): ");
    scanf("%d", &linha);
    printf("Escolha a coluna (1-8): ");
    scanf("%d", &coluna);

    // converter para índice da matriz
    x = linha - 1;
    y = coluna - 1;

    if (x < 0 || x > 7 || y < 0 || y > 7) {
        printf("\nCoordenada fora do tabuleiro! Rodada perdida.\n");
        return;
    }


    if (tabAtaque[x][y] == 'X' || tabAtaque[x][y] == 'O') {
    printf("\nVocê já jogou nessa posição! Rodada perdida.\n");
    return;
}

    // verificar se acertou FEITO
    if (tabDefesa[x][y] == 'N') {
        printf("\nAcertou o navio inimigo! \n");
        tabDefesa[x][y] = 'X';       // acerto
        tabAtaque[x][y] = 'X';   // marca acerto no tabuleiro de ataque

         if (jogador == 1) acertosJog1++;
         else acertosJog2++;
    }
    else {
        printf("\n ERROU! \n");
        tabAtaque[x][y] = 'O';       // marca erro no tabuleiro de ataque
        if (tabDefesa[x][y] == '~')
            tabDefesa[x][y] = 'O';   // marca erro também no tabuleiro de defesa

            if (jogador == 1) errosJog1++;
        else errosJog2++;
    }

    contador_rodadas++;

    // imprimir tabuleiro ataque FEITO
    printf("\nTabuleiro de ataques:\n");
    mostrartabuleiro(tabAtaque);

    // fim : indentificar quando todos os navios de um jogador foram derrubados FEITO
    int acabou = 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tabDefesa[i][j] == 'N') {
                acabou = 0; // ainda há navio vivo
            }
        }
    }

    if (acabou) {
        printf("\nTODOS OS NAVIOS DO INIMIGO FORAM DESTRUÍDOS! \n");
        printf("\nFIM DE JOGO! \n");
    }

     // todo fim de rodada: guardar informações no arquivo FAZER
}

int aindaTemNavio(char tab[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tab[i][j] == 'N')
                return 1; // ainda tem navio
        }
    }
    return 0; // nenhum navio vivo
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    char tab1[8][8], tab2[8][8]; // matrizes dos navios dos jogadores 1 e 2
    char ataque1[8][8], ataque2[8][8]; //matrizes dos ataques

    // inicializa tabuleiros com '~'
    inicializartabuleiro(tab1); // tabuleiro de navios do jogador 1
    inicializartabuleiro(tab2); // tabuleiro de navios do jogador 2
    inicializartabuleiro(ataque1); //tabuleiro de ataque do jogador 1
    inicializartabuleiro(ataque2); //tabuleiro de ataque do jogador 2

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
            printf("\n Novo jogo \n\n");

            // Jogador 1
            mostrartabuleiro(tab1); // exibe o tabuleiro do jogador 1
            posicionarNavio(tab1); // começa o posicionamento dos navios do jogador 1


            system("cls"); // limpa a tela - se for abrir no linux tem que colocar system("clear")
            printf("\nTabuleiro do Jogador 1 salvo!\nAgora o Jogador 2 irá posicionar os navios.\n");

            // Jogador 2
            mostrartabuleiro(tab2); // exibe o tabuleiro do jogador 2
            posicionarNavio(tab2); // começa o posicionamento dos navios do jogador 2

            system("cls"); // limpa a tela - se for abrir no linux tem que colocar system("clear")

                 printf("\n===== INICIANDO AS RODADAS =====\n");

                // loop até alguém vencer
               while (1) {

            printf("\nVEZ DO JOGADOR 1 \n");
            rodadas(tab2, ataque1, 1);

            if (!aindaTemNavio(tab2)) {
            printf("\nJOGADOR 1 VENCEU O JOGO! \n");

            printf("\n===== PLACAR FINAL =====\n");
            printf("Rodadas jogadas: %d\n\n", contador_rodadas);

            printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
            printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);
        break;
    }

            printf("\nVEZ DO JOGADOR 2 \n");
            rodadas(tab1, ataque2, 2);

            if (!aindaTemNavio(tab1)) {
            printf("\nJOGADOR 2 VENCEU O JOGO! \n");

            printf("\n===== PLACAR FINAL =====\n");
            printf("Rodadas jogadas: %d\n\n", contador_rodadas);

            printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
            printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);
        break;
    }
}

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
