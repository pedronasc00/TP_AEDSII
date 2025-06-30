#include "Patricia.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

TipoDib Patricia_Bit(TipoIndexAmp i, TipoChavePatricia k, int *comp) {
    (*comp)++; // Conta como uma comparação de bit

    int byteIndex = (i - 1) / 8;
    int bitIndex = 7 - ((i - 1) % 8);

    if (byteIndex >= strlen(k))
        return 0;

    unsigned char c = k[byteIndex];
    return (c >> bitIndex) & 1;
}

short Patricia_EhExterno(TipoArvore p) {
    return (p->nt == Externo);
}

TipoArvore Patricia_CriaNoInterno(int i, TipoArvore Esq, TipoArvore Dir) {
    TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
    p->nt = Interno;
    p->NO.NInterno.Index = i;
    p->NO.NInterno.Esq = Esq;
    p->NO.NInterno.Dir = Dir;
    return p;
}

TipoArvore Patricia_CriaNoExterno(TipoChavePatricia k, int idDoc) {
    TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
    p->nt = Externo;
    p->NO.NExterno.Palavra = strdup(k);

    FLVazia(&(p->NO.NExterno.Ocorrencias));
    Indice idx = {1, idDoc};
    LInsere(&(p->NO.NExterno.Ocorrencias), &idx);

    return p;
}

void Patricia_Pesquisa(TipoChavePatricia k, TipoArvore t, int *comp) {
    if (t == NULL) {
        printf("Arvore vazia\n");
        return;
    }

    if (Patricia_EhExterno(t)) {
        (*comp)++; // Comparação de string na folha
        if (strcmp(k, t->NO.NExterno.Palavra) == 0) {
            printf("Palavra '%s' ", k);
            LImprime(&(t->NO.NExterno.Ocorrencias));
            printf("\n");
        } else {
            printf("Palavra '%s' nao encontrada\n", k);
        }
        return;
    }

    if (Patricia_Bit(t->NO.NInterno.Index, k, comp) == 0)
        Patricia_Pesquisa(k, t->NO.NInterno.Esq, comp);
    else
        Patricia_Pesquisa(k, t->NO.NInterno.Dir, comp);
}

static TipoArvore Patricia_InsereEntre(TipoChavePatricia k, TipoArvore t, int i, int idDoc, int *comp) {
    TipoArvore p;

    if (Patricia_EhExterno(t) || i < t->NO.NInterno.Index) {
        p = Patricia_CriaNoExterno(k, idDoc);
        if (Patricia_Bit(i, k, comp) == 1)
            return Patricia_CriaNoInterno(i, t, p);
        else
            return Patricia_CriaNoInterno(i, p, t);
    } else {
        if (Patricia_Bit(t->NO.NInterno.Index, k, comp) == 1)
            t->NO.NInterno.Dir = Patricia_InsereEntre(k, t->NO.NInterno.Dir, i, idDoc, comp);
        else
            t->NO.NInterno.Esq = Patricia_InsereEntre(k, t->NO.NInterno.Esq, i, idDoc, comp);
        return t;
    }
}

TipoArvore Patricia_Insere(TipoChavePatricia k, TipoArvore t, int idDoc, int *comp) {
    if (t == NULL)
        return Patricia_CriaNoExterno(k, idDoc);

    TipoArvore p = t;

    while (!Patricia_EhExterno(p)) {
        if (Patricia_Bit(p->NO.NInterno.Index, k, comp) == 1)
            p = p->NO.NInterno.Dir;
        else
            p = p->NO.NInterno.Esq;
    }

    (*comp)++; // Comparação de string na folha
    if (strcmp(k, p->NO.NExterno.Palavra) == 0) {
        ApontadorLista aux = p->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
        while (aux != NULL) {
            (*comp)++;
            if (aux->idTermo.idDoc == idDoc) {
                aux->idTermo.qtde++;
                return t;
            }
            aux = aux->pProx;
        }

        Indice idx = {1, idDoc};
        LInsere(&(p->NO.NExterno.Ocorrencias), &idx);
        return t;
    }

    int i = 1;
    while (Patricia_Bit(i, k, comp) == Patricia_Bit(i, p->NO.NExterno.Palavra, comp)) {
        i++;
    }

    return Patricia_InsereEntre(k, t, i, idDoc, comp);
}

void IndiceInvertidoPatricia(TipoArvore t, int* cont) {
    if (t != NULL) {
        if (Patricia_EhExterno(t)) {
            printf("%d: %s -> ", (*cont)++, t->NO.NExterno.Palavra);
            LImprime(&(t->NO.NExterno.Ocorrencias));
            printf("\n");
        } else {
            IndiceInvertidoPatricia(t->NO.NInterno.Esq, cont);
            IndiceInvertidoPatricia(t->NO.NInterno.Dir, cont);
        }
    }
}

void Patricia_LiberaArvore(TipoArvore *t) {
    if (*t != NULL) {
        if (Patricia_EhExterno(*t)) {
            free((*t)->NO.NExterno.Palavra);

            ApontadorLista aux = (*t)->NO.NExterno.Ocorrencias.pPrimeiro;
            while (aux != NULL) {
                ApontadorLista temp = aux;
                aux = aux->pProx;
                free(temp);
            }

            free(*t);
            *t = NULL;
        } else {
            Patricia_LiberaArvore(&((*t)->NO.NInterno.Esq));
            Patricia_LiberaArvore(&((*t)->NO.NInterno.Dir));
            free(*t);
            *t = NULL;
        }
    }
}
