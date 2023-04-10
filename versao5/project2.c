/* iaed-23 - ist1106535 - project2 */

/* iaed-23 - ist1106535 - project1 */

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

typedef struct node_id {
    int id;
    struct node_id *next;
    struct node_id *prev;
} NodeId;

typedef struct carreira {
    int id;
    char *nome;
    int numLigacao;
    double custoTotal;
    double duracaoTotal;
    struct node_id *idLigacao;
    struct carreira *next;
} Carreira;

typedef struct paragem{
    int id;
    char *nome;
    double latitude;
    double longitude;
    int numCarreira;
    struct node_id *idCarreira;
    struct paragem *next;
} Paragem;

typedef struct ligacao{
    int id;
    int idCarreira;
    int idOrigem;
    int idDestino;
    double custo;
    double duracao;
    struct ligacao *next;
} Ligacao;

#define leDouble(A) {if (scanf("%lf", &A) == 1) {}}     /* Le um double */

int lePalavra(char **s);
Carreira *comandoCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig);
void semArgCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig);
Carreira *comArgCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig, Carreira carreira, char *inversao);
int verificaInversoOk(char *s);
void printParagensNormal(Paragem *headPar, Ligacao *headLig, NodeId *headId);
void printParagensInverso(Paragem *headPar, Ligacao *headLig, NodeId *headId);
Carreira *criaCarreira(Carreira *head, Carreira carreira);
Paragem *comandoParagem(Paragem *head);
Paragem leInputParagem();
void semArgParagem(Paragem *head);
void umArgParagem(Paragem *head, Paragem paragem);
Paragem *tresArgParagem(Paragem *head, Paragem paragem);
Paragem *criaParagem(Paragem *head, Paragem paragem);
Ligacao *comandoLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar);
int verificaCarreira(Carreira *head, char *nomeCarreira);
int verificaParagem(Paragem *head, char *nomeParagem);
int verificaLigacao(Paragem *headPar, Ligacao *headLig, Carreira carreira, Ligacao ligacao);
int verificaCustoDuracao(Ligacao ligacao);
Ligacao *criaLigacao(Carreira *headCar, Paragem *headPar, Ligacao *headLig, Ligacao ligacao, int tipoLigacao);
NodeId *adicionaIdNovo(int id);
NodeId *adicionaIdInicio(NodeId *head, int id);
NodeId *adicionaIdFim(NodeId *head, int id);
int existeCarreira(NodeId *head, int id);
void comandoIntersecao(Carreira *headCar, Paragem *headPar);
NodeId *sort(Carreira *headCar, NodeId *head);
int verificaExisteCarreira(Carreira *head);
Carreira *removeCarreiraCarreira(Carreira *head, int idCarreira);
Paragem *removeCarreiraParagem(Paragem *head, int idCarreira);
Ligacao *removeCarreiraLigacao(Ligacao *head, int idCarreira);
int verificaExisteParagem(Paragem *head);
Carreira *removeParagemCarreira(Carreira *headCar, Ligacao **headLig, int idParagem);
Paragem *removeParagemParagem(Paragem *head, int idParagem);
Ligacao *removeParagemLigacao(Ligacao *head, int idParagem);
Ligacao *uneLigacoes(Ligacao *head, Ligacao *ligacao1, Ligacao *ligacao2);
Carreira *procuraCarreira(Carreira *head, int id);
Paragem *procuraParagem(Paragem *head, int id);
Ligacao *procuraLigacao(Ligacao *head, int id);
Paragem *procuraOrigem(Paragem *headPar, Ligacao *headLig, int id);
Paragem *procuraDestino(Paragem *headPar, Ligacao *headLig, int id);
Paragem *procuraOrigemCarreira(Paragem *headPar, Ligacao *headLig, NodeId *head);
Paragem *procuraDestinoCarreira(Paragem *headPar, Ligacao *headLig, NodeId *head);
void apagaDados(Carreira *headCar, Paragem *headPar, Ligacao *headLig);

