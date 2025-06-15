#include "Hash.h"

void THFLVazia(TipoLista *Lista)
{ Lista->Primeiro = (TipoCelula *)malloc(sizeof(TipoCelula));
  Lista->Ultimo = Lista->Primeiro; Lista->Primeiro->Prox = NULL;
} 
short THVazia(TipoLista Lista)
{ return (Lista.Primeiro == Lista.Ultimo); }

void Ins(TipoItem x, TipoLista *Lista)
{ Lista->Ultimo->Prox = (TipoCelula *)malloc(sizeof(TipoCelula));
  Lista->Ultimo = Lista->Ultimo->Prox; Lista->Ultimo->Item = x;
  Lista->Ultimo->Prox = NULL;
}  

void Ret(TipoApontador p, TipoLista *Lista, TipoItem *Item)
{  /* -- Obs.: o item a ser retirado o seguinte ao apontado por p -- */
  TipoApontador q;
  if (THVazia(*Lista) || p == NULL || p->Prox == NULL) 
  { printf(" Erro Lista vazia ou posicao nao existe\n");
    return;
  }
  q = p->Prox; *Item = q->Item; p->Prox = q->Prox;
  if (p->Prox == NULL)
  Lista->Ultimo = p;
  free(q);
}void GeraPesos(TipoPesos p)
{ /* Gera valores randomicos entre 1 e 10.000 */
  int i, j;
  struct timeval semente;
  /* Utilizar o tempo como semente para a funcao srand() */
  gettimeofday(&semente, NULL); 
  srand((int)(semente.tv_sec + 1000000 * semente.tv_usec));
  for (i = 0; i < N; i++)
    for (j = 0; j < TAMALFABETO; j++)
      p[i][j] = 1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0));
}

TipoIndice h(TipoChave Chave, TipoPesos p)
{ int i; unsigned int Soma = 0; 
  int comp = strlen(Chave);
  for (i = 0; i < comp; i++) Soma += p[i][(unsigned int)Chave[i]];
  return (Soma % M);
}

void Inicializa(TipoDicionario T)
{ int i;
  for (i = 0; i < M; i++) THFLVazia(&T[i]);
}

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T)
{ /* Obs.: TipoApontador de retorno aponta para o item anterior da lista */
  TipoIndice i;
  TipoApontador Ap;
  i = h(Ch, p);
  if (THVazia(T[i])) return NULL;  /* Pesquisa sem sucesso */
  else 
  { Ap = T[i].Primeiro;
    while (Ap->Prox->Prox != NULL &&
        strncmp(Ch, Ap->Prox->Item.Chave, sizeof(TipoChave))) 
      Ap = Ap->Prox;
    if (!strncmp(Ch, Ap->Prox->Item.Chave, sizeof(TipoChave))) 
    return Ap;
    else return NULL;  /* Pesquisa sem sucesso */
  }
}
TipoItem* PesquisaItem(TipoChave Ch, TipoPesos p, TipoDicionario T) {
    TipoApontador pAnterior = Pesquisa(Ch, p, T);

    // Se pAnterior não for nulo, o item está na célula seguinte
    if (pAnterior != NULL) {
        return &pAnterior->Prox->Item;
    }

    return NULL; // Retorna NULL se não encontrou
}  

void Insere(TipoItem x, TipoPesos p, TipoDicionario T)
{ if (Pesquisa(x.Chave, p, T) == NULL)
  Ins(x, &T[h(x.Chave, p)]);
  else printf(" Registro ja  esta  presente\n");
} 

void Retira(TipoItem x, TipoPesos p, TipoDicionario T)
{ TipoApontador Ap; Ap = Pesquisa(x.Chave, p, T);
  if (Ap == NULL)
  printf(" Registro nao esta  presente\n");
  else Ret(Ap, &T[h(x.Chave, p)], &x);
}

void Imp(TipoLista Lista)
{ TipoApontador Aux;
  Aux = Lista.Primeiro->Prox;
  while (Aux != NULL) 
    { printf("Palavra: %s ", Aux->Item.Chave);
        LImprime(&Aux->Item.idPalavra);
        printf("\n");
      Aux = Aux->Prox;
    }
}

