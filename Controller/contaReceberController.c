#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "../bibliotecas/caixa.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"

ContasReceber *readContasReceber() {
    return getContasReceber();
}

// Cria um nova conta a receber e adiciona à lista
int createContasReceber(ContasReceber **contas, int codTransacao, time_t dataPagamento, float valor) {
    int tamanhoAtual = getTamanhoContasReceber(); // Obtém o tamanho atual da lista de contas a receber
    // Realoca a memória para incluir a nova conta a pagar
    ContasReceber *novaConta = realloc(*contas, (tamanhoAtual + 1) * sizeof(ContasReceber));
    if (novaConta == NULL) {
        printf("Erro ao alocar memória para Contas a receber.\n");
        return FALSE; // Retorna FALSE indicando falha
    }
    *contas = novaConta;
    int index = tamanhoAtual;
    // Preenche os dados da nova conta a pagar no array
    (*contas)[index].codigo = buscaNovoIDContasReceber(*contas);
    (*contas)[index].codTransacao = codTransacao;
    (*contas)[index].dataPagamento = dataPagamento;
    (*contas)[index].valor = valor;
    (*contas)[index].recebimento = 0; // define que ainda não tem uma data de recebimento, pois não foi pago
    (*contas)[index].pago = FALSE;

    setTamanhoContasReceber(); // Incrementa o tamanho da lista de contas a receber
    // Salva as transações no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setContasReceber(*contas);
    return TRUE;
}

int showContasReceber(ContasReceber *contas, int codigo) {
    if (contas == NULL) return FALSE; // Se o array de contas a pagar for nulo, retorna falso

    int posicao = 0;
    int tamanho = getTamanhoContasReceber(); // Obtém o tamanho do array de contas a receber

    // Procura a conta pelo código dentro do array
    while (posicao < tamanho && contas[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se a conta foi encontrada e se está paga (ativo)
    if (posicao == tamanho)
        return FALSE; // Retorna falso se a conta não foi encontrada ou está marcada como não paga

    return posicao; // Retorna a posição da conta encontrada
}

int deleteContasReceber(ContasReceber *contas, int codigo) {
    int posicao = showContasReceber(contas, codigo); // Localiza a conta pelo código

    if (posicao == FALSE) return FALSE; // Retorna FALSE se a conta não for encontrada

    ContasReceber *removeConta = malloc(sizeof(ContasReceber) * (getTamanhoContasReceber() - 1));
    int novoTamanho = 0;

    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (contas[i].codigo != codigo) {
            removeConta[novoTamanho] = contas[i];
            novoTamanho++;
        }
    }

    contas = removeConta;
    removeTamanhoContasReceber(); // Decrementa o tamanho das contas a pagar

    // Salva as contas no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setContasReceber(contas);

    return TRUE; // Retorna TRUE indicando sucesso
}

int contaRecebida(ContasReceber *contas, int codigo, float *valor_em_caixa, time_t dataPagamento) {
    int posicao = showContasReceber(contas, codigo);
    if (posicao == FALSE) return FALSE;


    //adiciona o valor da parcela paga no dinheiro do caixa.
    addDinheiroAoCaixa(contas[posicao].valor, valor_em_caixa);
    //muda a data que pagou
    contas[posicao].recebimento = dataPagamento;

    // muda a conta para paga
    contas[posicao].pago = TRUE;

    // Salva as contas no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setContasReceber(contas);

    return TRUE;
}

// Função para gerar um novo ID exclusivo para uma nova conta a pagar
int buscaNovoIDContasReceber(ContasReceber *contas) {
    int maior = 1;

    // Retorna 1 se não houver contas cadastradas
    if (getTamanhoContasReceber() == 0) return maior;

    // Percorre a lista de contas para encontrar o maior ID existente
    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (maior <= contas[i].codigo) {
            maior = contas[i].codigo + 1; // Define o próximo ID como o maior + 1
        }
    }

    return maior; // Retorna o novo ID gerado
}

int temContasReceber(ContasReceber *contas) {
    int tamanho = getTamanhoContasReceber();
    for (int i = 0; i < tamanho; i++) {
        if (contas[i].pago == FALSE)
            return TRUE;
    }
    return FALSE;
}

//verifica se tem contas pagas
int temContasRecebidas(ContasReceber *contas) {
    int tamanho = getTamanhoContasReceber();
    for (int i = 0; i < tamanho; i++) {
        if (contas[i].pago == TRUE)
            return TRUE;
    }
    return FALSE;
}
int saveContasReceberCSV(ContasReceber *contas, int tamanho) {
    char **string = malloc(sizeof(char) * tamanho);
    for (int i = 0; i < tamanho; i++) {
        string[i] = transformaString(&contas[i].codigo, 'i');
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas[i].codTransacao, 'i'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas[i].valor, 'f'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas[i].dataPagamento, 'd'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas[i].recebimento, 'd'));
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&contas[i].pago, 'i'));
    }
    escreverCSV(string, "contas_receber", tamanho);
    return TRUE;
}
ContasReceber *filterContasReceberIntervaloDatas(ContasReceber *contas_receber, time_t dataInicio, time_t dataFim,
                                                 int *Tamanho) {
    ContasReceber *filtro = malloc(sizeof(ContasReceber) * getTamanhoContasPagar());
    if (!filtro) {
        printf("Erro ao alocar memória para o filtro.\n");
        return NULL;
    }
    int novoTamanho = 0;
    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (contas_receber[i].pago == FALSE) {
            if (comparelimitesDatas(dataInicio, dataFim, contas_receber[i].dataPagamento) == TRUE) {
                filtro[novoTamanho] = contas_receber[i];
                novoTamanho++;
            }
        }
    }
    *Tamanho = novoTamanho;
    return filtro;
}
