/* iaed-23 - ist1106535 - project2 */

/*
    File: paragem.c
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com funcoes associadas ao comando p
*/

#include "global.h"

/* Comando paragem, executa funcoes diferentes dependendo do input */
Paragem *comandoParagem(Paragem *head) {

    Paragem paragem;
    paragem = leInputParagem();
    if (paragem.nome == NULL)
        semArgParagem(head);
    else if (!paragem.latitude && !paragem.longitude) {
        umArgParagem(head, paragem);
        free(paragem.nome);
    } else
        head = tresArgParagem(head, paragem);
    return head;

}

/* Le os argumentos do comando introduzido */
Paragem leInputParagem() {

    Paragem paragem;
    char *s = NULL;
    paragem.nome = NULL;
    paragem.latitude = 0;
    paragem.longitude = 0;
    if (getchar() == '\n')
        return paragem;
    else if (!lePalavra(&s))
        paragem.nome = s;
    else {
        paragem.nome = s;
        leDouble(paragem.latitude);
        leDouble(paragem.longitude);
        getchar();
    }
    return paragem;

}

/* Imprime o nome, a latitude, a longitude e o numero de carreiras de todas as
paragens pela ordem em que sao criadas */
void semArgParagem(Paragem *head) {

    while (head != NULL) {
        printf("%s: %16.12f %16.12f %d\n", head->nome, head->latitude,
        head->longitude, head->numCarreira);
        head = head->next;
    }

}

/* Imprime o nome, a latitude e longitude da paragem com o nome introduzido.
Se nao existir eh imprimido um erro */
void umArgParagem(Paragem *head, Paragem paragem) {

    while (head != NULL) {
        if (!strcmp(head->nome, paragem.nome)) {
            printf("%16.12f %16.12f\n", head->latitude, head->longitude);
            return;
        }
        head = head->next;
    }
    printf("%s: no such stop.\n", paragem.nome);

}

/* Cria uma paragem com os dados passados. Se existir uma paragem com o mesmo
nome eh imprimido um erro */
Paragem *tresArgParagem(Paragem *head, Paragem paragem) {

    Paragem *auxParagem = head;
    while(auxParagem != NULL) {
        if (!strcmp(auxParagem->nome, paragem.nome)) {
            printf("%s: stop already exists.\n", paragem.nome);
            free(paragem.nome);
            return head;
        }
        auxParagem = auxParagem->next;
    }
    head = criaParagem(head, paragem);
    return head;

}

/* Cria paragem com os argumentos passados */
Paragem *criaParagem(Paragem *head, Paragem paragem) {

    Paragem *newParagem = (Paragem*) malloc(sizeof(Paragem));
    Paragem *auxParagem = head;
    *newParagem = paragem;
    newParagem->numCarreira = 0;
    newParagem->carreiras = NULL;
    newParagem->next = NULL;
    if (head == NULL)
        head = newParagem;
    else {
        while (auxParagem->next != NULL)
            auxParagem = auxParagem->next;
        auxParagem->next = newParagem;
    }
    return head;

}

/* Imprime todas as paragens em que ha intersecao de carreiras e imprime as 
carreiras por ordem alfabetica */
void comandoIntersecao(Paragem *headPar) {
    
    NodeNome *auxNode;
    while (headPar != NULL) {
        if (headPar->numCarreira > 1) {
            auxNode = sort(headPar->carreiras);
            printf("%s %d:", headPar->nome, headPar->numCarreira);
            while(auxNode != NULL) {
                printf(" %s", auxNode->nome);
                auxNode = auxNode->next;
            }
            printf("\n");
        }
        headPar = headPar->next;
    }

}

/* Ordena as carreiras de forma alfabetica */
NodeNome *sort(NodeNome *headNome) {
    
    NodeNome *auxNode1 = headNome, *auxNode2;
    char *nome1, *nome2, *temp;
    while (auxNode1 != NULL) {
        auxNode2 = headNome;
        while (auxNode2->next != NULL) {
            nome1 = auxNode2->nome;
            nome2 = auxNode2->next->nome;
            if (strcmp(nome1, nome2) > 0) {
                temp = auxNode2->nome;
                auxNode2->nome = auxNode2->next->nome;
                auxNode2->next->nome = temp;
            }
            auxNode2 = auxNode2->next;
        }
        auxNode1 = auxNode1->next;
    }
    return headNome;

}

