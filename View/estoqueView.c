#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/Modulo2.h"
#include "../bibliotecas/peca.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/fornecedor.h"

// Função para exibir o menu de estoque
void menuEstoque(Estoque **estoques,Oficina * oficina, Peca **pecas, Fornecedor **fornecedores, Transacao **transacoes,
                 ContasPagar **contas_pagar, ContasReceber **contas_receber, float *valor_em_caixa) {
    int opcao;

    do {
        printf("\n=== MENU ESTOQUE ===\n");
        printf("1 - Registrar Nota de Entrada\n");
        printf("2 - Mostrar Nota\n");
        printf("3 - Listar Estoque Completo\n");
        printf("0 - Sair\n");
        printf("=> ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrarCompra(estoques,oficina,pecas,fornecedores,transacoes,contas_pagar,contas_receber,valor_em_caixa);
                break;

            case 2:
                mostrarEstoque(*estoques);
                break;

            case 3:
                printf("\n=== LISTA COMPLETA DO ESTOQUE ===\n");
            listarEstoque(*pecas);
                break;


            case 0:
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}

// Função para listar todas as peças no estoque
void listarEstoque(Peca *pecas) {
    for (int i = 0; i < getTamanhoPecas(); i++) {
        printf("Código: %d | Descrição: %s | Quantidade: %d | Preço Venda: %.2f\n",
               pecas[i].codigo, pecas[i].descricao, pecas[i].estoque, pecas[i].preco_venda);
    }
}

void registrarCompra(Estoque **estoques,Oficina * oficina, Peca **pecas, Fornecedor **fornecedores, Transacao **transacoes,
                     ContasPagar **contas_pagar, ContasReceber **contas_receber, float *valor_em_caixa) {
    Estoque *estoque = malloc(sizeof(estoque));
    if (estoque == NULL) {
        printf("Erro ao alocar memoria\n");
        return;
    }
    printf("=====FORNECEDORES=====\n");
    mostrarTodosFornecedores(*fornecedores);
    printf("======================\n");
    printf("Se O fornecedor não estiver nessa lista digite -1 para cadastrar\n");
    int codigoFornecedor = lerInt("Digite o codigo do fornecedor:");
    int posicaoF =showFornecedor(*fornecedores,codigoFornecedor);
    int idFornecedor = 0;
    if (posicaoF == FALSE) {
        printf("Fornecedor não cadastrado, por favor cadastre agora!\n");
        idFornecedor =novoFornecedor(fornecedores);
        estoque->codigoFornecedor = idFornecedor;
    } else {
        estoque->codigoFornecedor = (*fornecedores)[posicaoF].codigo;
        idFornecedor = (*fornecedores)[posicaoF].codigo;
    }
    int parar = FALSE;
    int tamanho = 0;
    estoque->codigoPecas = NULL;
    while (1) {
        int codigoPecaNova = 0;
        char *descricao = lerString("Digite a descriçao da peça: ");
        int posicaoP = encontraPecaNome(*pecas, descricao, idFornecedor);
        if (posicaoP == FALSE) {
            Peca *novaPeca = NULL;
            novaPeca = malloc(sizeof(Peca));
            // Solicita informações da peça
            strcpy(novaPeca->descricao,descricao);
            strcpy(novaPeca->fabricante, lerString("Digite o nome do fabricante: "));
            novaPeca->preco_custo = lerFloat("Digite o preço de custo da peça: ");
            novaPeca->estoque = lerInt("Digite a quantidade peças adquiridas: ");
            novaPeca->estoque_min = lerInt("Digite o estoque mínimo: ");
            novaPeca->fornecedor = idFornecedor;
              codigoPecaNova = createPeca(pecas, novaPeca);

        } else {
            int qtd = lerInt("Digite a quantidade peças adquiridas: ");
            (*pecas)[posicaoP].estoque +=qtd;
            (*pecas)[posicaoP].preco_custo = lerInt("Digite o preço de custo da peça: ");
            codigoPecaNova = (*pecas)[posicaoP].codigo;
            if(getTipoArquivo() != MEM ) setPecas(*pecas);
        }
        if (estoque->codigoPecas == NULL) estoque->codigoPecas = malloc(sizeof(int));
        else estoque->codigoPecas = realloc(estoque->codigoPecas,sizeof(int) * tamanho);

        estoque->codigoPecas[tamanho] = codigoPecaNova;
        tamanho++;
        parar = lerInt("Deseja continuar adicionando peças 1 -Sim 2- Não\n=>");
        if (parar == 2) break;
    }
    estoque->quantidadePecas = tamanho;
    estoque->imposto = lerInt("Digite o imposto:");
    estoque->frete = lerInt("Digite o frete:");
    if(createEstoque(estoques,estoque,oficina,pecas,fornecedores,transacoes,contas_pagar,contas_receber,valor_em_caixa)) {
        printf("Nota Registrada com sucesso!\n");
    }
}
void mostrarEstoque(Estoque *estoques) {
    if (getTamanhoEstoque() == 0) {
        printf("Nenhum estoque cadastrado até o momento\n");
        return;
    }
    printf("\tESTOQUES:\n");
    mostrarTodosEstoques(estoques);

    int codigo = lerInt("Digite o código do estoque que você deseja ver: ");
    int posicao = showEstoque(estoques, codigo);

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Código da Peça: %d\n"
               "Quantidade de Peças: %d\n"
               "Código do Fornecedor: %d\n"
               "Imposto: %.2f\n"
               "Frete: %.2f\n"
               "Valor Total: %.2f\n",
               estoques[posicao].codigo,
               estoques[posicao].codigoPecas,
               estoques[posicao].quantidadePecas,
               estoques[posicao].codigoFornecedor,
               estoques[posicao].imposto,
               estoques[posicao].frete,
               estoques[posicao].valorTotal
        );

        printf("Código da Transação: %d\n", estoques[posicao].codigoTransacao);
    } else {
        printf("Estoque não encontrado!\n");
    }
}
void mostrarTodosEstoques(Estoque *estoques) {
    for (int i = 0; i < getTamanhoEstoque(); i++) {
            printf("Estoque: %d Código: %d\n", i + 1, estoques[i].codigo);
    }
}



