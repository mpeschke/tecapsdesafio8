# APS de Técnicas de Programação - Desafio 8

Oitavo desafio da APS do professor Igor, 2º semestre de Ciência da Computação, 1º semestre de 2020, TÉCNICAS DE PROGRAMAÇÃO (P2N) - 202010.01041.01

Enunciado do desafio 8 descrito no arquivo tp_ListaDesafiosAPS.pdf.

## Passo 1: Dependências

Linux (qualquer distro)
Git >= 2.17.1, CMake >= 3.5 e GCC >= 7.5.0

Por exemplo, no Ubuntu:  
*#apt install -y git cmake make gcc*

## Passo 2: Instruções para executar o Desafio 8

Num terminal:

*$git clone ssh://git@github.com/mpeschke/tecapsdesafio8.git* (via ssh) ou
*$git clone https://github.com/mpeschke/tecapsdesafio8.git* (pergunta usuário e senha)  
*$cd tecapsdesafio8/*  
*$mkdir tecapsdesafio8_build*  
*$cd tecapsdesafio8_build/*  
*$cmake ../.*  
*$cmake --build .*  
*$./tecapsdesafio8*

## Passo 3: Instruções para executar os testes unitários do Desafio 8

Execute o passo 2, e sem sair da mesma sessão do terminal, execute:

*$ctest*
