#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/servico.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"



// Função para calcular valor previsto
float calcularValorPrevisto(int *codigosServicos, int qtdServicos, Servico *servicos) {
    float valorTotal = 0;
    for (int i = 0; i < qtdServicos; i++) { // Percorre apenas os serviços selecionados
        int codigoServico = codigosServicos[i];

        for (int j = 0; j < getTamanhoServicos(); j++) { // Percorre os serviços disponíveis
            if (servicos[j].codigo == codigoServico) {
                valorTotal += servicos[j].preco;
                break; // Evita verificações desnecessárias
            }
        }
    }
    return valorTotal;
}

// Função para ler todos os agendamentos existentes
Agendamento* readAgendamentos() {
    return getAgendamentos();
}

// Função para gerar um novo ID exclusivo para o agendamento
int buscaNovoIDAgendamento(Agendamento* agendamentos) {
    int maior = 1;
    if (getTamanhoAgendamentos() == 0) return maior; // Caso não haja nenhum agendamento registrado

    // Itera sobre a lista de agendamentos para encontrar o maior ID
    for (int i = 0; i < getTamanhoAgendamentos(); i++) {
        if (maior <= agendamentos[i].codigo) {
            maior = agendamentos[i].codigo + 1; // Define o próximo ID como maior + 1
        }
    }
    return maior;
}

Agendamento* encontrarAgendamento(Agendamento* agendamentos, int qtdAgendamentos, int codigoAgendamento) {
    for (int i = 0; i < qtdAgendamentos; i++) {
        if (agendamentos[i].codigo == codigoAgendamento && agendamentos[i].ativo != FALSE) {
            return &agendamentos[i];
        }
    }
    return NULL;
}

// Função para criar um novo agendamento
int createAgendamento(Agendamento** agendamentos, Agendamento * agendamento) {
    int tamanhoAtual = getTamanhoAgendamentos();
    Agendamento *novoAgendamentos = realloc(*agendamentos, (tamanhoAtual + 1) * sizeof(Agendamento));
    if (novoAgendamentos == NULL) {
        printf("Erro ao alocar mais memória para agendamentos.\n");
        return FALSE;
    }

    *agendamentos = novoAgendamentos;
    int index = tamanhoAtual;

    // Inicializa o novo agendamento
    (*agendamentos)[index].codigo = buscaNovoIDAgendamento(*agendamentos);
    (*agendamentos)[index].quantidadeServicos = agendamento->quantidadeServicos;
    // Copia os serviços
    (*agendamentos)[index].codigosServicos = malloc(sizeof(int) * agendamento->quantidadeServicos);
    if ((*agendamentos)[index].codigosServicos == NULL) {
        printf("Erro ao alocar memória para codigosServicos.\n");
        return FALSE;
    }
    memcpy((*agendamentos)[index].codigosServicos, agendamento->codigosServicos,
           sizeof(int) * agendamento->quantidadeServicos);
    /*(*agendamentos)[index].codigosServicos = malloc(sizeof(agendamento->quantidadeServicos));
    for (int j = 0; j < (*agendamentos)[index].quantidadeServicos; j++) {
        (*agendamentos)[index].codigosServicos[j] = agendamento->codigosServicos[j];
    }*/

    // Copia os outros dados
    (*agendamentos)[index].codigoFuncionario = agendamento->codigoFuncionario;
    (*agendamentos)[index].codigoCliente = agendamento->codigoCliente;
    (*agendamentos)[index].codigoVeiculo = agendamento->codigoVeiculo;
    (*agendamentos)[index].dataHora =  agendamento->dataHora;
    (*agendamentos)[index].valorPrevisto = agendamento->valorPrevisto;
    (*agendamentos)[index].ativo = TRUE;

    setTamanhoAgendamentos();

    if (getTipoArquivo() != MEM) setAgendamentos(*agendamentos);
    return TRUE;
}

