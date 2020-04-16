include_directories(../src)

add_executable(
    unit_tests
    ../src/desafio8.c
    tests_desafio8.c
    )

# Template para testes de Analisadores Léxicos
function(do_test lexicalanalyser target testid arg1 arg2 result)
  add_test(NAME ${lexicalanalyser}-Test-${testid} COMMAND ${target} ${arg1} "${arg2}")
  set_tests_properties(${lexicalanalyser}-Test-${testid}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

# Cenários válidos - valores que o usuário pode inserir.
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 1 0 "0 1" "Valor retornado: TRUE. Placar '0' x '1'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 2 0 "00 1" "Valor retornado: TRUE. Placar '0' x '1'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 3 0 "0 01" "Valor retornado: TRUE. Placar '0' x '1'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 4 0 "00 01" "Valor retornado: TRUE. Placar '0' x '1'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 5 0 "0  1" "Valor retornado: TRUE. Placar '0' x '1'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 6 0 "0   1" "Valor retornado: TRUE. Placar '0' x '1'.")

do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 7 0 "1 0" "Valor retornado: TRUE. Placar '1' x '0'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 8 0 "01 0" "Valor retornado: TRUE. Placar '1' x '0'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 9 0 "1 00" "Valor retornado: TRUE. Placar '1' x '0'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 10 0 "01 00" "Valor retornado: TRUE. Placar '1' x '0'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 11 0 "1  0" "Valor retornado: TRUE. Placar '1' x '0'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 12 0 "1   0" "Valor retornado: TRUE. Placar '1' x '0'.")

do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 13 0 "19 20" "Valor retornado: TRUE. Placar '19' x '20'.")
do_test("addcommandLexicalAnalyser_ValidScenario" unit_tests 14 0 "20 19" "Valor retornado: TRUE. Placar '20' x '19'.")

# Cenários inválidos:

# Formato esperado: (1 a 2) dígitos + espaço + (1 a 2) dígitos.
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 1 0 "0 x 1" "Valor retornado: FALSE.")
# Não pode haver espaços no início
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 2 0 " 00 01" "Valor retornado: FALSE.")
# Não pode estar vazio
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 3 0 "" "Valor retornado: FALSE.")
# Não pode haver empates
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 4 0 "1 1" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 5 0 "01 1" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 6 0 "1 01" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 7 0 "01 01" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 8 0 "1  1" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 9 0 "1   1" "Valor retornado: FALSE.")

do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 10 0 "11 11" "Valor retornado: FALSE.")

do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 11 0 "5 5" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 12 0 "05 5" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 13 0 "5 05" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 14 0 "05 05" "Valor retornado: FALSE.")
# O máximo de gols é 20.
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 15 0 "0 21" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 16 0 "00 21" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 17 0 "21 0" "Valor retornado: FALSE.")
do_test("addcommandLexicalAnalyser_InvalidScenario" unit_tests 18 0 "21 00" "Valor retornado: FALSE.")
