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
#define BUFFER 8  /* Numero maximo caracteres auxiliar */

#define NAO_DEF -1
#define NAO 0
#define SIM 1

#define NONE 0
#define INICIO 1
#define FIM 2
#define NOVO 3

typedef struct node_carreira {
    struct carreira *carreira;
    struct node_carreira *next;
} NodeCarreira;

typedef struct node_ligacao {
    struct ligacao *ligacao;
    struct node_ligacao *next;
    struct node_ligacao *prev;
} NodeLigacao;

typedef struct carreira {
    char *nome;
    struct paragem *origem;
    struct paragem *destino;
    int numLigacao;
    float custoTotal;
    float duracaoTotal;
    struct node_ligacao *ligacoes;
    struct carreira *next;
} Carreira;

typedef struct paragem{
    char *nome;
    double latitude;
    double longitude;
    int numCarreira;
    struct node_carreira *carreiras;
    struct paragem *next;
} Paragem;

typedef struct ligacao{
    Carreira *carreira;
    Paragem *origem;
    Paragem *destino;
    float custo;
    float duracao;
    struct ligacao *next;
} Ligacao;

#define leDouble(A) {if (scanf("%lf", &A) == 1) {}}     /* Le um double */
#define leFloat(A) {if (scanf("%f", &A) == 1) {}}     /* Le um float */

int lePalavra(char **s);
Carreira *comandoCarreira(Carreira *headCar);
void semArgCarreira(Carreira *headCar);
Carreira *comArgCarreira(Carreira *headCar, Carreira carreira, char *inversao);
int verificaInversoOk(char *s);
void printParagensNormal(NodeLigacao *head);
void printParagensInverso(NodeLigacao *head);
Carreira *criaCarreira(Carreira *head, Carreira carreira);
Paragem *comandoParagem(Paragem *head);
Paragem leInputParagem();
void semArgParagem(Paragem *head);
void umArgParagem(Paragem *head, Paragem paragem);
Paragem *tresArgParagem(Paragem *head, Paragem paragem);
Paragem *criaParagem(Paragem *head, Paragem paragem);
Ligacao *comandoLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar);
Carreira *verificaCarreira(Carreira *head, char *nomeCarreira);
Paragem *verificaParagem(Paragem *head, char *nomeParagem);
int verificaLigacao(Ligacao ligacao);
int verificaCustoDuracao(Ligacao ligacao);
Ligacao *criaLigacao(Ligacao *headLig, Ligacao ligacao, int tipoLigacao);
NodeLigacao *adicionaLigacaoNova(Ligacao *ligacao);
NodeLigacao *adicionaLigacaoInicio(NodeLigacao *head, Ligacao *ligacao);
NodeLigacao *adicionaLigacaoFim(NodeLigacao *head, Ligacao *ligacao);
int existeCarreira(NodeCarreira *head, Carreira *carreira);
NodeCarreira *adicionaCarreira(NodeCarreira *head, Carreira *carreira);
void comandoIntersecao(Paragem *head);
NodeCarreira *sort(NodeCarreira *head);
Carreira *removeCarreira(Carreira *headCar);
Carreira *removeCarreiraCarreira(Carreira *headCar, char *nomeCarreira);
Paragem *removeCarreiraParagem(Paragem *headPar, char *nomeCarreira);
Ligacao *removeCarreiraLigacao(Ligacao *headLig, char *nomeCarreira);
Carreira *procuraCarreira(Carreira *head, int i);
Paragem *procuraParagem(Paragem *head, int i);
Ligacao *procuraLigacao(Ligacao *head, int i);
void apagaDados(Carreira *headCar, Paragem *headPar, Ligacao *headLig);

int main() {

    Carreira *_carreiras = NULL;
    Paragem *_paragens = NULL;
    Ligacao *_ligacoes = NULL;
    char comando;
    while ((comando = getchar()) != 'q') {
        if (comando == 'c')
            _carreiras = comandoCarreira(_carreiras);
        else if (comando == 'p') {
            _paragens = comandoParagem(_paragens);
        }
        else if (comando == 'l')
            _ligacoes = comandoLigacao(_ligacoes, _carreiras, _paragens);
        else if (comando == 'i')
            comandoIntersecao(_paragens);
        else if (comando == 'r')
            _carreiras = removeCarreira(_carreiras);
        else if (comando == 'a')
            apagaDados(_carreiras, _paragens, _ligacoes);
    }
    while (_carreiras != NULL) {
        while (_carreiras->ligacoes != NULL) {
            printf("%s\n", _carreiras->ligacoes->ligacao->origem->nome);
            printf("%s\n", _carreiras->ligacoes->ligacao->destino->nome);
            _carreiras->ligacoes = _carreiras->ligacoes->next;
        }
        _carreiras = _carreiras->next;
    }
    apagaDados(_carreiras, _paragens, _ligacoes);
    return 0;

}

