#include "../bibliotecas/caixa.h"
#include "../bibliotecas/utils.h"

// Função para ler o valor atual do caixa usando a função getCaixa()
float readCaixa() {
    return getCaixa();
}

// Função para adicionar dinheiro ao caixa
int addDinheiroAoCaixa(float valor_adicionado, float *valor_em_caixa) {
    if (getTipoArquivo() != MEM) {
        // Atualiza o valor do caixa no arquivo se não estiver em memória
        setCaixa((*valor_em_caixa + valor_adicionado));
        *valor_em_caixa = getCaixa();
    } else {
        // Se estiver em memória, apenas soma diretamente
        *valor_em_caixa += valor_adicionado;
    }
    return TRUE; // Retorna 1 indicando sucesso
}

// Função para remover dinheiro do caixa
// Parâmetros: valor_removido - valor a ser removido do caixa
//             valor_em_caixa - ponteiro para o valor atual do caixa
int removeDinheiroDoCaixa(float valor_removido, float *valor_em_caixa) {
    if (*valor_em_caixa < valor_removido) {
        // Retorna -1 se o valor a ser removido for maior que o disponível
        return FALSE;
    }
    *valor_em_caixa -= valor_removido;
    if (getTipoArquivo() != MEM) {
        // Atualiza o valor do caixa no arquivo se não estiver em memória
        setCaixa(*valor_em_caixa);
        *valor_em_caixa = getCaixa();
    }
    return TRUE; // Retorna 1 indicando sucesso
}
