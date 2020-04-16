/*
* Desafio 8
* Matheus Peschke de Azevedo - RA: 20396209
* Gustavo Caldas de Souza    - RA: 21352329
* Marcelo                    - RA: 21359738
*
* Se você tiver uma conta no Github, envie seu endereço
* cadastrado no Github para 'mpeschke@gmail.com', para ter
* acesso ao repositório privado onde está essa solução, estruturada
* com testes unitários para validação de qualidade.
*/
#include "desafio8.h"

// Variáveis para ajudar a lidar com operações de buffer.
#define MAXJOGOS 15
#define MAXBUFFPLACAR 5
#define MINBUFFPLACAR 3
#define MAXBUFFGOLS 2
#define MAXGOLS 20

// Jogos totais, considerando todas as rodadas.
static stPlacar placares[MAXJOGOS];

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
 * token ('20' = gols do time 'm')
 * |  |
 * |  token ('19' = gols do time 'n')
 * |sentença ('20 19')
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
 * é igual a '0' ou 'zero', por exemplo.
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
    // 1) Deve haver algo no buffer, 2) o buffer não pode ser maior que '00 00', 3) a conversão pra número
    // deve ter sido realizada com sucesso, 4) 0 ou mais gols e 5) máximo de 20 gols.
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
     *       |
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

// Identifica o time da partida de oitavas.
unsigned int identifica_time_oitavas(unsigned int j, unsigned int i)
{return ('A' + 2U*i + j);}

// Identifica o time vencedor na partida da rodada anterior (oitavas).
unsigned int identifica_time_vencedor_oitavas(unsigned int j, unsigned int i)
{return placares[j + 2U*(i ^ 8U)].winner;}

// Identifica o time vencedor na partida da rodada anterior (quartas).
unsigned int identifica_time_vencedor_quartas(unsigned int j, unsigned int i)
{return placares[j+8U + 2U*(i ^ 12U)].winner;}

// Identifica o time vencedor na partida da rodada anterior (semifinal).
unsigned int identifica_time_vencedor_semifinais(unsigned int j, unsigned int i)
{return placares[j+12U + 2U*(i ^ 14U)].winner;}

// Assinatura das funções que identificam os times das rodadas e vencedores das anteriores.
typedef unsigned int(*pidentifica_time)(unsigned int, unsigned int j);

// Inicia uma determinada fase da copa, identificando corretamente quais os jogos iniciais e finais da rodada,
// assim como o jogo da rodada anterior do time, e seu resultado.
void iniciaFASE(const unsigned int primeirojogodafase, const unsigned int ultimojogodafase, pidentifica_time pfn)
{
    for(unsigned int i = primeirojogodafase; i < ultimojogodafase; i++)
    {
        BOOL valid = FALSE;
        while(!valid) // Não deixa avançar para o próximo placar enquanto a entrada não for válida.
        {
            // A função pfn vai trazer o time que participa dessa partida - se for
            // oitavas de final, sequencialmente de 'A' a 'P'. Nas rodadas seguintes,
            // as funções são inteligentes - buscam os vencedores dos dois jogos
            // da rodada anterior, que se encontraram na rodada atual.
            stPlacar placar = {
                .mplayer = pfn(0, i),
                .m = 0U,
                .nplayer = pfn(1, i),
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
                // Identifica o vencedor pelo placar.
                placar.winner = vencedordojogo(&placar);
                // Atualiza o banco de dados.
                placares[i] = placar;
            }
        }
    }
}

/*
 *
 ***************************************INICIA AS OITAVAS DE FINAL*****************
 *
*/
void iniciaOITAVAS(void) // Jogo inicial, final e função que identifica os resultados dos times na rodada anterior.
{iniciaFASE(0U, 8U, &identifica_time_oitavas);}

/*
 *
 ***************************************INICIA AS QUARTAS DE FINAL*****************
 *
*/
void iniciaQUARTAS(void) // Jogo inicial, final e função que identifica os resultados dos times na rodada anterior.
{iniciaFASE(8U, 12U, &identifica_time_vencedor_oitavas);}

/*
 *
 ***************************************INICIA AS SEMIFINAIS **********************
 *
*/
void iniciaSEMIFINAIS(void) // Jogo inicial, final e função que identifica os resultados dos times na rodada anterior.
{iniciaFASE(12U, 14U, &identifica_time_vencedor_quartas);}

/*
 *
 ***************************************INICIA A FINAL DA COPA DO MUNDO************
 *
*/
void iniciaFINAL(void) // Jogo inicial, final e função que identifica os resultados dos times na rodada anterior.
{iniciaFASE(14U, MAXJOGOS, &identifica_time_vencedor_semifinais);}

/*
 *
 ***************************************INICIA AS RODADAS DA COPA DO MUNDO*********
 *
*/
void iniciaCOPADOMUNDO(void) // Executa todas as rodadas da copa do mundo.
{
    // Obtém os placares das rodadas da copa, sequencialmente.
    iniciaOITAVAS();
    iniciaQUARTAS();
    iniciaSEMIFINAIS();
    iniciaFINAL();

    // Imprime o time campeão do mundo.
    printf("%c\n", placares[MAXJOGOS-1U].winner);
}

// li e concordo com os termos da aps
