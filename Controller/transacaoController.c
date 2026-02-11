#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../bibliotecas/caixa.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"

// Função para ler todas as transações cadastradas
Transacao *readTransacoes() {
    return getTransacoes(); // Retorna o ponteiro para a lista de transações
}

// Função para criar uma nova transação
int createTransacao(Transacao **transacoes, Transacao *transacao, float *valor_em_caixa, ContasPagar **contas_pagar,
                    ContasReceber **contas_receber, int qtd_parcelas) {
    int tamanhoAtual = getTamanhoTransacoes(); // Obtém o tamanho atual da lista de transações

    // Realoca a memória para incluir a nova transação
    Transacao *novoTransacoes = realloc(*transacoes, (tamanhoAtual + 1) * sizeof(Transacao));
    if (novoTransacoes == NULL) {
        printf("Erro ao alocar memória para Transações.\n");
        return FALSE; // Retorna FALSE indicando falha
    }

    *transacoes = novoTransacoes; // Atualiza o ponteiro da lista de transações
    int index = tamanhoAtual; // Define o índice da nova transação como o último da lista

    // Preenche os dados da nova transação no array
    (*transacoes)[index].codigo = buscaNovoIDTransacao(*transacoes); // Gera um novo ID único
    (*transacoes)[index].formaPagamento = transacao->formaPagamento; // Copia a forma de pagamento
    (*transacoes)[index].valorTotal = transacao->valorTotal; // Copia o valor total
    (*transacoes)[index].tipoTransacao = transacao->tipoTransacao;
    time_t dataAtual;
    time(&dataAtual);
    (*transacoes)[index].dataTransacao = dataAtual;
    // Copia o tipo de transação// Marca a transação como ativa

    setTamanhoTransacoes(); // Incrementa o tamanho da lista de transações

    // Salva as transações no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setTransacoes(*transacoes);

    //Se a transao for uma compra
    if (transacao->tipoTransacao == COMPRA) {
        if (transacao->formaPagamento == AVISTA) {
            if(removeDinheiroDoCaixa(transacao->valorTotal, valor_em_caixa) == FALSE) return FALSE;
            else return (*transacoes)[index].codigo;
        } else {
            time_t dataAtualC;
            time(&dataAtualC); // Pega a data atual
            dataAtualC = addtrintaDias(dataAtualC); // aumenta na data atual 30 dias
            createContasPagar(contas_pagar, (*transacoes)[index].codigo, dataAtualC); // cria uma conta a pagar
        }
    }
    //Se a transao for uma venda
    if (transacao->tipoTransacao == VENDA) {
        if (transacao->formaPagamento == AVISTA) {
            if(addDinheiroAoCaixa(transacao->valorTotal, valor_em_caixa) == FALSE) return FALSE;
            else return (*transacoes)[index].codigo;
        } else {
            //divide o valor total em parcelas
            float valorDeCadaParcela = transacao->valorTotal / qtd_parcelas;
            time_t dataAtualV;
            time(&dataAtualV); // Pega a data atual
            for (int i = 0; i < qtd_parcelas; i++) {
                dataAtualV = addtrintaDias(dataAtualV); // aumenta na data atual 30 dias
                createContasReceber(contas_receber, (*transacoes)[index].codigo, dataAtualV, valorDeCadaParcela);
            }
        }
    }

    return (*transacoes)[index].codigo; // Retorna codigo da transacao indicando sucesso
}

