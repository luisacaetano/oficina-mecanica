#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bibliotecas/caixa.h"
#include "bibliotecas/utils.h"
#include "bibliotecas/cliente.h"
#include "bibliotecas/veiculo.h"
#include "bibliotecas/fornecedor.h"
#include "bibliotecas/oficina.h"
#include "bibliotecas/peca.h"
#include "bibliotecas/funcionario.h"
#include "bibliotecas/servico.h"
#include "bibliotecas/importacaoExportacao.h"
#include "bibliotecas/modulo2.h"


int main(void) {
    setTipoArquivo(lerInt("DIGITE COM QUAL TIPO DE ARQUIVO DESEJA TRABALHAR:\n 1 - TXT\n 2 - BIN\n 3 - MEM\n=>"));

    int opc = 0;

    float valor_em_caixa = 0; // Variável para armazenar o valor em caixa da oficina

    Oficina *oficina_atual = migraDadosOficina();
    Cliente *cliente = NULL;
    cliente = migraDadosCliente();
    Veiculo *veiculo = migraDadosVeiculo();
    Peca *pecas = migraDadosPeca();
    Fornecedor *fornercedor = migraDadosFornecedor();
    Servico *servico = migraDadosServicos();
    Funcionario *funcionario = migraDadosFuncionario();
    Transacao *transacoes = migraDadosTransacao();
    ContasPagar *contas_pagar = migraDadosContasPagar();
    ContasReceber *contas_receber = migraDadosContasReceber();
    valor_em_caixa = migraDadosCaixa();
    OrdemServico * ordem_servicos = migraDadosOrdemServico();
    Agendamento *agendamentos = migraDadosAgendamento();
    Estoque * estoques = migraDadosEstoque();
    Comissao * comissoes = migraDadosComissao();



    if (getTipoArquivo() != MEM) {
        oficina_atual = readOficina();
        cliente = readClientes();
        veiculo = readVeiculos();
        pecas = readPecas();
        fornercedor = readFornecedores();
        servico = readServicos();
        funcionario = readFuncionarios();
        transacoes = readTransacoes();
        contas_pagar = readContasPagar();
        contas_receber = readContasReceber();
        valor_em_caixa = readCaixa();
        ordem_servicos = readOrdemServico();
        agendamentos = readAgendamentos();
        estoques = readEstoque();
        comissoes = readComisoes();
    }
    //cadastrarAgendamento(&agendamentos,cliente,veiculo,funcionario,servico,ordem);
    while (opc != 7) {
        if (getTipoArquivo() != MEM) oficina_atual = readOficina();
        //verifica se a oficina existe oficina
        if (oficina_atual != NULL) {
            printf("\n===============SISTEMA GERENCIAL DE UMA OFICINA===============\n\n \t\tSeja bem vindo!! %s \n\n",
                   oficina_atual->nome);
            opc = lerInt(
                "\t DIGITE UMA OPÇÃO:\n"
                "      \t\t\t 1- CADASTROS\n"
                "      \t\t\t 2- AGENDAMENTOS E CONTROLE\n"
                "      \t\t\t 3- ESTOQUE\n"
                "      \t\t\t 4- FINANCEIRO\n"
                "      \t\t\t 5- RELATORIOS\n"
                "      \t\t\t 6- IMPORTAÇÃO/EXPORTAÇÃO\n"
                "      \t\t\t 7-SAIR\n"
                "=>");
            switch (opc) {
                //aqui vai chamar para cadastro...
                case 1:
                    int cadastros = lerInt("\n=========MODULOS CADASTROS==============\n"
                        "\t DIGITE UMA OPÇÃO:\n"
                        "      \t\t\t 1- CLIENTES\n"
                        "      \t\t\t 2- VEICULOS\n"
                        "      \t\t\t 3- PEÇAS\n"
                        "      \t\t\t 4- FORNECEDORES\n"
                        "      \t\t\t 5- SERVIÇOS\n"
                        "      \t\t\t 6- FUNCIONARIOS\n"
                        "      \t\t\t 7- OFICINA\n"
                        "      \t\t\t 0- VOLTAR\n"
                        "=>");
                    switch (cadastros) {
                        case 1:
                            menuCliente(&cliente);
                            break;
                        case 2:

                            menuVeiculo(&veiculo);
                            break;
                        case 3:
                            //para  pecas preciso passar fornecedor e pecas

                            menuPecas(&pecas, fornercedor);
                            break;
                        case 4:
                            menuFornecedor(&fornercedor);
                            break;
                        case 5:
                            menuServicos(&servico);
                            break;
                        case 6:
                            menuFuncionario(&funcionario);
                            break;
                        case 7:
                            menuOficina(&oficina_atual);
                            break;
                        default:
                            opc = -1;
                            break;
                    }


                    break;
                case 2:
                    int ac = lerInt("\n=========AGENDAMENTO E CONTROLE ==============\n"
                       "\t DIGITE UMA OPÇÃO:\n"
                       "      \t\t\t 1- AGENDAMENTO\n"
                       "      \t\t\t 2- ORDEM SERVIÇOS\n"
                       "      \t\t\t 3- COMISÃO DE FUNCIONARIOS\n"
                       "      \t\t\t 0- VOLTAR\n"
                       "=>");
                switch (ac) {
                    case 1:
                        menuAgendamento(&agendamentos,cliente,veiculo,funcionario, servico,ordem_servicos);
                    break;
                    case 2:
                        menuOrdemServico(&ordem_servicos,agendamentos,servico,&pecas,&transacoes,&contas_pagar,&contas_receber,&valor_em_caixa,&comissoes);
                    break;
                    case 3:
                        menuComissoes(&comissoes,&valor_em_caixa);
                        break;
                    case 0:
                        break;
                    default:
                        printf("Opção invalida!\n");
                }
                    break;
                case 3:
                    menuEstoque(&estoques,oficina_atual,&pecas,&fornercedor,&transacoes,&contas_pagar,&contas_receber,&valor_em_caixa);
                    break;
                case 4:
                    int cadastrosF = lerInt("\n=========MODULOS FINANCEIRO==============\n"
                        "\t DIGITE UMA OPÇÃO:\n"
                        "      \t\t\t 1- TRANSAÇÃO\n"
                        "      \t\t\t 2- CONTAS A PAGAR\n"
                        "      \t\t\t 3- CONTAS A RECEBER\n"
                        "       \t\t\t 0 - SAIR\n"
                        "=>");
                    switch (cadastrosF) {
                        case 1:
                            menuTransacoes(&transacoes);
                            break;
                        case 2:
                            menuContasPagar(&contas_pagar, transacoes, &valor_em_caixa);
                            break;
                        case 3:
                            menuContasReceber(&contas_receber, &valor_em_caixa);
                            break;
                    }
                    break;
                case 5:
                    int filtro = lerInt("\n=========MODULOS RELATORIOS==============\n"
                        "\t DIGITE UMA OPÇÃO:\n"
                        "      \t\t\t 1- CLIENTES\n"
                        "      \t\t\t 2- VEICULOS\n"
                        "      \t\t\t 3- PEÇAS\n"
                        "      \t\t\t 4- FORNECEDORES\n"
                        "      \t\t\t 5- SERVIÇOS\n"
                        "      \t\t\t 6- FUNCIONARIOS\n"
                        "      \t\t\t 7- OFICINA\n"
                        "      \t\t\t 8- FINANCEIRO\n"
                        "=>");
                    switch (filtro) {
                        case 1:
                            filtrarClienteIDNome(cliente);
                            break;
                        case 2:
                            filtrarVeiculosIDModelo(veiculo);
                            break;
                        case 3:
                            filtrarPecasIDDescricao(pecas);
                            break;
                        case 4:
                            filtrarFornecedorIDNomeFantasia(fornercedor);
                            break;
                        case 5:
                            filtrarServicosIDDescricao(servico);
                            break;
                        case 6:
                            filtrarFuncionarioIDNome(funcionario);
                            break;
                        case 8:
                            int financeiro = lerInt("Digite uma opção:\n"
                                "1- TRANSAÇÃO\n"
                                "2- CONTAS A PAGAR\n"
                                "3- CONTAS A RECEBER\n"
                                "0 - SAIR\n"
                                "=>");
                            switch (financeiro) {
                                case 1:
                                    filtrarFinanceiro(transacoes, contas_pagar, contas_receber);
                                    break;
                                case 2:
                                    filtrarContasPagar(contas_pagar);
                                    break;
                                case 3:
                                    filtrarContasReceber(contas_receber);
                                    break;
                            }
                            break;
                    }
                    break;
                case 6:
                    void **cadastrosIE = malloc(sizeof(void *) * 7);
                if (cadastrosIE == NULL) {
                    perror("Erro ao alocar memória");
                    exit(1);
                }
                cadastrosIE[0] = cliente;
                cadastrosIE[1] = veiculo;
                cadastrosIE[2] = pecas;
                cadastrosIE[3] = fornercedor;
                cadastrosIE[4] = servico;
                cadastrosIE[5] = funcionario;
                cadastrosIE[6] = oficina_atual;

                menuImportacaoExportacao(cadastrosIE);

                cliente = cadastrosIE[0];
                veiculo = cadastrosIE[1];
                pecas = cadastrosIE[2];
                fornercedor = cadastrosIE[3];
                servico = cadastrosIE[4];
                funcionario = cadastrosIE[5];
                oficina_atual = cadastrosIE[6];

                free(cadastrosIE);
                break;

            }
        } else {
            Oficina *oficina_nova = migraDadosOficina();
            menuOficina(&oficina_nova);
        }
    }

    return 0;
}
