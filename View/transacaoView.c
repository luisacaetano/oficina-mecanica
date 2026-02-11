#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../bibliotecas/caixa.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"
#include "../bibliotecas/modulo2.h"
void menuTransacoes(Transacao **transacoes) {
    if (getTipoArquivo() != 3) {
        *transacoes = readTransacoes(); // Se o tipo de armazenamento não for memória, lê as transações do armazenamento
    }
    int opcao = -1;
    while (opcao != 0) {
        printf("\t==== MENU TRANSAÇÃO ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
            "\t\t\t1- VER TRANSAÇÃO\n"
            "\t\t\t0- SAIR\n"
            "=>");

        switch (opcao) {
            case 1:
                mostrarTransacao(*transacoes); // Função para exibir uma transação
                opcao = -1;
                break;
            case 4:
                apagarTransacao(*transacoes); // Função para excluir uma transação
                opcao = -1;
                break;
            default: // Opção inválida
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

int novaTransacao(Transacao **transacoes, int tipoTransacao, float valor_Total, ContasPagar **contas_pagar,
                  ContasReceber **contas_receber, float *valor_em_caixa) {
    Transacao *transacao = malloc(sizeof(Transacao));

     transacao->valorTotal = valor_Total;
    transacao->formaPagamento = lerInt("Qual será a forma de pagamento:\n"
        "0 - A VISTA\n"
        "1 - CARTÃO/PARCELADO\n"
        "=>");
    int parcelas = 0;
    transacao->tipoTransacao = tipoTransacao;
    if (tipoTransacao == VENDA) {
        // prestando serviço
        if (transacao->formaPagamento == CARTAO) {
            // prestação de serviço a prazo
            parcelas = lerInt("Em quantas parcelas deseja dividir: ");
            int result =createTransacao(transacoes, transacao, valor_em_caixa, contas_pagar, contas_receber, parcelas);
            if (result !=FALSE) {
                printf("Transação de venda criada com sucesso\n");
                return result;
            }else return FALSE;
        } else {
            // se a prestação de serviço for a vista
            if (addDinheiroAoCaixa(transacao->valorTotal, valor_em_caixa) == TRUE) {
                int result =createTransacao(transacoes, transacao, valor_em_caixa, contas_pagar, contas_receber, 0);
                if(result != FALSE) {
                    printf("Transação de compra criada com sucesso\n");
                }else return FALSE;
                printf("Dinheiro adicionado ao caixa\n");
                return result;
            }
        }
    }
    //Compra de peças
    if (tipoTransacao == COMPRA) {
        if (transacao->formaPagamento == CARTAO) {
            // compra a prazo
            int result =createTransacao(transacoes, transacao, valor_em_caixa, contas_pagar, contas_receber, 0);
            if ( result!=FALSE) {
                printf("Transação de compra criada com sucesso\n");
                return result;
            }
        } else {
            // se a compra for a vista
            if (removeDinheiroDoCaixa(transacao->valorTotal, valor_em_caixa) == FALSE) {
                printf("ATENÇÃO!!!\n");
                printf("DINHEIRO EM CAIXA INSUFICIENTE!\n");
                printf("Dinheiro em caixa: %.2f\n", *valor_em_caixa);
                return FALSE;
            } else {
                 int result = createTransacao(transacoes, transacao, valor_em_caixa, contas_pagar, contas_receber, 0);
                if(result != FALSE) {
                    printf("Transação de compra criada com sucesso\n");
                    printf("Dinheiro removido ao caixa\n");
                    return result;
                } else return FALSE;
            }
        }
    }
    return FALSE;
}

void mostrarTransacao(Transacao *transacoes) {
    if (getTamanhoTransacoes() == 0) {
        printf("Nenhuma transação cadastrada até o momento\n");
        return;
    }

    printf("\tTRANSAÇÕES:\n");
    mostrarTodasTransacoes(transacoes);

    int codigo = lerInt("Digite o código da transação que você deseja ver: ");
    int posicao = showTransacao(transacoes, codigo);

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Valor Total: %.2f\n",
               transacoes[posicao].codigo,
               transacoes[posicao].valorTotal
        ); // Exibe as informações da transação
        printf("Data da Transação: ");
        printData(transacoes[posicao].dataTransacao);
        if (transacoes[posicao].formaPagamento == AVISTA) printf("Forma de Pagamento: A Vista \n");
        else printf("Forma de Pagamento: No Cartão \n");
    } else {
        printf("Transação não encontrada!\n");
    }
}

void apagarTransacao(Transacao *transacoes) {
    if (getTamanhoTransacoes() == 0) {
        printf("Nenhuma transação cadastrada\n");
        return;
    }
    printf("\tTRANSAÇÕES:\n");
    mostrarTodasTransacoes(transacoes);
    int codigo = lerInt("Digite o código da transação que você deseja apagar: ");
    if (deleteTransacao(transacoes, codigo) == TRUE) {
        printf("Transação apagada com sucesso\n");
    } else {
        printf("Transação não existe\n");
    }
}

// Função para exibir todas as transações
void mostrarTodasTransacoes(Transacao *transacoes) {
    for (int i = 0; i < getTamanhoTransacoes(); i++) {
        if (transacoes[i].tipoTransacao == COMPRA) printf("Transação: Compra, ");
        else printf("Transação: Venda, ");
        printf("Codigo: %d\n", transacoes[i].codigo);
        printf("Data da Transação: ");
        printData(transacoes[i].dataTransacao);
    }
}
void printTransacoes(Transacao  * transacoes, int tam) {
    printf("==== TRANSAÇÕES ====\n");
    for(int i = 0; i < tam; i++) {
        printf("Código: %d\n"
               "Valor Total: %.2f\n",
               transacoes[i].codigo,
               transacoes[i].valorTotal
        ); // Exibe as informações da transação
        printf("Data da Transação: ");
        printData(transacoes[i].dataTransacao);
        if (transacoes[i].formaPagamento == AVISTA) printf("Forma de Pagamento: A Vista \n");
        else printf("Forma de Pagamento: No Cartão \n");
    }

}
void filtrarFinanceiro(Transacao *transacoes, ContasPagar *contas_pagar, ContasReceber *contas_receber) {
    printf("\t==== FILTRO FINCEIRO ====\n");
    int opcao = lerInt("Você seja filtrar por Intervalo de Datas, Codigo do Cliente ou Codigo do Fornecedor?\n"
        "1 - Intervalo de datas\n"
        "2 - Codigo do Cliente\n"
        "3 - Codigo do Fornecedor\n"
        "=>");
    if (opcao == 1) {
        int diaI = lerInt("Digite o dia de inicio: ");
        int mesI = lerInt("Digite o mes de inicio: ");
        int anoI = lerInt("Digite o ano de inicio: ");
        int diaF = lerInt("Digite o dia de fim: ");
        int mesF = lerInt("Digite o mes de fim: ");
        int anoF = lerInt("Digite o ano de fim: ");
        time_t dataInicio = converteData(diaI, mesI, anoI);
        time_t dataFim = converteData(diaF, mesF, anoF);
        int TamanhoT = 0, TamanhoCP = 0, TamanhoCR = 0;
        void **filtro = filterIntervaloDatas(transacoes, contas_pagar, contas_receber, dataInicio, dataFim, &TamanhoT,
                                             &TamanhoCP, &TamanhoCR);

        Transacao *filtroTransacao = malloc(sizeof(Transacao) * TamanhoT);
        ContasPagar *filtroCP = malloc(sizeof(ContasPagar) * TamanhoCP);
        ContasReceber *filtroCR = malloc(sizeof(ContasReceber) * TamanhoCR);
        if (!filtroTransacao || !filtroCP || !filtroCR) {
            printf("Erro ao alocar memória para os filtros.\n");
            free(filtroTransacao);
            free(filtroCP);
            free(filtroCR);
            free(filtro);
            return;
        }

        for (int i = 0; i < TamanhoT; i++) {
            filtroTransacao[i] = *(Transacao *) filtro[i];
        }
        for (int i = 0; i < TamanhoCP; i++) {
            filtroCP[i] = *(ContasPagar *) filtro[TamanhoT + i];
        }
        for (int i = 0; i < TamanhoCR; i++) {
            filtroCR[i] = *(ContasReceber *) filtro[TamanhoT + TamanhoCP + i];
        }
        printTransacoes(filtroTransacao, TamanhoT);
        printContasPagar(filtroCP, TamanhoCP);
        printContasReceber(filtroCR, TamanhoCR);
        printf("Aqui chegou\n");
        int op = lerInt("Você gostaria de salvar no arquivo CSV?\n"
            "1 - Sim\n"
            "0 - Não\n"
            "=>");
        if(op ==1)
        saveFinanceiroCSV(filtro,TamanhoT, TamanhoCP , TamanhoCR);
        // Liberando memória do filtro original
        free(filtro);
    }
}