int main() {

    Carreira *_carreiras = NULL;
    Paragem *_paragens = NULL;
    Ligacao *_ligacoes = NULL;
    char comando;
    int id;
    while ((comando = getchar()) != 'q') {
        if (comando == 'c')
            _carreiras = comandoCarreira(_carreiras, _paragens, _ligacoes);
        else if (comando == 'p')
            _paragens = comandoParagem(_paragens);
        else if (comando == 'l')
            _ligacoes = comandoLigacao(_ligacoes, _carreiras, _paragens);
        else if (comando == 'i')
            comandoIntersecao(_carreiras, _paragens);
        else if (comando == 'r') {
            if ((id = verificaExisteCarreira(_carreiras)) != NAO_DEF) {
                _carreiras = removeCarreiraCarreira(_carreiras, id);
                _paragens = removeCarreiraParagem(_paragens, id);
                _ligacoes = removeCarreiraLigacao(_ligacoes, id);
            }
        } else if (comando == 'e') {
            if ((id = verificaExisteParagem(_paragens)) != NAO_DEF) {
                _carreiras = removeParagemCarreira(_carreiras, &_ligacoes, id);
                _paragens = removeParagemParagem(_paragens, id);
                _ligacoes = removeParagemLigacao(_ligacoes, id);
            }
        } else if (comando == 'a') {
            apagaDados(_carreiras, _paragens, _ligacoes);
            _carreiras = NULL;
            _paragens = NULL;
            _ligacoes = NULL;
        }
    }
    apagaDados(_carreiras, _paragens, _ligacoes);
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

Carreira *comandoCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {

    Carreira carreira;
    char *inversao = NULL;
    if (getchar() == '\n')
        semArgCarreira(headCar, headPar, headLig);
    else {
        if (lePalavra(&carreira.nome))
            lePalavra(&inversao);
        headCar = comArgCarreira(headCar, headPar, headLig, carreira, inversao);
    }
    free(inversao);
    return headCar;

}

void semArgCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {

    while (headCar != NULL) {
        if (headCar->numLigacao > 0) {
            printf("%s %s %s %d %.2f %.2f\n", headCar->nome, procuraOrigemCarreira(headPar, headLig, headCar->idLigacao)->nome,
            procuraDestinoCarreira(headPar, headLig, headCar->idLigacao)->nome, headCar->numLigacao + 1, headCar->custoTotal, 
            headCar->duracaoTotal);
        } else
            printf("%s %d %.2f %.2f\n", headCar->nome, 0, 0.0, 0.0);
        headCar = headCar->next;
    }

}

Carreira *comArgCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig, Carreira carreira, char *inversao) {

    int e_carreira = NAO;
    Carreira *auxCarreira = headCar;
    while (auxCarreira != NULL) {
        if (!strcmp(auxCarreira->nome, carreira.nome)) {
            e_carreira = SIM;
            if (auxCarreira->numLigacao > 0) {
                if (inversao == NULL)
                    printParagensNormal(headPar, headLig, auxCarreira->idLigacao);
                else if (verificaInversoOk(inversao))
                    printParagensInverso(headPar, headLig, auxCarreira->idLigacao);
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

void printParagensNormal(Paragem *headPar, Ligacao *headLig, NodeId *headId) {

    printf("%s", procuraOrigem(headPar, headLig, headId->id)->nome);
    while (headId != NULL) {
        printf(", %s", procuraDestino(headPar, headLig, headId->id)->nome);
        headId = headId->next;
    }
    printf("\n");

}

void printParagensInverso(Paragem *headPar, Ligacao *headLig, NodeId *headId) {

    while (headId->next != NULL)
        headId = headId->next;
    printf("%s", procuraDestino(headPar, headLig, headId->id)->nome);
    while (headId != NULL) {
        printf(", %s", procuraOrigem(headPar, headLig, headId->id)->nome);
        headId = headId->prev;
    }
    printf("\n");
    
}

Carreira *criaCarreira(Carreira *head, Carreira carreira) {

    static int id = 0;
    Carreira *newCarreira = (Carreira*) malloc(sizeof(Carreira));
    Carreira *auxCarreira = head;
    *newCarreira = carreira;
    newCarreira->id = id;
    newCarreira->numLigacao = 0;
    newCarreira->custoTotal = 0;
    newCarreira->duracaoTotal = 0;
    newCarreira->idLigacao = NULL;
    newCarreira->next = NULL;
    if (head == NULL)
        head = newCarreira;
    else {
        while (auxCarreira->next != NULL)
            auxCarreira = auxCarreira->next;
        auxCarreira->next = newCarreira;
    }
    id++;
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

    static int id = 0;
    Paragem *newParagem = (Paragem*) malloc(sizeof(Paragem));
    Paragem *auxParagem = head;
    *newParagem = paragem;
    newParagem->id = id;
    newParagem->numCarreira = 0;
    newParagem->idCarreira = NULL;
    newParagem->next = NULL;
    if (head == NULL)
        head = newParagem;
    else {
        while (auxParagem->next != NULL)
            auxParagem = auxParagem->next;
        auxParagem->next = newParagem;
    }
    id++;
    return head;

}

Ligacao *comandoLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar) {

    Ligacao ligacao;
    Carreira *auxCarreira;
    char *nomeCarreira, *nomeOrigem, *nomeDestino;
    int tipoLigacao;
    lePalavra(&nomeCarreira);
    lePalavra(&nomeOrigem);
    lePalavra(&nomeDestino);
    leDouble(ligacao.custo);
    leDouble(ligacao.duracao);
    if ((ligacao.idCarreira = verificaCarreira(headCar, nomeCarreira)) == NAO_DEF) {
        free(nomeCarreira);
        free(nomeOrigem);
        free(nomeDestino);
        return headLig;
    }
    if ((ligacao.idOrigem = verificaParagem(headPar, nomeOrigem)) == NAO_DEF) {
        free(nomeOrigem);
        free(nomeDestino);
        return headLig;
    }
    if ((ligacao.idDestino = verificaParagem(headPar, nomeDestino)) == NAO_DEF) {
        free(nomeDestino);
        return headLig;
    }
    auxCarreira = procuraCarreira(headCar, ligacao.idCarreira);
    if ((tipoLigacao = verificaLigacao(headPar, headLig, *auxCarreira, ligacao)) == NONE)
        return headLig;
    if (!verificaCustoDuracao(ligacao))
        return headLig;
    headLig = criaLigacao(auxCarreira, headPar, headLig, ligacao, tipoLigacao);
    return headLig;

}

int verificaCarreira(Carreira *head, char *nomeCarreira) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira)) {
            free(nomeCarreira);
            return head->id;
        }
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    return NAO_DEF;

}

