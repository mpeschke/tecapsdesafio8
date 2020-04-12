/*
* Desafio 8
* Matheus Peschke de Azevedo - RA: 20396209
* Gustavo Caldas de Souza - RA: 21352329
*
* Se você tiver uma conta no Github, envie seu endereço
* cadastrado no Github para 'mpeschke@gmail.com', para ter
* acesso ao repositório privado onde está essa solução, estruturada
* com testes unitários para validação de qualidade.
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

// Jogos totais, considerando todas as rodadas.
static stPlacar jogos[MAXJOGOS];

/*
 *
 *********************ANALISADOR LÉXICO (TOKENIZADOR)*****************************************
 *
 * Observação: o mesmo conceito de análise léxica foi criando anteriormente
 * no desafio #1, por esse mesmo time.
 * Reutilizando aqui o mesmo conceito, por também haver a necessidade de análise léxica de entradas
 * do usuário.
 *
 * Definições que fazem parte de um analisador léxico:
 *
 * 'sentença' = a linha de comando com todos os seus parâmetros
 * 'token' = um item que esteja separados dos outros por um ou mais espaços.
 * 'parâmetro' = qualquer um dos tokens.
 *
 * Exemplo:
 *          10
 *          |
 * 0123456789
 * 20 19
 * ^  ^
 * |  |
 * token (gols do time 'm')
 *    |
 *    token (gols do time 'n')
*/
static const char SENTENCETOKENSEPARATOR = ' ';

/* A partir do cursor inicial fornecido, avança até encontrar o próximo token.
 *
 * Exemplo:
 *          10
 *          |
 * 0123456789
 * 20^19
 *   ||
 *   ||
 *   *pinitialposition = 2.
 *    |
 *    função retorna posição do próximo token, '19' = 3.
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
 *          10
 *          |
 * 0123456789
 * 20^19
 *   | ^
 *   | |
 *   *pposicaoleiturainicial = 2.
 *     |
 *     *pposicaoleiturafinal = 4.
 *     token = '19'
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
 *          10
 *          |
 * 0123456789
 * 20^19
 *   |^^
 *   |||
 *   | token (gols do time 'n')
 *   |||
 *   *pposicaoleiturainicial = 2.
 *    ||
 *    *pposicaoleiturainicial = 3.
 *     |
 *     *pposicaoleiturafinal = 4.
 *     token = '19'
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
* Analisador léxico para identificar o placar de um jogo.
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

    // Regra #1: é necessário haver o total de gols do time 'm', entre 0 e um máximo de 20.
    int tokeninitialpos = 0;
    int tokenfinalpos = tokeninitialpos;

    tokeninitialpos = advancetonexttoken(sentence, &tokeninitialpos, &sentencesize);
    if(!getsentencetoken(&tokeninitialpos, &tokenfinalpos, sentence, &sentencesize, tokenbuffer, &maxbuffergols))
        return FALSE;

    if(!validatetokengols(tokenbuffer, &(pplacar->m)))
        return FALSE;

    // Regra #2: é necessário haver o total de gols do time 'n', entre 0 e um máximo de 20.
    if(!getnextsentencetoken(&tokeninitialpos, &tokenfinalpos, sentence, &sentencesize, tokenbuffer, &maxbuffergols))
        return FALSE;

    if(!validatetokengols(tokenbuffer, &(pplacar->n)))
        return FALSE;

    // Regra #3: não pode haver empate.
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
{buff[strcspn(buff, "\r\n")] = '\0';}

/*
 * Infelizmente o Ruindows considera como terminador de
 * linha dois caracteres, '\r' e '\n'. Como não sabemos se o
 * exercício será avaliado num Ruindows, tratamos as diferenças
 * de plataforma nessa função.
*/
char* platformindependent_terminatorchar(char* buff)
{
    // Retorna a posição ao encontrar o primeiro desses caracteres terminadores.
    unsigned long pos = strcspn(buff, "\r\n");
    // Como pode encontrar o caracter terminador APÓS a string,
    // Identificamos como não encontrado (string vazia).
    if(pos >= strlen(buff))
        return NULL;
    else
        return &buff[pos];
}

/*
 * fgets, por segurança, vai continuamente alimentando o buffer
 * até o final da entrada (stdin), até encontrar um caracter terminador.
 * Por isso tratamos a função de acordo, considerando apenas a primeira
 * vez em que ela preenche o buffer, e ignorando o resto das entradas.
*/
void read_fgets_firstbuffer(char* buff, char* finalbuff)
{
    /* BUFF[MAXBUFFPLACAR+2U]
     *          |10
     * 01234567890123456789
     * 20 19n0
     *      ^^
     *      ||
     *      fgets vai adicionar na posição 5 o terminador (se houver)
     *      e terminar a string adicionando \0 na posição 6.
     *       |tamanho real do buffer: (MAXBUFFPLACAR+2U).
    */
    char* presult = fgets(buff, MAXBUFFPLACAR+1U, stdin);
    // Nos interessa apenas o conteúdo da primeira vez em
    // que o fgets preenche o buffer vindo de stdin (entrada do teclado).
    strcpy(finalbuff, buff);
    char* newlinefound = platformindependent_terminatorchar(presult);
    zero_fgets_trailchars(finalbuff);
    while(newlinefound == NULL)
    {
        // Ainda não encontrou o terminador nessa cópia do
        // buffer - preenche continuamente o buffer até encontrar.
        presult = fgets(buff, MAXBUFFPLACAR+1U, stdin);
        newlinefound = platformindependent_terminatorchar(presult);
    }
}

