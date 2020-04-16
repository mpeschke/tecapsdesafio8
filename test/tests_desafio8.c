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
#include "tests_desafio8.h"

void test_unittests_desafio_8(const int lexicalanalyser, const char* sentence)
{
    stPlacar placar = { .mplayer = '\0', .m = 0, .nplayer = '\0', .n = 0, .winner = '\0'};

    switch(lexicalanalyser)
    {
        case 0:// Testa cenário para o analisador léxico 'placarlexicalanalyser'
        {
            BOOL ret = placarlexicalanalyser(sentence, &placar);
            printf("Valor retornado: %s. Placar '%d' x '%d'.",
                   ret == TRUE ? "TRUE" : "FALSE",
                   placar.m,
                   placar.n);
            break;
        }
        default:
            break;
    };
}

int main(int argc, char* argv[])
{
    test_unittests_desafio_8(atoi(argv[1]), argv[2]);

    return 0;
}

// li e concordo com os termos da aps
