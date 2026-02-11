#ifndef CAIXA_H
#define CAIXA_H

#include <stdio.h>

// Declaração das funções relacionadas ao gerenciamento de dados do caixa

// Model Caixa

// Função para migrar dados entre arquivos diferentes (txt, binário ou memória)
float migraDadosCaixa();

// Função para definir o valor atual do caixa no armazenamento
void setCaixa(float valor_em_caixa);

// Função para obter o valor atual do caixa
float getCaixa();

// Função para ler o valor do caixa de um arquivo texto
float ler_arquivo_txt_caixa(FILE *buffer);

// Função para ler o valor do caixa de um arquivo binário
float ler_arquivo_bin_caixa(FILE *buffer);

// Função para escrever o valor do caixa em um arquivo texto
void escrever_arquivo_txt_caixa(FILE *buffer, float valor_em_caixa);

// Função para escrever o valor do caixa em um arquivo binário
void escrever_arquivo_bin_caixa(FILE *buffer, float valor_em_caixa);


// Caixa Controller

// Função que retorna o valor atual do caixa usando `getCaixa`
float readCaixa();

// Função para adicionar um valor ao caixa
// `valor_adicionado`: Valor a ser somado ao caixa
// `valor_em_caixa`: Ponteiro para o valor atual do caixa
int addDinheiroAoCaixa(float valor_adicionado, float *valor_em_caixa);

// Função para remover um valor do caixa
// `valor_removido`: Valor a ser subtraído do caixa
// `valor_em_caixa`: Ponteiro para o valor atual do caixa
int removeDinheiroDoCaixa(float valor_removido, float *valor_em_caixa);

#endif // CAIXA_H
