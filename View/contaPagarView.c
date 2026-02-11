#include <stdio.h>
#include <stdlib.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"

void menuContasPagar(ContasPagar **contas, Transacao *transacoes, float *valor_em_caixa) {
    if (getTipoArquivo() != 3) {
        *contas = readContasPagar(); // Se o tipo de armazenamento não for memória, lê as contas do armazenamento
        transacoes = readTransacoes();
    }
    int opcao = -1;
    while (opcao != 0) {
        printf("\t==== MENU CONTAS A PAGAR ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
            "\t\t\t1- VER CONTA A PAGAR\n"
            "\t\t\t2- VER CONTA PAGAS\n"
            "\t\t\t3- EXCLUIR CONTA\n"
            "\t\t\t4- PAGAR CONTA\n"
            "\t\t\t0- SAIR\n"
            "=>");

        switch (opcao) {
            case 1:
                mostrarContaPagar(*contas, transacoes); // Função para exibir uma conta a pagar
                opcao = -1;
                break;
            case 2:
                mostrarContasPaga(*contas, transacoes); // Função para exibir uma conta paga
                opcao = -1;
                break;
            case 3:
                apagarContaPagar(*contas, transacoes); // Função para excluir uma conta
                opcao = -1;
                break;
            case 4:
                pagarConta(*contas, transacoes, valor_em_caixa);
                opcao = -1;
                break;
            default: // Opção inválida
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

// mostra contas a serem pagas
void mostrarContaPagar(ContasPagar *contas, Transacao *transacoes) {
    if (getTamanhoContasPagar() && temContasPagar(contas) == FALSE) {
        printf("Nenhuma conta a pagar registrada no momento");
        return;
    }
    printf("\tCONTAS A PAGAR:\n");
    mostrarTodasContasPagar(contas, transacoes); // Função que exibe todas as contas a pagar
    int codigo = lerInt("Digite o código da conta que você deseja ver: ");
    int posicao = showContasPagar(contas, codigo); // Assume que showTransacao foi implementada
    if (contas[posicao].pago == TRUE || posicao == FALSE) {
        printf("Codigo da conta invalido!\n");
        return;
    }
    int index = showTransacao(transacoes, contas[posicao].codTransacao); // Localiza a transação associada
    printf("Codigo: %d\n"
           "Valor a ser pago: %.2f\n"
           "Transacao associada: %d\n",
           contas[posicao].codigo,
           transacoes[index].valorTotal,
           contas[posicao].codTransacao); // Exibe as informações da conta a pagar
    printf("Data de vencimento: ");
    printData(contas[posicao].dataPagamento);
}

// Mostra contas já pagas
void mostrarContasPaga(ContasPagar *contas, Transacao *transacoes) {
    if (getTamanhoContasPagar() == 0 && temContasPagas(contas) == FALSE) {
        printf("Nenhuma conta paga registrada no momento.\n");
        return;
    }
    printf("\tCONTAS JÁ PAGAS:\n");
    mostrarTodasContasPagas(contas, transacoes); // Função que exibe todas as contas pagas
    int codigo = lerInt("Digite o código da conta que você deseja ver: ");
    int posicao = showContasPagar(contas, codigo);
    if (posicao == FALSE || contas[posicao].pago == FALSE) {
        printf("Código da conta inválido ou conta ainda não foi paga!\n");
        return;
    }
    int index = showTransacao(transacoes, contas[posicao].codTransacao); // Localiza a transação associada
    if (index == FALSE) {
        printf("Transação associada não encontrada.\n");
        return;
    }
    printf("Código da Conta: %d\n"
           "Valor Pago: %.2f\n"
           "Transação Associada: %d\n",
           contas[posicao].codigo,
           transacoes[index].valorTotal,
           contas[posicao].codTransacao); // Exibe as informações da conta paga
    printf("Data de vencimento: ");
    printData(contas[posicao].dataPagamento);
    printf("Data de pagamento: ");
    printData(contas[posicao].dataEfeituacaoPagamento);
}

//mostra todas as contas que devem ser pagas
void mostrarTodasContasPagar(ContasPagar *contas, Transacao *transacoes) {
    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (contas[i].pago == FALSE) {
            int posicao = showTransacao(transacoes, contas[i].codTransacao);
            printf("Valor a pagar: %.2f,"
                   "Codigo: %d\n",
                   transacoes[posicao].valorTotal,
                   contas[i].codigo);
            printf("Data de vencimento: ");
            printData(contas[i].dataPagamento);
        }
    }
}

//mostra todas as contas já pagas
void mostrarTodasContasPagas(ContasPagar *contas, Transacao *transacoes) {
    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (contas[i].pago == TRUE) {
            int posicao = showTransacao(transacoes, contas[i].codTransacao);
            printf("Valor pago: %.2f,"
                   "Codigo: %d\n",
                   transacoes[posicao].valorTotal,
                   contas[i].codigo);
            printf("Data de vencimento: ");
            printData(contas[i].dataPagamento);
            printf("Data de pagamento: ");
            printData(contas[i].dataEfeituacaoPagamento);
        }
    }
}

//Função para apagar uma conta a pagar
void apagarContaPagar(ContasPagar *contas, Transacao *transacoes) {
    if (getTamanhoContasPagar() == 0) {
        printf("Nenhuma conta a pagar cadastrada\n");
        return;
    }
    printf("\tCONTAS A PAGAR:\n");
    mostrarTodasContas(contas, transacoes); // Exibe todas as contas a pagar
    int codigo = lerInt("Digite o código da conta a pagar que você deseja apagar: ");
    if (deleteContasPagar(contas, codigo) == TRUE) {
        printf("Conta a pagar apagada com sucesso\n");
    } else {
        printf("Conta a pagar não existe\n");
    }
}

//mostra todas as contas, independente se já estão pagas ou não
void mostrarTodasContas(ContasPagar *contas, Transacao *transacoes) {
    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        int posicao = showTransacao(transacoes, contas[i].codTransacao);
        printf("Valor pago: %.2f, "
               "Codigo: %d, ",
               transacoes[posicao].valorTotal,
               contas[i].codigo);
        if (contas[i].pago == TRUE) printf("Paga: Sim\n");
        else printf("Paga: Não\n");
    }
}

