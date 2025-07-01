#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Relevancia_Patricia.h"

// Conta quantos termos distintos aparecem no documento com idDoc
int ContaTermosDistintosNoDoc(TipoArvore arvore, int idDoc)
{
    if (arvore == NULL)
        return 0;

    if (Patricia_EhExterno(arvore))
    {
        ApontadorLista aux = arvore->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
        while (aux != NULL)
        {
            if (aux->idTermo.idDoc == idDoc)
                return 1;
            aux = aux->pProx;
        }
        return 0;
    }
    else
    {
        return ContaTermosDistintosNoDoc(arvore->NO.NInterno.Esq, idDoc) +
               ContaTermosDistintosNoDoc(arvore->NO.NInterno.Dir, idDoc);
    }
}
int comparaRelevanciaPatricia(const void *a, const void *b) {
    ResultadoRelevanciaPatricia *resA = (ResultadoRelevanciaPatricia *)a;
    ResultadoRelevanciaPatricia *resB = (ResultadoRelevanciaPatricia *)b;
    if (resA->relevancia < resB->relevancia) return 1;
    if (resA->relevancia > resB->relevancia) return -1;
    return 0;
}
// Calcula e imprime a relevância dos documentos para uma consulta
void CalculaRelevanciaPatricia(TipoArvore arvore, char **consulta, int numTermosConsulta, int numDocs) {
    ResultadoRelevancia resultados[numDocs];
    printf("\n==== Calculando Relevância ====\n");
    // Inicializa resultados
    for (int i = 0; i < numDocs; i++)
    {
        resultados[i].idDoc = i + 1;
        resultados[i].relevancia = 0.0;
    }
    // Para cada termo da consulta
    for (int t = 0; t < numTermosConsulta; t++) {
        TipoArvore no = arvore;
        int teste;
        // Busca o termo na Patricia
        while (no != NULL && !Patricia_EhExterno(no))
        {
            if (Patricia_Bit(no->NO.NInterno.Index, consulta[t], &teste) == 0)
                no = no->NO.NInterno.Esq;
            else
                no = no->NO.NInterno.Dir;
        }
        if (no != NULL && strcmp(no->NO.NExterno.Palavra, consulta[t]) == 0)
        {
            // Termo encontrado
            int dj = LTamanho(&(no->NO.NExterno.Ocorrencias));

            if (dj == 0)
                continue;

            // Percorre a lista de ocorrencias desse termo
            ApontadorLista aux = no->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
            while (aux != NULL)
            {
                int idDoc = aux->idTermo.idDoc;
                int f_ij = aux->idTermo.qtde;

                // Conta ni (número de termos distintos no documento)
                int ni = ContaTermosDistintosNoDoc(arvore, idDoc);
                if (ni == 0)
                    ni = 1; // Evitar divisão por zero

                // Cálculo do peso w_ij
                double w_ij = (double)f_ij * log2((double)numDocs / dj);

                // Acumula na relevância do documento
                resultados[idDoc - 1].relevancia += w_ij / ni;

                aux = aux->pProx;
            }
        }
    }
    // Ordena os resultados por relevância decrescente (bubble sort simples)
    for (int i = 0; i < numDocs - 1; i++)
    {
        for (int j = i + 1; j < numDocs; j++)
        {
            if (resultados[j].relevancia > resultados[i].relevancia)
            {
                ResultadoRelevancia temp = resultados[i];
                resultados[i] = resultados[j];
                resultados[j] = temp;
            }
        }
    }

    // Imprime os documentos ordenados por relevância
    printf("\n==== Resultado da Busca ====\n");
    for (int i = 0; i < numDocs; i++)
    {
        if (resultados[i].relevancia > 0.0)
        {
            printf("Documento %d -> Relevancia: %.4lf\n",
                   resultados[i].idDoc,
                   resultados[i].relevancia);
        }
    }
}


void RelevanciaPatricia(TipoArvore arvore, char* arqtexto[], int numDocs, int *comparacoes){
    if (arvore == NULL || numDocs == 0) {
        printf("Árvore Patricia vazia ou número de documentos inválido.\n");
        return;
    }

    int num_termos;
    printf("Digite a quantidade de termos: ");
    scanf("%d", &num_termos);

    char **vetorTermos = (char**)malloc(num_termos * sizeof(char*));
    for (int i = 0; i < num_termos; i++) {
        vetorTermos[i] = (char*)malloc(32 * sizeof(char));
        printf("Termo: ");
        scanf("%s", vetorTermos[i]);
        Patricia_Pesquisa(vetorTermos[i], arvore, comparacoes);
    }

    ResultadoRelevanciaPatricia resultados[numDocs];
    for (int i = 0; i < numDocs; i++) {
        resultados[i].idDoc = i + 1;
        resultados[i].relevancia = 0.0;
        strcpy(resultados[i].nomeArquivo, arqtexto[i]);
    }

    // Para cada termo da consulta
    for (int t = 0; t < num_termos; t++) {
        TipoArvore no = arvore;
        int teste;

        // Busca o termo na árvore Patricia
        while (no != NULL && !Patricia_EhExterno(no)) {
            if (Patricia_Bit(no->NO.NInterno.Index, vetorTermos[t], &teste) == 0)
                no = no->NO.NInterno.Esq;
            else
                no = no->NO.NInterno.Dir;
        }

        if (no != NULL) {
            if (strcmp(no->NO.NExterno.Palavra, vetorTermos[t]) == 0) {
                // Termo encontrado
                int dj = LTamanho(&(no->NO.NExterno.Ocorrencias));
                if (dj == 0) continue;

                ApontadorLista aux = no->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
                while (aux != NULL) {
                    int idDoc = aux->idTermo.idDoc;
                    int f_ij = aux->idTermo.qtde;
                    int ni = ContaTermosDistintosNoDoc(arvore, idDoc);
                    if (ni == 0) ni = 1;

                    double w_ij = (double)f_ij * log2((double)numDocs / dj);
                    resultados[idDoc - 1].relevancia += w_ij / ni;

                    aux = aux->pProx;
                }
            }
        }
    }

    // Ordena os resultados por relevância
    qsort(resultados, numDocs, sizeof(ResultadoRelevanciaPatricia), comparaRelevanciaPatricia);

    // Exibe o resultado
    printf("\n--- Relevância dos Documentos (Patricia) ---\n");
    for (int i = 0; i < numDocs; i++) {
        if (resultados[i].relevancia > 0) {
            printf("Relevancia: %-8.4f | Doc: %s\n",
                   resultados[i].relevancia, resultados[i].nomeArquivo);
        }
    }
    printf("--------------------------------------------\n");

    printf("Total de comparações realizadas na pesquisa de termos: %d\n", *comparacoes);

    // Libera memória
    for (int i = 0; i < num_termos; i++) {
        free(vetorTermos[i]);
    }
    free(vetorTermos);
}


