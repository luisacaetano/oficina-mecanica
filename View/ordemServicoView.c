#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/peca.h"
#include "../bibliotecas/modulo2.h"


void menuOrdemServico(OrdemServico **ordem_servico, Agendamento *agendamentos, Servico *servicos, Peca **pecas,
                      Transacao **transacoes, ContasPagar **contas_pagar, ContasReceber **contas_receber,
                      float *valor_em_caixa, Comissao ** comissoes) {
    if (getTipoArquivo() != 3) {
        *ordem_servico = readOrdemServico();
    }
    int opcao = -1;
    int qtdOrdemServicos = 0;

    while (opcao != 0) {
        printf("\n=======================================\n");
        printf("    GERENCIAMENTO DE ORDEM DE SERVIÇO   \n");
        printf("=======================================\n\n");

        opcao = lerInt("Escolha uma opção:\n"
            "  1 - Cadastrar nova ordem de serviço\n"
            "  2 - Visualizar ordem de serviço\n"
            "  3 - Finalizar Ordem de serviços\n"
            "  0 - Sair\n"
            "Digite sua opção: ");

        switch (opcao) {
            case 1:
                cadastrarOrdemServico(ordem_servico, agendamentos, pecas);
                if (getTipoArquivo() != MEM) *ordem_servico = readOrdemServico();
            // Atualiza as ordem de servico se não estiver em memória
                opcao = -1;
                break;
            case 2:
                mostrarOrdemServico(*ordem_servico);
            break;
            case 3:
                finalizarOrdemServico(*ordem_servico, pecas, servicos, transacoes, contas_pagar, contas_receber,
                                      valor_em_caixa,comissoes);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

void cadastrarOrdemServico(OrdemServico **ordens_servico, Agendamento *agendamentos, Peca **pecas) {
    // Verificar se existem agendamentos
    if (getTamanhoAgendamentos() == 0) {
        printf("\nNão existem agendamentos cadastrados para criar um agendamento para criar uma ordem de serviço.\n");
        return;
    }
    printf("=====AGENDAMENTOS=====\n");
    mostrarTodosAgendamentos(agendamentos);
    printf("======================\n");
    int id = lerInt("Qual é o codigo do agendamento:");
    int indexA = showAgendamento(agendamentos, id);
    if (indexA == FALSE) {
        printf("Codigo invalido!\n");
        return;
    }
    OrdemServico *ordem_servico = malloc(sizeof(OrdemServico));
    if (!ordem_servico) {
        printf("\nErro de alocação de memória!\n");
        return;
    }
    //dados iguais vindo do agendamento
    ordem_servico->codigoAgendamento = agendamentos[indexA].codigo;
    ordem_servico->codigoCliente = agendamentos[indexA].codigoCliente;
    ordem_servico->codigoVeiculo = agendamentos[indexA].codigoVeiculo;
    ordem_servico->codigoFuncionario = agendamentos[indexA].codigoFuncionario;
    ordem_servico->dataHoraInicio = agendamentos[indexA].dataHora;

    ordem_servico->quantidadeServicos = agendamentos[indexA].quantidadeServicos;
    ordem_servico->codigoServicos = malloc(sizeof(int) * ordem_servico->quantidadeServicos);

    memcpy(ordem_servico->codigoServicos, agendamentos[indexA].codigosServicos,
           sizeof(int) * ordem_servico->quantidadeServicos);

    strcpy(ordem_servico->descricao, lerString("Digite a descrição dos serviços a serem feitos: "));

    printf("===PEÇAS===\n");
    mostrarTodasPecas(*pecas);
    printf("=============\n");
    ordem_servico->codigosPecas = malloc(sizeof(int));
    int parar = FALSE;
    int isPimeiro = FALSE;
    int tamanho = 0;

    while (1) {
        if (isPimeiro == FALSE) isPimeiro = TRUE;
        else ordem_servico->codigosPecas = realloc(ordem_servico->codigosPecas, sizeof(int) * tamanho + 1);
        int aux = lerInt("Qual peça deseja adicionar? ");
        if (showPeca(*pecas, aux) == FALSE) printf("Peça não encontrado, por favor cadastre outro\n");
        else {
            ordem_servico->codigosPecas[tamanho] = aux;
            tamanho++;
        }
        parar = lerInt("Deseja parar de adicionar peças 1 -Sim 2- Não\n=>");
        if (parar == 1) break;
    }
    ordem_servico->quantidadePecas = tamanho;

    ordem_servico->valorTotal = 0;
    ordem_servico->dataHoraFim = 0;
    ordem_servico->codigoTransacao = 0;
    ordem_servico->feito = FALSE;

    if (createOrdemServico(ordens_servico, ordem_servico,pecas) == TRUE) {
        printf("Ordem de serviço Criado com sucesso!\n");
    } else printf("Erro ao criar a Ordem de serviço\n");
    free(ordem_servico);
}

void finalizarOrdemServico(OrdemServico *ordem_servicos, Peca **pecas, Servico *servicos,
                           Transacao **transacoes, ContasPagar **contas_pagar,
                           ContasReceber **contas_receber, float *valor_em_caixa,Comissao ** comissoes) {
    OrdemServico *ordem_servico = malloc(sizeof(OrdemServico));
    printf("=====ORDENS DE SERVIÇOS=====\n");
    mostrarTodasOrdensServico(ordem_servicos);
    printf("======================\n");
    int codigo = lerInt("Qual ordem de serviço você deseja finalizar: ");
    int index = showOrdemServico(ordem_servicos, codigo);
    if (index == FALSE) {
        printf("Ordem de serviço invalida");
        return;
    }
    ordem_servico->codigo = codigo;
    int dia = lerInt("Digite o dia que finalizou a ordem de serviço:");
    int mes = lerInt("Digite o mes que finalizou a ordem de serviço:");
    int ano = lerInt("Digite o ano que finalizou a ordem de serviço:");
    int hora = lerInt("Digite a hora que finalizou a ordem de serviço:");

    ordem_servico->dataHoraFim = converteDataHora(dia, mes, ano, hora);

    int mudou = lerInt("Os servicos mudaram? 1 -Sim 2 - Não");
    if (mudou == TRUE) {
        printf("===SERVIÇOS===\n");
        mostrarTodosServicos(servicos);
        printf("=============\n");
        ordem_servico->codigoServicos = malloc(sizeof(int));
        int parar = FALSE;
        int isPimeiro = FALSE;
        int tamanho = 0;

        while (1) {
            if (isPimeiro == FALSE) isPimeiro = TRUE;
            else ordem_servico->codigoServicos = realloc(ordem_servico->codigoServicos, sizeof(int) * tamanho + 1);
            int aux = lerInt("Qual serviço deseja adicionar? ");
            if (showServico(servicos, aux) == FALSE) printf("serviço não encontrado, por favor cadastre outro\n");
            else {
                ordem_servico->codigoServicos[tamanho] = aux;
                tamanho++;
            }
            parar = lerInt("Deseja parar de adicionar Serviços 1 -Sim 2- Não\n=>");
            if (parar == 1) break;
        }
        ordem_servico->quantidadeServicos = tamanho;
    }else {
          ordem_servico->quantidadeServicos = ordem_servicos[index].quantidadeServicos;
        ordem_servico->codigoServicos = malloc(sizeof(int) * ordem_servico->quantidadeServicos);
        memcpy(ordem_servico->codigoServicos,ordem_servicos[index].codigoServicos,
               sizeof(int) * ordem_servico->quantidadeServicos);
    }

    mudou = lerInt("As peças mudaram? 1 -Sim 2 - Não:");
    int  * qtdRemovida = malloc(sizeof(int));
    if (mudou == TRUE) {
        printf("===PEÇAS===\n");
        mostrarTodasPecas(*pecas);
        printf("=============\n");
        ordem_servico->codigosPecas = malloc(sizeof(int));
        int parar = FALSE;
        int isPimeiro = FALSE;
        int tamanho = 0;

        while (1) {
            if (isPimeiro == FALSE) isPimeiro = TRUE;
            else ordem_servico->codigosPecas = realloc(ordem_servico->codigosPecas, sizeof(int) * tamanho + 1);
            int aux = lerInt("Qual peça deseja adicionar? ");
            if (showPeca(*pecas, aux) == FALSE) printf("Peça não encontrado, por favor cadastre outro\n");
            else {
                qtdRemovida = realloc(qtdRemovida, sizeof(int) * tamanho + 1);
                int usou= lerInt("Quantas peças foram usadas: ");
                qtdRemovida[tamanho] = usou;

                ordem_servico->codigosPecas[tamanho] = aux;
                tamanho++;
            }
            parar = lerInt("Deseja parar de adicionar peças 1 -Sim 2- Não\n=>");
            if (parar == 1) break;
        }
        ordem_servico->quantidadePecas = tamanho;
    } else {
        ordem_servico->quantidadePecas = ordem_servicos[index].quantidadePecas;
        ordem_servico->codigosPecas = malloc(sizeof(int) * ordem_servico->quantidadePecas);
        memcpy(ordem_servico->codigosPecas,ordem_servicos[index].codigosPecas,
               sizeof(int) * ordem_servico->quantidadePecas);
        qtdRemovida = malloc(sizeof(int) *ordem_servico->quantidadePecas );
        for(int i = 0; i< ordem_servico->quantidadePecas; i++) {
            int j = showPeca(*pecas,ordem_servico->codigosPecas[i]);
            printf("Peça usada: %s, Codigo: %d Quantidade: %d\n",(*pecas)[j].descricao,(*pecas)[j].codigo,(*pecas)[j].estoque);
            int usou= lerInt("Quantas peças foram usadas: ");
            if(usou >(*pecas)[j].estoque ) {
                printf("Você não pode usar mais do que tem em estoque\n");
                i--;
            }
            qtdRemovida[i] = usou;
        }
    }
    if (finalOrdemServiço(ordem_servicos, ordem_servico, pecas, servicos, transacoes, contas_pagar, contas_receber,
                          valor_em_caixa,qtdRemovida,comissoes) == TRUE) {
        printf("Ordem de serviço finalizada!\n");
    } else printf("Erro ao finalizada Ordem de serviço !\n");
}


void mostrarOrdemServico(OrdemServico *ordem_servicos) {
    if (getTamanhoOrdemServico() == 0) {
        printf("Nenhuma ordem de serviço cadastrada até o momento\n");
        return;
    }
    printf("\tORDENS DE SERVIÇO:\n");
    mostrarTodasOrdensServico(ordem_servicos);

    int codigo = lerInt("Digite o código da ordem de serviço que você deseja ver: ");
    int posicao = showOrdemServico(ordem_servicos, codigo);

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Código do Cliente: %d\n"
               "Código do Agendamento: %d\n"
               "Código do Veículo: %d\n"
               "Código do Funcionário: %d\n"
               "Valor Total: %.2f\n",
               ordem_servicos[posicao].codigo,
               ordem_servicos[posicao].codigoCliente,
               ordem_servicos[posicao].codigoAgendamento,
               ordem_servicos[posicao].codigoVeiculo,
               ordem_servicos[posicao].codigoFuncionario,
               ordem_servicos[posicao].valorTotal
        );

        printf("Data e Hora de Início: ");
        printDataHora(ordem_servicos[posicao].dataHoraInicio);
        printf("Data e Hora de Fim: ");
        printDataHora(ordem_servicos[posicao].dataHoraFim);

        printf("Código dos serviços: ");
        for (int i = 0; i < ordem_servicos[posicao].quantidadeServicos; i++) {
            printf("%d ", ordem_servicos[posicao].codigoServicos[i]);
        }
        printf("\n");

        printf("Código das peças: ");
        for (int i = 0; i < ordem_servicos[posicao].quantidadePecas; i++) {
            printf("%d ", ordem_servicos[posicao].codigosPecas[i]);
        }
        printf("\n");
    } else {
        printf("Ordem de serviço não encontrada!\n");
    }
}

void mostrarTodasOrdensServico(OrdemServico *ordem_servicos) {
    for (int i = 0; i < getTamanhoOrdemServico(); i++) {
        if (ordem_servicos[i].ativo != FALSE)
            printf("Ordem de Serviço: %d Código: %d\n", i + 1, ordem_servicos[i].codigo);
    }
}