//Função para pagar uma conta
void pagarConta(ContasPagar *contas, Transacao *transacoes, float *valor_em_caixa) {
    mostrarTodasContasPagar(contas, transacoes);
    int codigoConta = lerInt("Digite o codigo da conta que você deseja pagar: ");
    int posicao = showContasPagar(contas, codigoConta);
    if (posicao != FALSE) {
        if (contas[posicao].pago == FALSE) {
            if (contaPaga(contas, codigoConta, transacoes, valor_em_caixa) == TRUE) {
                printf("Valor pago e debitado no caixa da empresa!\n");
            } else {
                printf("ATENÇÃO!!!\n");
                printf("DINHEIRO EM CAIXA INSUFICIENTE!\n");
                printf("Dinheiro em caixa: %.2f\n", *valor_em_caixa);
            }
        } else printf("A conta já está paga!\n");
    } else printf("Codigo da conta invalido!\n");
}
// Função que exibe as informações de todas as contas a pagar
void printContasPagar(ContasPagar * contas_pagar, int tam) {
    printf("==== CONTAS A PAGAR ====\n");
    for(int i = 0; i < tam; i++) {
        printf("Código: %d\n"
               "Código da Transação: %d\n",
               contas_pagar[i].codigo,
               contas_pagar[i].codTransacao); // Exibe as informações da conta a pagar
        printf("Data de Pagamento: ");
        printData(contas_pagar[i].dataPagamento);
        if(contas_pagar[i].dataEfeituacaoPagamento != 0) {
            printf("Data de Efetuação do Pagamento: ");
            printData(contas_pagar[i].dataEfeituacaoPagamento);
        }
    }
}

void filtrarContasPagar(ContasPagar * contas_pagar) {
    printf("\t==== FILTRO CONTAS A PAGAR ====\n");
    int opcao = lerInt("Você seja filtrar por Intervalo de Datas, Codigo do Cliente ou Codigo do Fornecedor?\n"
        "1 - Intervalo de datas\n"
        "2 - Codigo do Cliente\n"
        "3 - Codigo do Fornecedor\n"
        "=>");
    if (opcao == 1) {
        int diaInicio = lerInt("Digite o dia de início: ");
        int mesInicio = lerInt("Digite o mês de início: ");
        int anoInicio = lerInt("Digite o ano de início: ");
        time_t dataInicio = converteData(diaInicio, mesInicio, anoInicio);
        int diaFim = lerInt("Digite o dia de fim: ");
        int mesFim = lerInt("Digite o mês de fim: ");
        int anoFim = lerInt("Digite o ano de fim: ");
        time_t dataFim = converteData(diaFim, mesFim, anoFim);
        int TamanhoCP = 0;
        void *filtroCP = filterContasPagarIntervaloDatas(contas_pagar, dataInicio, dataFim, &TamanhoCP);

        printContasPagar(filtroCP, TamanhoCP);
    }
}