/* funcao auxiliar que permite ler argumentos em forma de strings com ou
sem aspas */
int lePalavra(char **s) {
    
    int i = 0, aspas = NAO, dentro = NAO, max = BUFFER;
    char c;
    char *buffer = (char*) malloc(sizeof(char) * max);
    while ((c = getchar()) != '\n') {
        if (c == ' ') {
            if (dentro && aspas) {
                buffer[i++] = c;
                if (i == max) {
                    max *= 2;
                    buffer = realloc(buffer, sizeof(char) * max);
                }
            } else if (dentro && !aspas) {
                buffer[i] = '\0';
                *s = realloc(buffer, sizeof(char) * (i + 1));
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
                if (i == max) {
                    max *= 2;
                    buffer = realloc(buffer, sizeof(char) * max);
                }
            }
        }
        buffer[i] = '\0';
    }
    *s = realloc(buffer, sizeof(char) * (i + 1));
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
            printf("%s %s %s %d %.2f %.2f\n", headCar->nome, headCar->origem->nome,
            headCar->destino->nome, headCar->numLigacao + 1, headCar->custoTotal, 
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

void printParagensNormal(NodeLigacao *head) {

    printf("%s", head->ligacao->origem->nome);
    while (head != NULL) {
        printf(", %s", head->ligacao->destino->nome);
        head = head->next;
    }
    printf("\n");

}

void printParagensInverso(NodeLigacao *head) {

    while (head->next != NULL)
        head = head->next;
    printf("%s", head->ligacao->destino->nome);
    while (head != NULL) {
        printf(", %s", head->ligacao->origem->nome);
        head = head->prev;
    }
    printf("\n");
    
}

Carreira *criaCarreira(Carreira *head, Carreira carreira) {

    Carreira *newCarreira = (Carreira*) malloc(sizeof(Carreira));
    Carreira *auxCarreira = head;
    *newCarreira = carreira;
    newCarreira->origem = NULL;
    newCarreira->destino = NULL;
    newCarreira->numLigacao = 0;
    newCarreira->custoTotal = 0;
    newCarreira->duracaoTotal = 0;
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

Ligacao *comandoLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar) {

    Ligacao ligacao;
    char *nomeCarreira, *nomeOrigem, *nomeDestino;
    int tipoLigacao;
    lePalavra(&nomeCarreira);
    lePalavra(&nomeOrigem);
    lePalavra(&nomeDestino);
    leFloat(ligacao.custo);
    leFloat(ligacao.duracao);
    if ((ligacao.carreira = verificaCarreira(headCar, nomeCarreira)) == NULL) {
        free(nomeCarreira);
        free(nomeOrigem);
        free(nomeDestino);
        return headLig;
    }
    if ((ligacao.origem = verificaParagem(headPar, nomeOrigem)) == NULL) {
        free(nomeOrigem);
        free(nomeDestino);
        return headLig;
    }
    if ((ligacao.destino = verificaParagem(headPar, nomeDestino)) == NULL) {
        free(nomeDestino);
        return headLig;
    }
    if ((tipoLigacao = verificaLigacao(ligacao)) == NONE)
        return headLig;
    if (!verificaCustoDuracao(ligacao))
        return headLig;
    headLig = criaLigacao(headLig, ligacao, tipoLigacao);
    return headLig;

}

Carreira *verificaCarreira(Carreira *head, char *nomeCarreira) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira)) {
            free(nomeCarreira);
            return head;
        }
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    return NULL;

}

Paragem *verificaParagem(Paragem *head, char *nomeParagem) {
    
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem)) {
            free(nomeParagem);
            return head;
        }
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    return NULL;

}

