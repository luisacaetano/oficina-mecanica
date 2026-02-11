#include <stdio.h>
#include <stdlib.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"

void menuContasReceber(ContasReceber **contas, float *valor_em_caixa) {
    if (getTipoArquivo() != 3) {
        *contas = readContasReceber(); // Se o tipo de armazenamento não for memória, lê as contas do armazenamento
    }
    int opcao = -1;
    while (opcao != 0) {
        printf("\t==== MENU CONTAS A RECEBER ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
            "\t\t\t1- VER CONTA A RECEBER\n"
            "\t\t\t2- VER CONTAS RECEBIDAS\n"
            "\t\t\t3- EXCLUIR CONTA\n"
            "\t\t\t4- RECEBER CONTA\n"
            "\t\t\t0- SAIR\n"
            "=>");

        switch (opcao) {
            case 1:
                mostrarContasReceber(*contas); // Função para exibir uma conta a receber
                opcao = -1;
                break;
            case 2:
                mostrarContasRecebidas(*contas); // Função para exibir contas recebidas
                opcao = -1;
                break;
            case 3:
                apagarContaReceber(*contas); // Função para excluir uma conta
                opcao = -1;
                break;
            case 4:
                receberConta(*contas, valor_em_caixa); // Função para registrar o recebimento de uma conta
                opcao = -1;
                break;
            default: // Opção inválida
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

// Função para registrar o recebimento de uma conta
void receberConta(ContasReceber *contas, float *valor_em_caixa) {
    mostrarTodasContasReceber(contas);
    int codigoConta = lerInt("Digite o codigo da conta que você deseja pagar: ");
    int posicao = showContasReceber(contas, codigoConta);
    if (posicao != FALSE) {
        if (contas[posicao].pago == FALSE) {
            int dia = lerInt("Digite o dia foi pago: ");
            int mes = lerInt("Digite o mes foi pago: ");
            int ano = lerInt("Digite o ano foi pago: ");
            time_t dataPagamento = converteData(dia, mes, ano);
            if (contaRecebida(contas, codigoConta, valor_em_caixa,dataPagamento) == TRUE) {
                printf("Valor recebido e creditado no caixa da empresa!\n");
            }
        } else printf("A conta já está paga!\n");
    } else printf("Codigo da conta invalido!\n");
}
// Função para exibir todas as contas a receber
void mostrarTodasContasReceber(ContasReceber *contas) {
    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (contas[i].pago == FALSE) {
            printf("Valor a ser recebido: %.2f,"
                   "Codigo: %d, ",
                   contas[i].valor,
                   contas[i].codigo);
            printf("Data de vencimento: ");
            printData(contas[i].dataPagamento);
        }
    }
}
// Função para exibir conta a recebidas
void mostrarContasReceber(ContasReceber *contas) {
    if (getTamanhoContasReceber() && temContasReceber(contas) == FALSE) {
        printf("Nenhuma conta a receber registrada no momento");
        return;
    }
    printf("\tCONTAS A RECEBER:\n");
    mostrarTodasContasReceber(contas); // Função que exibe todas as contas a receber
    int codigo = lerInt("Digite o código da conta que você deseja ver: ");
    int posicao = showContasReceber(contas, codigo); // Assume que showTransacao foi implementada
    if (contas[posicao].pago == TRUE || posicao == FALSE) {
        printf("Codigo da conta invalido!\n");
        return;
    }
    printf("Codigo: %d\n"
           "Valor a ser recebido: %.2f\n"
           "Transacao associada: %d\n",
           contas[posicao].codigo,
           contas[posicao].valor,
           contas[posicao].codTransacao); // Exibe as informações da conta a receber
    printf("Data de vencimento: ");
    printData(contas[posicao].dataPagamento);
}
// Função para exibir conta recebidas
void mostrarContasRecebidas(ContasReceber *contas) {
    if (getTamanhoContasReceber() == 0 && temContasRecebidas(contas) == FALSE) {
        printf("Nenhuma conta recebida registrada no momento.\n");
        return;
    }
    printf("\tCONTAS JÁ RECEBIDAS:\n");
    mostrarTodasContasRecebidas(contas); // Função que exibe todas as contas recebidas
    int codigo = lerInt("Digite o código da conta que você deseja ver: ");
    int posicao = showContasReceber(contas, codigo);
    if (posicao == FALSE || contas[posicao].pago == FALSE) {
        printf("Código da conta inválido ou conta ainda não foi recebida!\n");
        return;
    }
    printf("Código da Conta: %d\n"
           "Valor Recebido: %.2f\n"
           "Transação Associada: %d\n",
           contas[posicao].codigo,
           contas[posicao].valor,
           contas[posicao].codTransacao); // Exibe as informações da conta recebida
    printf("Data de vencimento: ");
    printData(contas[posicao].dataPagamento);
    printf("Data de recebimento: ");
    printData(contas[posicao].recebimento);
}
// Função para exibir todas as contas recebidas
void mostrarTodasContasRecebidas(ContasReceber *contas) {
    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (contas[i].pago == TRUE) {
            printf("Valor recebido: %.2f,"
                   "Codigo: %d, ",
                   contas[i].valor,
                   contas[i].codigo);
            printf("Data de vencimento: ");
            printData(contas[i].dataPagamento);
            printf("Data de recebimento: ");
            printData(contas[i].recebimento);
        }
    }
}
// Função para excluir uma conta a receber
void apagarContaReceber(ContasReceber *contas) {
    mostrarTodasContasReceber(contas);
    int codigo = lerInt("Digite o código da conta que você deseja excluir: ");
    int posicao = showContasReceber(contas, codigo);
    if (posicao != FALSE) {
        if (deleteContasReceber(contas, codigo) == TRUE) {
            printf("Conta excluída com sucesso!\n");
        }
    } else printf("Código da conta inválido!\n");
}
// Função para exibir todas as contas a receber
void mostrarTodasContaReceber(ContasReceber *contas) {
    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        printf("Valor pago: %.2f, "
               "Codigo: %d, ",
              contas[i].valor,
               contas[i].codigo);
        if (contas[i].pago == TRUE) printf("Recebida: Sim\n");
        else printf("Paga: Não\n");
        printf("Data de vencimento: ");
        printData(contas[i].dataPagamento);
        if(contas[i].recebimento == 0) printf("Data de recebimento: Ainda não foi paga\n");
        else {
            printf("Data de recebimento: ");
            printData(contas[i].recebimento);
        }
    }

}
 void printContasReceber(ContasReceber * contas_receber, int tam) {
    printf("==== CONTAS A RECEBER ====\n");
    for(int i = 0; i < tam; i++) {
        printf("Código: %d\n"
               "Código da Transação: %d\n"
               "Valor: %.2f\n",
               contas_receber[i].codigo,
               contas_receber[i].codTransacao,
               contas_receber[i].valor);
        printf("Data de Pagamento: ");
        printData(contas_receber[i].dataPagamento);
        if(contas_receber[i].recebimento != 0) {
            printf("Data de Efeituacao do Pagamento: ");
            printData(contas_receber[i].recebimento);
        }
    }
}

void filtrarContasReceber(ContasReceber * contas_receber) {
    printf("\t==== FILTRO CONTAS A RECEBER ====\n");
    int opcao = lerInt("Você seja filtrar por Intervalo de Datas, Codigo do Cliente ou Codigo do Fornecedor?\n"
        "1 - Intervalo de datas\n"
        "2 - Codigo do Cliente\n"
        "3 - Codigo do Fornecedor\n"
        "=>");
    if (opcao== 1) {
        int diaInicio = lerInt("Digite o dia de início: ");
        int mesInicio = lerInt("Digite o mês de início: ");
        int anoInicio = lerInt("Digite o ano de início: ");
        time_t dataInicio = converteData(diaInicio, mesInicio, anoInicio);
        int diaFim = lerInt("Digite o dia de fim: ");
        int mesFim = lerInt("Digite o mês de fim: ");
        int anoFim = lerInt("Digite o ano de fim: ");
        time_t dataFim = converteData(diaFim, mesFim, anoFim);
        int TamanhoCR = 0;
        void *filtroCR = filterContasReceberIntervaloDatas(contas_receber, dataInicio, dataFim, &TamanhoCR);

        printContasPagar(filtroCR, TamanhoCR);
    }
}