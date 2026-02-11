#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include "../bibliotecas/caixa.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"
// Função para ler todas as contas a pagar cadastradas
ContasPagar *readContasPagar() {
    return getContasPagar(); // Retorna o ponteiro para a lista de contas a pagar
}

// Cria um nova conta a pagar e adiciona à lista
int createContasPagar(ContasPagar **contas, int codTransacao, time_t dataPagamento) {
    int tamanhoAtual = getTamanhoContasPagar(); // Obtém o tamanho atual da lista de contas a pagar
    // Realoca a memória para incluir a nova conta a pagar
    ContasPagar *novaConta = realloc(*contas, (tamanhoAtual + 1) * sizeof(ContasPagar));
    if (novaConta == NULL) {
        printf("Erro ao alocar memória para Contas a pagar.\n");
        return FALSE; // Retorna FALSE indicando falha
    }
    *contas = novaConta;
    int index = tamanhoAtual;
    // Preenche os dados da nova conta a pagar no array
    (*contas)[index].codigo = buscaNovoIDContasPagar(*contas);
    (*contas)[index].codTransacao = codTransacao;
    (*contas)[index].pago = FALSE;
    (*contas)[index].dataPagamento = dataPagamento;
    (*contas)[index].dataEfeituacaoPagamento = 0; // define que ainda não tem uma data de pagamento, pois não foi pago

    setTamanhoContasPagar(); // Incrementa o tamanho da lista de contas a pagar
    // Salva as transações no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setContasPagar(*contas);
    return TRUE;
}

int showContasPagar(ContasPagar *contas, int codigo) {
    if (contas == NULL) return FALSE; // Se o array de contas a pagar for nulo, retorna falso

    int posicao = 0;
    int tamanho = getTamanhoContasPagar(); // Obtém o tamanho do array de contas a pagar

    // Procura a conta pelo código dentro do array
    while (posicao < tamanho && contas[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se a conta foi encontrada e se está paga (ativo)
    if (posicao == tamanho)
        return FALSE; // Retorna falso se a conta não foi encontrada ou está marcada como não paga

    return posicao; // Retorna a posição da conta encontrada
}

// Função para apagar uma conta a pagar
int deleteContasPagar(ContasPagar *contas, int codigo) {
    int posicao = showContasPagar(contas, codigo); // Localiza a conta pelo código

    if (posicao == FALSE) return FALSE; // Retorna FALSE se a conta não for encontrada

    ContasPagar *removeConta = malloc(sizeof(ContasPagar) * (getTamanhoContasPagar() - 1));
    int novoTamanho = 0;

    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (contas[i].codigo != codigo) {
            removeConta[novoTamanho] = contas[i];
            novoTamanho++;
        }
    }

    contas = removeConta;
    removeTamanhoContasPagar(); // Decrementa o tamanho das contas a pagar

    // Salva as contas no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setContasPagar(contas);

    return TRUE; // Retorna TRUE indicando sucesso
}

//Troca a conta para conta paga
int contaPaga(ContasPagar *contas, int codigo, Transacao *transacoes, float *valor_em_caixa) {
    int posicao = showContasPagar(contas, codigo);
    if (posicao == FALSE) return FALSE;
    //pega a posicão onde está a transação para verificar o valor da conta as ser paga
    int index = showTransacao(transacoes, contas[posicao].codTransacao);

    //remove o valor total da conta a ser paga no dinheiro do caixa.
    if (removeDinheiroDoCaixa(transacoes[index].valorTotal, valor_em_caixa) == FALSE)
        return FALSE;
    //Aqui coloca a data atual como data de pagamento
    time_t dataAtual;
    time(&dataAtual); // Pega a data atual
    contas[posicao].dataEfeituacaoPagamento = dataAtual; // define a data que foi pago

    // muda a conta para paga
    contas[posicao].pago = TRUE;

    // Salva as contas no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setContasPagar(contas);
    return TRUE;
}

// Função para gerar um novo ID exclusivo para uma nova conta a pagar
int buscaNovoIDContasPagar(ContasPagar *contas) {
    int maior = 1;

    // Retorna 1 se não houver contas cadastradas
    if (getTamanhoContasPagar() == 0) return maior;

    // Percorre a lista de contas para encontrar o maior ID existente
    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (maior <= contas[i].codigo) {
            maior = contas[i].codigo + 1; // Define o próximo ID como o maior + 1
        }
    }

    return maior; // Retorna o novo ID gerado
}

// verifica se tem contas a pagar
int temContasPagar(ContasPagar *contas) {
    int tamanho = getTamanhoContasPagar();
    for (int i = 0; i < tamanho; i++) {
        if (contas[i].pago == FALSE)
            return TRUE;
    }
    return FALSE;
}

//verifica se tem contas pagas
int temContasPagas(ContasPagar *contas) {
    int tamanho = getTamanhoContasPagar();
    for (int i = 0; i < tamanho; i++) {
        if (contas[i].pago == TRUE)
            return TRUE;
    }
    return FALSE;
}

int saveContasPagarCSV(ContasPagar *contas_pagar, int tamanho) {
    char **string = malloc(sizeof(char) * tamanho);
    for (int i = 0; i < tamanho; i++) {
        string[i] = transformaString(&contas_pagar[i].codigo, 'i');
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas_pagar[i].codTransacao, 'i'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas_pagar[i].dataPagamento, 'd'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas_pagar[i].dataEfeituacaoPagamento, 'd'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas_pagar[i].pago, 'i'));
    }
    escreverCSV(string, "contas_pagar", tamanho);
    return TRUE;
}

ContasPagar *filterContasPagarIntervaloDatas(ContasPagar *contas, time_t dataInicio, time_t dataFim, int *Tamanho) {
    ContasPagar *filtro = malloc(sizeof(ContasPagar) * getTamanhoContasPagar());
    if (!filtro) {
        printf("Erro ao alocar memória para o filtro.\n");
        return NULL;
    }
    int novoTamanho = 0;
    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (contas[i].pago == FALSE) {
            if (comparelimitesDatas(dataInicio, dataFim, contas[i].dataPagamento) == TRUE) {
                filtro[novoTamanho] = contas[i];
                novoTamanho++;
            }
        }
    }
    *Tamanho = novoTamanho;
    return filtro;
}
