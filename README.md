# 🚢 Batalha Naval em C

Jogo de Batalha Naval em C executado no terminal, para dois jogadores, com sistema de turnos, placar e salvamento de partida.


## Funcionalidades

* 2 jogadores locais
* Posicionamento manual de navios
* Sistema de turnos
* Registro de acertos e erros
* Identificação de navios afundados
* Salvar e continuar jogo

---

## Tabuleiro

* Tamanho: 8x8
* `~` água | `N` navio | `X` acerto | `O` erro

---

## Como executar

```bash
gcc batalha_naval.c -o batalha_naval
./batalha_naval
```

---

## Menu

1 - Novo Jogo
2 - Continuar Jogo
3 - Instruções
4 - Sair

---

## Salvamento

Arquivos gerados:

* `rodadas.txt`
* `tabuleiros_jogador1.txt`
* `tabuleiros_jogador2.txt`

Apenas o último jogo pode ser continuado.

---

## Fim de jogo

O jogo termina quando todos os navios de um jogador são destruídos. O placar final é salvo em `placar_final.txt`.

---

Projeto para prática de programação em C.
