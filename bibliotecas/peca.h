#ifndef PECA_H
#define PECA_H

#include "fornecedor.h"

// Estrutura que armazena as informações de uma peça
typedef struct {
    int codigo;               // Código identificador da peça
    char descricao[100];      // Descrição detalhada da peça
    char fabricante[50];      // Nome do fabricante da peça
    int fornecedor;           // Código do fornecedor associado à peça
    float preco_custo;        // Preço de custo da peça
    float preco_venda;        // Preço de venda da peça
    int estoque;              // Quantidade atual em estoque
    int estoque_min;          // Quantidade mínima em estoque
    int ativo;                // Indicador de ativo (1 para ativo, -1 para inativo)
} Peca;

// Model Peça

int getTamanhoPecas();                         // Retorna a quantidade de peças cadastradas
void setTamanhoPecas();                        // Incrementa a quantidade de peças cadastradas
void editTamanhoPecas(int tamanho);            // Atualiza a quantidade de peças cadastradas
Peca *migraDadosPeca();                        // Migra os dados das peças entre diferentes formatos de arquivo
Peca *getPecas();                              // Retorna todas as peças cadastradas
void setPecas(Peca *pecas);                    // Atualiza a lista de peças cadastradas
Peca *ler_arquivo_txt_peca(FILE *buffer);      // Lê os dados de peças de um arquivo de texto
void escrever_arquivo_txt_peca(FILE *buffer, Peca *pecas);  // Escreve os dados de peças em um arquivo de texto
Peca *ler_arquivo_bin_peca(FILE *buffer);      // Lê os dados de peças de um arquivo binário
void escrever_arquivo_bin_peca(FILE *buffer, Peca *pecas);  // Escreve os dados de peças em um arquivo binário

// Controller Peças

Peca* readPecas();                             // Retorna a lista de peças cadastradas
int createPeca(Peca **pecas, Peca *peca);      // Cria uma nova peça
int showPeca(Peca *pecas, int codigo);         // Exibe as informações de uma peça com base no código
int updatePeca(Peca *pecas, Peca *peca);       // Atualiza os dados de uma peça
int deletePeca(Peca* pecas, int codigo);       // Remove uma peça pelo código
int buscaNovoIDPeca(Peca *pecas);              // Busca um novo ID válido para cadastro de uma nova peça
Peca *filterPecaDescricao(Peca *pecas, char *descricao, int *tamanho); // Filtra as peças por descrição
int savePecaCSV(Peca *pecas, int tamanho);    // Salva as peças em um arquivo CSV

void menuPecas(Peca **pecas, Fornecedor *fornecedores);  // Exibe o menu para manipulação das peças
void novaPeca(Peca **pecas, Fornecedor *fornecedores);   // Interage com o usuário para cadastrar uma nova peça
void mostrarPeca(Peca *pecas);                           // Mostra as informações de uma peça específica
void editarPeca(Peca *pecas, Fornecedor *fornecedores);  // Permite editar as informações de uma peça
void apagarPeca(Peca *pecas);                            // Remove uma peça
void mostrarTodasPecas(Peca *pecas);                     // Exibe todas as peças cadastradas
void filtrarPecasIDDescricao(Peca *pecas); // Filtra as peças por ID ou descrição
#endif // PECA_H