// Função para localizar uma transação pelo código
int showTransacao(Transacao *transacoes, int codigo) {
    if (transacoes == NULL) return FALSE; // Retorna FALSE se a lista de transações for nula

    int posicao = 0;
    int tamanho = getTamanhoTransacoes(); // Obtém o tamanho atual da lista de transações

    // Procura a transação com o código especificado
    while (posicao < tamanho && transacoes[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se a transação foi encontrada e se está ativa
    if (posicao == tamanho)
        return FALSE;

    return posicao; // Retorna a posição da transação encontrada
}

int deleteTransacao(Transacao *transacoes, int codigo) {
    int posicao = showTransacao(transacoes, codigo); // Localiza a transação pelo código

    if (posicao == FALSE) return FALSE; // Retorna FALSE se a transação não for encontrada

    Transacao *removeTransacao = malloc(sizeof(Transacao) * (getTamanhoTransacoes() - 1));
    int novoTamanho = 0;
    for (int i = 0; i < getTamanhoTransacoes(); i++) {
        if (transacoes[i].codigo != codigo) {
            removeTransacao[novoTamanho] = transacoes[i];
            novoTamanho++;
        }
    }
    transacoes = removeTransacao;
    removeTamanhoTransacoes();
    // Salva as transações no arquivo se o tipo de armazenamento não for memória
    if (getTipoArquivo() != MEM) setTransacoes(transacoes);

    return TRUE; // Retorna TRUE indicando sucesso
}

// Função para gerar um novo ID exclusivo para uma nova transação
int buscaNovoIDTransacao(Transacao *transacoes) {
    int maior = 1;

    // Retorna 1 se não houver transações cadastradas
    if (getTamanhoTransacoes() == 0) return maior;

    // Percorre a lista de transações para encontrar o maior ID existente
    for (int i = 0; i < getTamanhoTransacoes(); i++) {
        if (maior <= transacoes[i].codigo) {
            maior = transacoes[i].codigo + 1; // Define o próximo ID como o maior + 1
        }
    }

    return maior; // Retorna o novo ID gerado
}

/*int saveFinanceiroCSV(void **filtro, int tamT, int tamCP, int CR) {
    char **string = malloc(sizeof(char) * (tamT + tamCP + CR));
    int tamanhoTotal = tamT + tamCP + CR;
    if (tamT > 0) {
        tamanhoTotal += 1;
        Transacao *transacoes = malloc(sizeof(Transacao) * tamT + 1);
        string = realloc(string, sizeof(char) * (tamanhoTotal));
        string[0] = "codigo;formaPagamento;valorTotal;tipoTransacao;dataTransacao";
        for (int i = 1; i < tamT+1; i++) {
            transacoes[i-1] = *(Transacao *) filtro[i];
            string[i] = transformaString(&transacoes[i-1].codigo, 'i');
            string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&transacoes[i-1].formaPagamento, 'i'));
            string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&transacoes[i-1].valorTotal, 'f'));
            string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&transacoes[i-1].tipoTransacao, 'i'));
            string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&transacoes[i-1].dataTransacao, 'd'));
        }
    }
    if(tamCP > 0){
        tamanhoTotal += 1;
        ContasPagar *contas_pagar = malloc(sizeof(ContasPagar) * tamCP+1);
        string = realloc(string, sizeof(char) * (tamanhoTotal));
        string[tamT+1] = "codigo;codTransacao;dataPagamento;dataEfeituacaoPagamento;pago";
    for (int i = 1; i < tamCP+1; i++) {
        contas_pagar[i] = *(ContasPagar *) filtro[i + tamT];
        string[i + tamT+1] = transformaString(&contas_pagar[i-1].codigo, 'i');
        string[i + tamT+1] = concatenarStringPontoEVirgula(string[i + tamT+1],transformaString(&contas_pagar[i-1].codTransacao, 'i'));
        string[i + tamT+1] = concatenarStringPontoEVirgula(string[i + tamT+1],transformaString(&contas_pagar[i-1].dataPagamento, 'd'));
        string[i + tamT+1] = concatenarStringPontoEVirgula(string[i + tamT+1],transformaString(&contas_pagar[i-1].dataEfeituacaoPagamento, 'd'));
        string[i + tamT+1] =concatenarStringPontoEVirgula(string[i + tamT+1], transformaString(&contas_pagar[i-1].pago, 'i'));
    }
}
    if(CR > 0) {
        tamanhoTotal += 1;
        string[tamT + tamCP+1] = "codigo;codTransacao;dataPagamento;valor;recebimento;pago";
        ContasReceber *contas_receber = malloc(sizeof(ContasReceber) * CR+1);
        for (int i = 1; i < CR+1; i++) {
            contas_receber[i] = *(ContasReceber *) filtro[i + tamT + tamCP];
            string[i + tamT + tamCP] = transformaString(&contas_receber[i].codigo, 'i');
            string[i + tamT + tamCP] = concatenarStringPontoEVirgula(string[i + tamT + tamCP],
                                                                     transformaString(
                                                                         &contas_receber[i].codTransacao, 'i'));
            string[i + tamT + tamCP] = concatenarStringPontoEVirgula(string[i + tamT + tamCP],
                                                                     transformaString(
                                                                         &contas_receber[i].dataPagamento, 'd'));
            string[i + tamT + tamCP] = concatenarStringPontoEVirgula(string[i + tamT + tamCP],
                                                                     transformaString(&contas_receber[i].valor, 'f'));
            string[i + tamT + tamCP] = concatenarStringPontoEVirgula(string[i + tamT + tamCP],
                                                                     transformaString(&contas_receber[i].recebimento, 'd'));
            string[i + tamT + tamCP] = concatenarStringPontoEVirgula(string[i + tamT + tamCP],
                                                                     transformaString(&contas_receber[i].pago, 'i'));
        }
    }
    escreverCSV(string, "financeiro", tamT + tamCP + CR);
    return TRUE;
}
*/
int saveFinanceiroCSV(void **filtro, int tamT, int tamCP, int tamCR) {
    int tamanhoTotal = tamT + tamCP + tamCR + 3; // +3 para os cabeçalhos
    char **string = malloc(sizeof(char *) * tamanhoTotal);
    int indice = 0;

    // Inserção das Transações
    if (tamT > 0) {
        string[indice++] = "codigo;formaPagamento;valorTotal;tipoTransacao;dataTransacao";
        Transacao *transacoes = (Transacao *)filtro[0]; // Supondo que estejam no início
        for (int i = 0; i < tamT; i++) {
            string[indice] = transformaString(&transacoes[i].codigo, 'i');
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&transacoes[i].formaPagamento, 'i'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&transacoes[i].valorTotal, 'f'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&transacoes[i].tipoTransacao, 'i'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&transacoes[i].dataTransacao, 'd'));
            indice++;
        }
    }

    // Inserção das Contas a Pagar
    if (tamCP > 0) {
        string[indice++] = "codigo;codTransacao;dataPagamento;dataEfeituacaoPagamento;pago";
        ContasPagar *contas_pagar = (ContasPagar *)filtro[tamT]; // Após as transações
        for (int i = 0; i < tamCP; i++) {
            string[indice] = transformaString(&contas_pagar[i].codigo, 'i');
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_pagar[i].codTransacao, 'i'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_pagar[i].dataPagamento, 'd'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_pagar[i].dataEfeituacaoPagamento, 'd'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_pagar[i].pago, 'i'));
            indice++;
        }
    }

    // Inserção das Contas a Receber
    if (tamCR > 0) {
        string[indice++] = "codigo;codTransacao;dataPagamento;valor;recebimento;pago";
        ContasReceber *contas_receber = (ContasReceber *)filtro[tamT + tamCP]; // Após as contas a pagar
        for (int i = 0; i < tamCR; i++) {
            string[indice] = transformaString(&contas_receber[i].codigo, 'i');
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_receber[i].codTransacao, 'i'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_receber[i].dataPagamento, 'd'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_receber[i].valor, 'f'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_receber[i].recebimento, 'd'));
            string[indice] = concatenarStringPontoEVirgula(string[indice], transformaString(&contas_receber[i].pago, 'i'));
            indice++;
        }
    }

    escreverCSV(string, "financeiro", indice);
    free(string);

    return TRUE;
}


