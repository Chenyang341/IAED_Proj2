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

typedef struct id {
    int value;
    struct id *next;
    struct id *prev;
} ID;

typedef struct carreira {
    char *nome;
    int idOrigem;
    int idDestino;
    int numLigacao;
    float custoTotal;
    float duracaoTotal;
    struct id *idLigacao;
    struct carreira *next;
} Carreira;

typedef struct paragem{
    char *nome;
    double latitude;
    double longitude;
    int numCarreira;
    struct id *idCarreira;
    struct paragem *next;
} Paragem;

typedef struct ligacao{
    int idCarreira;
    int idOrigem;
    int idDestino;
    float custo;
    float duracao;
    struct ligacao *next;
} Ligacao;

#define leDouble(A) {if (scanf("%lf", &A) == 1) {}}     /* Le um double */
#define leFloat(A) {if (scanf("%f", &A) == 1) {}}     /* Le um float */

int lePalavra(char **s);
Carreira *comandoCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig);
void semArgCarreira(Carreira *headCar, Paragem *headPar);
Carreira *comArgCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig, Carreira carreira, char *inversao);
int verificaInversoOk(char *s);
void printParagensNormal(Paragem *headPar, Ligacao *headLig, ID *headId);
void printParagensInverso(Paragem *headPar, Ligacao *headLig, ID *headId);
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
int verificaLigacao(Carreira *head, Ligacao ligacao);
int verificaCustoDuracao(Ligacao ligacao);
Ligacao *criaLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar, Ligacao ligacao, int tipoLigacao);
ID *adicionaIdNovo(ID *head, int id);
ID *adicionaIdInicio(ID *head, int id);
ID *adicionaIdFim(ID *head, int id);
int existeCarreira(ID *head, int idCarreira);
void comandoIntersecao(Carreira *headCar, Paragem *headPar);
ID *sort(Carreira *headCar, ID *headId);
Carreira *removeCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig);
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
            _carreiras = comandoCarreira(_carreiras, _paragens, _ligacoes);
        else if (comando == 'p')
            _paragens = comandoParagem(_paragens);
        else if (comando == 'l')
            _ligacoes = comandoLigacao(_ligacoes, _carreiras, _paragens);
        else if (comando == 'i')
            comandoIntersecao(_carreiras, _paragens);
        else if (comando == 'a')
            apagaDados(_carreiras, _paragens, _ligacoes);
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

Carreira *comandoCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {

    Carreira carreira;
    char *inversao = NULL;
    if (getchar() == '\n')
        semArgCarreira(headCar, headPar);
    else {
        if (lePalavra(&carreira.nome))
            lePalavra(&inversao);
        headCar = comArgCarreira(headCar, headPar, headLig, carreira, inversao);
    }
    free(inversao);
    return headCar;

}

void semArgCarreira(Carreira *headCar, Paragem *headPar) {
    
    int i;
    for(i = 0; headCar != NULL; i++) {
        if (headCar->idOrigem != NAO_DEF) {
            printf("%s %s %s %d %.2f %.2f\n", headCar->nome, procuraParagem(headPar, headCar->idOrigem)->nome,
            procuraParagem(headPar, headCar->idDestino)->nome, headCar->numLigacao, headCar->custoTotal, headCar->duracaoTotal);
        } else
            printf("%s %d %.2f %.2f\n", headCar->nome, headCar->numLigacao, headCar->custoTotal, headCar->duracaoTotal);
        headCar = headCar->next;
    }

}

