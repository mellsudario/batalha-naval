#ifndef NAVIOS_H // se não foi incluído ainda
#define NAVIOS_H // marca como incluído


// guarda as coordenadas de um navio
typedef struct {
    int x;
    int y;
} ponto;

// porta-aviõe que ocupa 5 espaços
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

void posicaonavio(ponto inicial);  // lê uma coordenada válida
void posicionarNavio(char tab[8][8]); // inicia posicionamento dos navios

#endif //fim do bloco iniciado por #ifndef.
