#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../bibliotecas/caixa.h"
#include "../bibliotecas/Modulo2.h"
#include "../bibliotecas/utils.h"

Comissao *readComisoes() {
    return getComissoes();
}

int createComissao(Comissao **comissoes, int codigo, float valorComissao, double tempoGasto, int codigoFuncionario) {
    int tamanhoAtual = getTamanhoComissoes(); // Obtém o tamanho atual da lista de comissões

    // Realoca a memória para incluir a nova comissão
    Comissao *novasComissoes = realloc(*comissoes, (tamanhoAtual + 1) * sizeof(Comissao));
    if (novasComissoes == NULL) {
        printf("Erro ao alocar memória para comissões.\n");
        return FALSE; // Retorna em caso de falha na alocação
    }

    *comissoes = novasComissoes; // Atualiza o ponteiro para o novo bloco de memória
    int index = tamanhoAtual; // O índice da nova comissão é igual ao tamanho atual da lista

    // Preenche os dados da nova comissão
    (*comissoes)[index].codigo = buscaNovoIDComissao(*comissoes); // Atribui um novo ID à comissão
    (*comissoes)[index].CodigoOrdem = codigo;
    (*comissoes)[index].valorComisao = valorComissao;
    (*comissoes)[index].tempoGasto = tempoGasto;
    (*comissoes)[index].codigoFuncionario = codigoFuncionario;
    (*comissoes)[index].pago = FALSE;

    setTamanhoComissoes(); // Atualiza o tamanho total da lista de comissões

    // Salva os dados no arquivo se o modo de armazenamento não for somente memória
    if (getTipoArquivo() != MEM) setComissoes(*comissoes);

    return TRUE;
}

// Função para exibir uma comissão com base no código
int showComissao(Comissao *comissoes, int codigo) {
    if (getTamanhoComissoes() == 0) return FALSE; // Retorna FALSE se a lista de comissões for vazia

    int posicao = 0;
    int tamanho = getTamanhoComissoes(); // Obtém o tamanho atual da lista de comissões

    // Procura pela comissão com o código especificado
    while (posicao < tamanho && comissoes[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se a comissão foi encontrada
    if (posicao == tamanho)
        return FALSE;

    return posicao; // Retorna a posição da comissão na lista
}

// Função para gerar um novo ID exclusivo para uma comissão
int buscaNovoIDComissao(Comissao *comissoes) {
    int maior = 1; // Inicializa o maior ID como 1
    if (getTamanhoComissoes() == 0) return maior; // Retorna 1 se não houver comissões

    // Itera sobre a lista de comissões para encontrar o maior ID
    for (int i = 0; i < getTamanhoComissoes(); i++) {
        if (maior <= comissoes[i].codigo) {
            maior = comissoes[i].codigo + 1; // Define o próximo ID como maior + 1
        }
    }
    return maior; // Retorna o novo ID
}

int pagandoComisao(Comissao **comissoes, int posicao, float *valor_em_caixa) {
    if (removeDinheiroDoCaixa((*comissoes)[posicao].valorComisao, valor_em_caixa) == FALSE) return FALSE;
    (*comissoes)[posicao].pago = TRUE;
    if (getTipoArquivo() != MEM)setComissoes(*comissoes);
    return TRUE;
}
