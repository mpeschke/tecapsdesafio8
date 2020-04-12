/*
* Desafio 8
* Matheus Peschke de Azevedo - RA: 20396209
* Gustavo Caldas de Souza - RA: 21352329
*/
#include "desafio8.h"

// Não há tipo booleano em C, portanto
// vamos criar um para facilitar a leitura
// de operações booleanas no código:
#define BOOL int
#define TRUE 1
#define FALSE 0

#define MAXJOGOS 15
#define MAXBUFFPLACAR 5
#define MINBUFFPLACAR 3
#define MAXBUFFGOLS 2
#define MAXGOLS 20

/*
 * Estrutura para organizar as informações de um indivíduo.
 *
*/
struct Placar {
    char mplayer;
    unsigned int m;
    char nplayer;
    unsigned int n;
    char winner;
} typedef stPlacar;

static stPlacar jogos[MAXJOGOS];

/*
 *
 *********************ANALISADOR LÉXICO (TOKENIZADOR)*****************************************
 *
 * Definições que fazem parte de um analisador léxico:
 *
 * 'sentença' = a linha de comando com todos os seus parâmetros
 * 'token' = um item que esteja separados dos outros por um ou mais espaços.
 * 'verbo' = o primeiro token, que identifica o tipo de comando
 * 'parâmetro' = todos os outros tokens, após o token de verbo.
 *
 * Exemplo:
 *
 * add               123               Igor               Borges               25/12/1990               +55-11-2222-3333
 * ^                 ^                 ^                  ^                    ^                        ^
 * |                 |                 |                  |                    |                        |
 * token (verbo)     token (parâmetro) token (parâmetro)  token (parâmetro)    token (parâmetro)        token (parâmetro)
*/
static const char SENTENCETOKENSEPARATOR = ' ';

/* A partir do cursor inicial fornecido, avança até encontrar o próximo token.
 *
 * Exemplo:
 *          10        20        30        40        50        60        70        80        90        100       110
 *          |         |         |         |         |         |         |         |         |         |         |
 * 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
 * add  ^            123               Igor               Borges               25/12/1990               +55-11-2222-3333
 * ^    |            ^                 ^                  ^                    ^                        ^
 * |    |            |                 |                  |                    |                        |
 * token|(verbo)     token (parâmetro) token (parâmetro)  token (parâmetro)    token (parâmetro)        token (parâmetro)
 *      |            |
 *      *pinitialposition = 6.
 *                   |
 *                   função retorna posição do próximo token, '123' = 19.
*/
int advancetonexttoken(const char *const    sentence,
                       const int *const     pinitialposition,
                       const int *const     psentencesize)
{
    // Para procurar pelo próximo token, deve estar apontando para um separador de tokens.
    // Se já estiver num caracter válido para token, retorna essa posição como o início de
    // um token válido.
    int i = *pinitialposition;

    while((i < *psentencesize) && (sentence[i] == SENTENCETOKENSEPARATOR))
        i++;

    return i;
}

/* Copia o token para um buffer, identificando também onde termina esse token na sentença.
 *
 * Exemplo:
 *          10        20        30        40        50        60        70        80        90        100       110
 *          |         |         |         |         |         |         |         |         |         |         |
 * 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
 * add  ^            123               Igor               Borges               25/12/1990               +55-11-2222-3333
 * ^    |              ^               ^                  ^                    ^                        ^
 * |    |              |               |                  |                    |                        |
 * token|(verbo)     token (parâmetro) token (parâmetro)  token (parâmetro)    token (parâmetro)        token (parâmetro)
 *      |              |
 *      *pposicaoleiturainicial = 6.
 *                     |
 *                     *pposicaoleiturafinal = 21.
 *                   token = '123'
*/
BOOL getsentencetoken(const int* const  pposicaoleiturainicial,
                      int*              pposicaoleiturafinal,
                      const char* const sentence,
                      const int* const  pmaxsizesentence,
                      char*             token,
                      const int* const  pmaxsizetoken)
{
    // Limpa o buffer para receber um novo token.
    memset((void*)token, '\0', *pmaxsizetoken);

    int i = 0;
    int j = -1;
    *pposicaoleiturafinal = *pposicaoleiturainicial;
    for(i = *pposicaoleiturainicial; i < *pmaxsizesentence; i++)
    {
        if(sentence[i] != SENTENCETOKENSEPARATOR)
        {
            // O buffer do token tem um tamanho limitado, não necessariamente igual
            // ao buffer da sentença.
            if(j+1 < *pmaxsizetoken)
            {
                *token = sentence[i];
                token++;
            }
            j++;
        }
        else
            break;
    }

    *pposicaoleiturafinal = *pposicaoleiturainicial + j;
    return (j != -1);
}