int showAgendamento(Agendamento *agendamentos, int codigo) {
    if (agendamentos == NULL) return FALSE;

    int posicao = 0;
    int tamanho = getTamanhoAgendamentos();

    // Procura o agendamento com o código especificado dentro do limite do array
    while (posicao < tamanho && agendamentos[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se o agendamento foi encontrado
    if (posicao == tamanho)
        return FALSE;

    return posicao;
}


int updateAgendamento(Agendamento *agendamentos, Agendamento *agendamento) {
    int posicao = showAgendamento(agendamentos, agendamento->codigo); // Busca o agendamento pelo código

    if (posicao == FALSE) return FALSE; // Retorna FALSE se o agendamento não for encontrado ou estiver inativo

    // Atualiza os campos do agendamento
    agendamentos[posicao].codigo = agendamento->codigo;
    agendamentos[posicao].quantidadeServicos = agendamento->quantidadeServicos;
    agendamentos[posicao].codigoFuncionario = agendamento->codigoFuncionario;
    agendamentos[posicao].codigoCliente = agendamento->codigoCliente;
    agendamentos[posicao].codigoVeiculo = agendamento->codigoVeiculo;
    agendamentos[posicao].dataHora = agendamento->dataHora;
    agendamentos[posicao].valorPrevisto = agendamento->valorPrevisto;
    agendamentos[posicao].ativo = agendamento->ativo; // Atualiza o status de ativo

    // Atualiza os serviços do agendamento
    agendamentos[posicao].codigosServicos = malloc(sizeof(int) * agendamento->quantidadeServicos);
    memcpy(agendamentos[posicao].codigosServicos, agendamento->codigosServicos,
           sizeof(int) * agendamento->quantidadeServicos);

    // Salva os dados no arquivo se o modo de armazenamento não for somente memória
    if (getTipoArquivo() != MEM) setAgendamentos(agendamentos);
    return TRUE;
}


// Deleta (desativa) o agendamento
int deleteAgendamento(Agendamento *agendamentos, int codigo) {
    int posicao = showAgendamento(agendamentos, codigo);

    if (posicao == FALSE) return FALSE;

    agendamentos[posicao].ativo = FALSE;

    if (getTipoArquivo() != MEM) setAgendamentos(agendamentos);
    return TRUE;
}
int getFuncionarioDisponivel(Agendamento *agendamentos, OrdemServico *ordensServico, Funcionario *funcionarios,
                                 time_t datahora) {
    for (int i = 0; i < getTamanhoFuncionarios(); i++) { // Percorre todos os funcionários
        int ocupado = FALSE; // Assume que o funcionário está disponível

        // Verifica se o funcionário já está agendado no horário especificado
        for (int agen = 0; agen < getTamanhoAgendamentos(); agen++) {
            if (funcionarios[i].codigo == agendamentos[agen].codigoFuncionario) {
                if( datahora == agendamentos[agen].dataHora) {
                    ocupado = TRUE;
                    break; // Se o funcionário estiver ocupado, para a verificação
                }
            }
        }

        if (ocupado == FALSE) {
            // Verifica se o funcionário está ocupado em alguma ordem de serviço no horário
            for (int os = 0; os < getTamanhoOrdemServico(); os++) {
                if(funcionarios[i].codigo == ordensServico[os].codigoFuncionario) {
                    if(ordensServico[os].dataHoraFim != 0) {
                        if (datahora >= ordensServico[os].dataHoraInicio && datahora <= ordensServico[os].dataHoraFim) {
                            ocupado = TRUE;
                            break;
                        }
                    }else {
                        if (datahora == ordensServico[os].dataHoraInicio ) {
                            ocupado = TRUE;
                            break;
                        }
                    }
                }
            }
        }

        if (ocupado == FALSE) return i; // Retorna o primeiro funcionário disponível
    }

    return FALSE; // Nenhum funcionário disponível
}
