#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/fornecedor.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/peca.h"

// Função que exibe o menu de peças e realiza operações com as peças
void menuPecas(Peca **pecas, Fornecedor *fornecedores) {
    // Verifica o tipo de arquivo e carrega os dados das peças e fornecedores, se necessário
    if (getTipoArquivo() != MEM) {
        *pecas = readPecas();
        fornecedores = readFornecedores();
    }

    int opcao = -1; // Variável para armazenar a opção do usuário
    while (opcao != 0) {
        // Enquanto a opção não for 0 (sair)
        // Exibe o menu
        printf("==== MENU PEÇAS ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
            "\t\t\t1- CADASTRAR NOVA PEÇA\n"
            "\t\t\t2- VER PEÇA\n"
            "\t\t\t3- EDITAR PEÇA\n"
            "\t\t\t4- EXCLUIR PEÇA\n"
            "\t\t\t0- SAIR\n"
            "=>");


        switch (opcao) {
            case 1: // Cadastrar nova peça
                novaPeca(pecas, fornecedores);
                if (getTipoArquivo() != MEM) *pecas = readPecas(); // Recarrega as peças se necessário
                opcao = -1; // Reseta a opção
                break;

            case 2: // Mostrar peça
                mostrarPeca(*pecas);
                opcao = -1;
                break;

            case 3: // Editar peça
                editarPeca(*pecas, fornecedores);
                opcao = -1;
                break;

            case 4: // Excluir peça
                apagarPeca(*pecas);
                opcao = -1;
                break;
        }
    }
}

// Função que cadastra uma nova peça
void novaPeca(Peca **pecas, Fornecedor *fornecedores) {
    // Aloca memória para a nova peça
    Peca *peca = malloc(sizeof(Peca));
    if (peca == NULL) {
        printf("Erro ao alocar memória para a nova peça!\n");
        return;
    }

    // Solicita informações da peça
    strcpy(peca->descricao, lerString("Digite a descrição da peça: "));
    strcpy(peca->fabricante, lerString("Digite o nome do fabricante: "));
    printf("\tFORNECEDORES: \n");
    mostrarTodosFornecedores(fornecedores);

    peca->fornecedor = lerInt("Digite o código do fornecedor: ");
    // Verifica se o fornecedor existe
    if (showFornecedor(fornecedores, peca->fornecedor) == FALSE) {
        printf("Fornecedor não existe, por favor tente novamente\n");
        free(peca); // Libera a memória caso o fornecedor não seja encontrado
        return;
    }

    // Solicita os outros dados da peça
    peca->preco_custo = lerFloat("Digite o preço de custo da peça: ");
    peca->preco_venda = lerFloat("Digite o preço de venda da peça: ");
    peca->estoque = lerInt("Digite a quantidade em estoque: ");
    peca->estoque_min = lerInt("Digite o estoque mínimo: ");

    // Cria a nova peça no sistema
    if (createPeca(pecas, peca) != FALSE) {
        printf("Peça cadastrada com sucesso!\n");
    } else {
        printf("Erro no cadastro da peça!\n");
    }


    free(peca); // Libera a memória da peça após o cadastro
}

// Função que exibe as informações de uma peça
void mostrarPeca(Peca *pecas) {
    // Verifica se há peças cadastradas
    if (getTamanhoPecas() == 0) {
        printf("Nenhuma peça cadastrada até o momento\n");
        return;
    }

    printf("\tPEÇAS:\n");
    mostrarTodasPecas(pecas); // Exibe todas as peças cadastradas

    // Solicita o código da peça a ser visualizada
    int codigo = lerInt("Digite o código da peça que você deseja ver: ");
    int posicao = showPeca(pecas, codigo); // Localiza a peça pelo código

    // Se a peça for encontrada, exibe seus dados
    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Descrição: %s\n"
               "Fabricante: %s\n"
               "Código do Fornecedor: %d\n"
               "Preço de Custo: %.2f\n"
               "Preço de Venda: %.2f\n"
               "Estoque: %d\n"
               "Estoque Mínimo: %d\n",
               pecas[posicao].codigo,
               pecas[posicao].descricao,
               pecas[posicao].fabricante,
               pecas[posicao].fornecedor,
               pecas[posicao].preco_custo,
               pecas[posicao].preco_venda,
               pecas[posicao].estoque,
               pecas[posicao].estoque_min
        );
    } else {
        printf("Peça não encontrada!\n");
    }
}

