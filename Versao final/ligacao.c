/* iaed-23 - ist1106535 - project2 */

/*
    File: ligacao.c
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com funcoes associadas ao comando l
*/

#include "global.h"

/* Comando ligacao, executa funcoes diferentes dependendo do input */
void comandoLigacao(Carreira *headCar, Paragem *headPar) {
    NodeLigacao ligacao;
    Carreira *auxCarreira;
    char *nomeCarreira;
    int tipoLigacao;
    lePalavra(&nomeCarreira);
    lePalavra(&ligacao.origem);
    lePalavra(&ligacao.destino);
    leDouble(ligacao.custo);
    leDouble(ligacao.duracao);
    if (!verificaCarreira(headCar, nomeCarreira)) {
        freeArgumentos(nomeCarreira, ligacao.origem, ligacao.destino);
        return;
    }
    if (!verificaParagem(headPar, ligacao.origem)) {
        freeArgumentos(nomeCarreira, ligacao.origem, ligacao.destino);
        return;
    }
    if (!verificaParagem(headPar, ligacao.destino)) {
        freeArgumentos(nomeCarreira, ligacao.origem, ligacao.destino);
        return;
    }
    auxCarreira = procuraCarreira(headCar, nomeCarreira);
    if ((tipoLigacao = verificaLigacao(*auxCarreira, ligacao)) == NONE) {
        freeArgumentos(nomeCarreira, ligacao.origem, ligacao.destino);
        return;
    }
    if (!verificaCustoDuracao(ligacao)) {
        freeArgumentos(nomeCarreira, ligacao.origem, ligacao.destino);
        return;
    }
    free(nomeCarreira);
    criaLigacao(auxCarreira, headPar, ligacao, tipoLigacao);
}

/* Liberta a memoria associada aos argumentos passados */
void freeArgumentos(char *nomeCar, char *nomeOri, char *nomeDes) {

    free(nomeCar);
    free(nomeOri);
    free(nomeDes);

}

/* Verifica se existe uma carreira com o nome passado */
int verificaCarreira(Carreira *head, char *nomeCarreira) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira))
            return SIM;
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    return NAO;

}

/* Verifica se existe uma paragem com o nome passado */
int verificaParagem(Paragem *head, char *nomeParagem) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem))
            return SIM;
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    return NAO;

}

/* Verifica se a ligacao pode ser ligada a carreira */
int verificaLigacao(Carreira carreira, NodeLigacao ligacao) {
    
    int tipoLigacao;
    if (carreira.numLigacao == 0)
        tipoLigacao = NOVO;
    else if (!strcmp(nomeDestinoCarreira(carreira.ligacoes), ligacao.origem))
        tipoLigacao = FIM;
    else if (!strcmp(nomeOrigemCarreira(carreira.ligacoes), ligacao.destino))
        tipoLigacao = INICIO;
    else {
        tipoLigacao = NONE;
        printf("link cannot be associated with bus line.\n");
    }
    return tipoLigacao;

}

/* Verifica se o custo e a duracao sao numeros positivos */
int verificaCustoDuracao(NodeLigacao ligacao) {

    if (ligacao.custo >= 0 && ligacao.duracao >= 0)
        return SIM;
    else {
        printf("negative cost or duration.\n");
        return NAO;
    }

}

/* Cria ligacao com os argumentos passados */
void criaLigacao(Carreira *carreira, Paragem *headPar, NodeLigacao ligacao,
int tipoLigacao) {
    
    Paragem *auxOrigem, *auxDestino;
    auxOrigem = procuraParagem(headPar, ligacao.origem);
    auxDestino = procuraParagem(headPar, ligacao.destino);
    if (tipoLigacao == INICIO)
        carreira->ligacoes = adicionaLigacaoIni(carreira->ligacoes, ligacao);
    else if (tipoLigacao == FIM)
        carreira->ligacoes = adicionaLigacaoFim(carreira->ligacoes, ligacao);
    else
        carreira->ligacoes = adicionaLigacaoNovo(ligacao);
    carreira->numLigacao++;
    carreira->custoTotal += ligacao.custo;
    carreira->duracaoTotal += ligacao.duracao;
    adicionaCarreira(auxOrigem, auxDestino, carreira->nome);

}

/* Adiciona ligacao no inicio da carreira */
NodeLigacao *adicionaLigacaoIni(NodeLigacao *headLig, NodeLigacao ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    *newNode = ligacao;
    newNode->next = headLig;
    headLig->prev = newNode;
    newNode->prev = NULL;
    return newNode;

}

/* Adiciona ligacao no fim da carreira */
NodeLigacao *adicionaLigacaoFim(NodeLigacao *headLig, NodeLigacao ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao)),
    *auxNode = headLig;
    *newNode = ligacao;
    newNode->next = NULL;
    while (auxNode->next != NULL)
        auxNode = auxNode->next;
    auxNode->next = newNode;
    newNode->prev = auxNode;
    return headLig;

}

/* Adiciona a primeira ligacao a carreira */
NodeLigacao *adicionaLigacaoNovo(NodeLigacao ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    *newNode = ligacao;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;

}

/* Adiciona carreira a paragens */
void adicionaCarreira(Paragem *origem, Paragem *destino, char *nomeCarreira) {

    if (origem->carreiras == NULL) {
        origem->carreiras = adicionaNome(origem->carreiras, nomeCarreira);
        origem->numCarreira++;
    } else {
        if (!existeCarreira(origem->carreiras, nomeCarreira)) {
            origem->carreiras = adicionaNome(origem->carreiras, nomeCarreira);
            origem->numCarreira++;
        }
    }
    if (origem != destino) {
        if (destino->carreiras == NULL) {
            destino->carreiras = adicionaNome(destino->carreiras,
            nomeCarreira);
            destino->numCarreira++;
        } else {
            if (!existeCarreira(destino->carreiras, nomeCarreira)) {
                destino->carreiras = adicionaNome(destino->carreiras,
                nomeCarreira);
                destino->numCarreira++;
            }
        }
    }

}

/* Adiciona o nome da carreira a paragem */
NodeNome *adicionaNome(NodeNome *headNome, char *nomeCarreira) {

    NodeNome *newNode = (NodeNome*) malloc(sizeof(NodeNome));
    newNode->nome = (char*) malloc(sizeof(char) * (strlen(nomeCarreira) + 1));
    strcpy(newNode->nome, nomeCarreira);
    newNode->next = headNome;
    return newNode;

}