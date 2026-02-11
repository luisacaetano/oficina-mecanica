#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../bibliotecas/cliente.h"
#include "../bibliotecas/funcionario.h"
#include "../bibliotecas/veiculo.h"
#include "../bibliotecas/servico.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"


int codigoCliente, codigoVeiculo, codigoFuncionario;

void menuAgendamento(Agendamento **agendamentos,
                     Cliente *clientes, Veiculo *veiculos, Funcionario *funcionarios, Servico *servicos,
                     OrdemServico *ordem_servicos) {
    if (getTipoArquivo() != MEM) {
        *agendamentos = readAgendamentos();
    }

    int opcao = -1;

    while (opcao != 0) {
        printf("\n=======================================\n");
        printf("        GERENCIAMENTO DE AGENDAMENTOS  \n");
        printf("=======================================\n\n");

        opcao = lerInt("Escolha uma opção:\n"
            "  1 - Cadastrar novo agendamento\n"
            "  2 - Visualizar todos agendamentos\n"
            "  3 - Editar agendamento\n"
            "  4 - Excluir agendamento\n"
            "  0 - Sair\n"
            "Digite sua opção: ");

        switch (opcao) {
            case 1:
                cadastrarAgendamento(agendamentos, clientes, veiculos, funcionarios, servicos, ordem_servicos);
                opcao = -1;
                break;
            case 2:
                 mostrarAgendamento(*agendamentos);
                opcao = -1;
                break;
            case 3:
                editarAgendamento( *agendamentos, clientes, veiculos, funcionarios,servicos,ordem_servicos);
                opcao = -1;
                break;
            case 4:
                apagarAgendamento(*agendamentos);
                opcao = -1;
                break;
             case 0:
                 printf("Saindo do menu de agendamentos.\n");
                 break;
             default:
                 printf("Opção inválida. Tente novamente.\n");
                 break;
        }
    }
}

// Função para exibir a lista de clientes ativos

void cadastrarAgendamento(Agendamento **agendamentos, Cliente *clientes,
                          Veiculo *veiculos, Funcionario *funcionarios,
                          Servico *servicos, OrdemServico *ordem_servico) {
    // Verificações iniciais
    if (getTamanhoClientes() == 0 || getTamanhoVeiculos() == 0 || getTamanhoFuncionarios() == 0 ||
        getTamanhoServicos() == 0) {
        printf("\nÉ necessário ter clientes, serviços, veículos e funcionários cadastrados.\n");
        return;
    }

    Agendamento *agendamento = malloc(sizeof(Agendamento));
    if (agendamento == NULL) {
        printf("\nErro de alocação de memória!\n");
        return;
    }
    // idenficar cliente
    printf("===CLIENTES===\n");
    mostrarTodosClientes(clientes);
    printf("=============\n");
    int id = lerInt("Digite o codigo do cliente deseja agendar? ");
    if (showCliente(clientes, id) == FALSE) {
        printf("cliente não encontrado\n");
    } else {
        agendamento->codigoCliente = id;
    }
    printf("===VEICULOS===\n");
    mostrarTodosVeiculos(veiculos);
    printf("=============\n");
    id = lerInt("Qual o codigo do veiculo que deseja agendar? ");
    if (showVeiculo(veiculos, id) == FALSE) {
        printf("veiculo não encontrado\n");
    } else {
        agendamento->codigoVeiculo = id;
    }
    printf("===SERVIÇOS===\n");
    mostrarTodosServicos(servicos);
    printf("=============\n");
    agendamento->codigosServicos = malloc(sizeof(Agendamento));
    int parar = FALSE;
    int isPimeiro = FALSE;
    int tamanho = 0;

    while (1) {
        if (isPimeiro == FALSE) isPimeiro = TRUE;
        else agendamento->codigosServicos = realloc(agendamento->codigosServicos, sizeof(int) * tamanho + 1);
        int aux = lerInt("Qual serviço deseja adicionar? ");
        if (showServico(servicos, aux) == FALSE) printf("Serviço não encontrado, por favor cadastre outro\n");
        else {
            agendamento->codigosServicos[tamanho] = aux;
            tamanho++;
        }
        parar = lerInt("Deseja parar de adicionar seviços 1 -Sim 2- Não\n=>");
        if (parar == 1) break;
    }
    agendamento->quantidadeServicos = tamanho;
    int dia = lerInt("Digite o dia que seja agendar:");
    int mes = lerInt("Digite o mes que seja agendar:");
    int ano = lerInt("Digite o ano que seja agendar:");
    int hora = lerInt("Digite a hora que seja agendar:");

    time_t dataHora = converteDataHora(dia, mes, ano, hora);
    agendamento->dataHora = dataHora;


    // Verificar disponibilidade do funcionário
    int indexFuncionario =getFuncionarioDisponivel(agendamento,ordem_servico,funcionarios,agendamento->dataHora);
    agendamento->codigoFuncionario = funcionarios[indexFuncionario].codigo;
    agendamento->valorPrevisto = calcularValorPrevisto(agendamento->codigosServicos, agendamento->quantidadeServicos,
                                                       servicos);
    printf("O valor previsto do serviço é:%.2f\n", agendamento->valorPrevisto);
    if (createAgendamento(agendamentos, agendamento) == TRUE) {
        printf("Agendamento Criado com sucesso!\n");
    } else printf("Erro ao criar o agendamento\n");
    free(agendamento);
}