// Função que edita uma peça existente
void editarPeca(Peca *pecas, Fornecedor *fornecedores) {
    // Verifica se há peças cadastradas
    if (getTamanhoPecas() == 0) {
        printf("Nenhuma peça cadastrada\n");
        return;
    }

    // Aloca memória para a nova versão da peça
    Peca *novaPeca = malloc(sizeof(Peca));
    if (novaPeca == NULL) {
        printf("Erro ao alocar memória para a edição da peça\n");
        return;
    }

    printf("\tPEÇAS:\n");
    mostrarTodasPecas(pecas); // Exibe todas as peças cadastradas

    novaPeca->codigo = lerInt("Digite o código da peça que você deseja editar: ");

    // Procura pela peça com o código informado
    int posicao = showPeca(pecas, novaPeca->codigo);

    // Se a peça não for encontrada, exibe uma mensagem e encerra a função
    if (posicao == FALSE) {
        printf("Peça não encontrada.\n");
        free(novaPeca);
        return;
    }

    // Solicita ao usuário se ele deseja editar os dados da peça
    printf("A descrição atual da peça é: %s\n", pecas[posicao].descricao);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaPeca->descricao, lerString("Digite a nova descrição da peça: "));
    } else {
        strcpy(novaPeca->descricao, pecas[posicao].descricao); // Mantém o valor atual se não for editado
    }

    // Repete o processo para os outros dados (fabricante, fornecedor, preço, etc.)
    printf("O fabricante atual da peça é: %s\n", pecas[posicao].fabricante);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaPeca->fabricante, lerString("Digite o novo nome do fabricante: "));
    } else {
        strcpy(novaPeca->fabricante, pecas[posicao].fabricante);
    }

    printf("\tFORNECEDORES:\n");
    mostrarTodosFornecedores(fornecedores);

    printf("O fornecedor atual é: %d\n", pecas[posicao].fornecedor);
    if (lerInt("Deseja editar o fornecedor? (1 - Sim, 0 - Não): ") == TRUE) {
        int fornecedor = lerInt("Digite o código do novo fornecedor: ");
        if (showFornecedor(fornecedores, fornecedor) == FALSE) {
            printf("Fornecedor não encontrado. Usando o fornecedor atual.\n");
            novaPeca->fornecedor = pecas[posicao].fornecedor;
        } else {
            novaPeca->fornecedor = fornecedor;
        }
    } else {
        novaPeca->fornecedor = pecas[posicao].fornecedor;
    }

    // Solicita os novos valores para os preços e estoques
    printf("O preço de custo atual é: %.2f\n", pecas[posicao].preco_custo);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        novaPeca->preco_custo = lerFloat("Digite o novo preço de custo: ");
    } else {
        novaPeca->preco_custo = pecas[posicao].preco_custo;
    }

    printf("O preço de venda atual é: %.2f\n", pecas[posicao].preco_venda);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        novaPeca->preco_venda = lerFloat("Digite o novo preço de venda: ");
    } else {
        novaPeca->preco_venda = pecas[posicao].preco_venda;
    }

    printf("O estoque atual é: %d\n", pecas[posicao].estoque);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        novaPeca->estoque = lerInt("Digite a nova quantidade em estoque: ");
    } else {
        novaPeca->estoque = pecas[posicao].estoque;
    }

    printf("O estoque mínimo atual é: %d\n", pecas[posicao].estoque_min);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        novaPeca->estoque_min = lerInt("Digite o novo estoque mínimo: ");
    } else {
        novaPeca->estoque_min = pecas[posicao].estoque_min;
    }

    // Atualiza a peça no sistema
    updatePeca(pecas, novaPeca);

    printf("Peça editada com sucesso!\n");
    free(novaPeca); // Libera a memória após editar
}

