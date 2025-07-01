# Estudo comparativo entre Árvore PATRICIA e Tabela HASH como estruturas para implementar arquivo invertido

## RESUMO

Este projeto prático implementa um algoritmo de busca e análise de relevância de documentos, e para isso utilizamos duas estrutura de dados distintas (Tabela HASH e Árvore PATRICIA). Também, foi construído a estrutura de índice invertido a partir de uma coleção de documentos de texto, pesquisa por termos e classificação de documentos por relevância com base na consulta.

## ESTRUTURA DE PROJETO

O projeto foi organizado da seguinte maneira:
- IndiceInvertido/: Contém os arquivos para a criação da lista de índices e funções auxiliares para processamento dos arquivos, como Tokenização;
- Hash/: Contém os arquivos .h e .c referentes à implementação da Tabela Hash;
- Patricia/: Contém os arquivos de .h e de .c referentes à implementação da Árvore Patricia;
- POCs/: Contém os documentos de texto utilizados como base para a análise;
- main.c: Arquivo principal do porjeto, possuindo o menu e a lógica de execução das funções;
- makefile: Arquivo para automação da compilação do projeto;
- entrada.txt: Arquivo de exemplo que lista os documentos a serem processados.

## ARQUIVOS DE TESTE

Para os arquivos de teste, que seram enviados para execução, deve ser escrito na primeira linha a quantidade de documentos que seram lidos pelo projeto. E, as linhas seguintes serão descrevendo o nome dos arquivos a serem lidos.
**IMPORTANTE:** Não se esqueça de colocar o nome da pasta antes dos titulos POC's de teste que estaram no arquivo de leitura. Exemplos: ***POCs/Analysis_Impact*** deve ser escrito dessa forma dentro do arquivo de teste.

## COMPILAÇÃO

Para compilar o programa e executar o programa, utilize o comando **make** ou **mingw32-make**.

## FUNCIONALIDADES

1. **Construir Indice Invertido (Tabela HASH):** Cria o índice invertido de todas as palavras dos documentos utilizados no arquivo de teste;
2. **Construir Indice Invertido (PATRICIA):** Cria o índice invertido de todas as palavras dos documentos utilizados no arquivo de teste;
3. **Pesquisa e Relevância (HASH):** Permite ao usuário inserir termo(s) para pesquisa na tabela HASH, com isso exibe o índice invertido referente ao(s) termo(s) e também calcula o valor da relevância dos termos e exibe os documentos com maior valor.
4. **Pesquisa e Relevância (PATRICIA):** Permite ao usuário inserir termo(s) para pesquisa na árvore PATRICIA, com isso exibe o índice invertido referente ao(s) termo(s) e também calcula o valor da relevância dos termos e exibe os documentos com maior valor.
5. **Estatistica de Comparação:** Exibe estatísticas comparativas de desempenho (número de comparações) entre a Tabela Hash e a Árvore Patricia para as operações de inserção e busca.
6. **Sair:** Encerra a execução do programa.