/* Combina 'advancetonexttoken' e 'getsentencetoken' para facilitar a sequência de leitura
 * consecutiva dos tokens numa mesma sentença.
 * Copia o token para um buffer, identificando também onde termina esse token na sentença.
 *
 * Exemplo:
 *          10        20        30        40        50        60        70        80        90        100       110
 *          |         |         |         |         |         |         |         |         |         |         |
 * 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
 * add  ^            123               Igor               Borges               25/12/1990               +55-11-2222-3333
 * ^    |            ^ ^               ^                  ^                    ^                        ^
 * |    |            | |               |                  |                    |                        |
 * token|(verbo)     token (parâmetro) token (parâmetro)  token (parâmetro)    token (parâmetro)        token (parâmetro)
 *      |            | |
 *      *pposicaoleiturainicial = 6.
 *                   |
 *                   *pposicaoleiturainicial = 19.
 *                     |
 *                     *pposicaoleiturafinal = 21.
 *                   token = '123'
*/
BOOL getnextsentencetoken(int* const        pposicaoleiturainicial,
                          int*              pposicaoleiturafinal,
                          const char* const sentence,
                          const int* const  pmaxsizesentence,
                          char*             token,
                          const int* const  pmaxsizetoken)
{
    const int sentencesize = strlen(sentence);

    *pposicaoleiturainicial = ++(*pposicaoleiturafinal);
    *pposicaoleiturainicial = *pposicaoleiturafinal = advancetonexttoken(sentence, pposicaoleiturainicial, &sentencesize);
    return getsentencetoken(pposicaoleiturainicial, pposicaoleiturafinal, sentence, pmaxsizesentence, token, pmaxsizetoken);
}

/*
 * Contornando o clássico problema de conversão léxica de string
 * para números. A função atoi() é bem conhecida por inúmeras falhas,
 * sendo a mais infame o retorno do mesmo número '0' quando a string
 * é igual a '0' ou 'SHIT', por exemplo.
 * Para evitar isso, utilizamos 'strtol', que provê um mecanismo
 * sólido para identificar falha ou sucesso na conversão léxica.
*/
BOOL validatetokengols(const char *const golsparam, unsigned int* pgols)
{
    char *eptr;
    int length = strlen(golsparam);
    errno = 0;
    long gols = strtol(golsparam, &eptr, 10); // Número de base 10.
    // Testa somente EINVAL (erro de conversão), e não ERANGE (erro de intervalo)
    // porque o buffer é pequeno demais para ter números muito grandes.
    BOOL conversiontonum = (errno != EINVAL) && (golsparam != eptr);

    *pgols = gols;
    return ((length > 0) && (length <= MAXBUFFGOLS) && conversiontonum && ((gols >= 0U) && (gols <= MAXGOLS)));
}

