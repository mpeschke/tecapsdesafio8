#ifndef CABECALHO_DESAFIO8_H_INCLUDED
#define CABECALHO_DESAFIO8_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

// Não há tipo booleano em C, portanto
// vamos criar um para facilitar a leitura
// de operações booleanas no código:
#define BOOL int
#define TRUE 1
#define FALSE 0

/*
 * Estrutura para organizar as informações de um jogo.
 *
*/
struct Placar {
    char mplayer;
    unsigned int m;
    char nplayer;
    unsigned int n;
    char winner;
} typedef stPlacar;

// Analisador léxico do placar
BOOL placarlexicalanalyser(const char *const sentence, stPlacar* pplacar);

/*
 * Inicia a captura dos comandos digitados pelo usuário.
 *
*/
void iniciaCOPADOMUNDO(void);

#endif