Carreira *comArgCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig, Carreira carreira, char *inversao) {

    int i, e_carreira = NAO;
    Carreira *auxCarreira = headCar;
    for (i = 0; auxCarreira != NULL; i++) {
        if (!strcmp(auxCarreira->nome, carreira.nome)) {
            e_carreira = SIM;
            if (auxCarreira->idOrigem != NAO_DEF) {
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

void printParagensNormal(Paragem *headPar, Ligacao *headLig, ID *headId) {

    Ligacao *auxLigacao = procuraLigacao(headLig, headId->value);
    printf("%s", procuraParagem(headPar, auxLigacao->idOrigem)->nome);
    while (headId != NULL) {
        auxLigacao = procuraLigacao(headLig, headId->value);
        printf(", %s", procuraParagem(headPar, auxLigacao->idDestino)->nome);
        headId = headId->next;
    }
    printf("\n");

}

void printParagensInverso(Paragem *headPar, Ligacao *headLig, ID *headId) {

    Ligacao *auxLigacao;
    while (headId->next != NULL)
        headId = headId->next;
    auxLigacao = procuraLigacao(headLig, headId->value);
    printf("%s", procuraParagem(headPar, auxLigacao->idDestino)->nome);
    while (headId != NULL) {
        auxLigacao = procuraLigacao(headLig, headId->value);
        printf(", %s", procuraParagem(headPar, auxLigacao->idOrigem)->nome);
        headId = headId->prev;
    }
    printf("\n");
    
}

Carreira *criaCarreira(Carreira *head, Carreira carreira) {

    Carreira *newCarreira = (Carreira*) malloc(sizeof(Carreira));
    Carreira *auxCarreira = head;
    *newCarreira = carreira;
    newCarreira->idOrigem = NAO_DEF;
    newCarreira->idDestino = NAO_DEF;
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
    } else {
        head = tresArgParagem(head, paragem);
    }
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
    newParagem->idCarreira = NULL;
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
    if ((tipoLigacao = verificaLigacao(headCar, ligacao)) == NONE)
        return headLig;
    if (!verificaCustoDuracao(ligacao))
        return headLig;
    headLig = criaLigacao(headLig, headCar, headPar, ligacao, tipoLigacao);
    return headLig;

}

int verificaCarreira(Carreira *head, char *nomeCarreira) {

    int i = 0;
    while (head != NULL) {
        if (!strcmp(head->nome, nomeCarreira)) {
            free(nomeCarreira);
            return i;
        }
        i++;
        head = head->next;
    }
    printf("%s: no such line.\n", nomeCarreira);
    return NAO_DEF;

}

int verificaParagem(Paragem *head, char *nomeParagem) {

    int i = 0;
    while (head != NULL) {
        if (!strcmp(head->nome, nomeParagem)) {
            free(nomeParagem);
            return i;
        }
        i++;
        head = head->next;
    }
    printf("%s: no such stop.\n", nomeParagem);
    return NAO_DEF;

}

int verificaLigacao(Carreira *head, Ligacao ligacao) {

    int tipoLigacao;
    int idCarreira = ligacao.idCarreira, idOrigem = ligacao.idOrigem,
    idDestino = ligacao.idDestino;
    Carreira *auxCarreira = procuraCarreira(head, idCarreira);
    if (auxCarreira->idDestino == idOrigem)
        tipoLigacao = FIM;
    else if (auxCarreira->idOrigem == idDestino)
        tipoLigacao = INICIO;
    else if (auxCarreira->idOrigem == NAO_DEF &&
    auxCarreira->idDestino == NAO_DEF) {
        tipoLigacao = NOVO;
    } else {
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

Ligacao *criaLigacao(Ligacao *headLig, Carreira *headCar, Paragem *headPar, Ligacao ligacao, int tipoLigacao) {

    static int idLigacao = 0;
    Carreira *auxCarreira = procuraCarreira(headCar, ligacao.idCarreira);
    Ligacao *newLigacao = (Ligacao*) malloc(sizeof(Ligacao));
    Ligacao *auxLigacao = headLig;
    Paragem *auxParagem;
    if (tipoLigacao == INICIO) {
        auxCarreira->idLigacao = adicionaIdInicio(auxCarreira->idLigacao, idLigacao);
        auxCarreira->idOrigem = ligacao.idOrigem;
        auxCarreira->numLigacao++;
    } else if (tipoLigacao == FIM) {
        auxCarreira->idLigacao = adicionaIdFim(auxCarreira->idLigacao, idLigacao);
        auxCarreira->idDestino = ligacao.idDestino;
        auxCarreira->numLigacao++;
    } else {
        auxCarreira->idLigacao = adicionaIdNovo(auxCarreira->idLigacao, idLigacao);
        auxCarreira->idOrigem = ligacao.idOrigem;
        auxCarreira->idDestino = ligacao.idDestino;
        auxCarreira->numLigacao += 2;
    }
    auxCarreira->custoTotal += ligacao.custo;
    auxCarreira->duracaoTotal += ligacao.duracao;
    auxParagem = procuraParagem(headPar, ligacao.idOrigem);
    if (auxParagem->idCarreira == NULL) {
        auxParagem->idCarreira = adicionaIdNovo(auxParagem->idCarreira, ligacao.idCarreira);
        auxParagem->numCarreira++;
    } else if (!existeCarreira(auxParagem->idCarreira, ligacao.idCarreira)) {
        auxParagem->idCarreira = adicionaIdInicio(auxParagem->idCarreira, ligacao.idCarreira);
        auxParagem->numCarreira++;
    }
    if (ligacao.idOrigem != ligacao.idDestino) {
        auxParagem = procuraParagem(headPar, ligacao.idDestino);
        if (auxParagem->idCarreira == NULL) {
            auxParagem->idCarreira = adicionaIdNovo(auxParagem->idCarreira, ligacao.idCarreira);
            auxParagem->numCarreira++;
        } else if (!existeCarreira(auxParagem->idCarreira, ligacao.idCarreira)) {
            auxParagem->idCarreira = adicionaIdInicio(auxParagem->idCarreira, ligacao.idCarreira);
            auxParagem->numCarreira++;
        }
    }
    *newLigacao = ligacao;
    newLigacao->next = NULL;
    if (headLig == NULL)
        headLig = newLigacao;
    else {
        while (auxLigacao->next != NULL)
            auxLigacao = auxLigacao->next;
        auxLigacao->next = newLigacao;
    }
    idLigacao++;
    return headLig;

}

ID *adicionaIdNovo(ID *head, int id) {

    head = (ID*) malloc(sizeof(ID));
    head->value = id;
    head->next = NULL;
    head->prev = NULL;
    return head;

}

ID *adicionaIdInicio(ID *head, int id) {

    ID *newId = (ID*) malloc(sizeof(ID));
    newId->value = id;
    newId->next = head;
    head->prev = newId;
    newId->prev = NULL;
    return newId;

}

ID *adicionaIdFim(ID *head, int id) {

    ID *auxId = head;
    ID *newId = (ID*) malloc(sizeof(ID));
    newId->value = id;
    newId->next = NULL;
    while (auxId->next != NULL)
        auxId = auxId->next;
    auxId->next = newId;
    newId->prev = auxId;
    return head;

}

int existeCarreira(ID *head, int idCarreira) {

    while (head != NULL) {
        if (head->value == idCarreira)
            return SIM;
        head = head->next;
    }
    return NAO;

}

void comandoIntersecao(Carreira *headCar, Paragem *headPar) {
    
    ID *auxId;
    while (headPar != NULL) {
        if (headPar->numCarreira > 1) {
            auxId = sort(headCar, headPar->idCarreira);
            printf("%s %d:", headPar->nome, headPar->numCarreira);
            while(auxId != NULL) {
                printf(" %s", procuraCarreira(headCar, auxId->value)->nome);
                auxId = auxId->next;
            }
            printf("\n");
        }
        headPar = headPar->next;
    }

}

ID *sort(Carreira *headCar, ID *headId) {
    
    ID *auxId1 = headId, *auxId2;
    char *nome1, *nome2;
    int temp;
    while (auxId1 != NULL) {
        auxId2 = headId;
        while (auxId2->next != NULL) {
            nome1 = procuraCarreira(headCar, auxId2->value)->nome;
            nome2 = procuraCarreira(headCar, auxId2->next->value)->nome;
            if (strcmp(nome1, nome2) > 0) {
                temp = auxId2->value;
                auxId2->value = auxId2->next->value;
                auxId2->next->value = temp;
            }
            auxId2 = auxId2->next;
        }
        auxId1 = auxId1->next;
    }
    return headId;

}

Carreira *removeCarreira(Carreira *headCar, Paragem *headPar, Ligacao *headLig) {

    Carreira *auxCarreira, *prevCarreira;
    Paragem *auxParagem;
    Ligacao *auxLigacao, *prevLigacao;
    ID *auxId, *tempId;
    char *nomeCarreira;
    int i, existeCarreira = NAO, idCarreira;
    lePalavra(&nomeCarreira);
    if ((auxCarreira = headCar) != NULL) {
        if (!strcmp(auxCarreira->nome, nomeCarreira)) {
            existeCarreira = SIM;
            idCarreira = 0;
            headCar = auxCarreira->next;
            free(auxCarreira);
        } else {
            for (i = 0; auxCarreira != NULL; i++) {
                if (!strcmp(auxCarreira->nome, nomeCarreira)) {
                    existeCarreira = SIM;
                    idCarreira = i;
                    prevCarreira->next = auxCarreira->next;
                    free(auxCarreira);
                }
                prevCarreira = auxCarreira;
                auxCarreira = auxCarreira->next;
            }
        }
    }
    if (!existeCarreira) {
        printf("%s: no such line.", nomeCarreira);
        return headCar;
    }
    if ((auxParagem = headCar) != NULL) {
        auxId = auxParagem->idCarreira;
        while (auxId != NULL) {
            if (auxId->value == idCarreira) {
                auxId->prev->next = auxId->next;
                auxId->next->prev = auxId->prev;
                tempId = auxId;
                auxId = auxId->next;
                free(tempId);
            } else {
                if (auxId->value > idCarreira)
                    auxId->value--;
                auxId = auxId->next;
            }
        }
    }
    auxLigacao = headLig;
    while (auxLigacao != NULL) {
        if (auxLigacao->idCarreira == idCarreira) {
            prevLigacao->next = auxLigacao->next;
        }
        prevLigacao = auxLigacao;
        auxLigacao = auxLigacao->next;
    }

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
    ID *auxId;
    while (headCar != NULL) {
        while (headCar->idLigacao != NULL) {
            auxId = headCar->idLigacao;
            headCar->idLigacao = headCar->idLigacao->next;
            free(auxId);
        }
        auxCarreira = headCar;
        headCar = headCar->next;
        free(auxCarreira->nome);
        free(auxCarreira);
    }
    while (headPar != NULL) {
        while (headPar->idCarreira != NULL) {
            auxId = headPar->idCarreira;
            headPar->idCarreira = headPar->idCarreira->next;
            free(auxId);
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