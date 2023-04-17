/* iaed-23 - ist1106535 - project2 */

/*
    File: carreira.c
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com funcoes associadas ao comando c
*/

#include "global.h"

/* Comando carreira, executa funcoes diferentes dependendo do input */
Carreira *comandoCarreira(Carreira *headCar) {

    Carreira carreira;
    char *inv = NULL;
    if (getchar() == '\n')
        semArgCarreira(headCar);
    else {
        if (lePalavra(&carreira.nome))
            lePalavra(&inv);
        headCar = comArgCarreira(headCar, carreira, inv);
    }
    free(inv);
    return headCar;

}

/* Imprime o nome da carreira, a origem, o destino, o numero de paragens, o
custo total e a duracal total */
void semArgCarreira(Carreira *headCar) {

    while (headCar != NULL) {
        if (headCar->numLigacao > 0) {
            printf("%s %s %s %d %.2f %.2f\n", headCar->nome,
            nomeOrigemCarreira(headCar->ligacoes),
            nomeDestinoCarreira(headCar->ligacoes), headCar->numLigacao + 1,
            headCar->custoTotal, headCar->duracaoTotal);
        } else
            printf("%s %d %.2f %.2f\n", headCar->nome, 0, 0.0, 0.0);
        headCar = headCar->next;
    }

}

/* Imprime todas as paragens da carreira pela ordem normal ou inversa. Caso
a carreira nao exista eh criada uma com esse nome */
Carreira *comArgCarreira(Carreira *headCar, Carreira carreira, char *inv) {

    int e_carreira = NAO;
    Carreira *auxCarreira = headCar;
    while (auxCarreira != NULL) {
        if (!strcmp(auxCarreira->nome, carreira.nome)) {
            e_carreira = SIM;
            if (auxCarreira->numLigacao > 0) {
                if (inv == NULL)
                    printParagensNormal(auxCarreira->ligacoes);
                else if (verificaInversoOk(inv))
                    printParagensInverso(auxCarreira->ligacoes);
                else
                    printf("incorrect sort option.\n");
            }
        }
        auxCarreira = auxCarreira->next;
    }
    if (!e_carreira)
        headCar = criaCarreira(headCar, carreira);
    else
        free(carreira.nome);
    return headCar;

}

/* Imprime as paragens da carreira pela ordem normal */
void printParagensNormal(NodeLigacao *headLig) {

    printf("%s", headLig->origem);
    while (headLig != NULL) {
        printf(", %s", headLig->destino);
        headLig = headLig->next;
    }
    printf("\n");

}

/* Imprime as paranges da carreira pela ordem inversa */
void printParagensInverso(NodeLigacao *headLig) {

    while (headLig->next != NULL)
        headLig = headLig->next;
    printf("%s", headLig->destino);
    while (headLig != NULL) {
        printf(", %s", headLig->origem);
        headLig = headLig->prev;
    }
    printf("\n");
    
}

/* Cria carreira */
Carreira *criaCarreira(Carreira *head, Carreira carreira) {

    Carreira *newCarreira = (Carreira*) malloc(sizeof(Carreira));
    Carreira *auxCarreira = head;
    *newCarreira = carreira;
    newCarreira->custoTotal = 0;
    newCarreira->duracaoTotal = 0;
    newCarreira->numLigacao = 0;
    newCarreira->ligacoes = NULL;
    newCarreira->next = NULL;
    if (head == NULL)
        head = newCarreira;
    else {
        while (auxCarreira->next != NULL)
            auxCarreira = auxCarreira->next;
        auxCarreira->next = newCarreira;
    }
    return head;

}

/* Verifica se a carreira passa paragem */
int existeCarreira(NodeNome *headNome, char *nomeCarreira) {

    while (headNome != NULL) {
        if (!strcmp(headNome->nome, nomeCarreira))
            return SIM;
        headNome = headNome->next;
    }
    return NAO;

}