int verificaParagem(Paragem *head, char *nomeParagem) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem)) {
            free(nomeParagem);
            return head->id;
        }
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    return NAO_DEF;

}

int verificaLigacao(Paragem *headPar, Ligacao *headLig, Carreira carreira, Ligacao ligacao) {
    
    int tipoLigacao;
    if (carreira.numLigacao == 0)
        tipoLigacao = NOVO;
    else if (procuraDestinoCarreira(headPar, headLig, carreira.idLigacao)->id == ligacao.idOrigem)
        tipoLigacao = FIM;
    else if (procuraOrigemCarreira(headPar, headLig, carreira.idLigacao)->id == ligacao.idDestino)
        tipoLigacao = INICIO;
    else {
        tipoLigacao = NONE;
        printf("link cannot be associated with bus line.\n");
    }
    return tipoLigacao;

}

int verificaCustoDuracao(Ligacao ligacao) {

    if (ligacao.custo >= 0 && ligacao.duracao >= 0)
        return SIM;
    else {
        printf("negative cost or duration.\n");
        return NAO;
    }

}

Ligacao *criaLigacao(Carreira *carreira, Paragem *headPar, Ligacao *headLig, Ligacao ligacao, int tipoLigacao) {
    
    Paragem *auxOrigem, *auxDestino;
    Ligacao *newLigacao = (Ligacao*) malloc(sizeof(Ligacao));
    *newLigacao = ligacao;
    if (headLig == NULL)
        newLigacao->id = 0;
    else
        newLigacao->id = headLig->id + 1;
    newLigacao->next = headLig;
    auxOrigem = procuraParagem(headPar, newLigacao->idOrigem);
    auxDestino = procuraParagem(headPar, newLigacao->idDestino);
    if (tipoLigacao == INICIO)
        carreira->idLigacao = adicionaIdInicio(carreira->idLigacao, newLigacao->id);
    else if (tipoLigacao == FIM)
        carreira->idLigacao = adicionaIdFim(carreira->idLigacao, newLigacao->id);
    else
        carreira->idLigacao = adicionaIdNovo(newLigacao->id);
    carreira->numLigacao++;
    carreira->custoTotal += newLigacao->custo;
    carreira->duracaoTotal += newLigacao->duracao;
    if (auxOrigem->idCarreira == NULL) {
        auxOrigem->idCarreira = adicionaIdNovo(carreira->id);
        auxOrigem->numCarreira++;
    } else {
        if (!existeCarreira(auxOrigem->idCarreira, carreira->id)) {
            auxOrigem->idCarreira = adicionaIdInicio(auxOrigem->idCarreira, carreira->id);
            auxOrigem->numCarreira++;
        }
    }
    if (auxOrigem != auxDestino) {
        if (auxDestino->idCarreira == NULL) {
            auxDestino->idCarreira = adicionaIdNovo(carreira->id);
            auxDestino->numCarreira++;
        } else {
            if (!existeCarreira(auxDestino->idCarreira, carreira->id)) {
                auxDestino->idCarreira = adicionaIdInicio(auxDestino->idCarreira, carreira->id);
                auxDestino->numCarreira++;
            }
        }
    }
    return newLigacao;

}

