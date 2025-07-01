#include "Hash/Hash.h"
#include <sys/time.h> // Adicionado para gettimeofday

// Inicia uma lista (de colisões) com um nó cabeça.
void THFLVazia(TipoLista *Lista) {
    Lista->Primeiro = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Primeiro;
    Lista->Primeiro->Prox = NULL;
} 

// Verifica se a lista de colisões está vazia.
short THVazia(TipoLista Lista) {
    return (Lista.Primeiro == Lista.Ultimo);
}

// Insere um item no fim da lista.
void Ins(TipoItem x, TipoLista *Lista) {
    Lista->Ultimo->Prox = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Ultimo->Prox;
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}   

// Remove um item da lista.
void Ret(TipoApontador p, TipoLista *Lista, TipoItem *Item) {
    TipoApontador q;
    if (THVazia(*Lista) || p == NULL || p->Prox == NULL) {
        printf(" Erro Lista vazia ou posicao nao existe\n");
        return;
    }
    q = p->Prox;
    *Item = q->Item;
    p->Prox = q->Prox;
    if (p->Prox == NULL)
        Lista->Ultimo = p;
    free(q);
}

// Gera pesos aleatórios para a função de hash, garantindo boa distribuição das chaves.
void GeraPesos(TipoPesos p) {
    int i, j;
    struct timeval semente;
    // Usa o tempo atual com microsegundos para gerar uma semente única.
    gettimeofday(&semente, NULL); 
    srand((int)(semente.tv_sec + 1000000 * semente.tv_usec));
    for (i = 0; i < N; i++)
        for (j = 0; j < TAMALFABETO; j++)
            p[i][j] = 1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0));
}

// Calcula o índice da tabela hash para uma chave usando os pesos gerados.
TipoIndice h(TipoChave Chave, TipoPesos p, int M) {
    int i;
    unsigned int Soma = 0; 
    int comp = strlen(Chave);
    for (i = 0; i < comp; i++)
        Soma += p[i][(unsigned int)Chave[i]];
    return (Soma % M);
}

// Inicia todas as listas da tabela hash.
void Inicializa(TipoLista *Tabela, int M) {
    for (int i = 0; i < M; i++)
        THFLVazia(&Tabela[i]);
}

// Busca uma chave na tabela e retorna o ponteiro para o item ANTERIOR a ela.
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M, int *comparacoes) {
    TipoIndice i = h(Ch, p, M);
    TipoApontador Ap;

    *comparacoes = 0;   // inicia a contagem

    if (THVazia(Tabela[i])) return NULL;

    Ap = Tabela[i].Primeiro;
    while (Ap->Prox->Prox != NULL && strcmp(Ch, Ap->Prox->Item.Chave) != 0) {
        Ap = Ap->Prox;
        (*comparacoes)++; 
    }

    (*comparacoes)++;   // conta a última comparação
    if (strcmp(Ch, Ap->Prox->Item.Chave) == 0)
        return Ap;

    return NULL;
}

// Busca e retorna o ponteiro para o ITEM encontrado, usando a função Pesquisa.
TipoItem* PesquisaItem(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M, int *comparacoes) {
    TipoApontador pAnterior = Pesquisa(Ch, p, Tabela, M, comparacoes);
    if (pAnterior != NULL)
        return &pAnterior->Prox->Item;
    return NULL;
}

// Insere um item na tabela, se ele já não existir.
int Insere(TipoItem x, TipoPesos p, TipoLista *Tabela, int M, int *totalComparacoes) {
    int comparacoes = 0;
    if (Pesquisa(x.Chave, p, Tabela, M, &comparacoes) == NULL) {
        Ins(x, &Tabela[h(x.Chave, p, M)]);
        *totalComparacoes += comparacoes;
        return 1; // inseriu nova palavra
    } else {
        *totalComparacoes += comparacoes;
        return 0; // palavra já existente
    }
}

// Remove um item da tabela hash.
void Retira(TipoItem x, TipoPesos p, TipoLista *Tabela, int M) {
    int comparacoes;
    TipoApontador Ap = Pesquisa(x.Chave, p, Tabela, M,&comparacoes);
    if (Ap == NULL)
        printf(" Registro nao esta presente\n");
    else
        Ret(Ap, &Tabela[h(x.Chave, p, M)], &x);
}

