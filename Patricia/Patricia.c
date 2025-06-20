#include "Patricia.h"


TipoDib Patricia_Bit(TipoIndexAmp i, TipoChavePatricia k) {
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

    FLVazia(&(p->NO.NExterno.Ocorrencias)); // Inicializa a lista
    Indice idx;
    idx.idDoc = idDoc;
    idx.qtde = 1;
    LInsere(&(p->NO.NExterno.Ocorrencias), &idx);

    return p;
}


void Patricia_Pesquisa(TipoChavePatricia k, TipoArvore t) {
    if (t == NULL) {
        printf("Arvore vazia\n");
        return;
    }

    if (Patricia_EhExterno(t)) {
        if (strcmp(k, t->NO.NExterno.Palavra) == 0) {
            printf("Palavra '%s' encontrada nas ocorrências: ", k);
            LImprime(&(t->NO.NExterno.Ocorrencias));
            printf("\n");
        } else {
            printf("Palavra '%s' nao encontrada\n", k);
        }
        return;
    }

    if (Patricia_Bit(t->NO.NInterno.Index, k) == 0)
        Patricia_Pesquisa(k, t->NO.NInterno.Esq);
    else
        Patricia_Pesquisa(k, t->NO.NInterno.Dir);
}


static TipoArvore Patricia_InsereEntre(TipoChavePatricia k, TipoArvore t, int i, int idDoc) {
    TipoArvore p;

    if (Patricia_EhExterno(t) || i < t->NO.NInterno.Index) {
        p = Patricia_CriaNoExterno(k, idDoc);
        if (Patricia_Bit(i, k) == 1)
            return Patricia_CriaNoInterno(i, t, p);
        else
            return Patricia_CriaNoInterno(i, p, t);
    } else {
        if (Patricia_Bit(t->NO.NInterno.Index, k) == 1)
            t->NO.NInterno.Dir = Patricia_InsereEntre(k, t->NO.NInterno.Dir, i, idDoc);
        else
            t->NO.NInterno.Esq = Patricia_InsereEntre(k, t->NO.NInterno.Esq, i, idDoc);
        return t;
    }
}

TipoArvore Patricia_Insere(TipoChavePatricia k, TipoArvore t, int idDoc) {
    if (t == NULL)
        return Patricia_CriaNoExterno(k, idDoc);

    TipoArvore p = t;

    while (!Patricia_EhExterno(p)) {
        if (Patricia_Bit(p->NO.NInterno.Index, k) == 1)
            p = p->NO.NInterno.Dir;
        else
            p = p->NO.NInterno.Esq;
    }

    if (strcmp(k, p->NO.NExterno.Palavra) == 0) {
        // Verifica se já existe ocorrência para o mesmo idDoc
        ApontadorLista aux = p->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
        while (aux != NULL) {
            if (aux->idTermo.idDoc == idDoc) {
                aux->idTermo.qtde++;
                return t;
            }
            aux = aux->pProx;
        }

        // Se não encontrou, insere novo
        Indice idx;
        idx.idDoc = idDoc;
        idx.qtde = 1;
        LInsere(&(p->NO.NExterno.Ocorrencias), &idx);

        return t;
    }

    int i = 1;
    while (Patricia_Bit(i, k) == Patricia_Bit(i, p->NO.NExterno.Palavra) && i <= D) {
        i++;
    }

    return Patricia_InsereEntre(k, t, i, idDoc);
}


void Patricia_ImprimeArvore(TipoArvore t) {
    if (t != NULL) {
        if (Patricia_EhExterno(t)) {
            printf("Palavra: %s -> ", t->NO.NExterno.Palavra);
            LImprime(&(t->NO.NExterno.Ocorrencias));
            printf("\n");
        } else {
            Patricia_ImprimeArvore(t->NO.NInterno.Esq);
            Patricia_ImprimeArvore(t->NO.NInterno.Dir);
        }
    }
}


void Patricia_LiberaArvore(TipoArvore *t) {
    if (*t != NULL) {
        if (Patricia_EhExterno(*t)) {
            free((*t)->NO.NExterno.Palavra);

            // Libera Lista
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
