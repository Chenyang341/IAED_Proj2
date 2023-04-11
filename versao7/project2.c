/* iaed-23 - ist1106535 - project2 */

/*
    Autor: Chenyang Ying 106535
    Descricao: ficheiro com conteudo do segundo projeto
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 65535  /* Numero maximo de caracteres numa linha */

#define NAO_DEF -1
#define NAO 0
#define SIM 1

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
    char *nomeOrigem;
    char *nomeDestino;
    double custo;
    double duracao;
    struct node_ligacao *prev;
    struct node_ligacao *next;
} NodeLigacao;

#define leDouble(A) {if (scanf("%lf", &A) == 1) {}}     /* Le um double */

int lePalavra(char **s);
Carreira *comandoCarreira(Carreira *headCar);
void semArgCarreira(Carreira *headCar);
Carreira *comArgCarreira(Carreira *headCar, Carreira carreira, char *inversao);
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
int verificaCarreira(Carreira *head, char *nomeCarreira);
int verificaParagem(Paragem *head, char *nomeParagem);
int verificaLigacao(Carreira carreira, NodeLigacao ligacao);
int verificaCustoDuracao(NodeLigacao ligacao);
void criaLigacao(Carreira *headCar, Paragem *headPar, NodeLigacao ligacao, int tipoLigacao);
NodeLigacao *adicionaLigacaoNovo(NodeLigacao ligacao);
NodeLigacao *adicionaLigacaoInicio(NodeLigacao *headLig, NodeLigacao ligacao);
NodeLigacao *adicionaLigacaoFim(NodeLigacao *headLig, NodeLigacao ligacao);
NodeNome *adicionaCarreira(NodeNome *headNome, char *nomeCarreira);
int existeCarreira(NodeNome *headNome, char *nomeCarreira);
void comandoIntersecao(Paragem *headPar);
NodeNome *sort(NodeNome *head);
char *verificaExisteCarreira(Carreira *headCar);
Carreira *removeCarreiraCarreira(Carreira *headCar, char *nomeCarreira);
Paragem *removeCarreiraParagem(Paragem *headPar, char *nomeCarreira);
char *verificaExisteParagem(Paragem *headPar);
Carreira *removeParagemCarreira(Carreira *headCar, char *nomeParagem);
Paragem *removeParagemParagem(Paragem *head, char *nomeParagem);
NodeLigacao *uneLigacoes(NodeLigacao *ligacao1, NodeLigacao *ligacao2);
Carreira *procuraCarreira(Carreira *head, char *nomeCarreira);
Paragem *procuraParagem(Paragem *head, char *nomeParagem);
char *nomeOrigemCarreira(NodeLigacao *head);
char *nomeDestinoCarreira(NodeLigacao *head);
void apagaDados(Carreira *headCar, Paragem *headPar);

int main() {

    Carreira *_carreiras = NULL;
    Paragem *_paragens = NULL;
    char comando;
    char *nome;
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
        }
        else if (comando == 'e') {
            if ((nome = verificaExisteParagem(_paragens)) != NULL) {
                _carreiras = removeParagemCarreira(_carreiras, nome);
                _paragens = removeParagemParagem(_paragens, nome);
                free(nome);
            }
        }
        else if (comando == 'a') {
            apagaDados(_carreiras, _paragens);
            _carreiras = NULL;
            _paragens = NULL;
        }
    }
    apagaDados(_carreiras, _paragens);
    return 0;

}

/* funcao auxiliar que permite ler argumentos em forma de strings com ou
sem aspas */
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

Carreira *comandoCarreira(Carreira *headCar) {

    Carreira carreira;
    char *inversao = NULL;
    if (getchar() == '\n')
        semArgCarreira(headCar);
    else {
        if (lePalavra(&carreira.nome))
            lePalavra(&inversao);
        headCar = comArgCarreira(headCar, carreira, inversao);
    }
    free(inversao);
    return headCar;

}