int verificaLigacao(Ligacao ligacao) {
    
    int tipoLigacao;
    if (ligacao.carreira->numLigacao == 0)
        tipoLigacao = NOVO;
    else if (!strcmp(ligacao.carreira->destino->nome, ligacao.origem->nome))
        tipoLigacao = FIM;
    else if (!strcmp(ligacao.carreira->origem->nome, ligacao.destino->nome))
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

Ligacao *criaLigacao(Ligacao *headLig, Ligacao ligacao, int tipoLigacao) {
    
    Ligacao *newLigacao = (Ligacao*) malloc(sizeof(Ligacao));
    *newLigacao = ligacao;
    newLigacao->next = headLig;
    if (tipoLigacao == INICIO) {
        newLigacao->carreira->ligacoes = adicionaLigacaoInicio(newLigacao->carreira->ligacoes, newLigacao);
        newLigacao->carreira->origem = newLigacao->origem;
    } else if (tipoLigacao == FIM) {
        newLigacao->carreira->ligacoes = adicionaLigacaoFim(newLigacao->carreira->ligacoes, newLigacao);
        newLigacao->carreira->destino = newLigacao->destino;
    } else {
        newLigacao->carreira->ligacoes = adicionaLigacaoNova(newLigacao);
        newLigacao->carreira->origem = newLigacao->origem;
        newLigacao->carreira->destino = newLigacao->destino;
    }
    newLigacao->carreira->numLigacao++;
    newLigacao->carreira->custoTotal += newLigacao->custo;
    newLigacao->carreira->duracaoTotal += newLigacao->duracao;
    if (!existeCarreira(newLigacao->origem->carreiras, newLigacao->carreira)) {
        newLigacao->origem->carreiras = adicionaCarreira(newLigacao->origem->carreiras, newLigacao->carreira);
        newLigacao->origem->numCarreira++;
    }
    if (newLigacao->origem != newLigacao->destino && !existeCarreira(newLigacao->destino->carreiras, newLigacao->carreira)) {
        newLigacao->destino->carreiras = adicionaCarreira(newLigacao->destino->carreiras, newLigacao->carreira);
        newLigacao->destino->numCarreira++;
    }
    return newLigacao;

}

NodeLigacao *adicionaLigacaoNova(Ligacao *ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    newNode->ligacao = ligacao;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;

}

NodeLigacao *adicionaLigacaoInicio(NodeLigacao *head, Ligacao *ligacao) {

    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    newNode->ligacao = ligacao;
    newNode->next = head;
    head->prev = newNode;
    newNode->prev = NULL;
    return newNode;

}

NodeLigacao *adicionaLigacaoFim(NodeLigacao *head, Ligacao *ligacao) {

    NodeLigacao *auxNode = head;
    NodeLigacao *newNode = (NodeLigacao*) malloc(sizeof(NodeLigacao));
    newNode->ligacao = ligacao;
    newNode->next = NULL;
    while (auxNode->next != NULL)
        auxNode = auxNode->next;
    auxNode->next = newNode;
    newNode->prev = auxNode;
    return head;

}

NodeCarreira *adicionaCarreira(NodeCarreira *head, Carreira *carreira) {

    NodeCarreira *newNode = (NodeCarreira*) malloc(sizeof(NodeCarreira));
    newNode->carreira = carreira;
    newNode->next = head;
    return newNode;

}

int existeCarreira(NodeCarreira *head, Carreira *carreira) {

    while (head != NULL) {
        if (head->carreira == carreira)
            return SIM;
        head = head->next;
    }
    return NAO;

}

void comandoIntersecao(Paragem *head) {
    
    NodeCarreira *auxNode;
    while (head != NULL) {
        if (head->numCarreira > 1) {
            auxNode = sort(head->carreiras);
            printf("%s %d:", head->nome, head->numCarreira);
            while(auxNode != NULL) {
                printf(" %s", auxNode->carreira->nome);
                auxNode = auxNode->next;
            }
            printf("\n");
        }
        head = head->next;
    }

}

NodeCarreira *sort(NodeCarreira *head) {
    
    NodeCarreira *auxNode1 = head, *auxNode2;
    Carreira *temp;
    char *nome1, *nome2;
    while (auxNode1 != NULL) {
        auxNode2 = head;
        while (auxNode2->next != NULL) {
            nome1 = auxNode2->carreira->nome;
            nome2 = auxNode2->next->carreira->nome;
            if (strcmp(nome1, nome2) > 0) {
                temp = auxNode2->carreira;
                auxNode2->carreira = auxNode2->next->carreira;
                auxNode2->next->carreira = temp;
            }
            auxNode2 = auxNode2->next;
        }
        auxNode1 = auxNode1->next;
    }
    return head;

}

Carreira *removeCarreira(Carreira *headCar) {
    
    char *nomeCarreira;
    lePalavra(&nomeCarreira);
    headCar = removeCarreiraCarreira(headCar, nomeCarreira);
    free(nomeCarreira);
    return headCar;

}

Carreira *removeCarreiraCarreira(Carreira *headCar, char *nomeCarreira) {

    Carreira *auxCarreira = headCar, *prev;
    NodeLigacao *auxNodeLigacao;
    if (headCar == NULL)
        return NULL;
    if (!strcmp(headCar->nome, nomeCarreira)) {
        if (headCar->origem != NULL) {
            headCar->origem = removeCarreiraParagem(headCar->origem, nomeCarreira);
            if (strcmp(headCar->origem->nome, headCar->destino->nome))
                headCar->destino = removeCarreiraParagem(headCar->destino, nomeCarreira);
        }
        auxCarreira = headCar->next;
        while (headCar->ligacoes != NULL) {
            auxNodeLigacao = headCar->ligacoes;
            headCar->ligacoes = headCar->ligacoes->next;
            free(auxNodeLigacao);
        }
        free(headCar->nome);
        free(headCar);
        return auxCarreira;
    }
    while (auxCarreira != NULL) {
        if (!strcmp(auxCarreira->nome, nomeCarreira)) {
            if (auxCarreira->origem != NULL) {
                auxCarreira->origem = removeCarreiraParagem(auxCarreira->origem, nomeCarreira);
                if (strcmp(auxCarreira->origem->nome, auxCarreira->destino->nome))
                    auxCarreira->destino = removeCarreiraParagem(auxCarreira->destino, nomeCarreira);
            }
            prev->next = auxCarreira->next;
            while (auxCarreira->ligacoes != NULL) {
                auxNodeLigacao = auxCarreira->ligacoes;
                auxCarreira->ligacoes = auxCarreira->ligacoes->next;
                free(auxNodeLigacao);
            }
            free(auxCarreira->nome);
            free(auxCarreira);
            return headCar;
        }
        prev = auxCarreira;
        auxCarreira = auxCarreira->next;
    }
    printf("%s: no such line.", nomeCarreira);
    return headCar;

}

Paragem *removeCarreiraParagem(Paragem *head, char *nomeCarreira) {

    Paragem *auxParagem = head;
    NodeCarreira *auxNodeCarreira, *prev;
    if (!strcmp(auxParagem->carreiras->carreira->nome, nomeCarreira)) {
        auxNodeCarreira = auxParagem->carreiras;
        auxParagem->carreiras = auxParagem->carreiras->next;
        auxParagem->numCarreira--;
        free(auxNodeCarreira);
    } else {
        auxNodeCarreira = auxParagem->carreiras;
        while (auxNodeCarreira != NULL) {
            if (!strcmp(auxNodeCarreira->carreira->nome, nomeCarreira)) {
                prev->next = auxNodeCarreira->next;
                auxParagem->numCarreira--;
                free(auxNodeCarreira);
            }
            prev = auxNodeCarreira;
            auxNodeCarreira = auxNodeCarreira->next;
        }
    }
    return head;

}

Ligacao *removeCarreiraLigacao(Ligacao *head, char *nomeCarreira) {

    Ligacao *auxLigacao, *prev, *tempLigacao;
    if (!strcmp(head->carreira->nome, nomeCarreira)) {
        auxLigacao = head;
        head = head->next;
        free(auxLigacao);
    }
    auxLigacao = head;
    prev = NULL;
    while (auxLigacao != NULL) {
        if (!strcmp(auxLigacao->carreira->nome, nomeCarreira)) {
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

Carreira *procuraCarreira(Carreira *head, int i) {

    int j;
    for (j = 0; j < i; j++)
        head = head->next;
    return head;

}

Paragem *procuraParagem(Paragem *head, int i) {

    int j;
    for (j = 0; j < i; j++)
        head = head->next;
    return head;

}

Ligacao *procuraLigacao(Ligacao *head, int i) {

    int j;
    for (j = 0; j < i; j++)
        head = head->next;
    return head;

}

void apagaDados(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {
    
    Carreira *auxCarreira;
    Paragem *auxParagem;
    Ligacao *auxLigacao;
    NodeLigacao *auxNodeLigacao;
    NodeCarreira *auxNodeCarreira;
    while (headCar != NULL) {
        while (headCar->ligacoes != NULL) {
            auxNodeLigacao = headCar->ligacoes;
            headCar->ligacoes = headCar->ligacoes->next;
            free(auxNodeLigacao);
        }
        auxCarreira = headCar;
        headCar = headCar->next;
        free(auxCarreira->nome);
        free(auxCarreira);
    }
    while (headPar != NULL) {
        while (headPar->carreiras != NULL) {
            printf("%s\n", headPar->nome);
            auxNodeCarreira = headPar->carreiras;
            headPar->carreiras = headPar->carreiras->next;
            free(auxNodeCarreira);
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