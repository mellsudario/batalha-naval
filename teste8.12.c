#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd> // serve para que o sistema operacional modifique algo (como mudar o tamanho de um arquivo, criar um novo processo, ou ler dados de baixo nível)

FILE*jogadas,*tabjog1,*tabjog2;


void instrucoes() { // exibe explicação das regras
    printf("\n===== INSTRUÇÕES =====\n");
    printf("1. O jogo segue o modelo tradicional de Batalha Naval.\n");
    printf("2. Dois jogadores posicionam seus navios no tabuleiro.\n");
    printf("3. Os jogadores alternam turnos tentando acertar os navios adversários.\n");
    printf("4. Vence quem destruir todos os navios inimigos.\n\n");
}

int menuSECUNDARIO(void) {
	int m;
	do {
		printf("\n======== MENU =========\n");
		printf(" 1 - CONTINUAR JOGANDO\n");
		printf(" 2 - SALVAR E SAIR DO JOGO\n");
		scanf("%d",&m);
		if(m != 1 && m != 2) {
			printf("Opção inválida! Tente novamente.\n");
		}
	} while(m != 1 && m != 2);
	return m;
}


typedef struct {
	int x;
	int y;
} ponto;

typedef struct {
	ponto posicao [5];
} portaAvioes; // Porta-aviC5es - 5 cC)lulas

typedef struct {
	ponto posicao [4];
} navioTanque; // Navio-tanque - 4 cC)lulas
typedef struct {
	ponto posicao [3];
} submarino; // Submarino - 3 cC)lulas

typedef struct {
	ponto posicao[2];
} bote; // Bote 2 cC)lulas


// cria as estruturas
portaAvioes pA1;
navioTanque nT1;
submarino sub1;
bote bote1, bote2;

// VariC!veis globais de Placar
int acertosJog1 = 0;
int errosJog1 = 0;
int acertosJog2 = 0;
int errosJog2 = 0;
int contador_rodadas = 0;


// Essa função vai ler o placar atual (acertos, erros, rodadas) do arquivo
void ler_placar_salvo() {
	FILE *arq = fopen("rodadas.txt", "r");
	if (arq == NULL) {
		// Se o arquivo nC#o existir, retorna para usar as variC!veis globais (que sC#o 0)
		return;
	}

	char linha[200];
	int lendoJog1 = 0, lendoJog2 = 0;

	// As variC!veis globais (acertosJog1, etc) serC#o atualizadas com o C:ltimo valor lido

	while (fgets(linha, sizeof(linha), arq)) {
		// Tenta ler o contador de rodadas
		sscanf(linha, "Rodadas jogadas: %d", &contador_rodadas); // sscanf = scanf de string

		if (strstr(linha, "Jogador 1:")) { // strstr é uma função da biblioteca string.h que procura/ verifica palavras específicas em uma frase.
			lendoJog1 = 1;
			lendoJog2 = 0;
		} else if (strstr(linha, "Jogador 2:")) {
			lendoJog1 = 0;
			lendoJog2 = 1;
		} else if (lendoJog1) {
			// Se estiver na seC'C#o do Jogador 1, tenta ler os acertos/erros
			sscanf(linha, "Acertos: %d", &acertosJog1);
			sscanf(linha, "Erros: %d", &errosJog1);
		} else if (lendoJog2) {
			// Se estiver na seC'C#o do Jogador 2, tenta ler os acertos/erros
			sscanf(linha, "Acertos: %d", &acertosJog2);
			sscanf(linha, "Erros: %d", &errosJog2);
		}
	}
	fclose(arq);
}
void mostrartabuleiro(char tab1[8][8]) { // funC'ao para mostrar tabuleiro de posiC'ao dos navios do jogador 1

	printf("  "); // recurso estC)tico
	for(int p=1; p<9; p++) //numeraC'C#o das colunas
		printf("%d ",p);
	printf("\n");
	for(int i=0; i<8; i++) {
		printf("%d ",i+1); // numeraC'C#o das linhas
		for(int k=0; k<8; k++) {
			printf("%c ",tab1[i][k]); // composicao da matriz
		}
		printf("\n");
	}
}


