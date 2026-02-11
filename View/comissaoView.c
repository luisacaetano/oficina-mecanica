#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"


void menuComissoes(Comissao **comissoes,float * valor_em_caixa) {
    if (getTipoArquivo() != 3) {
        *comissoes = readComisoes();
    }
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=======================================\n");
        printf("          GERENCIAMENTO DE COMISSÕES   \n");
        printf("=======================================\n\n");

        opcao = lerInt("Escolha uma opção:\n"
            "  1 - Visualizar comissão\n"
            "  2 - Pagar comissão para Funcionario\n"
            "  0 - Sair\n"
            "Digite sua opção: ");

        switch (opcao) {
            case 1:
                mostrarComissao(*comissoes);
                opcao = -1;
                break;
            case 2:
                pagarComissao(comissoes,valor_em_caixa);
                opcao = -1;
                break;
            case 0:
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

void mostrarComissao(Comissao *comissoes) {
    if (getTamanhoComissoes() == 0) {
        printf("Nenhuma comissão cadastrada até o momento\n");
        return;
    }
    printf("\tCOMISSÕES:\n");
    mostrarTodasComissoes(comissoes);

    int codigo = lerInt("Digite o código da comissão que você deseja ver: ");
    int posicao = showComissao(comissoes, codigo);

    if (posicao != FALSE) {
        printf("Código da Comissão: %d\n"
               "Código do Funcionário: %d\n"
               "Código da Ordem de Serviço: %d\n"
               "Valor da Comissão: %.2f\n"
               "Tempo gasto no serviço: %.2f\n",
               comissoes[posicao].codigo,
               comissoes[posicao].codigoFuncionario,
               comissoes[posicao].CodigoOrdem,
               comissoes[posicao].valorComisao,
               comissoes[posicao].tempoGasto
        );
    } else {
        printf("Comissão não encontrada!\n");
    }
}

void mostrarTodasComissoes(Comissao *comissoes) {
    for (int i = 0; i < getTamanhoComissoes(); i++) {
        printf("Código: %d Funcionario: %d\n", comissoes[i].codigo, comissoes[i].codigoFuncionario);
    }
}

void pagarComissao(Comissao **comissoes,float * valor_em_caixa) {
    printf("====COMISSÕES=====\n");
    mostrarComissao(*comissoes);
    printf("==================\n");
    int codigo = lerInt("Digite o codigo da comissão que você deseja pagar:");
    int posicao = showComissao(*comissoes, codigo);
    if (posicao == FALSE) {
        printf("Codigo invalido!\n");
        return;
    }
    if(pagandoComisao(comissoes,posicao,valor_em_caixa) == TRUE) printf("Comissão paga com sucesso!\n");
    else printf("Erro ao pagar comissão\n");

}