/*
* Dada a peculiaridade incomum do enunciado do exercício, no qual é possível
* identificar a existência de um indivíduo já cadastrado pelo seu ID através
* de um comando 'add' lexicamente INCORRETO:
*
* 'add 123 Joao Souza 11/10/2000 103-99'
*                                ^
*                                |
*                                token de telefone inválido
*
* temos que utilizar um analisador léxico customizado para esse cenário.
*/
BOOL placarlexicalanalyser(const char *const sentence, stPlacar* pplacar)
{
    static const int maxbuffergols = MAXBUFFGOLS;
    int sentencesize = strlen(sentence);
    if (sentencesize > MAXBUFFPLACAR)
        return FALSE;

    if (sentencesize < MINBUFFPLACAR)
        return FALSE;

    char tokenbuffer[MAXBUFFGOLS];

    int tokeninitialpos = 0;
    int tokenfinalpos = tokeninitialpos;

    tokeninitialpos = advancetonexttoken(sentence, &tokeninitialpos, &sentencesize);
    if(!getsentencetoken(&tokeninitialpos, &tokenfinalpos, sentence, &sentencesize, tokenbuffer, &maxbuffergols))
        return FALSE;

    if(!validatetokengols(tokenbuffer, &(pplacar->m)))
        return FALSE;

    if(!getnextsentencetoken(&tokeninitialpos, &tokenfinalpos, sentence, &sentencesize, tokenbuffer, &maxbuffergols))
        return FALSE;

    if(!validatetokengols(tokenbuffer, &(pplacar->n)))
        return FALSE;

    if(pplacar->m == pplacar->n)
        return FALSE;

    return TRUE;
}

/*
 * fgets, ao contrário da função (insegura) scanf, inclui
 * os caracteres terminadores de linha (\n no Linux, \r\n no Windows).
 * Essa função coloca um caracter nulo na primeira instância desses
 * caracteres terminadores, garantindo um buffer contendo apenas
 * o token da sentença.
*/
void zero_fgets_trailchars(char* buff)
{buff[strcspn(buff, "\r\n")] = 0;}

char vencedordojogo(const stPlacar *const pplacar)
{return pplacar->m > pplacar->n ? pplacar->mplayer : pplacar->nplayer;}

/*
 *
 ***************************************CRUD ENGINE*******************************************
 *
*/
void iniciaOITAVAS(void)
{
    static const char selecao = 'A';
    for(unsigned int i = 0U; i < 8U; i++)
    {
        BOOL valid = FALSE;
        while(!valid)
        {
            stPlacar placar = {
                .mplayer = selecao + 2*i,
                .m = 0U,
                .nplayer = selecao + 2*i + 1,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+1] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            fgets(BUFF, MAXBUFFPLACAR, stdin);
            zero_fgets_trailchars(BUFF);

            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

void iniciaQUARTAS(void)
{
    for(unsigned int i = 8U; i < 12U; i++)
    {
        BOOL valid = FALSE;
        while(!valid)
        {
            stPlacar placar = {
                .mplayer = jogos[0 + 2*(i ^ 8)].winner,
                .m = 0U,
                .nplayer = jogos[1 + 2*(i ^ 8)].winner,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+1] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            fgets(BUFF, MAXBUFFPLACAR, stdin);
            zero_fgets_trailchars(BUFF);

            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

void iniciaSEMIFINAIS(void)
{
    for(unsigned int i = 12U; i < 14U; i++)
    {
        BOOL valid = FALSE;
        while(!valid)
        {
            stPlacar placar = {
                .mplayer = jogos[8 + 2*(i ^ 12)].winner,
                .m = 0U,
                .nplayer = jogos[9 + 2*(i ^ 12)].winner,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+1] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            fgets(BUFF, MAXBUFFPLACAR, stdin);
            zero_fgets_trailchars(BUFF);

            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

void iniciaFINAL(void)
{
    for(unsigned int i = 14U; i < MAXJOGOS; i++)
    {
        BOOL valid = FALSE;
        while(!valid)
        {
            stPlacar placar = {
                .mplayer = jogos[12 + 2*(i ^ 14)].winner,
                .m = 0U,
                .nplayer = jogos[12 + 2*(i ^ 14)].winner,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+1] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            fgets(BUFF, MAXBUFFPLACAR, stdin);
            zero_fgets_trailchars(BUFF);

            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

void iniciaCOPADOMUNDO(void)
{
    iniciaOITAVAS();
    iniciaQUARTAS();
    iniciaSEMIFINAIS();
    iniciaFINAL();

    printf("%c\n", jogos[MAXJOGOS-1U].winner);
}

// li e concordo com os termos da aps
