#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// variáveis globais. elas podem ser utilizadas em qualquer função.
FILE* jogadas, *tabjog1, *tabjog2;


void instrucoes() { //exibe explicação das regras
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
		if (scanf("%d", &m) != 1) { // Leitura
            printf("Entrada inválida! Digite apenas números.\n");
            while (getchar() != '\n'); // Limpa buffer
            m = 0; // Força repetição
        }

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
} portaAvioes; // Porta-aviões - 5 células

typedef struct {
	ponto posicao [4];
} navioTanque; // Navio-tanque - 4 células
typedef struct {
	ponto posicao [3];
} submarino; // Submarino - 3 células

typedef struct {
	ponto posicao[2];
} bote; // Bote 2 células


// cria as estruturas do JOGADOR 1
portaAvioes pA1;
navioTanque nT1;
submarino sub1;
bote bote1, bote2;

// Estruturas do JOGADOR 2
portaAvioes pA2;
navioTanque nT2;
submarino sub2;
bote bote3, bote4; // numeração diferentes para evitar que haja confusão

// Variáveis globais do placar
int acertosJog1 = 0;
int errosJog1 = 0;
int acertosJog2 = 0;
int errosJog2 = 0;
int contador_rodadas = 0;

// Essa função vai ler o placar reesceito e a posição dos navios

void ler_placar_e_navios() {
	FILE *arq = fopen("rodadas.txt", "r");
	if (arq == NULL) {
  // Se o arquivo não existir, retorna para usar as variáveis globais (que são 0, ja que deu erro)

		return;
	}

	char linha[200];
	char tipoNavio[10];

	// Reseta o contador de rodadas e placares
	contador_rodadas = 0;
	acertosJog1 = 0; errosJog1 = 0;
	acertosJog2 = 0; errosJog2 = 0;
	int jogadorAtual = 0;

	// as variáveis globais serão atualizadas com o último valor lido
	  while (fgets(linha, sizeof(linha), arq)) {

        // Rodadas
        if (sscanf(linha, "Rodadas jogadas: %d", &contador_rodadas) == 1)
            continue;

        // Identifica jogador
        if (strncmp(linha, "Jogador 1:", 10) == 0) {
            jogadorAtual = 1;
            continue;
        }

        if (strncmp(linha, "Jogador 2:", 10) == 0) {
            jogadorAtual = 2;
            continue;
        }

        // Acertos
        if (sscanf(linha, "Acertos: %d",
            (jogadorAtual == 1 ? &acertosJog1 : &acertosJog2)) == 1)
            continue;

        // Erros
        if (sscanf(linha, "Erros: %d",
            (jogadorAtual == 1 ? &errosJog1 : &errosJog2)) == 1)
            continue;


		// tenta ler a etiqueta do navio, ou seja, como ele eh identificado, e todas as coordenadas (Geral)
		int coords[10];
		int num_lidos = sscanf(linha, "%s %d %d %d %d %d %d %d %d %d %d",
							   tipoNavio, &coords[0], &coords[1], &coords[2], &coords[3],
							   &coords[4], &coords[5], &coords[6], &coords[7], &coords[8],
							   &coords[9]);

		// Se conseguiu ler pelo menos a etiqueta e 2 coordenadas (para um Bote)
		if (num_lidos >= 3) {

			// Verifica o tipo de navio e carrega os dados
			if (strcmp(tipoNavio, "PA1") == 0 && num_lidos == 11) {
				for (int i = 0; i < 5; i++) {
					pA1.posicao[i].x = coords[i * 2];
					pA1.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "NT1") == 0 && num_lidos == 9) {
				for (int i = 0; i < 4; i++) {
					nT1.posicao[i].x = coords[i * 2];
					nT1.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "SUB1") == 0 && num_lidos == 7) {
				for (int i = 0; i < 3; i++) {
					sub1.posicao[i].x = coords[i * 2];
					sub1.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "BOTE1") == 0 && num_lidos == 5) {
				for (int i = 0; i < 2; i++) {
					bote1.posicao[i].x = coords[i * 2];
					bote1.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "BOTE2") == 0 && num_lidos == 5) {
				for (int i = 0; i < 2; i++) {
					bote2.posicao[i].x = coords[i * 2];
					bote2.posicao[i].y = coords[i * 2 + 1];
				}
			}
			// agora o jogador 2
			else if (strcmp(tipoNavio, "PA2") == 0 && num_lidos == 11) {
				for (int i = 0; i < 5; i++) {
					pA2.posicao[i].x = coords[i * 2];
					pA2.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "NT2") == 0 && num_lidos == 9) {
				for (int i = 0; i < 4; i++) {
					nT2.posicao[i].x = coords[i * 2];
					nT2.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "SUB2") == 0 && num_lidos == 7) {
				for (int i = 0; i < 3; i++) {
					sub2.posicao[i].x = coords[i * 2];
					sub2.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "BOTE3") == 0 && num_lidos == 5) {
				for (int i = 0; i < 2; i++) {
					bote3.posicao[i].x = coords[i * 2];
					bote3.posicao[i].y = coords[i * 2 + 1];
				}
			} else if (strcmp(tipoNavio, "BOTE4") == 0 && num_lidos == 5) {
				for (int i = 0; i < 2; i++) {
					bote4.posicao[i].x = coords[i * 2];
					bote4.posicao[i].y = coords[i * 2 + 1];
				}
			}
		}
	}
	fclose(arq);
}
void mostrartabuleiro(char tab1[8][8]) { // função para mostrar tabuleiro de posiçao dos navios do jogador 1

	printf("  "); // recurso estético
	for(int p=1; p<9; p++) //numeração das colunas
		printf("%d ",p);
	printf("\n");
	for(int i=0; i<8; i++) {
		printf("%d ",i+1); // numeração das linhas
		for(int k=0; k<8; k++) {
			printf("%c ",tab1[i][k]); // composicao da matriz
		}
		printf("\n");
	}
}


