#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

FILE*jogadas,*tabjog1,*tabjog2;


void instrucoes() { // exibe explicação das regras
    printf("\n===== INSTRUÇÕES =====\n");
    printf("1. O jogo segue o modelo tradicional de Batalha Naval.\n");
    printf("2. Dois jogadores posicionam seus navios no tabuleiro.\n");
    printf("3. Os jogadores alternam turnos tentando acertar os navios adversários.\n");
    printf("4. Vence quem destruir todos os navios inimigos.\n\n");
}

int menuSECUNDARIO(void){
    int m;
    do{
        printf("\n======== MENU =========\n");
        printf(" 1 - CONTINUAR JOGANDO\n");
        printf(" 2 - SALVAR E SAIR DO JOGO\n");
        scanf("%d",&m);
        if(m != 1 && m != 2){
            printf("Opção inválida! Tente novamente.\n");
        }
    }while(m != 1 && m != 2);
    return m;
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


    // cria as estruturas
    portaAvioes pA1;
    navioTanque nT1;
    submarino sub1;
    bote bote1, bote2;


void mostrartabuleiro(char tab1[8][8]){ // funçao para mostrar tabuleiro de posiçao dos navios do jogador 1
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

void gravarTabuleiro(char tab1[8][8],FILE *arq){
    for(int i = 0 ; i< 8 ; i++){
        for(int p = 0 ; p < 8; p++){
            fputc(tab1[i][p],arq);
        }
        fputc('\n', arq);//deixar como está
    }
    fputc('\n', arq);// aspa simples pq estou adicionando um char, não uma string 
}

void lerTabuleiro(FILE *arq, char tab[8][8]){
    char c;
    for (int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
    /* le 1 caractere(ignora o \n)*/
    /*igora os caracteres de quebra de linha*/
        do{
            c =fgetc(arq);
        } while (c == '\n' || c == '\r'); /* \r - volta para o inicio da linha atual.*/
        tab[i][j] = c; /* guarda os caracteres na matriz*/
        }
    }
}

void posicaonavio (char tab[8][8], ponto posicoes[], int tamanho) {
    int linha, coluna, direcao;

    while (1) { // repete até colocar o navio corretamente

        do{
        printf("Escolha a linha (1-8): ");
        scanf("%d", &linha);
        if(linha < 1|| linha > 8)
            printf("Posição inválida! Fora do tabuleiro!");
        }while(linha < 1 || linha > 8);
        do{
        printf("Escolha a coluna (1-8): ");
        scanf("%d", &coluna);
        if(coluna < 1|| coluna > 8)
            printf("Posição inválida! Fora do tabuleiro!");
        }while(coluna < 1 || coluna > 8);

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

void posicionarNavio(char tab1[8][8],int jogador,FILE*arq){

    fprintf("Tabuleiro Defesa Jogador %d",jogador);

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

    gravarTabuleiro(tab1,arq);// grava o tabuleiro de posicoes inicial do tabuleiro de defesa
}

// função para checar se um navio foi afundado
int navioAfundado(ponto posicoes[], int tamanho, char tab[8][8]) {
    for (int i = 0; i < tamanho; i++) {
        if (tab[posicoes[i].x][posicoes[i].y] != 'X') {
        return 0; // ainda tem parte viva
        }
    }
    return 1; // navio afundado
}


int acertosJog1 = 0;
int errosJog1 = 0;
int acertosJog2 = 0;
int errosJog2 = 0;
int contador_rodadas = 0;

void sair(){
    // fechando arquivos
    fclose(jogadas);
    fclose(tabjog1);
    fclose(tabjog2);
}

void fim_de_jogo(){
    sair();
 // removendo arquivos
 remove("rodadas.txt");
 remove("tabuleiros_jogador1.txt");
 remove("tabuleiros_jogador2.txt");

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

int rodadas(char tabDefesa[8][8], char tabAtaque[8][8], int jogador) {

    int linha, coluna,continua;
    int x, y;


    printf("\nPosiscione seu ataque: \n");
    printf("Escolha a linha (1-8): "); // receber as coordenadas
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

    //posição repetida
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

        // avisa se o navio afundou
if (navioAfundado(pA1.posicao, 5, tabDefesa))
    printf("O Porta-Aviões foi AFUNDADO!\n");

if (navioAfundado(nT1.posicao, 4, tabDefesa))
    printf("O Navio-Tanque foi AFUNDADO!\n");

if (navioAfundado(sub1.posicao, 3, tabDefesa))
    printf("O Submarino foi AFUNDADO!\n");

if (navioAfundado(bote1.posicao, 2, tabDefesa))
    printf("O Bote 1 foi AFUNDADO!\n");

if (navioAfundado(bote2.posicao, 2, tabDefesa))
    printf("O Bote 2 foi AFUNDADO!\n");

    }
    else {
        printf("\nERROU!\n");
        tabAtaque[x][y] = 'O';       // marca erro no tabuleiro de ataque
        if (tabDefesa[x][y] == '~')
            tabDefesa[x][y] = 'O';   // marca erro também no tabuleiro de defesa

            if (jogador == 1) errosJog1++;
        else errosJog2++;
    }

    // imprimir tabuleiro ataque FEITO
    printf("\nTabuleiro de ataques:\n");
    mostrartabuleiro(tabAtaque);

if (jogador == 2){
    contador_rodadas++;
    printf("\n===== PLACAR =====\n");
    printf("Rodadas jogadas: %d\n\n", contador_rodadas);

    printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
    printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);
    // todo fim de rodada: guardar informações no arquivo
    // Obs: rodada termina depois que o jogador 2 jogar
    fprintf(jogadas,"Rodadas jogadas: %d\n\n", contador_rodadas);

    fprintf(jogadas,"Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
    fprintf(jogadas,"Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);

    gravarTabuleiro(tabDefesa,tabjog1);//grava tabuleiro de defesa do jogador 1

    // para saber se continua o jogo ou salva para continuar depois
    continua = menuSECUNDARIO();
    if(continua == 2){
        sair();// fechar arquivos
        return 1;// indicador de q quer sair do jogo    }
    }
}else{
    gravarTabuleiro(tabDefesa,tabjog2);//grava tabuleiro de defesa do jogador 2
}
    // fim : indentificar quando todos os navios de um jogador foram derrubados FEITO
    int acabou = aindaTemNavio(tabDefesa);

    if (acabou) {
        printf("\nTODOS OS NAVIOS DO INIMIGO FORAM DESTRUÍDOS! \n");
        printf("\nFIM DE JOGO! \n");
        fim_de_jogo();// fecha arquivos e apaga
    }
}

void loop_jogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]){
     // loop até alguém vencer ou querer parar o jogo
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
 if (rodadas(tab1, ataque2, 2)){
    break; // sair de novo jogo se rodadas retornar 1
    printf("\n Saindo e salvando o jogo...\n");
    printf("\nOBS: Você pode continuar esse jogo posteriormente, mas ele será perdido se iniciar um novo jogo\n");
 }
}
}


void continuarJogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]){
    FILE *jogadas, *tabjog1, *tabjog2;
    jogadas = fopen("rodadas.txt","r+"); // guarda erros,acertos e número de rodadas
    tabjog1 = fopen("tabuleiros_jogador1.txt","r+"); // guarda situaçao dos tabuleiros de ataque e defesa do jog 1
    tabjog2 = fopen("tabuleiros_jogador2.txt","r+"); // guarda situação dos tabuleiros de ataque e defesa do jog 2
    // mostrar erro se tentar continuar jogando sem haver um jogo anterior
    //nesse caso, não permitir e talvez levar ao menu novamente
    //pegar tabuleiro do arquivo.
    /* vê se os arquivos existe*/
if ( jogadas ==  NULL || tabjog1 == NULL ||tabjog2 == NULL){
    printf("Nenhum jogo encontrado.\n");
    exit(1); /* sai do loop*/
}
/*le os tabuleiros dos arquivos*/
lerTabuleiro(tabjog1, tab1); /* carrega o tabuleiro do jogador 1.*/
lerTabuleiro(tabjog2, tab2); /* carrega o tabuleiro do jogador2 */

fclose(tabjog1); /*fecha os arquivos*/
fclose(tabjog2);
    // Jogador 1
    mostrartabuleiro(tab1); // exibe o tabuleiro do jogador 1
    system("clear"); // limpa a tela - se for abrir no linux tem que colocar system("clear")

    // Jogador 2
    mostrartabuleiro(tab2); // exibe o tabuleiro do jogador 2
    system("clear"); // limpa a tela - se for abrir no linux tem que colocar system("clear")

    printf("\n===== CONTINUANDO AS RODADAS =====\n");
    loop_jogo(tab1,tab2,ataque1,ataque2);
}

void novoJogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]){
    // abrindo arquivos para nova partida
    jogadas = fopen("rodadas.txt","w+"); // guarda erros,acertos e número de rodadas
    tabjog1 = fopen("tabuleiros_jogador1.txt","w+"); // guarda situaçao dos tabuleiros de ataque e defesa do jog 1
    tabjog2 = fopen("tabuleiros_jogador2.txt","w+"); // guarda situação dos tabuleiros de ataque e defesa do jog 2

     // Jogador 1
    mostrartabuleiro(tab1); // exibe o tabuleiro do jogador 1
    posicionarNavio(tab1,1,tabjog1); // começa o posicionamento dos navios do jogador 1


    system("clear"); // limpa a tela - se for abrir no linux tem que colocar system("clear")
    printf("\nTabuleiro do Jogador 1 salvo!\nAgora o Jogador 2 irá posicionar os navios.\n");

    // Jogador 2
    mostrartabuleiro(tab2); // exibe o tabuleiro do jogador 2
    posicionarNavio(tab2,2,tabjog2); // começa o posicionamento dos navios do jogador 2

    system("clear"); // limpa a tela - se for abrir no linux tem que colocar system("clear")

    printf("\n===== INICIANDO AS RODADAS =====\n");

    loop_jogo(tab1,tab2,ataque1,ataque2);
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
        printf("Escolha uma opção (1-4): ");

        scanf("%d", &opcao); // lê opção do jogador

        switch(opcao) { // estrutura switch para tratar cada opção do menu
            case 1:
                //grava por cima do jogo anterior, se houver
                printf("\n Novo jogo \n\n");
                novoJogo(tab1,tab2,ataque1,ataque2);
                break;

            case 2:
                // Obs: só é possível resgatar o jogo imediatamente anterior
                printf("\n Continuar jogo :( \n");
                continuarJogo(tab1,tab2,ataque1,ataque2);
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

    } while (opcao < 1|| opcao > 4 || opcao == 3); // o menu repete para opção inválida e para instruções

    return 0;
}
