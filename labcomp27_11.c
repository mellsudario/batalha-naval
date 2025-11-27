#include <stdio.h>
#include <stdlib.h>
#include <locale.h> // permite usar acentuação
#include <string.h>

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

ponto casa_ocupada [16]; 


void instrucoes () { // função que mostra as instruções do jogo
    printf("\n");
    printf("\n");
    printf("\n===== INSTRUÇÕES =====\n");
    printf("1. O jogo segue o modelo tradicional de Batalha Naval.\n");
    printf("2. Dois jogadores posicionam seus navios no tabuleiro.\n");
    printf("3. Os jogadores alternam turnos tentando acertar os navios adversários.\n");
    printf("4. Vence quem destruir todos os navios inimigos.\n\n");
    printf("\n");
}

void novojogo(){
   
}

void pontoinicial (ponto inicial){
    
    int bool = 0;

    do {
    bool = 0;
    printf("Linha: \n");
    scanf("%d",&inicial.x);
    if(inicial.x > 8 || inicial.x < 1){
        printf("Coordenada inválida, navio fora do tabuleiro."); // checa se esta dentro do tabuleiro
        bool = 1;
    } 
    else{ 
      for(int p = 0; p<16 ; p++){ // percorrer o vetor 
            if(casa_ocupada[p].x == inicial.x) {// mostrar que a casa ja está ocupada
                printf("Coordenada inválida, casa já ocupada.");
                bool = 1;
            }
        }
    }
    }while (bool == 1);

    do {
    bool = 0;
    printf("Coluna: \n");
    scanf("%d",&inicial.y);
    if(inicial.y > 8 || inicial.y < 1){
        printf("Coordenada inválida, navio fora do tabuleiro.");
        bool = 1;
        }
    else{ 
        for(int p = 0; p<16 ; p++){ // percorrer o vetor 
            if(casa_ocupada[p].y == inicial.y) {// mostrar que a casa ja está ocupada
                printf("Coordenada inválida, casa já ocupada.");
                bool = 1;
            }
        }
    }
    }while (bool == 1);



}

void pontofinal(){ // chega o ponto final, para verificar se a direção é permitida
    //

}

void posicionarNavio(char tab[8][8]){
    portaAvioes pA1;
    navioTanque nT1;
    submarino sub1;
    bote bote1, bote2;
    
    for(int i =0; i<16; i++){ // inicializado
        casa_ocupada[i].x = 0;
        casa_ocupada[i].y= 0;
    }


    printf("Vamos posicionar os navios! \n");
    printf("Vamos começar pelo Porta-Avião.\n");
    printf("Lembrando que ele ocupa 5 casas. \n");
    printf("Escolha a primeira posição: \n");
    pontoinicial(pA1.posicao[0]);
    


}




void mostrartabuleiro(char tab[8][8]){
    printf("  "); // recurso estético
    for(int p=1;p<9;p++) //numeração das colunas
        printf("%d ",p);
    printf("\n");
    for(int i=0; i<8;i++){
        printf("%d ",i+1); // numeração das linhas
        for(int k=0;k<8;k++){
        printf("%c ",tab[i][k]); // composicao da matriz
        }
        printf("\n");
    }
}

void inicializartab(char tab[8][8]){
    for(int i=0; i<8;i++){
        for(int k=0;k<8;k++){
            tab[i][k] = '~';
        }
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    char tab1[8][8], tab2[8][8]; // matrizes dos navios dos jogadores 1 e 2
    char ataque1[8][8], ataque2[8][8]; //matrizes dos ataques
    inicializartab(tab1);
    inicializartab(tab2);
    inicializartab(ataque1);
    inicializartab(ataque2);

   
    int opcao; // variável usada para guardar a opção escolhida pelo usuário no menu

    do {
        printf("\n==============================\n");
        printf(" BATALHA NAVAL - MENU\n");
        printf("==============================\n");
        printf("1 - Novo Jogo\n");
        printf("2 - Continuar Jogo\n");
        printf("3 - Instruções\n");
        printf("4 - Sair do Jogo\n");
        printf("Escolha uma opção (entre 1 - 4): ");

        scanf("%d", &opcao);

        switch (opcao) { // estrutura switch para tratar cada opção do menu

            case 1:
                printf("\n Novo jogo :( \n");
                mostrartabuleiro(tab1);
                posicionarNavio(tab1);
                break;

            case 2:
                printf("\n Continuar jogo :( \n");
                // ainda tem que fazer aq tb kkkkkkkk
                break;

            case 3:
                instrucoes();
                break;

            case 4:
                printf("\n Saindo do jogo :( \n");
                return 0;
                break;

            default: // se o usuário digitar algo diferente de 1 a 4
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 4); // o menu continua repetindo até que a opção 4 seja escolhida

    return 0;
}
