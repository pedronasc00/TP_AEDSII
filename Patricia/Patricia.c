#include "Patricia.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Retorna o valor (0 ou 1) do i-ésimo bit da chave (palavra).
TipoDib Patricia_Bit(TipoIndexAmp i, TipoChavePatricia k, int *comp) {
    (*comp)++; // Conta como uma comparação de bit

    // Calcula em qual byte e em qual bit dentro do byte está a informação.
    int byteIndex = (i - 1) / 8;
    int bitIndex = 7 - ((i - 1) % 8);

    // Se o índice for maior que a palavra, retorna 0.
    if (byteIndex >= strlen(k))
        return 0;

    // Isola e retorna o bit específico.
    unsigned char c = k[byteIndex];
    return (c >> bitIndex) & 1;
}

// Verifica se um nó é uma folha (externo), que contém a palavra.
short Patricia_EhExterno(TipoArvore p) {
    return (p->nt == Externo);
}

// Cria um nó de decisão (interno) da árvore.
TipoArvore Patricia_CriaNoInterno(int i, TipoArvore Esq, TipoArvore Dir) {
    TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
    p->nt = Interno;
    p->NO.NInterno.Index = i; // O bit a ser testado neste nó.
    p->NO.NInterno.Esq = Esq;
    p->NO.NInterno.Dir = Dir;
    return p;
}

// Cria um nó folha (externo) para armazenar uma nova palavra.
TipoArvore Patricia_CriaNoExterno(TipoChavePatricia k, int idDoc) {
    TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
    p->nt = Externo;
    p->NO.NExterno.Palavra = strdup(k); // Copia a palavra para o nó.

    // Inicia a lista de ocorrências para esta palavra.
    FLVazia(&(p->NO.NExterno.Ocorrencias));
    Indice idx = {1, idDoc}; // A primeira ocorrência, no doc 'idDoc'.
    LInsere(&(p->NO.NExterno.Ocorrencias), &idx);

    return p;
}

// Busca por uma palavra na árvore.
void Patricia_Pesquisa(TipoChavePatricia k, TipoArvore t, int *comp) {
    if (t == NULL) {
        printf("Arvore vazia\n");
        return;
    }

    // Se chega em uma folha, compara a palavra inteira.
    if (Patricia_EhExterno(t)) {
        (*comp)++; // Comparação de string na folha.
        if (strcmp(k, t->NO.NExterno.Palavra) == 0) {
            printf("Palavra '%s' ", k);
            LImprime(&(t->NO.NExterno.Ocorrencias)); // Imprime os docs onde ela aparece.
            printf("\n");
        } else {
            printf("Palavra '%s' nao encontrada\n", k);
        }
        return;
    }

    // Se for nó interno, decide se vai para a esquerda ou direita testando o bit.
    if (Patricia_Bit(t->NO.NInterno.Index, k, comp) == 0)
        Patricia_Pesquisa(k, t->NO.NInterno.Esq, comp);
    else
        Patricia_Pesquisa(k, t->NO.NInterno.Dir, comp);
}

// Função auxiliar recursiva que insere um novo nó no meio da árvore.
static TipoArvore Patricia_InsereEntre(TipoChavePatricia k, TipoArvore t, int i, int idDoc, int *comp) {
    TipoArvore p;

    // Se o nó atual é uma folha ou o novo bit de diferença é menor, cria a nova folha aqui.
    if (Patricia_EhExterno(t) || i < t->NO.NInterno.Index) {
        p = Patricia_CriaNoExterno(k, idDoc);
        // Cria um novo nó interno para ser o "pai" da folha antiga e da nova.
        if (Patricia_Bit(i, k, comp) == 1)
            return Patricia_CriaNoInterno(i, t, p);
        else
            return Patricia_CriaNoInterno(i, p, t);
    } else {
        // Continua descendo na árvore para encontrar o local certo da inserção.
        if (Patricia_Bit(t->NO.NInterno.Index, k, comp) == 1)
            t->NO.NInterno.Dir = Patricia_InsereEntre(k, t->NO.NInterno.Dir, i, idDoc, comp);
        else
            t->NO.NInterno.Esq = Patricia_InsereEntre(k, t->NO.NInterno.Esq, i, idDoc, comp);
        return t;
    }
}