void inicializartabuleiro(char tab1[8][8]) { // funC'ao para inicializar o tabuleiro
	for(int i=0; i<8; i++) {
		for(int k=0; k<8; k++) {
			tab1[i][k] = '~';
		}
	}
}

void gravarTabuleiro(char tab1[8][8],FILE *arq) {
	for(int i = 0 ; i< 8 ; i++) {
		for(int p = 0 ; p < 8; p++) {
			fputc(tab1[i][p],arq);
		}
		fputc('\n', arq);//deixar como estC!
	}
	fputc('\n', arq);// aspa simples pq estou adicionando um char, nC#o uma string
}

void lerTabuleiro(FILE *arq, char tab[8][8]) {
	char c;
	fseek(arq, 0, SEEK_SET); // Volta o ponteiro para o inC-cio para garantir a leitura do inC-cio
	for (int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			/* le 1 caractere(ignora o \n)*/
			//agora os caracteres de quebra de linha/
			do {
				c =fgetc(arq);
			} while (c == '\n' || c == '\r' || c == EOF); /* \r - volta para o inicio da linha atual.*/
			tab[i][j] = c; /* guarda os caracteres na matriz*/
		}
	}
}

void posicaonavio (char tab[8][8], ponto posicoes[], int tamanho) {
	int linha, coluna, direcao;

	while (1) { // repete atC) colocar o navio corretamente

		do {
			printf("Escolha a linha (1-8): ");
			scanf("%d", &linha);
			if(linha < 1|| linha > 8) {
				printf("Posicao invalida! Fora do tabuleiro!");
			} //Um navio nao pode ser posicionado fora do tabuleiro
		} while(linha < 1 || linha > 8);
		do {
			printf("Escolha a coluna (1-8): ");
			scanf("%d", &coluna);
			if(coluna < 1|| coluna > 8) {
				printf("Posicao invalida! Fora do tabuleiro!"); // Um navio nao pode ser posicionado fora do tabuleiro.
			}
			while(coluna < 1 || coluna > 8);

			// converte para C-ndice comeC'ando em 0
			linha--;
			coluna--;

			// escolhe direC'C#o
			printf("Escolha a direC'C#o:\n");
			do {
				printf("1 - Norte\n");
				printf("2 - Sul\n");
				printf("3 - Leste\n");
				printf("4 - Oeste\n");
				printf("Digite a direC'C#o (1-4): ");
				scanf("%d", &direcao);
				if (direcao < 1 || direcao > 4) {
					printf("\nDireC'C#o invC!lida! Escolha um valor entre 1 e 4.\n\n");
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

				// verifica colisC#o com outro navio
				if (tab[x][y] != '~') { // Dois navios nC#o podem ocupar a mesma posiC'C#o
					pode = 0;
					break;
				}
			}

			if (pode == 0) {
				printf("\n PosiC'C#o invC!lida! O navio nC#o cabe ou colide com outro.\n\n");
				continue; // repete desde o inC-cio
			}


			// se chegou aqui,pode==1 e agora grava posiC'C5es
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
		void posicionarNavio(char tab1[8][8],int jogador,FILE*arq) {

			printf("Tabuleiro Defesa Jogador %d",jogador);

			printf("\n");
			printf("Vamos posicionar os navios! \n");
			printf("Vamos comeC'ar pelo Porta-AviC#o.\n");

			printf("\n=== Porta-AviC5es (5 espaC'os) ===\n");
			posicaonavio(tab1, pA1.posicao, 5); // Porta-aviões - 5 células

			printf("\n=== Navio-Tanque (4 espaC'os) ===\n");
			posicaonavio(tab1, nT1.posicao, 4); // Navio-tanque - 4 células
			printf("\n=== Submarino (3 espaC'os) ===\n");
			posicaonavio(tab1, sub1.posicao, 3); //Submarino - 3 células

			printf("\n=== Bote 1 (2 espaC'os) ===\n");
			posicaonavio(tab1, bote1.posicao, 2); // Bote 2 células

			printf("\n=== Bote 2 (2 espaC'os) ===\n");
			posicaonavio(tab1, bote2.posicao, 2); // Bote 2 células

			gravarTabuleiro(tab1,arq);// grava o tabuleiro de posicoes inicial do tabuleiro de defesa
		}

// funC'C#o para checar se um navio foi afundado
		int navioAfundado(ponto posicoes[], int tamanho, char tab[8][8]) {
			for (int i = 0; i < tamanho; i++) {
				if (tab[posicoes[i].x][posicoes[i].y] != 'X') {
					return 0; // ainda tem parte viva
				}
			}
			return 1; // navio afundado
		}


		void sair() {
			// fechando arquivos
			fclose(jogadas);
			fclose(tabjog1);
			fclose(tabjog2);
		}

		void fim_de_jogo() {


			// Abrir o arquivo no modo 'w' para garantir que ele tenha apenas o placar final
			FILE *final_placar = fopen("placar_final.txt", "w");
			if (final_placar) {
				fprintf(final_placar,"\n===== PLACAR FINAL =====\n");
				fprintf(final_placar,"Rodadas jogadas: %d\n\n", contador_rodadas);
				fprintf(final_placar,"Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
				fprintf(final_placar,"Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);
				fclose(final_placar);
			}

			sair();
// removendo arquivos de jogo em andamento
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


			printf("\nPosiscione seu ataque: \n");// Cada jogada consiste em informar coordenadas (linha e coluna) para atacar o tabuleiro inimigo
			printf("Escolha a linha (1-8): "); // receber as coordenadas
			scanf("%d", &linha);
			printf("Escolha a coluna (1-8): ");
			scanf("%d", &coluna);

			// converter para índice da matriz
			x = linha - 1;
			y = coluna - 1;

			if (x < 0 || x > 7 || y < 0 || y > 7) {
				printf("\nCoordenada fora do tabuleiro! Rodada perdida.\n");
				return 0;
			}

			//posição repetida
			if (tabAtaque[x][y] == 'X' || tabAtaque[x][y] == 'O') {
				printf("\n Você já jogou nessa posição! Rodada perdida.\n");
				return 0;
			}

			// verificar se acertou
			if (tabDefesa[x][y] == 'N') {
				printf("\nAcertou o navio inimigo! \n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado)[cite: 17].
				tabDefesa[x][y] = 'X';       // acerto
				tabAtaque[x][y] = 'X';   // marca acerto no tabuleiro de ataque

				if (jogador == 1) acertosJog1++;
				else acertosJog2++;

				// avisa se o navio afundou
				if (navioAfundado(pA1.posicao, 5, tabDefesa))
					printf("O Porta-Aviões foi AFUNDADO!\n"); // O sistema vai indicar o resultado do ataque (acerto, erro ou navio afundado)

				if (navioAfundado(nT1.posicao, 4, tabDefesa))
					printf("O Navio-Tanque foi AFUNDADO!\n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado)

				if (navioAfundado(sub1.posicao, 3, tabDefesa))
					printf("O Submarino foi AFUNDADO!\n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado)
				if (navioAfundado(bote1.posicao, 2, tabDefesa))
					printf("O Bote 1 foi AFUNDADO!\n");// O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado)

				if (navioAfundado(bote2.posicao, 2, tabDefesa))
					printf("O Bote 2 foi AFUNDADO!\n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado)

			}
			else {
				printf("\nERROU!\n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado)[cite: 17].
				tabAtaque[x][y] = 'O';       // marca erro no tabuleiro de ataque
				if (tabDefesa[x][y] == '~')
					tabDefesa[x][y] = 'O';   // marca erro tambC)m no tabuleiro de defesa

				if (jogador == 1) errosJog1++;
				else errosJog2++;
			}

			// imprimir tabuleiro ataque
			printf("\nTabuleiro de ataques:\n");
			mostrartabuleiro(tabAtaque);

			// // fim : indentificar quando todos os navios de um jogador foram derrubados FEITO
			int acabou = aindaTemNavio(tabDefesa);

			if (!acabou) {
				// O jogo termina quando todos os navios de um jogador forem destruC-dos.
				printf("\nTODOS OS NAVIOS DO INIMIGO FORAM DESTRUIDOS! \n");
				printf("\nFIM DE JOGO! \n");
				// O vencedor sera exibido no loop_jogo
				return 1; // Indicador de que o jogo acabou (vencedor definido)
			}
// TODO = o que faremos no proximo passo.

			if (jogador == 2) {
				contador_rodadas++;
				printf("\n===== PLACAR =====\n");
				printf("Rodadas jogadas: %d\n\n", contador_rodadas);

				printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
				printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);

				// aqui ele vai sobrescrever o arquivo jogadas com o estado atual
				fseek(jogadas, 0, SEEK_SET);
				ftruncate(fileno(jogadas), 0);
				fprintf(jogadas,"Rodadas jogadas: %d\n\n", contador_rodadas);
				fprintf(jogadas,"Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
				fprintf(jogadas,"Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);

				// Gravar tabuleiros de defesa atualizados (Para o Jogador 1)
				fseek(tabjog1, 0, SEEK_SET);
				ftruncate(fileno(tabjog1), 0); //ftruncate executa a limpeza e o fileno eh o que permite que o arquivo ganhe um novo tamanho
				gravarTabuleiro(tabDefesa, tabjog1);

				// para saber se continua o jogo ou salva para continuar depois
				continua = menuSECUNDARIO();
				if(continua == 2) {
					sair();// fechar arquivos
					return 2;// indicador de q quer salvar e sair
				}
			} else {
				// Gravar tabuleiros de defesa atualizados (Para o Jogador 2)
				fseek(tabjog2, 0, SEEK_SET);
				ftruncate(fileno(tabjog2), 0);
				gravarTabuleiro(tabDefesa,tabjog2);
			}
			return 0; // Indicador de que o jogo continua
		}

		void loop_jogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]) {
			int status;
			// loop até alguém vencer ou querer parar o jogo
			while (1) {

				printf("\nVEZ DO JOGADOR 1 \n");
				status = rodadas(tab2, ataque1, 1);
				if (status == 1) { // Jogador 1 Venceu
					printf("\nJOGADOR 1 VENCEU O JOGO! \n");
					printf("\n===== PLACAR FINAL =====\n");
					printf("Rodadas jogadas: %d\n\n", contador_rodadas);
					printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
					printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);
					fim_de_jogo();
					break;
				}

				printf("\nVEZ DO JOGADOR 2 \n");
				status = rodadas(tab1, ataque2, 2);
				if (status == 1) { // Jogador 2 Venceu
					printf("\nJOGADOR 2 VENCEU O JOGO! \n");
					printf("\n===== PLACAR FINAL =====\n");
					printf("Rodadas jogadas: %d\n\n", contador_rodadas);
					printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
					printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);
					fim_de_jogo();
					break;
				} else if (status == 2) {
					printf("\n Saindo e salvando o jogo...\n");
					printf("\nOBS: Você pode continuar esse jogo posteriormente, mas ele será perdido se iniciar um novo jogo\n");
					break; // Sair para o menu principal
				}
			}
		}


		void continuarJogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]) {


			jogadas = fopen("rodadas.txt","r+"); // guarda erros,acertos e nC:mero de rodadas
			tabjog1 = fopen("tabuleiros_jogador1.txt","r+"); // guarda situaC'ao dos tabuleiros de ataque e defesa do jog 1
			tabjog2 = fopen("tabuleiros_jogador2.txt","r+"); // guarda situaC'C#o dos tabuleiros de ataque e defesa do jog 2
			// mostrar erro se tentar continuar jogando sem haver um jogo anterior
			//nesse caso, nC#o permitir e talvez levar ao menu novamente
			//pegar tabuleiro do arquivo.
			/* vC* se os arquivos existe*/
			// Checa se os arquivos existem (se algum for NULL, o jogo nC#o pode continuar)
			if ( jogadas == NULL || tabjog1 == NULL || tabjog2 == NULL) {
				printf("\nNenhum jogo salvo encontrado. Por favor, inicie um Novo Jogo.\n");

				// Garante que qualquer arquivo que tenha sido aberto seja fechado
				if (jogadas) fclose(jogadas);
				if (tabjog1) fclose(tabjog1);
				if (tabjog2) fclose(tabjog2);

				return;
			}

			// 1. Carrega os tabuleiros de DEFESA
			lerTabuleiro(tabjog1, tab1);
			lerTabuleiro(tabjog2, tab2);

			// 2. Carrega o Placar para ver as rodadas,  os acertos e os erros
			ler_placar_salvo(); // FunC'C#o  para restaurar o placar global




			// Exibe os tabuleiros carregados
			printf("\nTabuleiro Defesa Jogador 1 (Carregado):\n");
			mostrartabuleiro(tab1);

			printf("\nTabuleiro Defesa Jogador 2 (Carregado):\n");
			mostrartabuleiro(tab2);

			printf("\n===== CONTINUANDO AS RODADAS =====\n");

			loop_jogo(tab1,tab2,ataque1,ataque2);
		}

		void novoJogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]) {
			// abrindo arquivos para nova partida
			jogadas = fopen("rodadas.txt","w+"); // guarda erros,acertos e nC:mero de rodadas
			tabjog1 = fopen("tabuleiros_jogador1.txt","w+"); // guarda a situação dos tabuleiros de defesa do jog 1
			tabjog2 = fopen("tabuleiros_jogador2.txt","w+"); // guarda situação dos tabuleiros de defesa do jog 2

			// resetar placar e rodadas
			acertosJog1 = 0;
			errosJog1 = 0;
			acertosJog2 = 0;
			errosJog2 = 0;
			contador_rodadas = 0;

			// Jogador 1
			mostrartabuleiro(tab1); // exibe o tabuleiro do jogador 1
			posicionarNavio(tab1,1,tabjog1); // comeC'a o posicionamento dos navios do jogador 1


			system("clear"); // limpa a tela - se for abrir no linux tem que colocar system("clear")
			printf("\nTabuleiro do Jogador 1 salvo!\nAgora o Jogador 2 irC! posicionar os navios.\n");

			// Jogador 2
			mostrartabuleiro(tab2); // exibe o tabuleiro do jogador 2
			posicionarNavio(tab2,2,tabjog2); // comeC'a o posicionamento dos navios do jogador 2

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

			int opcao; // variC!vel usada para guardar a opC'C#o escolhida pelo usuC!rio no menu

			do {
				printf("\n==============================\n");
				printf("     BATALHA NAVAL - MENU\n");
				printf("==============================\n");
				printf("1 - Novo Jogo\n");
				printf("2 - Continuar Jogo\n");
				printf("3 - Instruções\n");
				printf("4 - Sair do Jogo\n");
				printf("Escolha uma opção (1-4): ");

				scanf("%d", &opcao); // lê opção do jogador

				switch(opcao) { // estrutura switch para lidar cm cada opção do menu
				case 1:
					//grava por cima do jogo anterior, se houver
					printf("\n Novo jogo \n\n");
					novoJogo(tab1,tab2,ataque1,ataque2);
					break;

				case 2:
					// Obs: será possivel resgatar o jogo imediatamente anterior
					printf("\n Tentando continuar jogo... \n");
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
					printf("\nOpC'C#o inválida! Tente novamente.\n");
				}

			} while (opcao < 1|| opcao > 4 || opcao == 3); // o menu repete para opção inválida e para instruções

    return 0;
}