NodeId *adicionaIdNovo(int id) {

    NodeId *newNode = (NodeId*) malloc(sizeof(NodeId));
    newNode->id = id;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;

}

NodeId *adicionaIdInicio(NodeId *head, int id) {

    NodeId *newNode = (NodeId*) malloc(sizeof(NodeId));
    newNode->id = id;
    newNode->next = head;
    head->prev = newNode;
    newNode->prev = NULL;
    return newNode;

}

NodeId *adicionaIdFim(NodeId *head, int id) {

    NodeId *newNode = (NodeId*) malloc(sizeof(NodeId)), *auxNode = head;
    newNode->id = id;
    newNode->next = NULL;
    while (auxNode->next != NULL)
        auxNode = auxNode->next;
    auxNode->next = newNode;
    newNode->prev = auxNode;
    return head;

}

int existeCarreira(NodeId *head, int id) {

    while (head != NULL) {
        if (head->id == id)
            return SIM;
        head = head->next;
    }
    return NAO;

}

void comandoIntersecao(Carreira *headCar, Paragem *headPar) {
    
    NodeId *auxNode;
    while (headPar != NULL) {
        if (headPar->numCarreira > 1) {
            auxNode = sort(headCar, headPar->idCarreira);
            printf("%s %d:", headPar->nome, headPar->numCarreira);
            while(auxNode != NULL) {
                printf(" %s", procuraCarreira(headCar, auxNode->id)->nome);
                auxNode = auxNode->next;
            }
            printf("\n");
        }
        headPar = headPar->next;
    }

}

NodeId *sort(Carreira *headCar, NodeId *headNode) {
    
    NodeId *auxNode1 = headNode, *auxNode2;
    char *nome1, *nome2;
    int temp;
    while (auxNode1 != NULL) {
        auxNode2 = headNode;
        while (auxNode2->next != NULL) {
            nome1 = procuraCarreira(headCar, auxNode2->id)->nome;
            nome2 = procuraCarreira(headCar, auxNode2->next->id)->nome;
            if (strcmp(nome1, nome2) > 0) {
                temp = auxNode2->id;
                auxNode2->id = auxNode2->next->id;
                auxNode2->next->id = temp;
            }
            auxNode2 = auxNode2->next;
        }
        auxNode1 = auxNode1->next;
    }
    return headNode;

}