/* Verifica se a paragem existe e devolve o seu nome caso exista */
char *verificaExisteParagem(Paragem *headPar) {

    char *nomeParagem;
    lePalavra(&nomeParagem);
    while (headPar != NULL) {
        if (!strcmp(headPar->nome, nomeParagem))
            return nomeParagem;
        headPar = headPar->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    free(nomeParagem);
    return NULL;

}

/* Remove todas as ligacoes com o nome da origem ou destino igual ao nome
passado */
Carreira *removeParagemCarreira(Carreira *headCar, char *nomeParagem) {
    Carreira *auxCarreira = headCar;
    NodeLigacao *auxNode = NULL;
    while (auxCarreira != NULL) {
        while (auxCarreira->ligacoes != NULL) {
            if (!strcmp(auxCarreira->ligacoes->origem, nomeParagem)) {
                if (auxCarreira->numLigacao == 1) {
                    ultimaLigacao(auxCarreira);
                    break;
                } else
                    ligacaoInicial(auxCarreira);
            } else if (!strcmp(auxCarreira->ligacoes->destino, nomeParagem)) {
                if (auxCarreira->numLigacao == 1) {
                    ultimaLigacao(auxCarreira);
                    break;
                } else {
                    ligacaoIntermedia(auxCarreira->ligacoes);
                    auxCarreira->numLigacao--;
                }
            } else {
                auxNode = auxCarreira->ligacoes->next;
                break;
            }
        }
        while (auxNode != NULL) {
            if (!strcmp(auxNode->destino, nomeParagem)) {
                if (auxNode->next == NULL) {
                    ligacaoFinal(auxCarreira, auxNode);
                    break;
                } else {
                    ligacaoIntermedia(auxNode);
                    auxCarreira->numLigacao--;
                }
            } else
                auxNode = auxNode->next;
        }
        auxCarreira = auxCarreira->next;
    }
    return headCar;
}

/* Remove a paragem com o nome passado e todas as suas componentes */
Paragem *removeParagemParagem(Paragem *head, char *nomeParagem) {
    Paragem *auxParagem, *tempParagem, *prev;
    if (!strcmp(head->nome ,nomeParagem)) {
        tempParagem = head;
        while (head->carreiras != NULL) {
            removeCarreiraAvan(head);
        }
        head = head->next;
        free(tempParagem->nome);
        free(tempParagem);
        return head;
    } else {
        prev = head;
        auxParagem = head->next;
        while (auxParagem != NULL) {
            if (!strcmp(auxParagem->nome, nomeParagem)) {
                prev->next = auxParagem->next;
                while (auxParagem->carreiras != NULL) {
                    removeCarreiraAvan(auxParagem);
                }
                free(auxParagem->nome);
                free(auxParagem);
                return head;
            }
            prev = auxParagem;
            auxParagem = auxParagem->next;
        }
    }
    return head;
}

/* Remocao da ligacao quando eh a unica */
void ultimaLigacao(Carreira *carreira) {

    carreira->custoTotal = 0;
    carreira->duracaoTotal = 0;
    carreira->numLigacao = 0;
    free(carreira->ligacoes->origem);
    free(carreira->ligacoes->destino);
    free(carreira->ligacoes);
    carreira->ligacoes = NULL;

}

/* Remocao da ligacao inicial */
void ligacaoInicial(Carreira *carreira) {

    NodeLigacao *tempNode = carreira->ligacoes;
    carreira->custoTotal -= carreira->ligacoes->custo;
    carreira->duracaoTotal -= carreira->ligacoes->duracao;
    carreira->ligacoes = carreira->ligacoes->next;
    carreira->ligacoes->prev = NULL;
    carreira->numLigacao--;
    free(tempNode->origem);
    free(tempNode->destino);
    free(tempNode);

}

/* Remocao da ligacao final */
void ligacaoFinal(Carreira *carreira, NodeLigacao *node) {

    NodeLigacao *tempNode = node;
    carreira->custoTotal -= node->custo;
    carreira->duracaoTotal -= node->duracao;
    node->prev->next = NULL;
    carreira->numLigacao--;
    free(tempNode->origem);
    free(tempNode->destino);
    free(tempNode);

}

/* Remocao da ligacao quando se encontra no meio */
void ligacaoIntermedia(NodeLigacao *node) {

    NodeLigacao *tempNode;
    node = uneLigacoes(node, node->next);
    tempNode = node->next;
    node->next = node->next->next;
    if (node->next != NULL)
        node->next->prev = node;
    free(tempNode->origem);
    free(tempNode);
    
}

/* Une duas ligacoes */
NodeLigacao *uneLigacoes(NodeLigacao *ligacao1, NodeLigacao *ligacao2) {

    free(ligacao1->destino);
    ligacao1->destino = ligacao2->destino;
    ligacao1->custo += ligacao2->custo;
    ligacao1->duracao += ligacao2->duracao;
    return ligacao1;

}

/* Procura e devolve a paragem com o nome passado */
Paragem *procuraParagem(Paragem *head, char *nomeParagem) {

    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem))
            return head;
        head = head->next;
    }
    return NULL;

}