void semArgCarreira(Carreira *headCar) {

    while (headCar != NULL) {
        if (headCar->numLigacao > 0) {
            printf("%s %s %s %d %.2f %.2f\n", headCar->nome, nomeOrigemCarreira(headCar->ligacoes),
            nomeDestinoCarreira(headCar->ligacoes), headCar->numLigacao + 1, headCar->custoTotal, 
            headCar->duracaoTotal);
        } else
            printf("%s %d %.2f %.2f\n", headCar->nome, 0, 0.0, 0.0);
        headCar = headCar->next;
    }

}

Carreira *comArgCarreira(Carreira *headCar, Carreira carreira, char *inversao) {

    int e_carreira = NAO;
    Carreira *auxCarreira = headCar;
    while (auxCarreira != NULL) {
        if (!strcmp(auxCarreira->nome, carreira.nome)) {
            e_carreira = SIM;
            if (auxCarreira->numLigacao > 0) {
                if (inversao == NULL)
                    printParagensNormal(auxCarreira->ligacoes);
                else if (verificaInversoOk(inversao))
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

void printParagensNormal(NodeLigacao *headLig) {

    printf("%s", headLig->nomeOrigem);
    while (headLig != NULL) {
        printf(", %s", headLig->nomeDestino);
        headLig = headLig->next;
    }
    printf("\n");

}

void printParagensInverso(NodeLigacao *headLig) {

    while (headLig->next != NULL)
        headLig = headLig->next;
    printf("%s", headLig->nomeDestino);
    while (headLig != NULL) {
        printf(", %s", headLig->nomeOrigem);
        headLig = headLig->prev;
    }
    printf("\n");
    
}

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

/* funcao que eh executada caso o comando seja "p" */
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

/* Funcao auxiliar que le os asgumentos do comando introduzido */
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

void semArgParagem(Paragem *head) {

    while (head != NULL) {
        printf("%s: %16.12f %16.12f %d\n", head->nome, head->latitude, head->longitude, head->numCarreira);
        head = head->next;
    }

}

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

void comandoLigacao(Carreira *headCar, Paragem *headPar) {

    NodeLigacao ligacao;
    Carreira *auxCarreira;
    char *nomeCarreira;
    int tipoLigacao;
    lePalavra(&nomeCarreira);
    lePalavra(&ligacao.nomeOrigem);
    lePalavra(&ligacao.nomeDestino);
    leDouble(ligacao.custo);
    leDouble(ligacao.duracao);
    if (!verificaCarreira(headCar, nomeCarreira)) {
        free(nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return;
    }
    if (!verificaParagem(headPar, ligacao.nomeOrigem)) {
        free(nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return;
    }
    if (!verificaParagem(headPar, ligacao.nomeDestino)) {
        free(nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return;
    }
    auxCarreira = procuraCarreira(headCar, nomeCarreira);
    free(nomeCarreira);
    if ((tipoLigacao = verificaLigacao(*auxCarreira, ligacao)) == NONE) {
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return;
    }
    if (!verificaCustoDuracao(ligacao)) {
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return;
    }
    criaLigacao(auxCarreira, headPar, ligacao, tipoLigacao);

}

int verificaCarreira(Carreira *head, char *nomeCarreira) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira))
            return SIM;
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    return NAO;

}

int verificaParagem(Paragem *head, char *nomeParagem) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem))
            return SIM;
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    return NAO;

}

int verificaLigacao(Carreira carreira, NodeLigacao ligacao) {
    
    int tipoLigacao;
    if (carreira.numLigacao == 0)
        tipoLigacao = NOVO;
    else if (!strcmp(nomeDestinoCarreira(carreira.ligacoes), ligacao.nomeOrigem))
        tipoLigacao = FIM;
    else if (!strcmp(nomeOrigemCarreira(carreira.ligacoes), ligacao.nomeDestino))
        tipoLigacao = INICIO;
    else {
        tipoLigacao = NONE;
        printf("link cannot be associated with bus line.\n");
    }
    return tipoLigacao;

}

int verificaCustoDuracao(NodeLigacao ligacao) {

    if (ligacao.custo >= 0 && ligacao.duracao >= 0)
        return SIM;
    else {
        printf("negative cost or duration.\n");
        return NAO;
    }

}

void criaLigacao(Carreira *carreira, Paragem *headPar, NodeLigacao ligacao, int tipoLigacao) {
    
    Paragem *auxOrigem, *auxDestino;
    auxOrigem = procuraParagem(headPar, ligacao.nomeOrigem);
    auxDestino = procuraParagem(headPar, ligacao.nomeDestino);
    if (tipoLigacao == INICIO)
        carreira->ligacoes = adicionaLigacaoInicio(carreira->ligacoes, ligacao);
    else if (tipoLigacao == FIM)
        carreira->ligacoes = adicionaLigacaoFim(carreira->ligacoes, ligacao);
    else
        carreira->ligacoes = adicionaLigacaoNovo(ligacao);
    carreira->numLigacao++;
    carreira->custoTotal += ligacao.custo;
    carreira->duracaoTotal += ligacao.duracao;
    if (auxOrigem->carreiras == NULL) {
        auxOrigem->carreiras = adicionaCarreira(auxOrigem->carreiras, carreira->nome);
        auxOrigem->numCarreira++;
    } else {
        if (!existeCarreira(auxOrigem->carreiras, carreira->nome)) {
            auxOrigem->carreiras = adicionaCarreira(auxOrigem->carreiras, carreira->nome);
            auxOrigem->numCarreira++;
        }
    }
    if (auxOrigem != auxDestino) {
        if (auxDestino->carreiras == NULL) {
            auxDestino->carreiras = adicionaCarreira(auxDestino->carreiras, carreira->nome);
            auxDestino->numCarreira++;
        } else {
            if (!existeCarreira(auxDestino->carreiras, carreira->nome)) {
                auxDestino->carreiras = adicionaCarreira(auxDestino->carreiras, carreira->nome);
                auxDestino->numCarreira++;
            }
        }
    }

}

NodeLigacao *adicionaLigacaoNovo(NodeLigacao ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    *newNode = ligacao;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;

}

NodeLigacao *adicionaLigacaoInicio(NodeLigacao *headLig, NodeLigacao ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    *newNode = ligacao;
    newNode->next = headLig;
    headLig->prev = newNode;
    newNode->prev = NULL;
    return newNode;

}

NodeLigacao *adicionaLigacaoFim(NodeLigacao *headLig, NodeLigacao ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao)), *auxNode = headLig;
    *newNode = ligacao;
    newNode->next = NULL;
    while (auxNode->next != NULL)
        auxNode = auxNode->next;
    auxNode->next = newNode;
    newNode->prev = auxNode;
    return headLig;

}