// Função que apaga uma peça
void apagarPeca(Peca *pecas) {
    // Verifica se há peças cadastradas
    if (getTamanhoPecas() == 0) {
        printf("Nenhuma peça cadastrada\n");
        return;
    }

    printf("\tPEÇAS:\n");
    mostrarTodasPecas(pecas); // Exibe todas as peças cadastradas

    // Solicita o código da peça a ser excluída
    int codigo = lerInt("Digite o código da peça que você deseja excluir: ");
    int posicao = showPeca(pecas, codigo);

    // Se a peça não for encontrada, exibe uma mensagem
    if (posicao == FALSE) {
        printf("Peça não encontrada.\n");
        return;
    }

    // Apaga a peça do sistema
    deletePeca(pecas, posicao);

    printf("Peça excluída com sucesso!\n");
}

// Função para listar todos as peças ativas
void mostrarTodasPecas(Peca *pecas) {
    for (int i = 0; i < getTamanhoPecas(); i++) {
        if (pecas[i].ativo != FALSE)
            printf("Peça: %s, Quantidade:%d Codigo: %d\n", pecas[i].descricao,pecas[i].estoque, pecas[i].codigo);
    }
}

void filtrarPecasIDDescricao(Peca *pecas) {
    printf("\t==== FILTRO DE PEÇAS ====\n");
    int opcao = lerInt("Você seja filtrar por Codigo ou por Descrição?\n"
        "1 - Codigo\n"
        "2 - Descrição\n"
        "=>");
    if (opcao == 1) {
        int id = lerInt("Qual o Codigo do veiculo que você deseja filtrar?\n=>");
        int posicao = showPeca(pecas, id);
        if (posicao != FALSE) {
            printf("Código: %d\n"
                   "Descrição: %s\n"
                   "Fabricante: %s\n"
                   "Fornecedor: %d\n"
                   "Preço de Custo: %.2f\n"
                   "Preço de Venda: %.2f\n"
                   "Estoque: %d\n"
                   "Estoque Mínimo: %d\n",
                   pecas[posicao].codigo,
                   pecas[posicao].descricao,
                   pecas[posicao].fabricante,
                   pecas[posicao].fornecedor,
                   pecas[posicao].preco_custo,
                   pecas[posicao].preco_venda,
                     pecas[posicao].estoque,
                     pecas[posicao].estoque_min
            ); // Exibe se o cliente está ativo
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                savePecaCSV(&pecas[posicao], 1);
            }
        } else {
            printf("Codigo não encontrado\n");
        }
    } else if (opcao == 2) {
        char *nome = lerString("Qual o descrição do peça que você deseja filtrar?\n=>");
        int tamanho = 0;
        Peca *pecasFiltrados = filterPecaDescricao(pecas, nome, &tamanho);
        if (pecasFiltrados != NULL) {
            for (int i = 0; i < tamanho; i++) {
                printf("Código: %d\n"
                   "Descrição: %s\n"
                   "Fabricante: %s\n"
                   "Fornecedor: %d\n"
                   "Preço de Custo: %.2f\n"
                   "Preço de Venda: %.2f\n"
                   "Estoque: %d\n"
                   "Estoque Mínimo: %d\n",
                       pecasFiltrados[i].codigo,
                       pecasFiltrados[i].descricao,
                       pecasFiltrados[i].fabricante,
                       pecasFiltrados[i].fornecedor,
                       pecasFiltrados[i].preco_custo,
                       pecasFiltrados[i].preco_venda,
                          pecasFiltrados[i].estoque,
                            pecasFiltrados[i].estoque_min
                ); // Exibe se o cliente está ativo
            }
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                savePecaCSV(pecasFiltrados, tamanho);
            }
        } else {
            printf("Descrição não encontrado\n");
        }
    } else {
        printf("Opção inválida\n");
    }
}