int verificaExisteCarreira(Carreira *head) {

    char *nomeCarreira;
    lePalavra(&nomeCarreira);
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira)) {
            free(nomeCarreira);
            return head->id;
        }
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    free(nomeCarreira);
    return NAO_DEF;

}

Carreira *removeCarreiraCarreira(Carreira *head, int idCarreira) {

    Carreira *auxCarreira = head, *tempCarreira, *prev;
    NodeId *tempNode;
    if (head->id == idCarreira) {
        tempCarreira = head;
        while (head->idLigacao != NULL) {
            tempNode = head->idLigacao;
            head->idLigacao = head->idLigacao->next;
            free(tempNode);
        }
        head = head->next;
        free(tempCarreira->nome);
        free(tempCarreira);
        return head;
    } else {
        while (auxCarreira != NULL) {
            if (auxCarreira->id == idCarreira) {
                prev->next = auxCarreira->next;
                while (auxCarreira->idLigacao != NULL) {
                    tempNode = auxCarreira->idLigacao;
                    auxCarreira->idLigacao = auxCarreira->idLigacao->next;
                    free(tempNode);
                }
                free(auxCarreira->nome);
                free(auxCarreira);
                return head;
            }
            prev = auxCarreira;
            auxCarreira = auxCarreira->next;
        }
    }
    return head;

}

Paragem *removeCarreiraParagem(Paragem *head, int idCarreira) {
    
    Paragem *auxParagem = head;
    NodeId *auxNode, *tempNode;
    while (auxParagem != NULL) {
        if (auxParagem->idCarreira != NULL) {
            if (auxParagem->idCarreira->id == idCarreira) {
                if (auxParagem->numCarreira == 1) {
                    free(auxParagem->idCarreira);
                    auxParagem->numCarreira = 0;
                    auxParagem->idCarreira = NULL;
                } else {
                    tempNode = auxParagem->idCarreira;
                    auxParagem->idCarreira = auxParagem->idCarreira->next;
                    auxParagem->idCarreira->prev = NULL;
                    auxParagem->numCarreira--;
                    free(tempNode);
                }
            } else {
                auxNode = auxParagem->idCarreira->next;
                while (auxNode != NULL) {
                    if (auxNode->id == idCarreira) {
                        tempNode = auxNode;
                        auxNode->prev->next = auxNode->next;
                        if (auxNode->next != NULL)
                            auxNode->next->prev = auxNode->prev;
                        auxNode = auxNode->next;
                        auxParagem->numCarreira--;
                        free(tempNode);
                        break;
                    }
                    auxNode = auxNode->next;
                }
            }
        }
        auxParagem = auxParagem->next;
    }
    return head;

}

Ligacao *removeCarreiraLigacao(Ligacao *head, int idCarreira) {

    Ligacao *auxLigacao = NULL, *prev, *tempLigacao;
    while (head != NULL) {
        if (head->idCarreira == idCarreira) {
            tempLigacao = head;
            head = head->next;
            free(tempLigacao);
        } else {
            auxLigacao = head->next;
            break;
        }
    }
    prev = head;
    while (auxLigacao != NULL) {
        if (auxLigacao->idCarreira == idCarreira) {
            prev->next = auxLigacao->next;
            tempLigacao = auxLigacao;
            auxLigacao = auxLigacao->next;
            free(tempLigacao);
        } else {
            prev = auxLigacao;
            auxLigacao = auxLigacao->next;
        }
    }
    return head;

}

int verificaExisteParagem(Paragem *head) {

    char *nomeParagem;
    lePalavra(&nomeParagem);
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem)) {
            free(nomeParagem);
            return head->id;
        }
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    free(nomeParagem);
    return NAO_DEF;

}

