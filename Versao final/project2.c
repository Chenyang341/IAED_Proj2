/* iaed-23 - ist1106535 - project2 */

/*
    File: project2.c
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com a funcao principal do projeto
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"

int main() {
    Carreira *_carreiras = NULL;
    Paragem *_paragens = NULL;
    char comando, *nome;
    while ((comando = getchar()) != 'q') {
        if (comando == 'c')
            _carreiras = comandoCarreira(_carreiras);
        else if (comando == 'p')
            _paragens = comandoParagem(_paragens);
        else if (comando == 'l')
            comandoLigacao(_carreiras, _paragens);
        else if (comando == 'i')
            comandoIntersecao(_paragens);
        else if (comando == 'r') {
            if ((nome = verificaExisteCarreira(_carreiras)) != NULL) {
                _carreiras = removeCarreiraCarreira(_carreiras, nome);
                _paragens = removeCarreiraParagem(_paragens, nome);
                free(nome);
            }
        } else if (comando == 'e') {
            if ((nome = verificaExisteParagem(_paragens)) != NULL) {
                _carreiras = removeParagemCarreira(_carreiras, nome);
                _paragens = removeParagemParagem(_paragens, nome);
                free(nome);
            }
        } else if (comando == 'a') {
            apagaDados(_carreiras, _paragens);
            _carreiras = NULL;
            _paragens = NULL;
        }
    }
    apagaDados(_carreiras, _paragens);
    return 0;
}