// Copia toda a tabela para um vetor, ordena e imprime o índice invertido.
void IndiceInvertidoHash(TipoLista* TabelaHash, int M){
    if (TabelaHash == NULL || M == 0) {
        return;
    }
    
    VFile vTermo;
    InicalizaVetor(&vTermo);

    // Drena todos os itens da tabela hash para um vetor temporário.
    for (int i = 0; i < M; i++) {
        TipoApontador pAux = TabelaHash[i].Primeiro->Prox;
        while (pAux != NULL) {
            vTermo.tamanho++;
            Word* temp_pr = (Word*)realloc(vTermo.VetorF, vTermo.tamanho * sizeof(Word));
            vTermo.VetorF = temp_pr;
            Word* novoTermo = &vTermo.VetorF[vTermo.tamanho - 1];
            strcpy(novoTermo->Palavra, pAux->Item.Chave);
            novoTermo->idPalavra = pAux->Item.idPalavra;

            pAux = pAux->Prox;
        }
    }
    
    // Ordena o vetor e o imprime, depois libera a memória.
    qsort(vTermo.VetorF, vTermo.tamanho, sizeof(Word), compare);
    ImprimeVetor(vTermo);
    free(vTermo.VetorF);
}

// Lê uma linha de texto do teclado.
void LerPalavra(char *p, int Tam) {
    char c; int i, j = 0;
    // fflush(stdin) é uma função não padronizada, pode não funcionar em todos os sistemas.
    fflush(stdin);
    while (((c = getchar()) != '\n') && j < Tam - 1)
        p[j++] = c;
    p[j] = '\0';
    while (c != '\n') c = getchar();
    for (i = j - 1; (i >= 0 && p[i] == ' '); i--)
        p[i] = '\0';
}

// Lê palavras de um arquivo e as insere/atualiza na tabela hash.
void ProcessaArquivo(FILE* arq, int idDoc, TipoPesos p, TipoLista *Tabela, int M, int *totalComparacoes) {
    if (arq == NULL) return;
    int comparacoes;
    char buffer[20]; 
    TipoChave termoToken;

    while (fscanf(arq, "%19s", buffer) != EOF) {
        TokenizacaoTermo(buffer, termoToken);

        if (strlen(termoToken) == 0) continue;

        TipoItem* itemEncontrado = PesquisaItem(termoToken, p, Tabela, M,&comparacoes);

        if (itemEncontrado != NULL) {
            ApontadorLista indiceDoc = BuscaIndice(&itemEncontrado->idPalavra, idDoc);
            if (indiceDoc != NULL) {
                indiceDoc->idTermo.qtde++;
            } else {
                Indice novoIndice = {1, idDoc};
                LInsere(&itemEncontrado->idPalavra, &novoIndice);
            }
        } else {
            TipoItem novoItem;
            strcpy(novoItem.Chave, termoToken);
            FLVazia(&novoItem.idPalavra);

            Indice novoIndice = {1, idDoc};
            LInsere(&novoItem.idPalavra, &novoIndice);

            Insere(novoItem, p, Tabela, M, totalComparacoes);
        }
    }
}

// Libera toda a memória alocada pela tabela e suas listas aninhadas.
void LiberaTabela(TipoLista *Tabela, int M) {
    for (int i = 0; i < M; i++) {
        TipoApontador pAtual = Tabela[i].Primeiro;
        while (pAtual != NULL) {
            TipoApontador pProximo = pAtual->Prox;
            if (pAtual != Tabela[i].Primeiro) {
                // É preciso liberar também as sub-listas de ocorrências de cada item.
                ApontadorLista pIndiceAtual = pAtual->Item.idPalavra.pPrimeiro;
                while (pIndiceAtual != NULL) {
                    ApontadorLista pIndiceProximo = pIndiceAtual->pProx;
                    free(pIndiceAtual);
                    pIndiceAtual = pIndiceProximo;
                }
            }
            free(pAtual);
            pAtual = pProximo;
        }
    }
}
// Realiza a busca de um ou mais termos na tabela e imprime os resultados.
void PesquisaNaTabelaHash(TipoLista* TabelaHash, char** vetorTermos, int numTermos, int M, TipoPesos p, int* comparacoes) {
    int termosEncontrados = 0;
    
    if (TabelaHash == NULL || M == 0) {
        printf("Tabela Hash nao foi criada ainda.\n");
        return;
    }
    for (int i = 0; i < numTermos; i++)
    {
        int comp_termo = 0;
        char* palavraBusca = vetorTermos[i];
        
        int indiceHash = h(palavraBusca, p, M);
        TipoItem* itemAchado = PesquisaItem(palavraBusca, p, TabelaHash, M, &comp_termo);
        
        if (itemAchado != NULL) {           
            termosEncontrados++;
            printf("\n%d: %s ", indiceHash, itemAchado->Chave);

            ApontadorLista pIndice = itemAchado->idPalavra.pPrimeiro->pProx;
            while (pIndice != NULL) {
                printf("<%d, %d> ", pIndice->idTermo.qtde, (pIndice->idTermo.idDoc)+1);
                pIndice = pIndice->pProx;
            }
            printf("\n");
        } else {
            printf("\nPalavra '%s' nao encontrada na tabela hash.\n", palavraBusca);
        }
        (*comparacoes) += comp_termo;
    }
    printf("---------------------------------\n");
}