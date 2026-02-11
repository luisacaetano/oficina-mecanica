#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/servico.h"

// Função que gerencia o menu principal para operações com serviços
void menuServicos(Servico **servicos) {
    if (getTipoArquivo() != 3) {
        *servicos = readServicos(); // Se o tipo de armazenamento não for memória, lê os clientes do armazenamento
    }
    int opcao = -1;
    while (opcao != 0) {
        printf("\t==== MENU SERVIÇO ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
            "\t\t\t1- CADASTRAR NOVO SERVIÇO\n"
            "\t\t\t2- VER SERVIÇO\n"
            "\t\t\t3- EDITAR SERVIÇO\n"
            "\t\t\t4- EXCLUIR SERVIÇO\n"
            "\t\t\t0- SAIR\n"
            "=>");

        switch (opcao) {
            case 1:
                novoServico(servicos); // Função para cadastrar um novo serviço
                if (getTipoArquivo() != MEM) *servicos = readServicos();
            // Atualiza os serviços se não estiver em memória
                opcao = -1;
                break;

            case 2:
                mostrarServico(*servicos); // Função para exibir um serviço
                opcao = -1;
                break;
            case 3:
                editarServico(*servicos); // Função para editar um serviço
                opcao = -1;
                break;
            case 4:
                apagarServico(*servicos); // Função para excluir um serviço
                opcao = -1;
                break;
            default: // Opção inválida
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

// Função para cadastrar um novo serviço
void novoServico(Servico **servicos) {
    Servico *servico = malloc(sizeof(Servico));
    if (servico == NULL) {
        printf("Erro ao alocar memória para o novo serviço!\n");
        return;
    }

    // Solicita a descrição e preço do serviço
    strcpy(servico->descricao, lerString("Digite a descrição do serviço: "));
    servico->preco = lerFloat("Digite o preço do serviço: ");
    servico->comicao = lerFloat("Digite a comissão sobre o serviço: ");

    // Tenta cadastrar o serviço
    if (createServico(servicos, servico) != FALSE) {
        printf("Serviço cadastrado com sucesso!\n");
    } else {
        printf("Erro no cadastro do serviço!\n");
    }

    free(servico); // Libera a memória do serviço após o cadastro
}

// Função para listar e exibir detalhes de um serviço
void mostrarServico(Servico *servicos) {
    if (getTamanhoServicos() == 0) {
        printf("Nenhum serviço cadastrado até o momento\n");
        return;
    }

    printf("\tSERVIÇOS:\n");
    mostrarTodosServicos(servicos);

    int codigo = lerInt("Digite o código do serviço que você deseja ver: ");
    int posicao = showServico(servicos, codigo); // Assume que showServico foi implementada

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Descrição: %s\n"
               "Preço: %.2f\n"
               "Comissão: %.2f\n",
               servicos[posicao].codigo,
               servicos[posicao].descricao,
               servicos[posicao].preco,
               servicos[posicao].comicao
        ); // Exibe as informações do serviço
    } else {
        printf("Serviço não encontrado!\n");
    }
}

// Função para editar os dados de um cliente
void editarServico(Servico *servicos) {
    if (getTamanhoServicos() == 0) {
        printf("Nenhum serviço cadastrado\n");
        return;
    }

    Servico *servico = malloc(sizeof(Servico));
    printf("\tSERVIÇOS:\n");
    mostrarTodosServicos(servicos);
    if (servico == NULL) {
        printf("Erro ao alocar memória para a edição do serviço\n");
        return;
    }

    servico->codigo = lerInt("Digite o código do serviço que você deseja editar: ");

    // Procurar o serviço
    int posicao = showServico(servicos, servico->codigo);

    if (posicao == FALSE) {
        printf("Serviço não encontrado.\n");
        free(servico);
        return;
    }

    // Edição do campo `descricao`
    printf("A descrição do serviço é: %s\n", servicos[posicao].descricao);
    int opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(servico->descricao, lerString("Digite a nova descrição do serviço: "));
    } else {
        strcpy(servico->descricao, servicos[posicao].descricao);
    }

    // Edição do campo `preco`
    printf("O preço do serviço é: %.2f\n", servicos[posicao].preco);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        servico->preco = lerFloat("Digite o novo preço do serviço: ");
    } else {
        servico->preco = servicos[posicao].preco;
    }

    // Edição do campo `comicao`
    printf("A comissão do serviço é: %.2f\n", servicos[posicao].comicao);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        servico->comicao = lerFloat("Digite a nova comissão do serviço: ");
    } else {
        servico->comicao = servicos[posicao].comicao;
    }

    if (updateServico(servicos, servico) == FALSE) {
        printf("Erro na edição dos dados do serviço\n");
    } else {
        printf("Serviço editado com sucesso!\n");
    }

    free(servico); // Libera a memória alocada
}

// Função para excluir um serviço
void apagarServico(Servico *servicos) {
    if (getTamanhoServicos() == 0) {
        printf("Nenhum serviço cadastrado\n");
        return;
    }
    printf("\tSERVIÇOS:\n");
    mostrarTodosServicos(servicos);
    int codigo = lerInt("Digite o código do serviço que você deseja apagar: ");
    if (deleteServico(servicos, codigo) == TRUE) {
        printf("Serviço apagado com sucesso\n");
    } else {
        printf("Serviço não existe\n");
    }
}

// Função para exibir todos os serviços
void mostrarTodosServicos(Servico *servicos) {
    for (int i = 0; i < getTamanhoServicos(); i++) {
        if (servicos[i].ativo != FALSE)
            printf("Serviço: %s Codigo: %d\n", servicos[i].descricao, servicos[i].codigo);
    }
}

void filtrarServicosIDDescricao(Servico *servicos) {
    printf("\t==== FILTRO DE SERVIÇOS ====\n");
    int opcao = lerInt("Você deseja filtrar por Código ou por Descrição?\n"
        "1 - Código\n"
        "2 - Descrição\n"
        "=>");
    if (opcao == 1) {
        int id = lerInt("Qual o Código do serviço que você deseja filtrar?\n=>");
        int posicao = showServico(servicos, id);
        if (posicao != FALSE) {
            printf("Código: %d\n"
                   "Descrição: %s\n"
                   "Preço: %.2f\n"
                   "Comissão: %.2f\n",
                   servicos[posicao].codigo,
                   servicos[posicao].descricao,
                   servicos[posicao].preco,
                   servicos[posicao].comicao
            );
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveServicoCSV(&servicos[posicao], 1);
            }
        } else {
            printf("Código não encontrado\n");
        }
    } else if (opcao == 2) {
        char *nome = lerString("Qual a descrição do serviço que você deseja filtrar?\n=>");
        int tamanho = 0;
        Servico *servicosFiltrados = filterServicoDescricao(servicos, nome, &tamanho);
        if (servicosFiltrados != NULL) {
            for (int i = 0; i < tamanho; i++) {
                printf("Código: %d\n"
                       "Descrição: %s\n"
                       "Preço: %.2f\n"
                       "Comissão: %.2f\n",
                       servicosFiltrados[i].codigo,
                       servicosFiltrados[i].descricao,
                       servicosFiltrados[i].preco,
                       servicosFiltrados[i].comicao
                );
            }
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveServicoCSV(servicosFiltrados, tamanho);
            }
        } else {
            printf("Descrição não encontrada\n");
        }
    } else {
        printf("Opção inválida\n");
    }
}