void **filterIntervaloDatas(Transacao *transacoes, ContasPagar *contas_pagar, ContasReceber *contas_receber,
                            time_t dataI, time_t dataF, int *TamanhoT, int *TamanhoCP, int *TamanhoCR) {
    void **filtroFinanceiro = NULL; // Ponteiro para armazenar os resultados
    int tamanhoFiltro = 0;

    for (int i = 0; i < getTamanhoTransacoes(); i++) {
        if (transacoes[i].formaPagamento == AVISTA) {
            if (comparelimitesDatas(dataI, dataF, transacoes[i].dataTransacao) == TRUE) {
                void **temp = realloc(filtroFinanceiro, sizeof(void *) * (tamanhoFiltro + 1));
                if (temp == NULL) {
                    // Em caso de erro na alocação, liberar memória e retornar NULL
                    free(filtroFinanceiro);
                    return NULL;
                }
                filtroFinanceiro = temp;
                filtroFinanceiro[tamanhoFiltro] = &transacoes[i];
                tamanhoFiltro++;
                *(TamanhoT) += 1;
            }
        }
    }


    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (contas_pagar[i].pago == TRUE) {
            if (comparelimitesDatas(dataI, dataF, contas_pagar[i].dataEfeituacaoPagamento) == TRUE) {
                void **temp = realloc(filtroFinanceiro, sizeof(void *) * (tamanhoFiltro + 1));
                if (temp == NULL) {
                    free(filtroFinanceiro);
                    return NULL;
                }
                filtroFinanceiro = temp;
                filtroFinanceiro[tamanhoFiltro] = &contas_pagar[i];
                tamanhoFiltro++;
                *(TamanhoCP) += 1;
            }
        }
    }

    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (contas_receber[i].pago == TRUE) {
            if (comparelimitesDatas(dataI, dataF, contas_receber[i].recebimento) == TRUE) {
                void **temp = realloc(filtroFinanceiro, sizeof(void *) * (tamanhoFiltro + 1));
                if (temp == NULL) {
                    free(filtroFinanceiro);
                    return NULL;
                }
                filtroFinanceiro = temp;
                filtroFinanceiro[tamanhoFiltro] = &contas_receber[i];
                tamanhoFiltro++;
                *(TamanhoCR) += 1;
            }
        }
    }

    return filtroFinanceiro;
}
