/* iaed-23 - ist1106535 - project2 */

/*
    File: global.h
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com todas as declaracoes
*/

#ifndef _GLOBAL_
#define _GLOBAL_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Definicao de constantes */

#define MAX_INPUT 65535

#define NAO_DEF -1
#define NAO 0
#define SIM 1

/* Tipos de ligacao */
#define NONE 0
#define INICIO 1
#define FIM 2
#define NOVO 3

typedef struct carreira {
    char *nome;
    double custoTotal;
    double duracaoTotal;
    int numLigacao;
    struct node_ligacao *ligacoes;
    struct carreira *next;
} Carreira;

typedef struct paragem{
    char *nome;
    double latitude;
    double longitude;
    int numCarreira;
    struct node_nome *carreiras;
    struct paragem *next;
} Paragem;

typedef struct node_nome {
    char *nome;
    struct node_nome *next;
} NodeNome;

typedef struct node_ligacao{
    char *origem;
    char *destino;
    double custo;
    double duracao;
    struct node_ligacao *prev;
    struct node_ligacao *next;
} NodeLigacao;
/* Le um double */
#define leDouble(A) {if (scanf("%lf", &A) == 1) {}}
/* prototipos */
int lePalavra(char **s);
Carreira *comandoCarreira(Carreira *headCar);
void semArgCarreira(Carreira *headCar);
Carreira *comArgCarreira(Carreira *headCar, Carreira carreira, char *inv);
int verificaInversoOk(char *s);
void printParagensNormal(NodeLigacao *headLig);
void printParagensInverso(NodeLigacao *headLig);
Carreira *criaCarreira(Carreira *head, Carreira carreira);
Paragem *comandoParagem(Paragem *head);
Paragem leInputParagem();
void semArgParagem(Paragem *head);
void umArgParagem(Paragem *head, Paragem paragem);
Paragem *tresArgParagem(Paragem *head, Paragem paragem);
Paragem *criaParagem(Paragem *head, Paragem paragem);
void comandoLigacao(Carreira *headCar, Paragem *headPar);
void freeArgumentos(char *nomeCar, char *nomeOri, char *nomeDes);
int verificaCarreira(Carreira *head, char *nomeCarreira);
int verificaParagem(Paragem *head, char *nomeParagem);
int verificaLigacao(Carreira carreira, NodeLigacao ligacao);
int verificaCustoDuracao(NodeLigacao ligacao);
void criaLigacao(Carreira *headCar, Paragem *headPar, NodeLigacao ligacao,
int tipoLigacao);
void adicionaCarreira(Paragem *origem, Paragem *destino, char *nomeCarreira);
NodeLigacao *adicionaLigacaoNovo(NodeLigacao ligacao);
NodeLigacao *adicionaLigacaoIni(NodeLigacao *headLig, NodeLigacao ligacao);
NodeLigacao *adicionaLigacaoFim(NodeLigacao *headLig, NodeLigacao ligacao);
NodeNome *adicionaNome(NodeNome *headNome, char *nomeCarreira);
int existeCarreira(NodeNome *headNome, char *nomeCarreira);
void comandoIntersecao(Paragem *headPar);
NodeNome *sort(NodeNome *head);
char *verificaExisteCarreira(Carreira *headCar);
Carreira *removeCarreiraCarreira(Carreira *headCar, char *nomeCarreira);
Paragem *removeCarreiraParagem(Paragem *headPar, char *nomeCarreira);
void removeLigacao(Carreira *carreira);
void removeCarreira(NodeNome *node);
char *verificaExisteParagem(Paragem *headPar);
Carreira *removeParagemCarreira(Carreira *headCar, char *nomeParagem);
Paragem *removeParagemParagem(Paragem *head, char *nomeParagem);
void removeCarreiraAvan(Paragem *paragem);
void ultimaCarreira(Paragem *paragem);
void ultimaLigacao(Carreira *carreira);
void ligacaoInicial(Carreira *carreira);
void ligacaoFinal(Carreira *carreira, NodeLigacao *node);
void ligacaoIntermedia(NodeLigacao *node);
NodeLigacao *uneLigacoes(NodeLigacao *ligacao1, NodeLigacao *ligacao2);
Carreira *procuraCarreira(Carreira *head, char *nomeCarreira);
Paragem *procuraParagem(Paragem *head, char *nomeParagem);
char *nomeOrigemCarreira(NodeLigacao *head);
char *nomeDestinoCarreira(NodeLigacao *head);
void apagaDados(Carreira *headCar, Paragem *headPar);

#endif