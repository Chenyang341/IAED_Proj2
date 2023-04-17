/* iaed-23 - ist1106535 - project2 */

/*
    File: global.c
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com funcoes usadas globalmente
*/

#include "global.h"

/* Le uma string com ou sem aspas. Devolve "nao" se for o ultimo argumento e
"sim" caso contrario */
int lePalavra(char **s) {
    int i = 0, aspas = NAO, dentro = NAO;
    char c, buffer[MAX_INPUT];
    while ((c = getchar()) != '\n') {
        if (c == ' ') {
            if (dentro && aspas) {
                buffer[i++] = c;
            } else if (dentro && !aspas) {
                buffer[i] = '\0';
                *s = (char*) malloc(sizeof(char) * (i + 1));
                strcpy(*s, buffer);
                return SIM;
            }
        } else {
            dentro = SIM;
            if (c == '"') {
                if (!aspas)
                    aspas = SIM;
                else
                    aspas = NAO;
            } else {
                buffer[i++] = c;
            }
        }
        buffer[i] = '\0';
    }
    *s = (char*) malloc(sizeof(char) * (i + 1));
    strcpy(*s, buffer);
    return NAO;
}

/* Verifica se a string e uma abreviacao de ate 3 caracteres de "inverso" */
int verificaInversoOk(char *s) {

    char *inv = "inverso";
    int length = strlen(s), i;
    if (length < 3 || length > 7)
        return NAO;
    for (i = 0; i < length; i++)
        if (inv[i] != s[i])
            return NAO;
    return SIM;

}

/* Apaga toda a memorial alocada */
void apagaDados(Carreira *headCar, Paragem *headPar) {
    Carreira *auxCarreira;
    Paragem *auxParagem;
    NodeLigacao *auxLigacao;
    NodeNome *auxNome;
    while (headCar != NULL) {
        while (headCar->ligacoes != NULL) {
            auxLigacao = headCar->ligacoes;
            headCar->ligacoes = headCar->ligacoes->next;
            free(auxLigacao->origem);
            free(auxLigacao->destino);
            free(auxLigacao);
        }
        auxCarreira = headCar;
        headCar = headCar->next;
        free(auxCarreira->nome);
        free(auxCarreira);
    }
    while (headPar != NULL) {
        while (headPar->carreiras != NULL) {
            auxNome = headPar->carreiras;
            headPar->carreiras = headPar->carreiras->next;
            free(auxNome->nome);
            free(auxNome);
        }
        auxParagem = headPar;
        headPar = headPar->next;
        free(auxParagem->nome);
        free(auxParagem);
    }
}