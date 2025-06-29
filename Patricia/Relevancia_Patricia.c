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
