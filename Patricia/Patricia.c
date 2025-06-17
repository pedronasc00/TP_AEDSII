#include "Patricia.h"

// Retorna se o nó é externo
short EExterno(TipoArvore p)
{
  return (p->nt == Externo);
}

// Cria um novo nó interno
TipoArvore CriaNoInterno(int index, char caractere, TipoArvore Esq, TipoArvore Dir)
{
  TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
  p->nt = Interno;
  p->NO.NInterno.Index = index;
  p->NO.NInterno.caractere = caractere;
  p->NO.NInterno.Esq = Esq;
  p->NO.NInterno.Dir = Dir;
  return p;
}

// Cria um novo nó externo (com a palavra e uma ocorrência)
TipoArvore CriaNoExterno(char *palavra, int idDoc)
{
  TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
  p->nt = Externo;
  p->NO.NExterno.Palavra = strdup(palavra); // copia a string

  FLVazia(&(p->NO.NExterno.Ocorrencias)); // inicializa a lista
  Indice novo;
  novo.qtde = 1;
  novo.idDoc = idDoc;
  LInsere(&(p->NO.NExterno.Ocorrencias), &novo);

  return p;
}

// Encontra a primeira posição em que as palavras diferem
int PrimeiraDiferenca(char *a, char *b)
{
  int i = 0;
  while (a[i] && b[i] && a[i] == b[i])
    i++;
  return i;
}

// Insere a palavra na árvore, entre dois nós, se necessário
TipoArvore InsereEntre(char *palavra, int idDoc, TipoArvore *t, TipoIndexAmp index)
{
  TipoArvore nova = CriaNoExterno(palavra, idDoc);
  char c = palavra[index];

  if (EExterno(*t))
  {
    char cExistente = (*t)->NO.NExterno.Palavra[index];
    if (c < cExistente)
      return CriaNoInterno(index, c, nova, *t);
    else
      return CriaNoInterno(index, c, *t, nova);
  }
  else if (index < (*t)->NO.NInterno.Index)
  {
    if (c < (*t)->NO.NInterno.caractere)
      return CriaNoInterno(index, c, nova, *t);
    else
      return CriaNoInterno(index, c, *t, nova);
  }
  else
  {
    if (palavra[(*t)->NO.NInterno.Index] < (*t)->NO.NInterno.caractere)
      (*t)->NO.NInterno.Esq = InsereEntre(palavra, idDoc, &(*t)->NO.NInterno.Esq, index);
    else
      (*t)->NO.NInterno.Dir = InsereEntre(palavra, idDoc, &(*t)->NO.NInterno.Dir, index);
    return *t;
  }
}

// Insere uma palavra e seu idDoc na árvore PATRICIA
TipoArvore InserePat(TipoArvore *raiz, char *palavra, int idDoc)
{
  if (*raiz == NULL)
    return CriaNoExterno(palavra, idDoc);

  TipoArvore p = *raiz;
  while (!EExterno(p))
  {
    if (palavra[p->NO.NInterno.Index] < p->NO.NInterno.caractere)
      p = p->NO.NInterno.Esq;
    else
      p = p->NO.NInterno.Dir;
  }

  // Palavra já existe?
  if (strcmp(palavra, p->NO.NExterno.Palavra) == 0)
  {
    // Atualiza ocorrência: adiciona ou incrementa
    ApontadorLista aux = p->NO.NExterno.Ocorrencias.pPrimeiro->pProx;
    while (aux != NULL)
    {
      if (aux->idTermo.idDoc == idDoc)
      {
        aux->idTermo.qtde++;
        return *raiz;
      }
      aux = aux->pProx;
    }
    Indice novo;
    novo.qtde = 1;
    novo.idDoc = idDoc;
    LInsere(&(p->NO.NExterno.Ocorrencias), &novo);
    return *raiz;
  }

  // Senão, encontrar primeira posição de diferença
  int diff = PrimeiraDiferenca(palavra, p->NO.NExterno.Palavra);
  return InsereEntre(palavra, idDoc, raiz, diff);
}

// Pesquisa uma palavra e imprime suas ocorrências
void PesquisaPAT(TipoArvore t, char *palavra)
{
  if (t == NULL)
  {
    printf("Arvore vazia\n");
    return;
  }

  while (!EExterno(t))
  {
    if (palavra[t->NO.NInterno.Index] < t->NO.NInterno.caractere)
      t = t->NO.NInterno.Esq;
    else
      t = t->NO.NInterno.Dir;
  }

  if (strcmp(t->NO.NExterno.Palavra, palavra) == 0)
  {
    printf("Encontrada: %s -> ", palavra);
    LImprime(&(t->NO.NExterno.Ocorrencias));
  }
  else
  {
    printf("Palavra %s nao encontrada\n", palavra);
  }
}

// Imprime todas as palavras e ocorrências da árvore (em ordem qualquer)
void ImprimePatricia(TipoArvore t)
{
  if (t == NULL)
    return;

  if (EExterno(t))
  {
    printf("%s: ", t->NO.NExterno.Palavra);
    LImprime(&(t->NO.NExterno.Ocorrencias));
    printf("\n");
    return;
  }

  ImprimePatricia(t->NO.NInterno.Esq);
  ImprimePatricia(t->NO.NInterno.Dir);
}