void mostrarAgendamento(Agendamento *agendamentos) {
    if (getTamanhoAgendamentos() == 0) {
        printf("Nenhum agendamento cadastrado até o momento\n");
        return;
    }
    printf("\tAGENDAMENTOS:\n");
    mostrarTodosAgendamentos(agendamentos);

    int codigo = lerInt("Digite o código do agendamento que você deseja ver: ");
    int posicao = showAgendamento(agendamentos, codigo);

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Código do Funcionário: %d\n"
               "Código do Cliente: %d\n"
               "Código do Veículo: %d\n"
               "Valor Previsto: %.2f\n",
               agendamentos[posicao].codigo,
               agendamentos[posicao].codigoFuncionario,
               agendamentos[posicao].codigoCliente,
               agendamentos[posicao].codigoVeiculo,
               agendamentos[posicao].valorPrevisto
        );
        printf("Data e Hora: ");
        printDataHora(agendamentos[posicao].dataHora);
        printf("Código dos serviços: ");
        for(int i = 0; i< agendamentos[posicao].quantidadeServicos; i++) {
            printf("%d ",agendamentos[posicao].codigosServicos[i]);
        }
        printf("\n");
    } else {
        printf("Agendamento não encontrado!\n");
    }
}

void editarAgendamento(Agendamento *agendamentos, Cliente *clientes, Veiculo *veiculos, Funcionario *funcionario, Servico *servicos, OrdemServico *ordem_servicos) {
    if (getTamanhoAgendamentos() == 0) {
        printf("Nenhum agendamento cadastrado\n");
        return;
    }

    Agendamento *agendamento = malloc(sizeof(Agendamento));
    if (agendamento == NULL) {
        printf("Erro ao alocar memória para a edição do agendamento\n");
        return;
    }

    printf("\tAGENDAMENTOS:\n");
    mostrarTodosAgendamentos(agendamentos);

    agendamento->codigo = lerInt("Digite o código do agendamento que você deseja editar: ");

    // Procurar o agendamento
    int posicao = showAgendamento(agendamentos, agendamento->codigo);

    if (posicao == FALSE) {
        printf("Agendamento não encontrado.\n");
        free(agendamento);
        return;
    }

    // Edição do campo `codigoFuncionario`
    printf("O código do funcionário é: %d\n", agendamentos[posicao].codigoFuncionario);
    int opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        agendamento->codigoFuncionario = lerInt("Digite o novo código do funcionário: ");
    } else {
        agendamento->codigoFuncionario = agendamentos[posicao].codigoFuncionario;
    }

    // Edição do campo `codigoCliente`
    printf("O código do cliente é: %d\n", agendamentos[posicao].codigoCliente);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        printf("===CLIENTES===\n");
        mostrarTodosClientes(clientes);
        printf("=============\n");
        agendamento->codigoCliente = lerInt("Digite o novo código do cliente: ");
    } else {
        agendamento->codigoCliente = agendamentos[posicao].codigoCliente;
    }

    // Edição do campo `codigoVeiculo`
    printf("O código do veículo é: %d\n", agendamentos[posicao].codigoVeiculo);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        printf("===VEICULOS===\n");
        mostrarTodosVeiculos(veiculos);
        printf("=============\n");
        agendamento->codigoVeiculo = lerInt("Digite o novo código do veículo: ");
    } else {
        agendamento->codigoVeiculo = agendamentos[posicao].codigoVeiculo;
    }

    // Edição do campo `dataHora`
    printf("A data e hora do agendamento é: ");
    printDataHora(agendamentos[posicao].dataHora);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        int dia = lerInt("Digite o dia: ");
        int mes = lerInt("Digite o mês: ");
        int ano = lerInt("Digite o ano: ");
        int hora = lerInt("Digite a hora: ");
        agendamento->dataHora = converteDataHora(dia, mes, ano, hora);
        agendamentos[posicao].codigoFuncionario = FALSE; // reseta  o funcionario
        int indexFuncionario =getFuncionarioDisponivel(agendamento,ordem_servicos,funcionario,agendamento->dataHora);
        agendamento->codigoFuncionario = funcionario[indexFuncionario].codigo;
    } else {
        agendamento->dataHora = agendamentos[posicao].dataHora;
        agendamento->codigoFuncionario =  agendamentos[posicao].codigoFuncionario;
    }

    // Edição do campo `codigosServicos`
    printf("Os serviços são: ");
    for (int i = 0; i < agendamentos[posicao].quantidadeServicos; i++) {
        printf("%d ", agendamentos[posicao].codigosServicos[i]);
    }
    printf("\n");

    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        int tamanho = 0;
        int parar = FALSE;
        int *novosServicos = NULL;

        while (1) {
            novosServicos = realloc(novosServicos, sizeof(int) * (tamanho + 1));
            if (novosServicos == NULL) {
                printf("Erro ao alocar memória para serviços.\n");
                free(agendamento);
                return;
            }

            int aux = lerInt("Qual serviço deseja adicionar? ");
            if (showServico(servicos, aux) == FALSE) {
                printf("Serviço não encontrado, por favor cadastre outro\n");
            } else {
                novosServicos[tamanho] = aux;
                tamanho++;
            }

            parar = lerInt("Deseja parar de adicionar serviços? (1 - Sim, 2 - Não): ");
            if (parar == 1) break;
        }

        agendamento->codigosServicos = novosServicos;
        agendamento->quantidadeServicos = tamanho;
        agendamento->valorPrevisto =calcularValorPrevisto(agendamento->codigosServicos,agendamento->quantidadeServicos,servicos);
        printf("O novo valor previsto é: %.2f \n",agendamento->valorPrevisto);
    } else {
        agendamento->quantidadeServicos = agendamentos[posicao].quantidadeServicos;
        agendamento->codigosServicos = malloc(sizeof(int) * agendamento->quantidadeServicos);
        if (agendamento->codigosServicos == NULL) {
            printf("Erro ao alocar memória para codigosServicos.\n");
            free(agendamento);
            return;
        }
        memcpy(agendamento->codigosServicos, agendamentos[posicao].codigosServicos,
               sizeof(int) * agendamento->quantidadeServicos);
        agendamento->valorPrevisto = agendamento[posicao].valorPrevisto;
    }

    if (updateAgendamento(agendamentos, agendamento) == FALSE) {
        printf("Erro na edição dos dados do agendamento\n");
    } else {
        printf("Agendamento editado com sucesso!\n");
    }

    free(agendamento->codigosServicos);
    free(agendamento);
}

void apagarAgendamento(Agendamento *agendamentos) {
    if (getTamanhoAgendamentos() == 0) {
        printf("Nenhum agendamento cadastrado\n");
        return;
    }
    printf("\tAGENDAMENTOS:\n");
    mostrarTodosAgendamentos(agendamentos);
    int codigo = lerInt("Digite o código do agendamento que você deseja apagar: ");
    if (deleteAgendamento(agendamentos, codigo) == TRUE) {
        printf("Agendamento apagado com sucesso\n");
    } else {
        printf("Agendamento não existe\n");
    }
}

void mostrarTodosAgendamentos(Agendamento *agendamentos) {
    for (int i = 0; i < getTamanhoAgendamentos(); i++) {
        if (agendamentos[i].ativo != FALSE)
            printf("Agendamento: %d Codigo: %d\n", i + 1, agendamentos[i].codigo);
    }
}

