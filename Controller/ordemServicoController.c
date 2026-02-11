#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/peca.h"
#include "../bibliotecas/modulo2.h"

#include "../bibliotecas/utils.h"  // Para ler dados

float calcularValorPrevistoPecas(int *codigosPecas, int qtdPecas, Peca *pecas) {
    float valorTotal = 0;
    for (int i = 0; i < qtdPecas; i++) {
        // Percorre apenas as peças selecionadas
        int codigoPeca = codigosPecas[i];

        for (int j = 0; j < getTamanhoPecas(); j++) {
            // Percorre as peças disponíveis
            if (pecas[j].codigo == codigoPeca) {
                valorTotal += pecas[j].preco_venda;
                break; // Evita verificações desnecessárias
            }
        }
    }
    return valorTotal;
}

OrdemServico *readOrdemServico() {
    return getOrdemServico(); // Obtém a lista de ordem de servico do arquivo ou memória
}

int finalOrdemServiço(OrdemServico *ordem_servicos, OrdemServico *ordem_servico, Peca **pecas, Servico *servicos,
                      Transacao **transacoes, ContasPagar **contas_pagar,
                      ContasReceber **contas_receber, float *valor_em_caixa, int * qtdRemovida,Comissao ** comissoes) {
    int index = showOrdemServico(ordem_servicos, ordem_servico->codigo);

    ordem_servicos[index].dataHoraFim = ordem_servico->dataHoraFim;

    ordem_servicos[index].quantidadeServicos = ordem_servico->quantidadeServicos;
    ordem_servicos[index].codigoServicos = malloc(sizeof(int) * ordem_servico->quantidadeServicos);
    memcpy(ordem_servicos[index].codigoServicos, ordem_servico->codigoServicos,
           sizeof(int) * ordem_servico->quantidadeServicos);
    int totalComissao = 0;
    for(int i = 0; i< ordem_servico->quantidadeServicos; i++) {
       int j = showServico(servicos,ordem_servico->codigoServicos[i]);
        totalComissao += servicos[j].comicao;
    }
    double diferencaEmSegundos = difftime(ordem_servicos[index].dataHoraFim, ordem_servicos[index].dataHoraInicio);
    diferencaEmSegundos /=3600;// trasforma em horas
    createComissao(comissoes,ordem_servicos[index].codigo,totalComissao,diferencaEmSegundos,ordem_servicos[index].codigoFuncionario);

    ordem_servicos[index].quantidadePecas = ordem_servico->quantidadePecas;
    ordem_servicos[index].codigosPecas = malloc(sizeof(int) * ordem_servico->quantidadePecas);
    memcpy(ordem_servicos[index].codigosPecas, ordem_servico->codigosPecas,
           sizeof(int) * ordem_servico->quantidadePecas);
    // remove a quantidade gasta de cada peça
    for(int i = 0; i <  ordem_servico->quantidadePecas; i++) {
        int j = showPeca(*pecas,ordem_servico->codigosPecas[i]);
        (*pecas)[j].estoque -= qtdRemovida[i];
    }
    //atualiza no arquivo
    if(getTipoArquivo()!=MEM) setPecas(*pecas);

    float valor = calcularValorPrevisto(ordem_servicos[index].codigoServicos, ordem_servicos[index].quantidadeServicos,
                                        servicos);
    valor += calcularValorPrevistoPecas(ordem_servicos[index].codigosPecas, ordem_servicos[index].quantidadePecas,
                                        *pecas);
    ordem_servicos[index].valorTotal = valor;

    int transacao = novaTransacao(
            transacoes,VENDA, ordem_servicos[index].valorTotal, contas_pagar, contas_receber, valor_em_caixa
        );
    if (transacao == FALSE) return FALSE;
    ordem_servicos[index].codigoTransacao = transacao;

    ordem_servicos[index].feito = TRUE;
    if (getTipoArquivo() != MEM) setOrdemServico(ordem_servicos);
    return TRUE;
}