Carreira *removeParagemCarreira(Carreira *headCar, Ligacao **headLig, int idParagem) {

    Carreira *auxCarreira = headCar;
    Ligacao *auxLigacao1, *auxLigacao2;
    NodeId *auxNode = NULL, *tempNode;
    while (auxCarreira != NULL) {
        while (auxCarreira->idLigacao != NULL) {
            if (procuraLigacao(*headLig, auxCarreira->idLigacao->id)->idOrigem == idParagem) {
                if (auxCarreira->numLigacao == 1) {
                    auxCarreira->custoTotal = 0;
                    auxCarreira->duracaoTotal = 0;
                    auxCarreira->numLigacao = 0;
                    free(auxCarreira->idLigacao);
                    auxCarreira->idLigacao = NULL;
                    break;
                } else {
                    tempNode = auxCarreira->idLigacao;
                    auxLigacao1 = procuraLigacao(*headLig, auxCarreira->idLigacao->id);
                    auxCarreira->idLigacao = auxCarreira->idLigacao->next;
                    auxCarreira->idLigacao->prev = NULL;
                    auxCarreira->custoTotal -= auxLigacao1->custo;
                    auxCarreira->duracaoTotal -= auxLigacao1->duracao;
                    auxCarreira->numLigacao--;
                    free(tempNode);
                }
            } else if (procuraLigacao(*headLig, auxCarreira->idLigacao->id)->idDestino == idParagem) {
                if (auxCarreira->numLigacao == 1) {
                    auxCarreira->custoTotal = 0;
                    auxCarreira->duracaoTotal = 0;
                    auxCarreira->numLigacao = 0;
                    free(auxCarreira->idLigacao);
                    auxCarreira->idLigacao = NULL;
                    break;
                } else {
                    auxLigacao1 = procuraLigacao(*headLig, auxCarreira->idLigacao->id);
                    auxLigacao2 = procuraLigacao(*headLig, auxCarreira->idLigacao->next->id);
                    *headLig = uneLigacoes(*headLig, auxLigacao1, auxLigacao2);
                    auxCarreira->idLigacao->id = (*headLig)->id;
                    tempNode = auxCarreira->idLigacao->next;
                    auxCarreira->idLigacao->next = auxCarreira->idLigacao->next->next;
                    if (auxCarreira->idLigacao->next != NULL)
                        auxCarreira->idLigacao->next->prev = auxCarreira->idLigacao;
                    auxCarreira->numLigacao--;
                    free(tempNode);
                }
            } else {
                auxNode = auxCarreira->idLigacao->next;
                break;
            }
        }
        while (auxNode != NULL) {
            if (procuraLigacao(*headLig, auxNode->id)->idDestino == idParagem) {
                if (auxNode->next == NULL) {
                    tempNode = auxNode;
                    auxLigacao1 = procuraLigacao(*headLig, auxNode->id);
                    auxNode->prev->next = NULL;
                    auxNode = auxNode->next;
                    auxCarreira->custoTotal -= auxLigacao1->custo;
                    auxCarreira->duracaoTotal -= auxLigacao1->duracao;
                    auxCarreira->numLigacao--;
                    free(tempNode);
                } else {
                    auxLigacao1 = procuraLigacao(*headLig, auxNode->id);
                    auxLigacao2 = procuraLigacao(*headLig, auxNode->next->id);
                    *headLig = uneLigacoes(*headLig, auxLigacao1, auxLigacao2);
                    auxNode->id = (*headLig)->id;
                    tempNode = auxNode->next;
                    auxNode->next = auxNode->next->next;
                    if (auxNode->next != NULL)
                        auxNode->next->prev = auxNode;
                    auxCarreira->numLigacao--;
                    free(tempNode);
                }
            } else
                auxNode = auxNode->next;
        }
        auxCarreira = auxCarreira->next;
    }
    return headCar;

}

