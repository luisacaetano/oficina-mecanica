#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/peca.h"
#include "../bibliotecas/fornecedor.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/Modulo2.h"
#include "../bibliotecas/oficina.h"

Estoque * readEstoque() {
    return getEstoque();
}
// retorna a posição do peca no vetor de peças atraves da busca por descricao
int encontraPecaNome(Peca *pecas, char * descricao,int idFornecedor) {
    int posicao = FALSE;
    for(int i = 0; i < getTamanhoFornecedores(); i++ ) {
        if(equalsString(pecas[i].descricao,descricao) == TRUE) {
            if(pecas[i].fornecedor == idFornecedor) {
                posicao = i;
                return posicao;
            }

        }
    }
    return posicao;
}
int createEstoque(Estoque **estoques,Estoque * estoque,Oficina * oficina, Peca **pecas, Fornecedor **fornecedores, Transacao **transacoes,
                     ContasPagar **contas_pagar, ContasReceber **contas_receber, float *valor_em_caixa) {
    int tamanhoAtual = getTamanhoEstoque(); // Obtém o tamanho atual da lista de estoques

    // Realoca a memória para incluir o novo estoque
    Estoque *novoEstoques = realloc(*estoques, (tamanhoAtual + 1) * sizeof(Estoque));
    if (novoEstoques == NULL) {
        printf("Erro ao alocar memória para estoques.\n");
        return FALSE; // Retorna -1 em caso de falha na alocação
    }

    *estoques = novoEstoques; // Atualiza o ponteiro para o novo bloco de memória
    int index = tamanhoAtual; // O índice do novo estoque é igual ao tamanho atual da lista

    // Preenche os dados do novo estoque
    (*estoques)[index].codigo = buscaNovoIDEstoque(*estoques);
    (*estoques)[index].codigoFornecedor = estoque->codigoFornecedor;
    (*estoques)[index].imposto = estoque->imposto;
    (*estoques)[index].frete = estoque->frete;
    (*estoques)[index].quantidadePecas = estoque->quantidadePecas;
    //int qtdPecasEstoque[estoque->quantidadePecas];
    int totalQtdPecasEstoque =0;
    // reune
    for(int i = 0; i < estoque->quantidadePecas; i++) {
        int posicao =showPeca(*pecas,estoque->codigoPecas[i]);
        totalQtdPecasEstoque+= (*pecas)[posicao].estoque;
    }
    float imposto = estoque->imposto/totalQtdPecasEstoque;
    float frete = estoque ->frete/totalQtdPecasEstoque;
    float valorTotal = 0;
    for(int i = 0; i < estoque->quantidadePecas; i++) {
        int posicao =showPeca(*pecas,estoque->codigoPecas[i]);

        float custos = (*pecas)[posicao].preco_custo + frete+ imposto ;
         float lucro = (*pecas)[posicao].preco_venda * (oficina->porcentagem_lucro/100);
        (*pecas)[posicao].preco_venda = custos+lucro;
        valorTotal += (*pecas)[posicao].preco_custo;
    }
    (*estoques)[index].valorTotal = valorTotal;
    if(getTipoArquivo() != MEM ) setPecas(*pecas);

    // Aloca e copia os códigos das peças
    if (estoque->quantidadePecas > 0) {
        (*estoques)[index].codigoPecas = malloc(sizeof(int) * estoque->quantidadePecas);
        if ((*estoques)[index].codigoPecas == NULL) {
            printf("Erro ao alocar memória para os códigos das peças.\n");
            return FALSE; // Retorna -1 em caso de falha na alocação
        }
        memcpy((*estoques)[index].codigoPecas, estoque->codigoPecas, sizeof(int) * estoque->quantidadePecas);
    }
    int codTransacao = novaTransacao(transacoes,COMPRA,(*estoques)[index].valorTotal,contas_pagar,contas_receber,valor_em_caixa);
    if( codTransacao== FALSE)
        return FALSE;
    (*estoques)[index].codigoTransacao = codTransacao;
    setTamanhoEstoque(); // Atualiza o tamanho total da lista de estoques

    // Salva os dados no arquivo se o modo de armazenamento não for somente memória
    if (getTipoArquivo() != MEM) setEstoque(*estoques);

    return TRUE;
}

int buscaNovoIDEstoque(Estoque *estoques) {
    int maior = 1; // Inicializa o maior ID como 1
    if (getTamanhoEstoque() == 0) return maior; // Retorna 1 se não houver estoques

    // Itera sobre a lista de estoques para encontrar o maior ID
    for (int i = 0; i < getTamanhoEstoque(); i++) {
        if (maior <= estoques[i].codigo) {
            maior = estoques[i].codigo + 1; // Define o próximo ID como maior + 1
        }
    }
    return maior; // Retorna o novo ID
}
int showEstoque(Estoque *estoques, int codigo) {
    if (estoques == NULL) return FALSE;

    int posicao = 0;
    int tamanho = getTamanhoEstoque();

    // Procura o estoque com o código especificado dentro do limite do array
    while (posicao < tamanho && estoques[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se o estoque foi encontrado
    if (posicao == tamanho)
        return FALSE;

    return posicao;
}

