#include "RelevanciaHASH.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparaRelevancia(const void *a, const void *b) {
    idRelevancia *resA = (idRelevancia *)a;
    idRelevancia *resB = (idRelevancia *)b;
    if (resA->relevancia < resB->relevancia) return 1;
    if (resA->relevancia > resB->relevancia) return -1;
    return 0;
}

void RelevanciaHASH(TipoLista* Tabela, int M, int numArqs, char* arqTexto[], TipoPesos p) {
    if (Tabela == NULL || M == 0) 
    {
        printf("Tabela Vazia");
        return;
    }
    
    //1. Leitura de Termos para relevancia
    int numTermos;
    printf("Quantidade de termos para pesquisa: ");
    scanf("%d", &numTermos);

    char** vetorTermos = (char**)malloc(numTermos * sizeof(char*));

    for (int i = 0; i < numTermos; i++)
    {
        vetorTermos[i] = (char*)malloc(N * sizeof(char));
        printf("Termo: ");
        scanf("%19s", vetorTermos[i]); 
    }

    //1.1 Incluir a função Pesquisa na função para calcular relevancia
    PesquisaNaTabelaHash(Tabela, vetorTermos, numTermos, M, p);
    
    //2. Estrutura de Dados
    double NumDocs = (double)numArqs;
    int* ni = TamanhoHASH_perDocs(Tabela, M, numArqs);
    if (ni == NULL) {return;}
    
    idRelevancia* resultados = (idRelevancia*)calloc(numArqs, sizeof(idRelevancia));

    for (int i = 0; i < numArqs; i++)
    {
        resultados[i].idDoc = i;
        strcpy(resultados[i].nomeArquivo, arqTexto[i]);
    }
    
    //3. Calcular Relevancia
    for (int j = 0; j < numTermos; j++)
    {
        int comp;
        TipoItem* ItemEncontrado = PesquisaItem(vetorTermos[j], p, Tabela, M, &comp);

        if (ItemEncontrado != NULL)
        {
            double dj = (double)LTamanho(&ItemEncontrado->idPalavra);
            ApontadorLista pIndice = ItemEncontrado->idPalavra.pPrimeiro->pProx;
            
            while (pIndice != NULL)
            {
                int idDoc = pIndice->idTermo.idDoc;
                double f_ji = (double)pIndice->idTermo.qtde;
                double w_ji = f_ji * log2(NumDocs / dj);
                resultados[idDoc].relevancia += w_ji;
                pIndice = pIndice->pProx;
            }
        }
    }
    
    //4. R(i)
    for (int i = 0; i < numArqs; i++)
    {
        if (ni[i] > 0)
        {
            // r(i) = (1/n_i) * Somatorio(Wji)
            resultados[i].relevancia = resultados[i].relevancia / ni[i];
        }
    }
    
    qsort(resultados, numArqs, sizeof(idRelevancia), comparaRelevancia);
    printf("\n ---Relevancia dos Documentos (TF-IDF)--- \n");
    for (int i = 0; i < numArqs; i++)
    {
        if (resultados[i].relevancia > 0)
        {
            printf("Relevancia: %-8.4f | Doc: %s\n", resultados[i].relevancia, resultados[i].nomeArquivo);
        }
    }
    printf("---------------------------------\n");

    free(ni);
    free(resultados);
    for (int i = 0; i < numTermos; i++) {
        free(vetorTermos[i]);
    }
    free(vetorTermos);
}