Paragem *removeParagemParagem(Paragem *head, int idParagem) {

    Paragem *auxParagem = head, *tempParagem, *prev;
    NodeId *tempNode;
    if (head->id == idParagem) {
        tempParagem = head;
        while (head->idCarreira != NULL) {
            tempNode = head->idCarreira;
            head->idCarreira = head->idCarreira->next;
            free(tempNode);
        }
        head = head->next;
        free(tempParagem->nome);
        free(tempParagem);
        return head;
    } else {
        while (auxParagem != NULL) {
            if (auxParagem->id == idParagem) {
                prev->next = auxParagem->next;
                while (auxParagem->idCarreira != NULL) {
                    tempNode = auxParagem->idCarreira;
                    auxParagem->idCarreira = auxParagem->idCarreira->next;
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

Ligacao *removeParagemLigacao(Ligacao *head, int idParagem) {

    Ligacao *auxLigacao = head, *tempLigacao, *prev;
    while (head != NULL) {
        if (head->idOrigem == idParagem || head->idDestino == idParagem) {
            tempLigacao = head;
            head = head->next;
            free(tempLigacao);
        } else {
            auxLigacao = head->next;
            break;
        }
    }
    prev = head;
    while (auxLigacao != NULL) {
        if (head->idOrigem == idParagem || head->idDestino == idParagem) {
            prev->next = auxLigacao->next;
            tempLigacao = auxLigacao;
            auxLigacao = auxLigacao->next;
            free(tempLigacao);
        } else {
            prev = auxLigacao;
            auxLigacao = auxLigacao->next;
        }
    }
    return head;

}

Ligacao *uneLigacoes(Ligacao *head, Ligacao *ligacao1, Ligacao *ligacao2) {

    Ligacao *newLigacao = (Ligacao*) malloc(sizeof(Ligacao));
    newLigacao->idCarreira = ligacao1->idCarreira;
    newLigacao->idOrigem = ligacao1->idOrigem;
    newLigacao->idDestino = ligacao2->idDestino;
    newLigacao->custo = ligacao1->custo + ligacao2->custo;
    newLigacao->duracao = ligacao1->duracao + ligacao2->duracao;
    newLigacao->id = head->id + 1;
    newLigacao->next = head;
    return newLigacao;

}

Carreira *procuraCarreira(Carreira *head, int id) {

    while (head != NULL) {
        if (head->id == id)
            return head;
        head = head->next;
    }
    return head;

}

Paragem *procuraParagem(Paragem *head, int id) {

    while (head != NULL) {
        if (head->id == id)
            return head;
        head = head->next;
    }
    return head;

}

Ligacao *procuraLigacao(Ligacao *head, int id) {

    while (head != NULL) {
        if (head->id == id)
            return head;
        head = head->next;
    }
    return head;

}

Paragem *procuraOrigem(Paragem *headPar, Ligacao *headLig, int id) {

    return procuraParagem(headPar, procuraLigacao(headLig, id)->idOrigem);

}

Paragem *procuraDestino(Paragem *headPar, Ligacao *headLig, int id) {

    return procuraParagem(headPar, procuraLigacao(headLig, id)->idDestino);

}

Paragem *procuraOrigemCarreira(Paragem *headPar, Ligacao *headLig, NodeId *headNode) {
    
    if (headNode == NULL)
        return NULL;
    return procuraParagem(headPar, procuraLigacao(headLig, headNode->id)->idOrigem);

}

Paragem *procuraDestinoCarreira(Paragem *headPar, Ligacao *headLig, NodeId *headNode) {

    if (headNode == NULL)
        return NULL;
    while (headNode->next != NULL)
        headNode = headNode->next;
    return procuraParagem(headPar, procuraLigacao(headLig, headNode->id)->idDestino);

}

void apagaDados(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {
    
    Carreira *auxCarreira;
    Paragem *auxParagem;
    Ligacao *auxLigacao;
    NodeId *auxNode;
    while (headCar != NULL) {
        while (headCar->idLigacao != NULL) {
            auxNode = headCar->idLigacao;
            headCar->idLigacao = headCar->idLigacao->next;
            free(auxNode);
        }
        auxCarreira = headCar;
        headCar = headCar->next;
        free(auxCarreira->nome);
        free(auxCarreira);
    }
    while (headPar != NULL) {
        while (headPar->idCarreira != NULL) {
            auxNode = headPar->idCarreira;
            headPar->idCarreira = headPar->idCarreira->next;
            free(auxNode);
        }
        auxParagem = headPar;
        headPar = headPar->next;
        free(auxParagem->nome);
        free(auxParagem);
    }
    while (headLig != NULL) {
        auxLigacao = headLig;
        headLig = headLig->next;
        free(auxLigacao);
    }

}