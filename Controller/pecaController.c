#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/peca.h"
#include "../bibliotecas/utils.h"

// Função para ler as peças do sistema
Peca *readPecas() {
    return getPecas();
}

// Função para criar uma nova peça
int createPeca(Peca **pecas, Peca *peca) {
    int tamanhoAtual = getTamanhoPecas(); // Obtém o tamanho atual do array de peças
    Peca *novoPecas = realloc(*pecas, (tamanhoAtual + 1) * sizeof(Peca)); // Realoca memória para mais uma peça

    if (novoPecas == NULL) {
        // Se a realocação falhar
        printf("Erro ao alocar mais memória para peças.\n");
        return FALSE; // Retorna 0 indicando falha
    }
    *pecas = novoPecas; // Atualiza o ponteiro para o array de peças
    int index = tamanhoAtual; // Define o índice da nova peça
    // Preenche os campos da nova peça
    (*pecas)[index].codigo = buscaNovoIDPeca(*pecas);
    strcpy((*pecas)[index].descricao, peca->descricao);
    strcpy((*pecas)[index].fabricante, peca->fabricante);
    (*pecas)[index].fornecedor = peca->fornecedor;
    (*pecas)[index].preco_custo = peca->preco_custo;
    (*pecas)[index].preco_venda = peca->preco_venda;
    (*pecas)[index].estoque = peca->estoque;
    (*pecas)[index].estoque_min = peca->estoque_min;
    (*pecas)[index].ativo = 1; // Marca a peça como ativa ao ser criada
    setTamanhoPecas(); // Atualiza o tamanho das peças

    if (getTipoArquivo() != MEM) setPecas(*pecas); // Se não estiver no modo MEM, grava as peças no arquivo

    return (*pecas)[index].codigo; // Retorna verdadeiro indicando sucesso
}

// Função para mostrar uma peça, dado seu código
int showPeca(Peca *pecas, int codigo) {
    if (pecas == NULL) return FALSE; // Se o array de peças for nulo, retorna falso

    int posicao = 0;
    int tamanho = getTamanhoPecas(); // Obtém o tamanho do array de peças

    // Procura a peça pelo código dentro do array
    while (posicao < tamanho && pecas[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se a peça foi encontrada e se está ativa
    if (posicao == tamanho || pecas[posicao].ativo == FALSE)
        return FALSE; // Retorna falso se a peça não foi encontrada ou está inativa

    return posicao; // Retorna a posição da peça encontrada
}

// Função para atualizar os dados de uma peça
int updatePeca(Peca *pecas, Peca *peca) {
    int posicao = showPeca(pecas, peca->codigo); // Busca a peça a ser atualizada

    if (posicao == FALSE) return FALSE; // Se a peça não foi encontrada, retorna falso

    // Atualiza os campos da peça com os novos valores
    pecas[posicao].codigo = peca->codigo;
    strcpy(pecas[posicao].descricao, peca->descricao);
    strcpy(pecas[posicao].fabricante, peca->fabricante);
    pecas[posicao].fornecedor = peca->fornecedor;
    pecas[posicao].preco_custo = peca->preco_custo;
    pecas[posicao].preco_venda = peca->preco_venda;
    pecas[posicao].estoque = peca->estoque;
    pecas[posicao].estoque_min = peca->estoque_min;
    pecas[posicao].ativo = peca->ativo; // Atualiza o campo ativo

    if (getTipoArquivo() != MEM) setPecas(pecas); // Se não estiver no modo MEM, grava as peças no arquivo
    return TRUE; // Retorna verdadeiro indicando sucesso
}

// Função para excluir uma peça (marcando-a como inativa)
int deletePeca(Peca *pecas, int codigo) {
    int posicao = showPeca(pecas, codigo); // Busca a peça a ser excluída

    if (posicao == FALSE) return FALSE; // Se a peça não for encontrada, retorna falso

    pecas[posicao].ativo = FALSE; // Marca a peça como inativa (não exclui do array)
    if (getTipoArquivo() != MEM) setPecas(pecas); // Se não estiver no modo MEM, grava as peças no arquivo
    return TRUE; // Retorna verdadeiro indicando sucesso
}

// Função para buscar o próximo ID disponível para uma nova peça
int buscaNovoIDPeca(Peca *pecas) {
    int maior = 1; // Inicia com o valor mínimo do código

    if (getTamanhoPecas() == 0) return maior; // Se não houver peças, o ID será 1

    // Itera sobre as peças para encontrar o maior código
    for (int i = 0; i < getTamanhoPecas(); i++) {
        if (maior <= pecas[i].codigo) {
            maior = pecas[i].codigo + 1; // Atualiza o maior código encontrado
        }
    }
    return maior; // Retorna o próximo código disponível
}

int savePecaCSV(Peca *pecas, int tamanho) {
    char **string = malloc(sizeof(char) * tamanho+1);
    string[0] ="codigo;descricao;fabricante;fornecedor;preco_custo;preco_venda;estoque;estoque_min";
    for (int i = 1; i < tamanho+1; i++) {
        string[i] = malloc(sizeof(char) * 150);
        string[i] = transformaString(&pecas[i-1].codigo, 'i');
        string[i] = concatenarStringPontoEVirgula(string[i], pecas[i-1].descricao);
        string[i] = concatenarStringPontoEVirgula(string[i], pecas[i-1].fabricante);
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&pecas[i-1].fornecedor, 'i'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&pecas[i-1].preco_custo, 'f'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&pecas[i-1].preco_venda, 'f'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&pecas[i-1].estoque, 'i'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&pecas[i-1].estoque_min, 'i'));
    }
    escreverCSV(string, "peca", tamanho+1);
    return TRUE;
}

Peca *filterPecaDescricao(Peca *pecas, char *descricao, int *tamanho) {
    Peca *pecaFiltrados = malloc(sizeof(Peca));

    int tamanhoTotal = getTamanhoPecas();

    for (int i = 0; i < tamanhoTotal; i++) {
        if (equalsString(pecas[i].descricao, descricao) == TRUE) {
            if (pecas[i].ativo == TRUE) {
                *(tamanho) = *(tamanho) + 1;
                pecaFiltrados = realloc(pecaFiltrados, (*tamanho) * sizeof(Peca));
                if (pecaFiltrados == NULL) {
                    printf("Erro ao alocar memória para peças filtrados.\n");
                    return NULL;
                }
                pecaFiltrados[*tamanho - 1] = pecas[i];
            }
        }
    }
    if (*tamanho == 0) return NULL;

    return pecaFiltrados;
}