NodeNome *adicionaCarreira(NodeNome *headNome, char *nomeCarreira) {

    NodeNome *newNode = (NodeNome*) malloc(sizeof(NodeNome));
    newNode->nome = (char*) malloc(sizeof(char) * (strlen(nomeCarreira) + 1));
    strcpy(newNode->nome, nomeCarreira);
    newNode->next = headNome;
    return newNode;

}

int existeCarreira(NodeNome *headNome, char *nomeCarreira) {

    while (headNome != NULL) {
        if (!strcmp(headNome->nome, nomeCarreira))
            return SIM;
        headNome = headNome->next;
    }
    return NAO;

}

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

Carreira *removeCarreiraCarreira(Carreira *headCar, char *nomeCarreira) {

    Carreira *auxCarreira = headCar, *tempCarreira, *prev;
    NodeLigacao *tempNode;
    if (!strcmp(headCar->nome, nomeCarreira)) {
        tempCarreira = headCar;
        while (headCar->ligacoes != NULL) {
            tempNode = headCar->ligacoes;
            headCar->ligacoes = headCar->ligacoes->next;
            free(tempNode->nomeOrigem);
            free(tempNode->nomeDestino);
            free(tempNode);
        }
        headCar = headCar->next;
        free(tempCarreira->nome);
        free(tempCarreira);
        return headCar;
    } else {
        while (auxCarreira != NULL) {
            if (!strcmp(auxCarreira->nome, nomeCarreira)) {
                prev->next = auxCarreira->next;
                while (auxCarreira->ligacoes != NULL) {
                    tempNode = auxCarreira->ligacoes;
                    auxCarreira->ligacoes = auxCarreira->ligacoes->next;
                    free(tempNode->nomeOrigem);
                    free(tempNode->nomeDestino);
                    free(tempNode);
                }
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

Paragem *removeCarreiraParagem(Paragem *headPar, char *nomeCarreira) {
    
    Paragem *auxParagem = headPar;
    NodeNome *auxNode, *tempNode, *prev;
    while (auxParagem != NULL) {
        if (auxParagem->carreiras != NULL) {
            if (!strcmp(auxParagem->carreiras->nome, nomeCarreira)) {
                if (auxParagem->numCarreira == 1) {
                    free(auxParagem->carreiras->nome);
                    free(auxParagem->carreiras);
                    auxParagem->numCarreira = 0;
                    auxParagem->carreiras = NULL;
                } else {
                    tempNode = auxParagem->carreiras;
                    auxParagem->carreiras = auxParagem->carreiras->next;
                    auxParagem->numCarreira--;
                    free(tempNode->nome);
                    free(tempNode);
                }
            } else {
                prev = auxParagem->carreiras;
                auxNode = auxParagem->carreiras->next;
                while (auxNode != NULL) {
                    if (!strcmp(auxNode->nome, nomeCarreira)) {
                        tempNode = auxNode;
                        prev->next = auxNode->next;
                        auxParagem->numCarreira--;
                        free(tempNode->nome);
                        free(tempNode);
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

Carreira *removeParagemCarreira(Carreira *headCar, char *nomeParagem) {

    Carreira *auxCarreira = headCar;
    NodeLigacao *auxNode = NULL, *tempNode;
    while (auxCarreira != NULL) {
        while (auxCarreira->ligacoes != NULL) {
            if (!strcmp(auxCarreira->ligacoes->nomeOrigem, nomeParagem)) {
                if (auxCarreira->numLigacao == 1) {
                    auxCarreira->custoTotal = 0;
                    auxCarreira->duracaoTotal = 0;
                    auxCarreira->numLigacao = 0;
                    free(auxCarreira->ligacoes->nomeOrigem);
                    free(auxCarreira->ligacoes->nomeDestino);
                    free(auxCarreira->ligacoes);
                    auxCarreira->ligacoes = NULL;
                    break;
                } else {
                    tempNode = auxCarreira->ligacoes;
                    auxCarreira->custoTotal -= auxCarreira->ligacoes->custo;
                    auxCarreira->duracaoTotal -= auxCarreira->ligacoes->duracao;
                    auxCarreira->ligacoes = auxCarreira->ligacoes->next;
                    auxCarreira->ligacoes->prev = NULL;
                    auxCarreira->numLigacao--;
                    free(tempNode->nomeOrigem);
                    free(tempNode->nomeDestino);
                    free(tempNode);
                }
            } else if (!strcmp(auxCarreira->ligacoes->nomeDestino, nomeParagem)) {
                if (auxCarreira->numLigacao == 1) {
                    auxCarreira->custoTotal = 0;
                    auxCarreira->duracaoTotal = 0;
                    auxCarreira->numLigacao = 0;
                    free(auxCarreira->ligacoes->nomeOrigem);
                    free(auxCarreira->ligacoes->nomeDestino);
                    free(auxCarreira->ligacoes);
                    auxCarreira->ligacoes = NULL;
                    break;
                } else {
                    auxCarreira->ligacoes = uneLigacoes(auxCarreira->ligacoes, auxCarreira->ligacoes->next);
                    tempNode = auxCarreira->ligacoes->next;
                    auxCarreira->ligacoes->next = auxCarreira->ligacoes->next->next;
                    if (auxCarreira->ligacoes->next != NULL)
                        auxCarreira->ligacoes->next->prev = auxCarreira->ligacoes;
                    auxCarreira->numLigacao--;
                    free(tempNode->nomeOrigem);
                    free(tempNode);
                }
            } else {
                auxNode = auxCarreira->ligacoes->next;
                break;
            }
        }
        while (auxNode != NULL) {
            if (!strcmp(auxNode->nomeDestino, nomeParagem)) {
                if (auxNode->next == NULL) {
                    auxCarreira->custoTotal -= auxNode->custo;
                    auxCarreira->duracaoTotal -= auxNode->duracao;
                    tempNode = auxNode;
                    auxNode->prev->next = NULL;
                    auxNode = auxNode->next;
                    auxCarreira->numLigacao--;
                    free(tempNode->nomeOrigem);
                    free(tempNode->nomeDestino);
                    free(tempNode);
                } else {
                    auxNode = uneLigacoes(auxNode, auxNode->next);
                    tempNode = auxNode->next;
                    auxNode->next = auxNode->next->next;
                    if (auxNode->next != NULL)
                        auxNode->next->prev = auxNode;
                    auxCarreira->numLigacao--;
                    free(tempNode->nomeOrigem);
                    free(tempNode);
                }
            } else
                auxNode = auxNode->next;
        }
        auxCarreira = auxCarreira->next;
    }
    return headCar;

}

Paragem *removeParagemParagem(Paragem *head, char *nomeParagem) {

    Paragem *auxParagem, *tempParagem, *prev;
    NodeNome *tempNode;
    if (!strcmp(head->nome ,nomeParagem)) {
        tempParagem = head;
        while (head->carreiras != NULL) {
            tempNode = head->carreiras;
            head->carreiras = head->carreiras->next;
            free(tempNode->nome);
            free(tempNode);
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
                    tempNode = auxParagem->carreiras;
                    auxParagem->carreiras = auxParagem->carreiras->next;
                    free(tempNode->nome);
                    free(tempNode);
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

NodeLigacao *uneLigacoes(NodeLigacao *ligacao1, NodeLigacao *ligacao2) {

    free(ligacao1->nomeDestino);
    ligacao1->nomeDestino = ligacao2->nomeDestino;
    ligacao1->custo += ligacao2->custo;
    ligacao1->duracao += ligacao2->duracao;
    return ligacao1;

}

Carreira *procuraCarreira(Carreira *head, char *nomeCarreira) {

    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira))
            return head;
        head = head->next;
    }
    return NULL;

}

Paragem *procuraParagem(Paragem *head, char *nomeParagem) {

    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem))
            return head;
        head = head->next;
    }
    return NULL;

}
char *nomeOrigemCarreira(NodeLigacao *head) {
    
    if (head == NULL)
        return NULL;
    return head->nomeOrigem;

}

char *nomeDestinoCarreira(NodeLigacao *head) {

    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head->nomeDestino;

}

void apagaDados(Carreira *headCar, Paragem *headPar) {
    
    Carreira *auxCarreira;
    Paragem *auxParagem;
    NodeLigacao *auxLigacao;
    NodeNome *auxNome;
    while (headCar != NULL) {
        while (headCar->ligacoes != NULL) {
            auxLigacao = headCar->ligacoes;
            headCar->ligacoes = headCar->ligacoes->next;
            free(auxLigacao->nomeOrigem);
            free(auxLigacao->nomeDestino);
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