void Imprime(TipoDicionario Tabela)
{ int i;
  for (i = 0; i < M; i++) 
    { printf("%d: ", i);
      if (!THVazia(Tabela[i]))
      Imp(Tabela[i]);
      putchar('\n');
    }
} 
 
void LerPalavra(char *p, int Tam)
{ char c; int i, j;
  fflush(stdin); j=0;
  while (((c=getchar())!='\n') && j<Tam-1) p[j++]= c;
  p[j]='\0';
  while(c != '\n') c=getchar();
    for(i=j-1;(i>=0 && p[i]==' ');i--) p[i]='\0';
}
void TokenizacaoTermo(char* in, char* out) {
    int c_fim = 0;
    for (int i = 0; in[i] != '\0'; i++) {
        if (isalpha((unsigned char) in[i])) {
            out[c_fim] = tolower((unsigned char) in[i]);
            c_fim++;
        }
    }
    out[c_fim] = '\0';
}
ApontadorLista BuscaIndice(LLista* listaI, int idDoc) {
    ApontadorLista pAux = listaI->pPrimeiro->pProx;
    while (pAux != NULL) {
        if (pAux->idTermo.idDoc == idDoc) {
            return pAux;
        }
        pAux = pAux->pProx;
    }
    return NULL;
}
void ProcessaArquivo(FILE* arq, int idDoc, TipoPesos p, TipoDicionario T) {
    if (arq == NULL) return;
    
    char buffer[100]; // Buffer para ler palavras do arquivo
    TipoChave termoToken;

    while (fscanf(arq, "%99s", buffer) != EOF) {
        TokenizacaoTermo(buffer, termoToken);
        
        if (strlen(termoToken) == 0) continue;

        // Procura o termo na tabela hash
        TipoItem* itemEncontrado = PesquisaItem(termoToken, p, T);

        if (itemEncontrado != NULL) {
            // O termo JÁ EXISTE na tabela.
            // Agora, verifique se ele já foi registrado para este documento.
            ApontadorLista indiceDoc = BuscaIndice(&itemEncontrado->idPalavra, idDoc);
            if (indiceDoc != NULL) {
                // Já existe para este doc, apenas incrementa a quantidade.
                indiceDoc->idTermo.qtde++;
            } else {
                // Primeira vez neste doc, insere um novo índice.
                Indice novoIndice = {1, idDoc};
                LInsere(&itemEncontrado->idPalavra, &novoIndice);
            }
        } else {
            // O termo é NOVO.
            // 1. Crie um novo item.
            TipoItem novoItem;
            strcpy(novoItem.Chave, termoToken);

            // 2. Inicialize sua lista de índices.
            FLVazia(&novoItem.idPalavra);
            
            // 3. Adicione o primeiro índice.
            Indice novoIndice = {1, idDoc};
            LInsere(&novoItem.idPalavra, &novoIndice);
            
            // 4. Insira o novo item completo na tabela hash.
            Insere(novoItem, p, T);
        }
    }
}
void LiberaTabela(TipoDicionario T) {
    for (int i = 0; i < M; i++) {
        TipoApontador pAtual = T[i].Primeiro;
        while (pAtual != NULL) {
            TipoApontador pProximo = pAtual->Prox;

            // Se for uma célula de dados (não o nó cabeça)
            if (pAtual != T[i].Primeiro) {
                // 1. Liberar a lista de índices interna (LLista)
                ApontadorLista pIndiceAtual = pAtual->Item.idPalavra.pPrimeiro;
                while (pIndiceAtual != NULL) {
                    ApontadorLista pIndiceProximo = pIndiceAtual->pProx;
                    free(pIndiceAtual);
                    pIndiceAtual = pIndiceProximo;
                }
            }

            // 2. Liberar a própria célula da tabela hash
            free(pAtual);
            pAtual = pProximo;
        }
    }
}