// Função principal que insere uma palavra na árvore Patricia.
TipoArvore Patricia_Insere(TipoChavePatricia k, TipoArvore t, int idDoc, int *comp) {
    // Se a árvore está vazia, a nova palavra vira a raiz.
    if (t == NULL)
        return Patricia_CriaNoExterno(k, idDoc);

    TipoArvore p = t;

    // 1. Desce na árvore até encontrar uma folha, seguindo os bits da palavra.
    while (!Patricia_EhExterno(p)) {
        if (Patricia_Bit(p->NO.NInterno.Index, k, comp) == 1)
            p = p->NO.NInterno.Dir;
        else
            p = p->NO.NInterno.Esq;
    }

    // 2. Compara a palavra a ser inserida com a palavra encontrada na folha.
    (*comp)++;
    if (strcmp(k, p->NO.NExterno.Palavra) == 0) {
        // Palavra já existe. Apenas atualiza a lista de ocorrências.
        ApontadorLista aux = p->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
        while (aux != NULL) {
            (*comp)++;
            if (aux->idTermo.idDoc == idDoc) {
                aux->idTermo.qtde++; // Se já apareceu nesse doc, incrementa contador.
                return t;
            }
            aux = aux->pProx;
        }
        // Se não apareceu nesse doc ainda, insere o novo ID de doc.
        Indice idx = {1, idDoc};
        LInsere(&(p->NO.NExterno.Ocorrencias), &idx);
        return t;
    }

    // 3. Se a palavra é nova, encontra o primeiro bit que a diferencia da palavra na folha.
    int i = 1;
    while (Patricia_Bit(i, k, comp) == Patricia_Bit(i, p->NO.NExterno.Palavra, comp)) {
        i++;
    }

    // 4. Insere a nova palavra na árvore, criando um novo nó de decisão (interno).
    return Patricia_InsereEntre(k, t, i, idDoc, comp);
}

// Percorre a árvore e imprime o índice invertido (palavra -> docs).
void IndiceInvertidoPatricia(TipoArvore t, int* cont) {
    if (t != NULL) {
        // Se for folha, imprime a palavra e sua lista de documentos.
        if (Patricia_EhExterno(t)) {
            printf("%d: %s -> ", (*cont)++, t->NO.NExterno.Palavra);
            LImprime(&(t->NO.NExterno.Ocorrencias));
            printf("\n");
        } else {
            // Se for nó interno, continua a busca nos dois filhos.
            IndiceInvertidoPatricia(t->NO.NInterno.Esq, cont);
            IndiceInvertidoPatricia(t->NO.NInterno.Dir, cont);
        }
    }
}

// Libera toda a memória alocada para a árvore.
void Patricia_LiberaArvore(TipoArvore *t) {
    if (*t != NULL) {
        if (Patricia_EhExterno(*t)) {
            // Libera a string da palavra.
            free((*t)->NO.NExterno.Palavra);

            // Libera a lista de ocorrências.
            ApontadorLista aux = (*t)->NO.NExterno.Ocorrencias.pPrimeiro;
            while (aux != NULL) {
                ApontadorLista temp = aux;
                aux = aux->pProx;
                free(temp);
            }

            // Libera o próprio nó.
            free(*t);
            *t = NULL;
        } else {
            // Libera recursivamente os filhos esquerdo e direito.
            Patricia_LiberaArvore(&((*t)->NO.NInterno.Esq));
            Patricia_LiberaArvore(&((*t)->NO.NInterno.Dir));
            free(*t);
            *t = NULL;
        }
    }
}