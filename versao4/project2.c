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

typedef struct node_nome {
    char *nome;
    struct node_nome *next;
    struct node_nome *prev;
} NodeNome;


typedef struct carreira {
    char *nome;
    int numParagem;
    double custoTotal;
    double duracaoTotal;
    struct node_nome *paragens;
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

typedef struct ligacao{
    char *nomeCarreira;
    char *nomeOrigem;
    char *nomeDestino;
    double custo;
    double duracao;
    struct ligacao *next;
} Ligacao;

#define leDouble(A) {if (scanf("%lf", &A) == 1) {}}     /* Le um double */

int lePalavra(char **s);
Carreira *comandoCarreira(Carreira *headCar);
void semArgCarreira(Carreira *headCar);
Carreira *comArgCarreira(Carreira *headCar, Carreira carreira, char *inversao);
int verificaInversoOk(char *s);
void printParagensNormal(NodeNome *head);
void printParagensInverso(NodeNome *head);
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
int verificaLigacao(Carreira carreira, Ligacao ligacao);
int verificaCustoDuracao(Ligacao ligacao);
Ligacao *criaLigacao(Carreira *headCar, Paragem *headPar, Ligacao *headLig, Ligacao ligacao, int tipoLigacao);
NodeNome *adicionaNomeNovo(char *nome);
NodeNome *adicionaNomeInicio(NodeNome *head, char *nome);
NodeNome *adicionaNomeFim(NodeNome *head, char *nome);
int existeCarreira(NodeNome *head, char *nome);
void comandoIntersecao(Paragem *head);
NodeNome *sort(NodeNome *head);
char *verificaExisteCarreira(Carreira *head);
Carreira *removeCarreiraCarreira(Carreira *head, char *nomeCarreira);
Paragem *removeCarreiraParagem(Paragem *head, char *nomeCarreira);
Ligacao *removeCarreiraLigacao(Ligacao *head, char *nomeCarreira);
char *verificaExisteParagem(Paragem *head);
Carreira *removeParagemCarreira(Carreira *headCar, Ligacao **headLig, char *nomeParagem);
Paragem *removeParagemParagem(Paragem *head, char *nomeParagem);
Ligacao *removeParagemLigacao(Ligacao *head, char *nomeParagem);
Ligacao *uneLigacoes(Ligacao *head, Ligacao *ligacao1, Ligacao *ligacao2);
Carreira *procuraCarreira(Carreira *head, char *nomeCarreira);
Paragem *procuraParagem(Paragem *head, char *nomeParagem);
Ligacao *procuraLigacao(Ligacao *head, char *nomeCarreira, char *nomeOrigem, char *nomeDestino);
char *procuraOrigem(NodeNome *head);
char *procuraDestino(NodeNome *head);
void apagaDados(Carreira *headCar, Paragem *headPar, Ligacao *headLig);

int main() {

    Carreira *_carreiras = NULL;
    Paragem *_paragens = NULL;
    Ligacao *_ligacoes = NULL;
    char comando, *nome;
    while ((comando = getchar()) != 'q') {
        if (comando == 'c')
            _carreiras = comandoCarreira(_carreiras);
        else if (comando == 'p')
            _paragens = comandoParagem(_paragens);
        else if (comando == 'l')
            _ligacoes = comandoLigacao(_ligacoes, _carreiras, _paragens);
        else if (comando == 'i')
            comandoIntersecao(_paragens);
        else if (comando == 'r') {
            if ((nome = verificaExisteCarreira(_carreiras)) != NULL) {
                _carreiras = removeCarreiraCarreira(_carreiras, nome);
                _paragens = removeCarreiraParagem(_paragens, nome);
                _ligacoes = removeCarreiraLigacao(_ligacoes, nome);
                free(nome);
            }
        }
        else if (comando == 'e') {
            if ((nome = verificaExisteParagem(_paragens)) != NULL) {
                _carreiras = removeParagemCarreira(_carreiras, &_ligacoes, nome);
                _paragens = removeParagemParagem(_paragens, nome);
                _ligacoes = removeParagemLigacao(_ligacoes, nome);
                free(nome);
            }
        }
        else if (comando == 'a') {
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
        if (headCar->numParagem > 0) {
            printf("%s %s %s %d %.2f %.2f\n", headCar->nome, procuraOrigem(headCar->paragens),
            procuraDestino(headCar->paragens), headCar->numParagem, headCar->custoTotal, 
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
            if (auxCarreira->numParagem > 0) {
                if (inversao == NULL)
                    printParagensNormal(auxCarreira->paragens);
                else if (verificaInversoOk(inversao))
                    printParagensInverso(auxCarreira->paragens);
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

void printParagensNormal(NodeNome *head) {

    printf("%s", head->nome);
    head = head->next;
    while (head != NULL) {
        printf(", %s", head->nome);
        head = head->next;
    }
    printf("\n");

}

void printParagensInverso(NodeNome *head) {

    while (head->next != NULL)
        head = head->next;
    printf("%s", head->nome);
    head = head->prev;
    while (head != NULL) {
        printf(", %s", head->nome);
        head = head->prev;
    }
    printf("\n");
    
}

Carreira *criaCarreira(Carreira *head, Carreira carreira) {

    Carreira *newCarreira = (Carreira*) malloc(sizeof(Carreira));
    Carreira *auxCarreira = head;
    *newCarreira = carreira;
    newCarreira->numParagem = 0;
    newCarreira->custoTotal = 0;
    newCarreira->duracaoTotal = 0;
    newCarreira->paragens = NULL;
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

Ligacao *comandoLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar) {

    Carreira *auxCarreira;
    Ligacao ligacao;
    int tipoLigacao;
    lePalavra(&ligacao.nomeCarreira);
    lePalavra(&ligacao.nomeOrigem);
    lePalavra(&ligacao.nomeDestino);
    leDouble(ligacao.custo);
    leDouble(ligacao.duracao);
    if (!verificaCarreira(headCar, ligacao.nomeCarreira)) {
        free(ligacao.nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return headLig;
    }
    if (!verificaParagem(headPar, ligacao.nomeOrigem)) {
        free(ligacao.nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return headLig;
    }
    if (!verificaParagem(headPar, ligacao.nomeDestino)) {
        free(ligacao.nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return headLig;
    }
    auxCarreira = procuraCarreira(headCar, ligacao.nomeCarreira);
    if ((tipoLigacao = verificaLigacao(*auxCarreira, ligacao)) == NONE) {
        free(ligacao.nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return headLig;
    }
    if (!verificaCustoDuracao(ligacao)) {
        free(ligacao.nomeCarreira);
        free(ligacao.nomeOrigem);
        free(ligacao.nomeDestino);
        return headLig;
    }
    headLig = criaLigacao(auxCarreira, headPar, headLig, ligacao, tipoLigacao);
    return headLig;

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

int verificaLigacao(Carreira carreira, Ligacao ligacao) {
    
    int tipoLigacao;
    if (carreira.numParagem == 0)
        tipoLigacao = NOVO;
    else if (!strcmp(procuraDestino(carreira.paragens), ligacao.nomeOrigem))
        tipoLigacao = FIM;
    else if (!strcmp(procuraOrigem(carreira.paragens), ligacao.nomeDestino))
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
    
    Ligacao *newLigacao = (Ligacao*) malloc(sizeof(Ligacao)), *auxLigacao = headLig;
    Paragem *auxOrigem, *auxDestino;
    *newLigacao = ligacao;
    newLigacao->next = NULL;
    auxOrigem = procuraParagem(headPar, newLigacao->nomeOrigem);
    auxDestino = procuraParagem(headPar, newLigacao->nomeDestino);
    if (tipoLigacao == INICIO) {
        carreira->paragens = adicionaNomeInicio(carreira->paragens, newLigacao->nomeOrigem);
        carreira->numParagem++;
    } else if (tipoLigacao == FIM) {
        carreira->paragens = adicionaNomeFim(carreira->paragens, newLigacao->nomeDestino);
        carreira->numParagem++;
    } else {
        carreira->paragens = adicionaNomeNovo(newLigacao->nomeDestino);
        carreira->paragens = adicionaNomeInicio(carreira->paragens, newLigacao->nomeOrigem);
        carreira->numParagem += 2;
    }
    carreira->custoTotal += newLigacao->custo;
    carreira->duracaoTotal += newLigacao->duracao;
    if (auxOrigem->carreiras == NULL) {
        auxOrigem->carreiras = adicionaNomeNovo(carreira->nome);
        auxOrigem->numCarreira++;
    } else {
        if (!existeCarreira(auxOrigem->carreiras, carreira->nome)) {
            auxOrigem->carreiras = adicionaNomeInicio(auxOrigem->carreiras, carreira->nome);
            auxOrigem->numCarreira++;
        }
    }
    if (auxOrigem != auxDestino) {
        if (auxDestino->carreiras == NULL) {
            auxDestino->carreiras = adicionaNomeNovo(carreira->nome);
            auxDestino->numCarreira++;
        } else {
            if (!existeCarreira(auxDestino->carreiras, carreira->nome)) {
                auxDestino->carreiras = adicionaNomeInicio(auxDestino->carreiras, carreira->nome);
                auxDestino->numCarreira++;
            }
        }
    }
    if (headLig == NULL)
        headLig = newLigacao;
    else {
        while (auxLigacao->next != NULL)
        auxLigacao = auxLigacao->next;
        auxLigacao->next = newLigacao;
    }
    return headLig;

}

NodeNome *adicionaNomeNovo(char *nome) {

    NodeNome *newNode = (NodeNome*) malloc(sizeof(NodeNome));
    newNode->nome = (char*) malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(newNode->nome, nome);
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;

}

NodeNome *adicionaNomeInicio(NodeNome *head, char *nome) {

    NodeNome *newNode = (NodeNome*) malloc(sizeof(NodeNome));
    newNode->nome = (char*) malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(newNode->nome, nome);
    newNode->next = head;
    head->prev = newNode;
    newNode->prev = NULL;
    return newNode;

}

NodeNome *adicionaNomeFim(NodeNome *head, char *nome) {

    NodeNome *auxNode = head;
    NodeNome *newNode = (NodeNome*) malloc(sizeof(NodeNome));
    newNode->nome = (char*) malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(newNode->nome, nome);
    newNode->next = NULL;
    while (auxNode->next != NULL)
        auxNode = auxNode->next;
    auxNode->next = newNode;
    newNode->prev = auxNode;
    return head;

}

int existeCarreira(NodeNome *head, char *nome) {

    while (head != NULL) {
        if (!strcmp(head->nome, nome))
            return SIM;
        head = head->next;
    }
    return NAO;

}

void comandoIntersecao(Paragem *head) {
    
    NodeNome *auxNode;
    while (head != NULL) {
        if (head->numCarreira > 1) {
            auxNode = sort(head->carreiras);
            printf("%s %d:", head->nome, head->numCarreira);
            while(auxNode != NULL) {
                printf(" %s", auxNode->nome);
                auxNode = auxNode->next;
            }
            printf("\n");
        }
        head = head->next;
    }

}

NodeNome *sort(NodeNome *head) {
    
    NodeNome *auxNode1 = head, *auxNode2;
    char *temp;
    char *nome1, *nome2;
    while (auxNode1 != NULL) {
        auxNode2 = head;
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
    return head;

}

char *verificaExisteCarreira(Carreira *head) {

    char *nomeCarreira;
    lePalavra(&nomeCarreira);
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira))
            return nomeCarreira;
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    free(nomeCarreira);
    return NULL;

}

Carreira *removeCarreiraCarreira(Carreira *head, char *nomeCarreira) {

    Carreira *auxCarreira = head, *tempCarreira, *prev;
    NodeNome *tempNode;
    if (!strcmp(head->nome, nomeCarreira)) {
        tempCarreira = head;
        while (head->paragens != NULL) {
            tempNode = head->paragens;
            head->paragens = head->paragens->next;
            free(tempNode->nome);
            free(tempNode);
        }
        head = head->next;
        free(tempCarreira->nome);
        free(tempCarreira);
        return head;
    } else {
        while (auxCarreira != NULL) {
            if (!strcmp(auxCarreira->nome, nomeCarreira)) {
                prev->next = auxCarreira->next;
                while (auxCarreira->paragens != NULL) {
                    tempNode = auxCarreira->paragens;
                    auxCarreira->paragens = auxCarreira->paragens->next;
                    free(tempNode->nome);
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

Paragem *removeCarreiraParagem(Paragem *head, char *nomeCarreira) {
    
    Paragem *auxParagem = head;
    NodeNome *auxNode, *tempNode;
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
                    auxParagem->carreiras->prev = NULL;
                    auxParagem->numCarreira--;
                    free(tempNode->nome);
                    free(tempNode);
                }
            } else {
                auxNode = auxParagem->carreiras->next;
                while (auxNode != NULL) {
                    if (!strcmp(auxNode->nome, nomeCarreira)) {
                        tempNode = auxNode;
                        auxNode->prev->next = auxNode->next;
                        if (auxNode->next != NULL)
                            auxNode->next->prev = auxNode->prev;
                        auxNode = auxNode->next;
                        auxParagem->numCarreira--;
                        free(tempNode->nome);
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

Ligacao *removeCarreiraLigacao(Ligacao *head, char *nomeCarreira) {

    Ligacao *auxLigacao = NULL, *prev, *tempLigacao;
    while (head != NULL) {
        if (!strcmp(head->nomeCarreira, nomeCarreira)) {
            tempLigacao = head;
            head = head->next;
            free(tempLigacao->nomeCarreira);
            free(tempLigacao->nomeOrigem);
            free(tempLigacao->nomeDestino);
            free(tempLigacao);
        } else {
            auxLigacao = head->next;
            break;
        }
    }
    prev = head;
    while (auxLigacao != NULL) {
        if (!strcmp(auxLigacao->nomeCarreira, nomeCarreira)) {
            prev->next = auxLigacao->next;
            tempLigacao = auxLigacao;
            auxLigacao = auxLigacao->next;
            free(tempLigacao->nomeCarreira);
            free(tempLigacao->nomeOrigem);
            free(tempLigacao->nomeDestino);
            free(tempLigacao);
        } else {
            prev = auxLigacao;
            auxLigacao = auxLigacao->next;
        }
    }
    return head;

}

char *verificaExisteParagem(Paragem *head) {

    char *nomeParagem;
    lePalavra(&nomeParagem);
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem))
            return nomeParagem;
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    free(nomeParagem);
    return NULL;

}

Carreira *removeParagemCarreira(Carreira *headCar, Ligacao **headLig, char *nomeParagem) {

    Carreira *auxCarreira = headCar;
    Ligacao *auxLigacao1, *auxLigacao2;
    NodeNome *auxNode = NULL, *tempNode;
    while (auxCarreira != NULL) {
        while (auxCarreira->paragens != NULL) {
            if (!strcmp(auxCarreira->paragens->nome, nomeParagem)) {
                if (auxCarreira->numParagem == 2) {
                    auxCarreira->custoTotal = 0;
                    auxCarreira->duracaoTotal = 0;
                    auxCarreira->numParagem = 0;
                    auxCarreira->paragens = NULL;
                    break;
                } else {
                    tempNode = auxCarreira->paragens;
                    auxLigacao1 = procuraLigacao(*headLig, auxCarreira->nome, auxCarreira->paragens->nome, auxCarreira->paragens->next->nome);
                    auxCarreira->paragens = auxCarreira->paragens->next;
                    auxCarreira->paragens->prev = NULL;
                    auxCarreira->custoTotal -= auxLigacao1->custo;
                    auxCarreira->duracaoTotal -= auxLigacao1->duracao;
                    auxCarreira->numParagem--;
                    free(tempNode->nome);
                    free(tempNode);
                }
            } else {
                auxNode = auxCarreira->paragens->next;
                break;
            }
        }
        while (auxNode != NULL) {
            if (!strcmp(auxNode->nome, nomeParagem)) {
                if (auxCarreira->numParagem == 2) {
                    auxCarreira->custoTotal = 0;
                    auxCarreira->duracaoTotal = 0;
                    auxCarreira->numParagem = 0;
                    auxCarreira->paragens = NULL;
                    break;
                } else {
                    tempNode = auxNode;
                    if (auxNode->next == NULL) {
                        auxLigacao1 = procuraLigacao(*headLig, auxCarreira->nome, auxNode->prev->nome, auxNode->nome);
                        auxCarreira->custoTotal -= auxLigacao1->custo;
                        auxCarreira->duracaoTotal -= auxLigacao1->duracao;
                        auxNode->prev->next = auxNode->next;
                    } else {
                        auxLigacao1 = procuraLigacao(*headLig, auxCarreira->nome, auxNode->prev->nome, auxNode->nome);
                        auxLigacao2 = procuraLigacao(*headLig, auxCarreira->nome, auxNode->nome, auxNode->next->nome);
                        *headLig = uneLigacoes(*headLig, auxLigacao1, auxLigacao2);
                        auxNode->prev->next = auxNode->next;
                        auxNode->next->prev = auxNode->prev;
                    }
                    auxCarreira->numParagem--;
                    auxNode = auxNode->next;
                    free(tempNode->nome);
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

    Paragem *auxParagem = head, *tempParagem, *prev;
    NodeNome *tempNode;
    if (!strcmp(head->nome, nomeParagem)) {
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

Ligacao *removeParagemLigacao(Ligacao *head, char *nomeParagem) {

    Ligacao *auxLigacao = head, *tempLigacao, *prev;
    while (head != NULL) {
        if (!strcmp(head->nomeOrigem, nomeParagem) || !strcmp(head->nomeOrigem, nomeParagem)) {
            tempLigacao = head;
            head = head->next;
            free(tempLigacao->nomeCarreira);
            free(tempLigacao->nomeOrigem);
            free(tempLigacao->nomeDestino);
            free(tempLigacao);
        } else {
            auxLigacao = head->next;
            break;
        }
    }
    prev = head;
    while (auxLigacao != NULL) {
        if (!strcmp(head->nomeOrigem, nomeParagem) || !strcmp(head->nomeOrigem, nomeParagem)) {
            prev->next = auxLigacao->next;
            tempLigacao = auxLigacao;
            auxLigacao = auxLigacao->next;
            free(tempLigacao->nomeCarreira);
            free(tempLigacao->nomeOrigem);
            free(tempLigacao->nomeDestino);
            free(tempLigacao);
        } else {
            prev = auxLigacao;
            auxLigacao = auxLigacao->next;
        }
    }
    return head;

}

Ligacao *uneLigacoes(Ligacao *head, Ligacao *ligacao1, Ligacao *ligacao2) {

    Ligacao *newLigacao = (Ligacao*) malloc(sizeof(Ligacao)), *auxLigacao = head;
    newLigacao->nomeCarreira = (char*) malloc(sizeof(char) * (strlen(ligacao1->nomeCarreira) + 1));
    newLigacao->nomeOrigem = (char*) malloc(sizeof(char) * (strlen(ligacao1->nomeOrigem) + 1));
    newLigacao->nomeDestino = (char*) malloc(sizeof(char) * (strlen(ligacao2->nomeDestino) + 1));
    strcpy(newLigacao->nomeCarreira, ligacao1->nomeCarreira);
    strcpy(newLigacao->nomeOrigem, ligacao1->nomeOrigem);
    strcpy(newLigacao->nomeDestino, ligacao2->nomeDestino);
    newLigacao->custo = ligacao1->custo + ligacao2->custo;
    newLigacao->duracao = ligacao1->duracao + ligacao2->duracao;
    newLigacao->next = NULL;
    while (auxLigacao->next != NULL)
        auxLigacao = auxLigacao->next;
    auxLigacao->next = newLigacao;
    return head;

}

Carreira *procuraCarreira(Carreira *head, char *nomeCarreira) {

    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira))
            return head;
        head = head->next;
    }
    return head;

}

Paragem *procuraParagem(Paragem *head, char *nomeParagem) {

    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem))
            return head;
        head = head->next;
    }
    return head;

}

Ligacao *procuraLigacao(Ligacao *head, char *nomeCarreira, char *nomeOrigem, char *nomeDestino) {

    while (head != NULL) {
        if (!strcmp(head->nomeCarreira, nomeCarreira) && !strcmp(head->nomeOrigem, nomeOrigem) && !strcmp(head->nomeDestino, nomeDestino))
            return head;
        head = head->next;
    }
    return head;

}

char *procuraOrigem(NodeNome *head) {
    
    if (head == NULL)
        return NULL;
    return head->nome;

}

char *procuraDestino(NodeNome *head) {

    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head->nome;

}

void apagaDados(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {
    
    Carreira *auxCarreira;
    Paragem *auxParagem;
    Ligacao *auxLigacao;
    NodeNome *auxNode;
    while (headCar != NULL) {
        while (headCar->paragens != NULL) {
            auxNode = headCar->paragens;
            headCar->paragens = headCar->paragens->next;
            free(auxNode->nome);
            free(auxNode);
        }
        auxCarreira = headCar;
        headCar = headCar->next;
        free(auxCarreira->nome);
        free(auxCarreira);
    }
    while (headPar != NULL) {
        while (headPar->carreiras != NULL) {
            auxNode = headPar->carreiras;
            headPar->carreiras = headPar->carreiras->next;
            free(auxNode->nome);
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
        free(auxLigacao->nomeCarreira);
        free(auxLigacao->nomeOrigem);
        free(auxLigacao->nomeDestino);
        free(auxLigacao);
    }

}