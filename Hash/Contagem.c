#include "Contagem.h"       // Supondo que VFile, InicalizaVetor, InsereTermo, ImprimeVetor estão aqui

#define MAX_PALAVRA 20
#define FatorCarga 1

typedef char Palavra[MAX_PALAVRA];

// Variáveis globais
VFile indiceGlobal;
int indiceConstruido = 0;

// Conta palavras únicas nos arquivos
int ContaPalavrasUnicas(char* arquivos[], int numArquivos) {
    Palavra* vetor = NULL;
    int tamanho = 0;

    for (int a = 0; a < numArquivos; a++) {
        FILE* arq = fopen(arquivos[a], "r");
        if (!arq) {
            printf("Erro ao abrir %s\n", arquivos[a]);
            continue;
        }

        char buffer[MAX_PALAVRA], palavraLimpa[MAX_PALAVRA];
        while (fscanf(arq, "%19s", buffer) != EOF) {
            TokenizacaoTermo(buffer, palavraLimpa);
            if (strlen(palavraLimpa) == 0) continue;

            if (bsearch(palavraLimpa, vetor, tamanho, sizeof(Palavra), compare) == NULL) {
                Palavra* temp = realloc(vetor, (tamanho + 1) * sizeof(Palavra));
                if (!temp) {
                    printf("Erro de alocação.\n");
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

// Verifica se n é primo
bool ehPrimo(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    int limite = (int)sqrt(n);
    for (int i = 5; i <= limite; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Encontra próximo número primo <= n
int proximoPrimo(int n) {
    while (!ehPrimo(n)) n--;
    return n;
}

// Calcula tamanho ideal da tabela hash
int calculaM(int totalPalavrasUnicas, float fatorCarga) {
    int minimoM = (int)ceil(totalPalavrasUnicas / fatorCarga);
    return proximoPrimo(minimoM);
}


void constroiIndiceInvertidoHASH(char* arqTexto[], int numArqs, TipoLista* Tabela, int* M_ptr) {

    TipoPesos p;
    GeraPesos(p);  // Gera os pesos aleatórios para a função de hash

    int totalUnicas = ContaPalavrasUnicas(arqTexto, numArqs);
    printf("Total de palavras unicas: %d\n", totalUnicas);

    int M = calculaM(totalUnicas, FatorCarga);
    printf("Tamanho da Tabela Hash: %d\n", M);

    Tabela = malloc(M * sizeof(TipoLista));
    if (Tabela == NULL) {
        printf("Erro ao alocar tabela hash.\n");
        return;
    }
    Inicializa(Tabela, M);

    for (int i = 0; i < numArqs; i++) {
        FILE* arq = fopen(arqTexto[i], "r");
        if (arq == NULL) {
            printf("Erro ao abrir %s\n", arqTexto[i]);
            continue;
        }
        ProcessaArquivo(arq, i + 1, p, Tabela, M);
        fclose(arq);
    }

    *M_ptr = M;

    Imprime(Tabela, M);
}