/* Verifica se a carreira existe e devolve o seu nome caso exista */
char *verificaExisteCarreira(Carreira *headCar) {

    char *nomeCarreira;
    lePalavra(&nomeCarreira);
    while (headCar != NULL) {
        if (!strcmp(headCar->nome, nomeCarreira))
            return nomeCarreira;
        headCar = headCar->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    free(nomeCarreira);
    return NULL;

}

/* Remove a carreira com o nome passado e todas as suas componentes */
Carreira *removeCarreiraCarreira(Carreira *headCar, char *nomeCarreira) {
    Carreira *auxCarreira = headCar, *tempCarreira, *prev;
    if (!strcmp(headCar->nome, nomeCarreira)) {
        tempCarreira = headCar;
        while (headCar->ligacoes != NULL)
            removeLigacao(headCar);
        headCar = headCar->next;
        free(tempCarreira->nome);
        free(tempCarreira);
        return headCar;
    } else {
        while (auxCarreira != NULL) {
            if (!strcmp(auxCarreira->nome, nomeCarreira)) {
                prev->next = auxCarreira->next;
                while (auxCarreira->ligacoes != NULL)
                    removeLigacao(auxCarreira);
                free(auxCarreira->nome);
                free(auxCarreira);
                return headCar;
            }
            prev = auxCarreira;
            auxCarreira = auxCarreira->next;
        }
    }
    return headCar;
}

/* Remove a carreira em todas as paragens que fazem paret dela */
Paragem *removeCarreiraParagem(Paragem *headPar, char *nomeCarreira) {
    Paragem *auxParagem = headPar;
    NodeNome *auxNode, *prev;
    while (auxParagem != NULL) {
        if (auxParagem->carreiras != NULL) {
            if (!strcmp(auxParagem->carreiras->nome, nomeCarreira)) {
                if (auxParagem->numCarreira == 1)
                    ultimaCarreira(auxParagem);
                else {
                    removeCarreiraAvan(auxParagem);
                    auxParagem->numCarreira--;
                }
            } else {
                prev = auxParagem->carreiras;
                auxNode = auxParagem->carreiras->next;
                while (auxNode != NULL) {
                    if (!strcmp(auxNode->nome, nomeCarreira)) {
                        prev->next = auxNode->next;
                        removeCarreira(auxNode);
                        auxParagem->numCarreira--;
                        break;
                    }
                    prev = auxNode;
                    auxNode = auxNode->next;
                }
            }
        }
        auxParagem = auxParagem->next;
    }
    return headPar;
}

/* remove Ligacao */
void removeLigacao(Carreira *carreira) {

    NodeLigacao *tempNode = carreira->ligacoes;
    tempNode = carreira->ligacoes;
    carreira->ligacoes = carreira->ligacoes->next;
    free(tempNode->origem);
    free(tempNode->destino);
    free(tempNode);

}

/* Remocao quando a carreira eh a ultima da paragem */
void ultimaCarreira(Paragem *paragem) {

    free(paragem->carreiras->nome);
    free(paragem->carreiras);
    paragem->numCarreira = 0;
    paragem->carreiras = NULL;

}

/* Remocao da carreira da paragem */
void removeCarreira(NodeNome *node) {

    NodeNome *tempNode = node;
    free(tempNode->nome);
    free(tempNode);

}

/* Remocao da carreira da paragem e avancando para a proxima carreira */
void removeCarreiraAvan(Paragem *paragem) {

    NodeNome *tempNode = paragem->carreiras;
    paragem->carreiras = paragem->carreiras->next;
    free(tempNode->nome);
    free(tempNode);

}

/* Procura e devolve a carreira com o nome passado */
Carreira *procuraCarreira(Carreira *head, char *nomeCarreira) {

    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira))
            return head;
        head = head->next;
    }
    return NULL;

}

/* Devolve o nome da paragem origem da carreira */
char *nomeOrigemCarreira(NodeLigacao *head) {
    
    if (head == NULL)
        return NULL;
    return head->origem;

}

/* Devolve o nome da paragem destino da carreira */
char *nomeDestinoCarreira(NodeLigacao *head) {

    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head->destino;

}