void inicializartabuleiro(char tab1[8][8]) { // funçao para inicializar o tabuleiro
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
		fputc('\n', arq);//deixar como está
	}
	fputc('\n', arq);// aspa simples pq estou adicionando um char, não uma string
}

void lerTabuleiro(FILE *arq, char tab[8][8]) {
	char c;
	// fseek garante a leitura do início
	fseek(arq, 0, SEEK_SET); // Volta o ponteiro para o início para garantir a leitura do início
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

	while (1) { // repete até colocar o navio corretamente

		do {
			printf("Escolha a linha (1-8): ");
			if (scanf("%d", &linha) != 1) { // leitura
                printf("Entrada inválida! Digite apenas números.\n");
                while (getchar() != '\n'); // Limpa buffer
                linha = 0; // força repetição
            }
			if(linha < 1|| linha > 8) {
				printf("Posicao invalida! Fora do tabuleiro!");
			} //Um navio nao pode ser posicionado fora do tabuleiro
		} while(linha < 1 || linha > 8);
		do {
			printf("Escolha a coluna (1-8): ");
			if (scanf("%d", &coluna) != 1) { // Leitura
                printf("Entrada inválida! Digite apenas números.\n");
                while (getchar() != '\n'); // Limpa buffer
                coluna = 0; // Força repetição
            }
			if(coluna < 1|| coluna > 8) {
				printf("Posicao invalida! Fora do tabuleiro!"); // Um navio nao pode ser posicionado fora do tabuleiro.
			}
			} while(coluna < 1 || coluna > 8);

			// converte para o índice começando em 0
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
				if (scanf("%d", &direcao) != 1) {
                    printf("Entrada inválida! Digite apenas números.\n");
                    while (getchar() != '\n'); // Limpa buffer
                    direcao = 0; // Força repetição
                }
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
				if (tab[x][y] != '~') { // Dois navios não podem ocupar a mesma posição
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
	void posicionarNavio (char tab1[8][8],int jogador,FILE*arq) {

			printf("Tabuleiro Defesa Jogador %d",jogador);

			printf("\n");
			printf("Vamos posicionar os navios! \n");
			printf("Vamos começar pelo Porta-Avião.\n");

			if (jogador == 1) {
          printf("\n=== Porta-Aviões (5 espaços) ===\n");
            posicaonavio(tab1, pA1.posicao, 5); // Porta-aviões - 5 células

			printf("\n=== Navio-Tanque (4 espaços) ===\n");
            posicaonavio(tab1, nT1.posicao, 4);

        printf("\n=== Submarino (3 espaços) ===\n");
       		posicaonavio(tab1, sub1.posicao, 3);

    		printf("\n=== Bote 1 (2 espaços) ===\n");
            posicaonavio(tab1, bote1.posicao, 2);

          printf("\n=== Bote 2 (2 espaços) ===\n");
        	posicaonavio(tab1, bote2.posicao, 2);

            } else { // Jogador 2
        printf("\n=== Porta-Aviões (5 espaços) ===\n");
         posicaonavio(tab1, pA2.posicao, 5);

          printf("\n=== Navio-Tanque (4 espaços) ===\n");
            posicaonavio(tab1, nT2.posicao, 4);
          printf("\n=== Submarino (3 espaços) ===\n");
         posicaonavio(tab1, sub2.posicao, 3);

          printf("\n=== Bote 1 (2 espaços) ===\n");
         posicaonavio(tab1, bote3.posicao, 2);

      	 printf("\n=== Bote 2 (2 espaços) ===\n");
       	 posicaonavio(tab1, bote4.posicao, 2);
            }
			gravarTabuleiro(tab1,arq);// grava o tabuleiro de posicoes inicial no tabuleiro de defesa
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


		void reconstruirNavio(char tab[8][8], ponto posicoes[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		int x = posicoes[i].x;
		int y = posicoes[i].y;
		// a função ler_placar_e_navios já restaurou as coordenadas do navio
		// agr vamos ter a certeza se a posição não acertada volta para 'N'
		if (tab[x][y] != 'X' && tab[x][y] != 'O') { // Se não foi acerto ('X') ou erro ('O')
			tab[x][y] = 'N'; // Garante que a posição do navio esteja marcada
		}
	}
}
		void sair() {
			// fechando arquivos
			if(jogadas) fclose(jogadas);
			if(tabjog1) fclose(tabjog1);
			if(tabjog2) fclose(tabjog2);
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


			printf("\nPosicione seu ataque: \n");// Cada jogada consiste em informar coordenadas (linha e coluna) para atacar o tabuleiro inimigo


            // Leitura e validação da LINHA
            while (1) {
                printf("Escolha a linha (1-8): ");
                if (scanf("%d", &linha) != 1) { // Garante que o input é um número
                    printf("Entrada inválida! Digite apenas números.\n");
                    while (getchar() != '\n'); // Limpa buffer
                } else if (linha < 1 || linha > 8) {
                    printf("Linha fora do tabuleiro! Escolha entre 1 e 8.\n");
                } else {
                    break;
                }
            }

            // Leitura e verificação da coluna
            while (1) {
                printf("Escolha a coluna (1-8): ");
                if (scanf("%d", &coluna) != 1) { // Garante que o input é um número
                    printf("Entrada inválida! Digite apenas números.\n");
                    while (getchar() != '\n'); // Limpa buffer
                } else if (coluna < 1 || coluna > 8) {
                    printf("Coluna fora do tabuleiro! Escolha entre 1 e 8.\n");
                } else {
                    break;
                }
            }

			// converter para índice da matriz
			x = linha - 1;
			y = coluna - 1;

			//posição repetida
			if (tabAtaque[x][y] == 'X' || tabAtaque[x][y] == 'O') {
				printf("\n Você já jogou nessa posição! Rodada perdida.\n");
				return 0;
			}

			// verificar se acertou
			if (tabDefesa[x][y] == 'N') {
				printf("\nAcertou o navio inimigo! \n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado).
				tabDefesa[x][y] = 'X';    // Acerto (Corrigido o caractere invisível)
				tabAtaque[x][y] = 'X';    // marca acerto no tabuleiro de ataque (Corrigido o caractere invisível)

				if (jogador == 1) acertosJog1++;
				else acertosJog2++;
				if (jogador == 1) { // JOGADOR 1 ATACANDO (Defensor é o JOGADOR 2)
          int defensor = 2; // O jogador 2 que está perdendo o navio.

          if (navioAfundado(pA2.posicao, 5, tabDefesa))
          	printf("O Porta-Aviões do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(nT2.posicao, 4, tabDefesa))
          	printf("O Navio-Tanque do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(sub2.posicao, 3, tabDefesa))
          	printf("O Submarino do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(bote3.posicao, 2, tabDefesa))
          	printf("O Bote 1 do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(bote4.posicao, 2, tabDefesa))
          	printf("O Bote 2 do Jogador %d foi AFUNDADO!\n", defensor);

        } else { // aqui o jogador 2 que esta atacando (defensor é o jogador 1)
          int defensor = 1; // O jogador 1 que está perdendo o navio.

          if (navioAfundado(pA1.posicao, 5, tabDefesa))
          	printf("O Porta-Aviões do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(nT1.posicao, 4, tabDefesa))
          	printf("O Navio-Tanque do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(sub1.posicao, 3, tabDefesa))
          	printf("O Submarino do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(bote1.posicao, 2, tabDefesa))
          	printf("O Bote 1 do Jogador %d foi AFUNDADO!\n", defensor);

          if (navioAfundado(bote2.posicao, 2, tabDefesa))
          	printf("O Bote 2 do Jogador %d foi AFUNDADO!\n", defensor);
        }
			}else {
				printf("\nERROU!\n"); // O sistema deve indicar o resultado do ataque (acerto, erro ou navio afundado).
				tabAtaque[x][y] = 'O';    // marca erro no tabuleiro de ataque (Corrigido o caractere invisível)
				if (tabDefesa[x][y] == '~')
					tabDefesa[x][y] = 'O';    // marca erro também no tabuleiro de defesa (Corrigido o caractere invisível)

				if (jogador == 1) errosJog1++;
				else errosJog2++;
			}

			// imprimir tabuleiro ataque
			printf("\nTabuleiro de ataques:\n");
			mostrartabuleiro(tabAtaque);

			// fim : indentificar quando todos os navios de um jogador foram derrubados FEITO
			int acabou = aindaTemNavio(tabDefesa);

			if (!acabou) {
				// O jogo termina quando todos os navios de um jogador forem destruídos.
				printf("\nTODOS OS NAVIOS DO INIMIGO FORAM DESTRUIDOS! \n");
				printf("\nFIM DE JOGO! \n");
				// O vencedor sera exibido no loop_jogo
				return 1; // Indicador de que o jogo acabou (vencedor definido)
			}


			if (jogador == 2) {
				contador_rodadas++;
				printf("\n===== PLACAR =====\n");
				printf("Rodadas jogadas: %d\n\n", contador_rodadas);
				printf("Jogador 1:\nAcertos: %d\nErros: %d\n\n", acertosJog1, errosJog1);
				printf("Jogador 2:\nAcertos: %d\nErros: %d\n\n", acertosJog2, errosJog2);

				// reabre a rodada em modo 'w' para LIMPAR o conteúdo
				jogadas = fopen("rodadas.txt", "w");
				if (jogadas == NULL) { /* Trata o erro */ return -1; }

				// 1. Imprime o novo placar
				fprintf(jogadas, "Rodadas jogadas: %d\n\n", contador_rodadas);

fprintf(jogadas, "Jogador 1:\n");
fprintf(jogadas, "Acertos: %d\n", acertosJog1);
fprintf(jogadas, "Erros: %d\n\n", errosJog1);

fprintf(jogadas, "Jogador 2:\n");
fprintf(jogadas, "Acertos: %d\n", acertosJog2);
fprintf(jogadas, "Erros: %d\n\n", errosJog2);

				// 2. vamos salvar as posiçoes dos navios (jogador 1 e 2), crucial para " continuar"
				fprintf(jogadas, "PA1 %d %d %d %d %d %d %d %d %d %d\n",
					pA1.posicao[0].x, pA1.posicao[0].y, pA1.posicao[1].x, pA1.posicao[1].y,
					pA1.posicao[2].x, pA1.posicao[2].y, pA1.posicao[3].x, pA1.posicao[3].y,
					pA1.posicao[4].x, pA1.posicao[4].y);
				fprintf(jogadas, "NT1 %d %d %d %d %d %d %d %d\n",
					nT1.posicao[0].x, nT1.posicao[0].y, nT1.posicao[1].x, nT1.posicao[1].y,
					nT1.posicao[2].x, nT1.posicao[2].y, nT1.posicao[3].x, nT1.posicao[3].y);
				fprintf(jogadas, "SUB1 %d %d %d %d %d %d\n",
					sub1.posicao[0].x, sub1.posicao[0].y, sub1.posicao[1].x, sub1.posicao[1].y,
					sub1.posicao[2].x, sub1.posicao[2].y);
				fprintf(jogadas, "BOTE1 %d %d %d %d\n",
					bote1.posicao[0].x, bote1.posicao[0].y, bote1.posicao[1].x, bote1.posicao[1].y);
				fprintf(jogadas, "BOTE2 %d %d %d %d\n",
					bote2.posicao[0].x, bote2.posicao[0].y, bote2.posicao[1].x, bote2.posicao[1].y);

				fprintf(jogadas, "PA2 %d %d %d %d %d %d %d %d %d %d\n",
					pA2.posicao[0].x, pA2.posicao[0].y, pA2.posicao[1].x, pA2.posicao[1].y,
					pA2.posicao[2].x, pA2.posicao[2].y, pA2.posicao[3].x, pA2.posicao[3].y,
					pA2.posicao[4].x, pA2.posicao[4].y);
				fprintf(jogadas, "NT2 %d %d %d %d %d %d %d %d\n",
					nT2.posicao[0].x, nT2.posicao[0].y, nT2.posicao[1].x, nT2.posicao[1].y,
					nT2.posicao[2].x, nT2.posicao[2].y, nT2.posicao[3].x, nT2.posicao[3].y);
				fprintf(jogadas, "SUB2 %d %d %d %d %d %d\n",
					sub2.posicao[0].x, sub2.posicao[0].y, sub2.posicao[1].x, sub2.posicao[1].y,
					sub2.posicao[2].x, sub2.posicao[2].y);
				fprintf(jogadas, "BOTE3 %d %d %d %d\n",
					bote3.posicao[0].x, bote3.posicao[0].y, bote3.posicao[1].x, bote3.posicao[1].y);
				fprintf(jogadas, "BOTE4 %d %d %d %d\n",
					bote4.posicao[0].x, bote4.posicao[0].y, bote4.posicao[1].x, bote4.posicao[1].y);

				// Reabre em "r+" para que o loop de ataque continue
				fclose(jogadas);
				jogadas = fopen("rodadas.txt", "r+");
				if (jogadas == NULL) { return -1; }


				//salvamento de tabjog1
				fclose(tabjog1);
				tabjog1 = fopen("tabuleiros_jogador1.txt", "w");
				if (tabjog1 == NULL) { return -1; }
				gravarTabuleiro(tabDefesa, tabjog1);
				fclose(tabjog1);
				tabjog1 = fopen("tabuleiros_jogador1.txt", "r+");
				if (tabjog1 == NULL) { return -1; }


				// para saber se continua o jogo ou salva para continuar depois
				continua = menuSECUNDARIO();
				if(continua == 2) {
					sair();// fechar arquivos
					return 2;// indicador de q quer salvar e sair
				}
			} else {
				fclose(tabjog2);
				tabjog2 = fopen("tabuleiros_jogador2.txt", "w");
				if (tabjog2 == NULL) { return -1; }
				gravarTabuleiro(tabDefesa,tabjog2);
				fclose(tabjog2);
				tabjog2 = fopen("tabuleiros_jogador2.txt", "r+");
				if (tabjog2 == NULL) { return -1; }
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

			jogadas = fopen("rodadas.txt","r+");
			tabjog1 = fopen("tabuleiros_jogador1.txt","r+");
			tabjog2 = fopen("tabuleiros_jogador2.txt","r+");

			// Checa se os arquivos existem (se algum for NULL, o jogo não pode continuar)
			if ( jogadas == NULL || tabjog1 == NULL || tabjog2 == NULL) {
				printf("\nNenhum jogo salvo encontrado. Por favor, inicie um Novo Jogo.\n");

				// Garante que qualquer arquivo que tenha sido aberto seja fechado
				if (jogadas) fclose(jogadas);
				if (tabjog1) fclose(tabjog1);
				if (tabjog2) fclose(tabjog2);

				return;
			}

			//  Carrega os tabuleiros de DEFESA
			lerTabuleiro(tabjog1, tab1);
			lerTabuleiro(tabjog2, tab2);

			//  Carrega o Placar E POSIÇÕES DOS NAVIOS
			ler_placar_e_navios(); // Função para restaurar o placar global e posições

		// reconstruir posições dos navios do jogador 1 e 2
		reconstruirNavio(tab1, pA1.posicao, 5);
		reconstruirNavio(tab1, nT1.posicao, 4);
		reconstruirNavio(tab1, sub1.posicao, 3);
		reconstruirNavio(tab1, bote1.posicao, 2);
		reconstruirNavio(tab1, bote2.posicao, 2);

		reconstruirNavio(tab2, pA2.posicao, 5);
		reconstruirNavio(tab2, nT2.posicao, 4);
		reconstruirNavio(tab2, sub2.posicao, 3);
		reconstruirNavio(tab2, bote3.posicao, 2);
		reconstruirNavio(tab2, bote4.posicao, 2);


			//trconstroi tabuleiro de ataque
			for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tab2[i][j] == 'X')
				ataque1[i][j] = 'X';
			else if (tab2[i][j] == 'O')
				ataque1[i][j] = 'O';
			else
				ataque1[i][j] = '~';

			if (tab1[i][j] == 'X')
				ataque2[i][j] = 'X';
			else if (tab1[i][j] == 'O')
				ataque2[i][j] = 'O';
			else
				ataque2[i][j] = '~';
		}
}


			printf("\n===== CONTINUANDO AS RODADAS =====\n");

			loop_jogo(tab1,tab2,ataque1,ataque2);
		}
		void novoJogo(char tab1[8][8], char tab2[8][8], char ataque1[8][8],char ataque2[8][8]) {

			// abrindo arquivos para nova partida
			jogadas = fopen("rodadas.txt","w+");
			tabjog1 = fopen("tabuleiros_jogador1.txt","w+");
			tabjog2 = fopen("tabuleiros_jogador2.txt","w+");

			// inicializa navios (código original)
			inicializartabuleiro(tab1);
			posicionarNavio(tab1,1,tabjog1);

			inicializartabuleiro(tab2);
			posicionarNavio(tab2,2,tabjog2);

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
				printf("      BATALHA NAVAL - MENU\n");
				printf("==============================\n");
				printf("1 - Novo Jogo\n");
				printf("2 - Continuar Jogo\n");
				printf("3 - Instruções\n");
				printf("4 - Sair do Jogo\n");
				printf("Escolha uma opção (1-4): ");

				if (scanf("%d", &opcao) != 1) { // Leitura  do Menu Principal
                    printf("\nOpção inválida! Digite apenas números.\n");
                    while (getchar() != '\n'); // Limpa buffer
                    opcao = 0; // Força repetição
                }

				switch(opcao) { // estrutura switch para lidar cm cada opção do menu
				case 1:
					//grava por cima do jogo anterior, se houver
					printf("\n Novo jogo \n\n");
					// Resetar placares  para Novo Jogo
					acertosJog1 = 0; errosJog1 = 0;
					acertosJog2 = 0; errosJog2 = 0;
					contador_rodadas = 0;
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
					printf("\nOpção inválida! Tente novamente.\n");
				}

			} while (opcao < 1|| opcao > 4 || opcao == 3); // o menu repete para opção inválida e para instruções

	return 0;
		}
