#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/veiculo.h"
// Função principal do menu de veículos
void menuVeiculo(Veiculo **veiculos) {
    // Carregar dados de veículos, dependendo do tipo de arquivo
    if (getTipoArquivo() != MEM) {
        *veiculos = readVeiculos();
    }

    int opcao = -1;
    // Loop principal do menu
    while (opcao != 0) {
        printf("\t==== MENU VEÍCULO ====\n");
        // Apresenta as opções do menu e lê a escolha do usuário
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
                    "\t\t\t1- CADASTRAR NOVO VEÍCULO\n"
                    "\t\t\t2- VER VEÍCULOS\n"
                    "\t\t\t3- EDITAR VEÍCULO\n"
                    "\t\t\t4- EXCLUIR VEÍCULO\n"
                    "\t\t\t0- SAIR\n"
                    "=>");
        // Executa a ação conforme a opção escolhida
        switch (opcao) {
            case 1:
                novoVeiculo(veiculos);
            if (getTipoArquivo() != MEM) *veiculos = getVeiculos();
            break;
            case 2:
                mostrarVeiculo(*veiculos);
            break;
            case 3:
                editarVeiculo(*veiculos);
            break;
            case 4:
                apagarVeiculo(*veiculos);
            break;
        }
    }
    // Libera a memória alocada para os veículos e define como NULL
}

// Função para cadastrar um novo veículo
void novoVeiculo(Veiculo **veiculos) {

    // Solicita informações sobre o veículo
    Veiculo *veiculo = malloc(sizeof(Veiculo));
    strcpy(veiculo->placa, lerString("Digite a placa do veículo: "));
    strcpy(veiculo->modelo, lerString("Digite o modelo do veículo: "));
    strcpy(veiculo->marca, lerString("Digite a marca do veículo: "));
    veiculo->anofabricacao = atoi(lerString("Digite o ano de fabricação do veículo: "));
    strcpy(veiculo->chassi, lerString("Digite o chassi do veículo: "));
    veiculo->ativo = 1; // Define o veiculo como ativo
    // Tenta cadastrar o veículo
    if (createVeiculo(veiculos, veiculo) != FALSE) {
        printf("Veículo cadastrado com sucesso!\n");
    } else {
        printf("Erro no cadastro do veículo!\n");
    }

    free(veiculo); // Libera a memória do veículo após o cadastro
}

// Função para mostrar informações de veículos
void mostrarVeiculo(Veiculo *veiculos) {
    if (getTamanhoVeiculos() == 0) {
        printf("Nenhum veículo cadastrado até o momento\n");
        return;
    }

    printf("\tVEÍCULOS:\n");
    mostrarTodosVeiculos(veiculos);
    // Solicita o código do veículo que o usuário deseja ver
    int codigo = lerInt("Digite o código do veículo que você deseja ver: ");
    int posicao = showVeiculo(veiculos, codigo);

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Placa: %s\n"
               "Modelo: %s\n"
               "Marca: %s\n"
               "Ano de Fabricação: %d\n"
               "Chassi: %s\n",
               veiculos[posicao].codigo,
               veiculos[posicao].placa,
               veiculos[posicao].modelo,
               veiculos[posicao].marca,
               veiculos[posicao].anofabricacao,
               veiculos[posicao].chassi
        ); // Exibe se o veiculo está ativo
    }
}