// Função para gerar um novo ID exclusivo para o agendamento
int buscaNovoIDOrdemServico(OrdemServico *ordem_servico) {
    int maior = 1;
    if (getTamanhoOrdemServico() == 0) return maior; // Caso não haja nenhuma ordem de servico registrado

    // Itera sobre a lista de agendamentos para encontrar o maior ID
    for (int i = 0; i < getTamanhoOrdemServico(); i++) {
        if (maior <= ordem_servico[i].codigo) {
            maior = ordem_servico[i].codigo + 1; // Define o próximo ID como maior + 1
        }
    }
    return maior;
}

// Cria uma nova ordem de serviço
int createOrdemServico(OrdemServico **ordensServico, OrdemServico *ordemServico,Peca **pecas) {
    int tamanhoAtual = getTamanhoOrdemServico();
    OrdemServico *novaOrdemServico = realloc(*ordensServico, (tamanhoAtual + 1) * sizeof(OrdemServico));
    if (novaOrdemServico == NULL) {
        printf("Erro ao alocar mais memória para ordens de serviço.\n");
        return FALSE;
    }

    *ordensServico = novaOrdemServico;
    int index = tamanhoAtual;

    // Inicializa a nova ordem de serviço
    (*ordensServico)[index].codigo = buscaNovoIDOrdemServico(*ordensServico);
    strcpy((*ordensServico)[index].descricao, ordemServico->descricao);
    (*ordensServico)[index].codigoCliente = ordemServico->codigoCliente;
    (*ordensServico)[index].codigoAgendamento = ordemServico->codigoAgendamento;
    (*ordensServico)[index].codigoVeiculo = ordemServico->codigoVeiculo;
    (*ordensServico)[index].codigoFuncionario = ordemServico->codigoFuncionario;
    (*ordensServico)[index].codigoTransacao = ordemServico->codigoTransacao;

    // Aloca e copia os serviços
    (*ordensServico)[index].quantidadeServicos = ordemServico->quantidadeServicos;
    (*ordensServico)[index].codigoServicos = malloc(sizeof(int) * ordemServico->quantidadeServicos);
    if ((*ordensServico)[index].codigoServicos == NULL) {
        printf("Erro ao alocar memória para codigosServicos.\n");
        return FALSE;
    }
    memcpy((*ordensServico)[index].codigoServicos, ordemServico->codigoServicos,
           sizeof(int) * ordemServico->quantidadeServicos);

    // Aloca e copia as peças
    (*ordensServico)[index].quantidadePecas = ordemServico->quantidadePecas;
    (*ordensServico)[index].codigosPecas = malloc(sizeof(int) * ordemServico->quantidadePecas);
    if ((*ordensServico)[index].codigosPecas == NULL) {
        printf("Erro ao alocar memória para codigosPecas.\n");
        return FALSE;
    }
    memcpy((*ordensServico)[index].codigosPecas, ordemServico->codigosPecas,
           sizeof(int) * ordemServico->quantidadePecas);

    // Copia os outros dados
    (*ordensServico)[index].dataHoraInicio = ordemServico->dataHoraInicio;
    (*ordensServico)[index].dataHoraFim = ordemServico->dataHoraFim;
    (*ordensServico)[index].valorTotal = ordemServico->valorTotal;
    (*ordensServico)[index].feito = ordemServico->feito;
    (*ordensServico)[index].ativo = TRUE;

    setTamanhoOrdemServico();

    if (getTipoArquivo() != MEM) setOrdemServico(*ordensServico);
    return TRUE;
}


// Função para exibir uma ordem de servico com base no código
int showOrdemServico(OrdemServico *ordem_servico, int codigo) {
    if (ordem_servico == NULL) return FALSE;

    int posicao = 0;
    int tamanho = getTamanhoOrdemServico();

    // Procura a ordem de servico com o código especificado dentro do limite do array
    while (posicao < tamanho && ordem_servico[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se a ordem de servico foi encontrado
    if (posicao == tamanho)
        return FALSE;

    return posicao;
}

