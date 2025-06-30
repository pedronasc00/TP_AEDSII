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

void constroiIndiceInvertidoHASH(char* arqTexto[], int numArqs, TipoLista **Tabela_ptr, int *M_ptr, TipoPesos p, int *comparacoesInsercaoHash) {
    *comparacoesInsercaoHash = 0;
    int totalUnicas = ContaPalavrasUnicas(arqTexto, numArqs);
    int M = calculaM(totalUnicas, FatorCarga);
    *Tabela_ptr = malloc(M * sizeof(TipoLista));
    if (*Tabela_ptr == NULL) {
        printf("Erro ao alocar tabela hash.\n");
        return;
    }

    Inicializa(*Tabela_ptr, M);

    for (int i = 0; i < numArqs; i++) {
        FILE* arq = fopen(arqTexto[i], "r");
        if (arq == NULL) {
            printf("Erro ao abrir %s\n", arqTexto[i]);
            continue;
        }

        int idDoc = i;  // define o identificador do documento
        ProcessaArquivo(arq, idDoc, p, *Tabela_ptr, M, comparacoesInsercaoHash);

        fclose(arq);
    }

    *M_ptr = M;
    IndiceInvertidoHash(*Tabela_ptr, M);
}

//Verifica a quantidade de termos na Tabela HASH por Documento lido
int* TamanhoHASH_perDocs(TipoLista* Tabela, int M, int numArqs) {
    if (Tabela == NULL || M == 0) {
        return 0;
    }

    int *Termo_perDocs = (int*)calloc(numArqs, sizeof(int));

    for (int i = 0; i < M; i++) {
        TipoApontador pTermo = Tabela[i].Primeiro->Prox;

        while (pTermo != NULL) {
            ApontadorLista pIndice = pTermo->Item.idPalavra.pPrimeiro->pProx;
            
            while (pIndice != NULL) {
                int idDoc = pIndice->idTermo.idDoc;

                if (idDoc >= 0 && idDoc < numArqs) {
                    Termo_perDocs[idDoc] += 1;
                }
                pIndice = pIndice->pProx;
            }
            pTermo = pTermo->Prox;
        }
    }
    return Termo_perDocs;
}