// Função para editar as informações de um veículo
void editarVeiculo(Veiculo *veiculos) {
    if (getTamanhoVeiculos() == 0) {
        printf("Nenhum veículo cadastrado\n");
        return;
    }

    Veiculo *veiculo = malloc(sizeof(Veiculo));
    if (veiculo == NULL) {
        printf("Erro ao alocar memória para a edição do veículo\n");
        return;
    }

    printf("\tVEÍCULOS:\n");
    mostrarTodosVeiculos(veiculos);
    // Solicita o código do veículo que será editado
    veiculo->codigo = lerInt("Digite o código do veículo que você deseja editar: ");

    // Procurar o veículo
    int posicao = showVeiculo(veiculos, veiculo->codigo);

    if (posicao == FALSE) {
        printf("Veículo não encontrado.\n");
        free(veiculo);
        return;
    }

    // Edição do campo `placa`
    printf("A placa do veículo é: %s\n", veiculos[posicao].placa);
    int opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(veiculo->placa, lerString("Digite a nova placa do veículo: "));
    } else {
        strcpy(veiculo->placa, veiculos[posicao].placa);
    }

    // Edição do campo `modelo`
    printf("O modelo do veículo é: %s\n", veiculos[posicao].modelo);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(veiculo->modelo, lerString("Digite o novo modelo do veículo: "));
    } else {
        strcpy(veiculo->modelo, veiculos[posicao].modelo);
    }

    // Edição do campo `marca`
    printf("A marca do veículo é: %s\n", veiculos[posicao].marca);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(veiculo->marca, lerString("Digite a nova marca do veículo: "));
    } else {
        strcpy(veiculo->marca, veiculos[posicao].marca);
    }

    // Edição do campo `anofabricacao`
    printf("O ano de fabricação do veículo é: %d\n", veiculos[posicao].anofabricacao);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        veiculo->anofabricacao = lerInt("Digite o novo ano de fabricação do veículo: ");
    } else {
        veiculo->anofabricacao = veiculos[posicao].anofabricacao;
    }

    // Edição do campo `chassi`
    printf("O chassi do veículo é: %s\n", veiculos[posicao].chassi);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(veiculo->chassi, lerString("Digite o novo chassi do veículo: "));
    } else {
        strcpy(veiculo->chassi, veiculos[posicao].chassi);
    }

    if (updateVeiculo(veiculos, veiculo) == FALSE) {
        printf("Erro na edição dos dados do veículo\n");
    } else {
        printf("Veículo editado com sucesso!\n");
    }

    free(veiculo); // Libera a memória alocada
}
// Função para apagar um veículo
void apagarVeiculo(Veiculo *veiculos) {
    if (getTamanhoVeiculos() == 0) {
        printf("Nenhum veículo cadastrado\n");
        return;
    }

    printf("\tVEÍCULOS:\n");
    mostrarTodosVeiculos(veiculos);
    int codigo = lerInt("Digite o código do veículo que você deseja apagar: ");
    if (deleteVeiculo(veiculos, codigo) == TRUE) {
        printf("Veículo apagado com sucesso\n");
    } else {
        printf("Veículo não existe\n");
    }
}
// Função para listar todos os veículos
void mostrarTodosVeiculos(Veiculo *veiculo) {
    for (int i = 0; i < getTamanhoVeiculos(); i++) {
        if (veiculo[i].ativo != FALSE)
            printf("Placa: %s Codigo: %d\n", veiculo[i].placa, veiculo[i].codigo);
    }
}

void filtrarVeiculosIDModelo(Veiculo *veiculos) {
    printf("\t==== FILTRO DE Veiculos ====\n");
    int opcao = lerInt("Você seja filtrar por Codigo ou por modelo?\n"
        "1 - Codigo\n"
        "2 - Modelo\n"
        "=>");
    if (opcao == 1) {
        int id = lerInt("Qual o Codigo do veiculo que você deseja filtrar?\n=>");
        int posicao = showVeiculo(veiculos, id);
        if (posicao != FALSE) {
            printf("Código: %d\n"
                   "Modelo: %s\n"
                   "Marca: %s\n"
                   "Ano de Fabricação: %d\n"
                   "Chassi: %s\n"
                   "Placa: %s\n",
                   veiculos[posicao].codigo,
                   veiculos[posicao].modelo,
                   veiculos[posicao].marca,
                   veiculos[posicao].anofabricacao,
                   veiculos[posicao].chassi,
                    veiculos[posicao].placa
            ); // Exibe se o cliente está ativo
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveVeiculoCSV(&veiculos[posicao], 1);
            }
        } else {
            printf("Codigo não encontrado\n");
        }
    } else if (opcao == 2) {
        char *nome = lerString("Qual o modelo do veiculo que você deseja filtrar?\n=>");
        int tamanho = 0;
        Veiculo *veiculosFiltrados = filterVeiculoModelo(veiculos, nome, &tamanho);
        if (veiculosFiltrados != NULL) {
            for (int i = 0; i < tamanho; i++) {
                printf("Código: %d\n"
                   "Modelo: %s\n"
                   "Marca: %s\n"
                   "Ano de Fabricação: %d\n"
                   "Chassi: %s\n"
                   "Placa: %s\n",
                   veiculosFiltrados[i].codigo,
                   veiculosFiltrados[i].modelo,
                   veiculosFiltrados[i].marca,
                   veiculosFiltrados[i].anofabricacao,
                   veiculosFiltrados[i].chassi,
                    veiculosFiltrados[i].placa
            ); // Exibe se o cliente está ativo
            }
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveVeiculoCSV(veiculosFiltrados, tamanho);
            }
        } else {
            printf("Modelo não encontrado\n");
        }
    }
    else {
        printf("Opção inválida\n");
    }
}
