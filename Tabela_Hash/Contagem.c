#include "Contagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRA 20
typedef char Palavra[MAX_PALAVRA];

void Tokeniza(char* in, char* out) {
    int j = 0;
    for (int i = 0; in[i] != '\0'; i++) {
        if (isalpha((unsigned char)in[i])) {
            out[j++] = tolower((unsigned char)in[i]);
        }
    }
    out[j] = '\0';
}

int compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

int ContaPalavrasUnicas(char* arquivos[], int numArquivos) {
    Palavra* vetor = NULL;
    int tamanho = 0;

    for (int a = 0; a < numArquivos; a++) {
        FILE* arq = fopen(arquivos[a], "r");
        if (!arq) continue;

        char buffer[MAX_PALAVRA], palavraLimpa[MAX_PALAVRA];
        while (fscanf(arq, "%19s", buffer) != EOF) {
            Tokeniza(buffer, palavraLimpa);
            if (strlen(palavraLimpa) == 0) continue;

            Palavra busca;
            strcpy(busca, palavraLimpa);
            if (bsearch(&busca, vetor, tamanho, sizeof(Palavra), compare) == NULL) {
                Palavra* temp = realloc(vetor, (tamanho + 1) * sizeof(Palavra));
                if (!temp) {
                    free(vetor);
                    fclose(arq);
                    return -1;
                }
                vetor = temp;
                strcpy(vetor[tamanho++], palavraLimpa);
                qsort(vetor, tamanho, sizeof(Palavra), compare);
            }
        }

        fclose(arq);
    }

    free(vetor);
    return tamanho;
}
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Função para verificar se um número é primo
bool ehPrimo(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    int limite = (int) sqrt(n);
    for (int i = 5; i <= limite; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Função para encontrar o próximo primo >= n
int proximoPrimo(int n) {
    while (!ehPrimo(n)) {
        n++;
    }
    return n;
}

// Função que retorna o tamanho M favorável para a tabela hash
int calculaM(int totalPalavrasUnicas, float fatorCarga) {
    int minimoM = (int) ceil(totalPalavrasUnicas / fatorCarga);
    return proximoPrimo(minimoM);
}

void constroiIndiceInvertidoHASH() {
    char* arqTexto[] = {
        "POCs/Beef_Cattle.txt",
        "POCs/Sticker_album.txt"
    };

    int numArq = sizeof(arqTexto) / sizeof(arqTexto[0]);
    
    TipoPesos p;
    GeraPesos(p);  // Gera os pesos aleatórios para a função de hash

    int totalUnicas = ContaPalavrasUnicas(arqTexto, numArq);
    printf("Total de palavras unicas: %d\n", totalUnicas);

    int M = calculaM(totalUnicas, FatorCarga);
    printf("Tamanho da Tabela Hash: %d\n", M);

    TipoLista* Tabela = malloc(M * sizeof(TipoLista));
    if (Tabela == NULL) {
        printf("Erro ao alocar tabela hash.\n");
        return;
    }
    Inicializa(Tabela, M);

    for (int i = 0; i < numArq; i++) {
        FILE* arq = fopen(arqTexto[i], "r");
        if (arq == NULL) {
            printf("Erro ao abrir %s\n", arqTexto[i]);
            continue;
        }
        ProcessaArquivo(arq, i + 1, p, Tabela, M);
        fclose(arq);
    }

    Imprime(Tabela, M);
    LiberaTabela(Tabela, M);
    free(Tabela);
}

