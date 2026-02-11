#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/servico.h"
#include "../bibliotecas/utils.h"

Servico *readServicos() {
    // Função para ler os clientes existentes
    return getServicos(); // Obtém a lista de clientes de um arquivo ou memória e retorna
}

// Função para criar um novo cliente
int createServico(Servico **servicos, Servico *servico) {
    int tamanhoAtual = getTamanhoServicos(); // Obtém o tamanho atual da lista de clientes
    Servico *novoServicos = realloc(*servicos, (tamanhoAtual + 1) * sizeof(Servico));
    if (novoServicos == NULL) {
        printf("Erro ao alocar mais memória para serviços.\n");
        return FALSE; // Retorna -1 indicando falha
    }

    *servicos = novoServicos; // Atualiza o ponteiro para o novo bloco de memória
    int index = tamanhoAtual; // O índice do novo cliente é igual ao tamanho atual da lista
    (*servicos)[index].codigo = buscaNovoIDServico(*servicos); // Função para buscar novo ID de serviço
    strcpy((*servicos)[index].descricao, servico->descricao);
    (*servicos)[index].preco = servico->preco;
    (*servicos)[index].comicao = servico->comicao;
    (*servicos)[index].ativo = 1;

    setTamanhoServicos(); // Atualiza o tamanho dos serviços

    // Grava os dados no arquivo, se o tipo não for MEM
    if (getTipoArquivo() != MEM) setServicos(*servicos);

    return TRUE; // Retorna 1 indicando sucesso
}

// Função para exibir um cliente com base no código
int showServico(Servico *servicos, int codigo) {
    if (servicos == NULL) return FALSE;

    int posicao = 0;
    int tamanho = getTamanhoServicos();

    // Procura o serviço com o código especificado dentro do limite do array
    while (posicao < tamanho && servicos[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se o serviço foi encontrado
    if (posicao == tamanho)
        return FALSE;

    return posicao;
}

// Função para atualizar os dados de um serviço existente
int updateServico(Servico *servicos, Servico *servico) {
    int posicao = showServico(servicos, servico->codigo);

    if (posicao == FALSE) return FALSE;

    // Atualiza os campos necessários
    servicos[posicao].codigo = servico->codigo;
    strcpy(servicos[posicao].descricao, servico->descricao);
    servicos[posicao].preco = servico->preco;
    servicos[posicao].comicao = servico->comicao;

    if (getTipoArquivo() != MEM) setServicos(servicos);
    return TRUE;
}

// Função para deletar (ou desativar) um serviço
int deleteServico(Servico *servicos, int codigo) {
    int posicao = showServico(servicos, codigo);

    if (posicao == FALSE) return FALSE; // Verifica se o serviço existe e está ativo

    servicos[posicao].ativo = FALSE; // Marca o serviço como inativo
    if (getTipoArquivo() != MEM) setServicos(servicos);
    return TRUE;
}

// Função para gerar um novo ID exclusivo para o novo serviço
int buscaNovoIDServico(Servico *servicos) {
    int maior = 1;
    if (getTamanhoServicos() == 0) return maior; // Caso não haja nenhum serviço registrado

    // Itera sobre a lista de clientes para encontrar o maior ID
    for (int i = 0; i < getTamanhoServicos(); i++) {
        if (maior <= servicos[i].codigo) {
            maior = servicos[i].codigo + 1; // Define o próximo ID como maior + 1
        }
    }
    return maior;
}

int saveServicoCSV(Servico *servicos, int tamanho) {
    char **string = malloc(sizeof(char) * tamanho+1);
    string[0] = "codigo;descricao;preco;comicao";
    for (int i = 1; i < tamanho+1; i++) {
        string[i] = malloc(sizeof(char) * 150);
        string[i] = transformaString(&servicos[i-1].codigo, 'i');
        string[i] = concatenarStringPontoEVirgula(string[i], servicos[i-1].descricao);
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&servicos[i-1].preco, 'f'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&servicos[i-1].comicao, 'f'));
    }
    escreverCSV(string, "servico", tamanho+1);
    return TRUE;
}

// Função para filtrar os serviços por descrição
Servico *filterServicoDescricao(Servico *servicos, char *descricao, int *tamanho) {
    Servico *servicoFiltrados = malloc(sizeof(Servico));

    int tamanhoTotal = getTamanhoServicos();

    for (int i = 0; i < tamanhoTotal; i++) {
        if (equalsString(servicos[i].descricao, descricao) == TRUE) {
            if (servicos[i].ativo == TRUE) {
                *(tamanho) = *(tamanho) + 1;
                servicoFiltrados = realloc(servicoFiltrados, (*tamanho) * sizeof(Servico));
                if (servicoFiltrados == NULL) {
                    printf("Erro ao alocar memória para serviços filtrados.\n");
                    return NULL;
                }
                servicoFiltrados[*tamanho - 1] = servicos[i];
            }
        }
    }
    if (*tamanho == 0) return NULL;

    return servicoFiltrados;
}