// O vencedor é o time com o maior número de gols.
char vencedordojogo(const stPlacar *const pplacar)
{return pplacar->m > pplacar->n ? pplacar->mplayer : pplacar->nplayer;}

/*
 *
 ***************************************INICIA AS OITAVAS DE FINAL*****************
 *
*/
void iniciaOITAVAS(void)
{
    static const char selecao = 'A';
    // Solicita os placares dos primeiros oito jogos (Oitavas de Final).
    for(unsigned int i = 0U; i < 8U; i++)
    {
        BOOL valid = FALSE;
        while(!valid) // Não deixa avançar para o próximo placar enquanto não for válido.
        {
            // Aproveita as interações das oitavas de final para
            // coletar os identificadores dos times ('A' a 'P').
            stPlacar placar = {
                .mplayer = selecao + 2*i,
                .m = 0U,
                .nplayer = selecao + 2*i + 1,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+2U] = {'\0'};
            char FINALBUFF[MAXBUFFPLACAR+2U] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            // Trata a função fgets adequadamente, de forma a ler apenas o limite do buffer
            // que acomoda o placar 'm x n'.
            read_fgets_firstbuffer(BUFF, FINALBUFF);

            // Valida se o placar é lexicamente correto.
            valid = placarlexicalanalyser(FINALBUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

/*
 *
 ***************************************INICIA AS QUARTAS DE FINAL*****************
 *
*/
void iniciaQUARTAS(void)
{
    // Solicita os placares dos subsequentes quatro jogos (Quartas de Final).
    for(unsigned int i = 8U; i < 12U; i++)
    {
        BOOL valid = FALSE;
        while(!valid) // Não deixa avançar para o próximo placar enquanto não for válido.
        {
            // Obtém os identificadores dos times de seus placares previamente preenchidos
            // durante as oitavas de final.
            stPlacar placar = {
                .mplayer = jogos[0 + 2*(i ^ 8)].winner,
                .m = 0U,
                .nplayer = jogos[1 + 2*(i ^ 8)].winner,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+2U] = {'\0'};
            char FINALBUFF[MAXBUFFPLACAR+2U] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            // Trata a função fgets adequadamente, de forma a ler apenas o limite do buffer
            // que acomoda o placar 'm x n'.
            read_fgets_firstbuffer(BUFF, FINALBUFF);

            // Valida se o placar é lexicamente correto.
            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

/*
 *
 ***************************************INICIA AS SEMIFINAIS **********************
 *
*/
void iniciaSEMIFINAIS(void)
{
    // Solicita os placares dos subsequentes dois jogos (Semifinais).
    for(unsigned int i = 12U; i < 14U; i++)
    {
        BOOL valid = FALSE;
        while(!valid) // Não deixa avançar para o próximo placar enquanto não for válido.
        {
            // Obtém os identificadores dos times de seus placares previamente preenchidos
            // durante as oitavas de final.
            stPlacar placar = {
                .mplayer = jogos[8 + 2*(i ^ 12)].winner,
                .m = 0U,
                .nplayer = jogos[9 + 2*(i ^ 12)].winner,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+2U] = {'\0'};
            char FINALBUFF[MAXBUFFPLACAR+2U] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            // Trata a função fgets adequadamente, de forma a ler apenas o limite do buffer
            // que acomoda o placar 'm x n'.
            read_fgets_firstbuffer(BUFF, FINALBUFF);

            // Valida se o placar é lexicamente correto.
            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

/*
 *
 ***************************************INICIA A FINAL DA COPA DO MUNDO************
 *
*/
void iniciaFINAL(void)
{
    // Solicita o placar do último jogo (Final da Copa do Mundo).
    for(unsigned int i = 14U; i < MAXJOGOS; i++)
    {
        BOOL valid = FALSE;
        while(!valid) // Não deixa avançar para o próximo placar enquanto não for válido.
        {
            // Obtém os identificadores dos times de seus placares previamente preenchidos
            // durante as oitavas de final.
            stPlacar placar = {
                .mplayer = jogos[12 + 2*(i ^ 14)].winner,
                .m = 0U,
                .nplayer = jogos[12 + 2*(i ^ 14)].winner,
                .n = 0U,
                .winner = '\0'
            };
            char BUFF[MAXBUFFPLACAR+2U] = {'\0'};
            char FINALBUFF[MAXBUFFPLACAR+2U] = {'\0'};

            printf("Digite o placar do jogo #%d no formato 'GOLS GOLS' - máximo de 20 gols por time e não pode haver empate: ", i+1U);
            // Trata a função fgets adequadamente, de forma a ler apenas o limite do buffer
            // que acomoda o placar 'm x n'.
            read_fgets_firstbuffer(BUFF, FINALBUFF);

            // Valida se o placar é lexicamente correto.
            valid = placarlexicalanalyser(BUFF, &placar);
            if(valid)
            {
                placar.winner = vencedordojogo(&placar);
                jogos[i] = placar;
            }
        }
    }
}

/*
 *
 ***************************************INICIA AS RODADAS DA COPA DO MUNDO*********
 *
*/
void iniciaCOPADOMUNDO(void)
{
    // Obtém os placares das rodadas da copa, sequencialmente.
    iniciaOITAVAS();
    iniciaQUARTAS();
    iniciaSEMIFINAIS();
    iniciaFINAL();

    // Imprime o time campeão do mundo.
    printf("%c\n", jogos[MAXJOGOS-1U].winner);
}

// li e concordo com os termos da aps
