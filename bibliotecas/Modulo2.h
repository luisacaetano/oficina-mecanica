#ifndef MODULO2_H
#define MODULO2_H
#include <time.h>

#include "../bibliotecas/servico.h"
#include "../bibliotecas/cliente.h"
#include "../bibliotecas/veiculo.h"
#include "../bibliotecas/peca.h"
#include "../bibliotecas/funcionario.h"
#include "../bibliotecas/oficina.h"

typedef struct {
    int codigo;
    int *codigosServicos;
    int quantidadeServicos;
    int codigoFuncionario;
    int codigoCliente;
    int codigoVeiculo;
    time_t dataHora;
    float valorPrevisto;
    int ativo;
} Agendamento;

typedef struct {
    int codigo;
    char descricao[100];
    int codigoCliente;
    int codigoAgendamento;
    int codigoVeiculo;
    int codigoFuncionario;
    int codigoTransacao;
    int *codigoServicos;
    int quantidadeServicos;
    int *codigosPecas;
    int quantidadePecas;
    int dataHoraInicio;
    int dataHoraFim;
    float valorTotal;
    int feito;
    int ativo;
} OrdemServico;

typedef struct {
    int codigo;
    int CodigoOrdem;
    float valorComisao;
    double tempoGasto;
    int codigoFuncionario;
    int pago;
} Comissao;

typedef struct {
    int codigo;
    int quantidadePecas;
    int *codigoPecas;
    int codigoFornecedor;
    float imposto;
    float frete;
    float valorTotal;
    int codigoTransacao;
} Estoque;

typedef struct {
    int codigo;
    int formaPagamento;
    float valorTotal;
    int tipoTransacao;
    time_t dataTransacao;
} Transacao;

typedef struct {
    int codigo;
    int codTransacao;
    time_t dataPagamento;
    time_t dataEfeituacaoPagamento;
    int pago;
} ContasPagar;

typedef struct {
    int codigo;
    int codTransacao;
    float valor;
    time_t dataPagamento;
    time_t recebimento;
    int pago;
} ContasReceber;

#define AVISTA 0 // Forma de pagamento
#define CARTAO 1 // Forma de pagamento
#define COMPRA 0 // Tipo de transação
#define VENDA 1 // Tipo de Transação


// Model
int getTamanhoOrdemServico();

void setTamanhoOrdemServico();

OrdemServico *getOrdemServico();

void editTamanhoOrdemServicos(int tamanho);

void setOrdemServico(OrdemServico *ordem_servico);

OrdemServico *migraDadosOrdemServico();

OrdemServico *ler_arquivo_txt_ordem_servico(FILE *buffer);

void escrever_arquivo_txt_ordem_servico(FILE *buffer, OrdemServico *ordensServico);

OrdemServico *ler_arquivo_bin_ordem_servico(FILE *buffer);

void escrever_arquivo_bin_ordem_servico(FILE *buffer, OrdemServico *ordensServico);

// Controller
OrdemServico *readOrdemServico();

int calcularTempoGasto(const char *horaInicio, const char *horaFim);

int buscaNovoIDOrdemServico(OrdemServico *ordem_servico);

int createOrdemServico(OrdemServico **ordensServico, OrdemServico *ordemServico,Peca **pecas);

int showOrdemServico(OrdemServico *ordem_servico, int codigo);

int updateOrdemServico(OrdemServico *ordensServico, OrdemServico *ordemServico);

int deleteOrdemServico(OrdemServico *ordem_servico, int codigo);

float calcularValorPrevistoPecas(int *codigosPecas, int qtdPecas, Peca *pecas);

int finalOrdemServiço(OrdemServico *ordem_servicos, OrdemServico *ordem_servico, Peca **pecas, Servico *servicos,
                      Transacao **transacoes, ContasPagar **contas_pagar,
                      ContasReceber **contas_receber, float *valor_em_caixa, int * qtdRemovida,Comissao ** comissoes);


// view Ordem Serviço
void cadastrarOrdemServico(OrdemServico **ordens_servico, Agendamento *agendamentos, Peca **pecas);

void mostrarTodasOrdensServico(OrdemServico *ordem_servicos);

void mostrarOrdemServico(OrdemServico *ordem_servicos);

void menuOrdemServico(OrdemServico **ordem_servico, Agendamento *agendamentos, Servico *servicos, Peca **pecas,
                      Transacao **transacoes, ContasPagar **contas_pagar, ContasReceber **contas_receber,
                      float *valor_em_caixa, Comissao ** comissoes);

void mostrarOrdemServico(OrdemServico *ordem_servicos);

void finalizarOrdemServico(OrdemServico *ordem_servicos, Peca **pecas, Servico *servicos,
                           Transacao **transacoes, ContasPagar **contas_pagar,
                           ContasReceber **contas_receber, float *valor_em_caixa,Comissao ** comissoes);
// Funções de manipulação de dados (model)
int getTamanhoAgendamentos();

void setTamanhoAgendamentos();

void editTamanhoAgendamentos(int tamanho);

Agendamento *getAgendamentos();

void setAgendamentos(Agendamento *agendamentos);

Agendamento *ler_arquivo_txt_agendamento(FILE *buffer);

void escrever_arquivo_txt_agendamento(FILE *buffer, Agendamento *agendamentos);

Agendamento *ler_arquivo_bin_agendamento(FILE *buffer);

void escrever_arquivo_bin_agendamento(FILE *buffer, Agendamento *agendamentos);

int buscaNovoIDAgendamento(Agendamento *agendamentos);

Agendamento *migraDadosAgendamento();

// Funções de controle (controller)
Agendamento *readAgendamentos();

int createAgendamento(Agendamento **agendamentos, Agendamento *agendamento);

int updateAgendamento(Agendamento *agendamentos, Agendamento *agendamento);

int deleteAgendamento(Agendamento *agendamentos, int codigo);

int showAgendamento(Agendamento *agendamentos, int codigo);

Agendamento *encontrarAgendamento(Agendamento *agendamentos, int qtdAgendamentos, int codigoAgendamento);

float calcularValorPrevisto(int *codigosServicos, int qtdServicos, Servico *servicos);

int getFuncionarioDisponivel(Agendamento *agendamentos, OrdemServico *ordensServico, Funcionario *funcionarios,
                             time_t datahora);

// Funções de visualização (view)
void menuAgendamento(Agendamento **agendamentos, Cliente *clientes, Veiculo *veiculos, Funcionario *funcionarios,
                     Servico *servicos, OrdemServico *ordem_servicos);

void cadastrarAgendamento(Agendamento **agendamentos, Cliente *clientes,
                          Veiculo *veiculos, Funcionario *funcionarios,
                          Servico *servicos, OrdemServico *ordem_servico);

void mostrarAgendamento(Agendamento *agendamentos);

void editarAgendamento(Agendamento *agendamentos, Cliente *clientes, Veiculo *veiculos, Funcionario *funcionario,
                       Servico *servicos, OrdemServico *ordem_servicos);

void apagarAgendamento(Agendamento *agendamentos);

void mostrarTodosAgendamentos(Agendamento *agendamentos);

void apagarAgendamento(Agendamento *agendamentos);


// Model Comissao

int getTamanhoComissoes(void);

void setTamanhoComissoes(void);

void editTamanhoComissoes(int tamanho);

Comissao *getComissoes(void);

void setComissoes(Comissao *comisoes);

Comissao *ler_arquivo_bin_comissao(FILE *buffer);

void *escrever_arquivo_bin_comisao(FILE *buffer, Comissao *comisoes);

Comissao *migraDadosComissao(void);

Comissao *ler_arquivo_txt_comissao(FILE *buffer);

void escrever_arquivo_txt_comissao(FILE *buffer, Comissao *comisoes);

//Controller Comissao
Comissao * readComisoes();

int createComissao(Comissao **comissoes, int codigo, float valorComissao, double tempoGasto, int codigoFuncionario);

int showComissao(Comissao *comissoes, int codigo);

int buscaNovoIDComissao(Comissao *comissoes);

int pagandoComisao(Comissao **comissoes, int posicao, float *valor_em_caixa);

// View Comissao

void menuComissoes(Comissao **comissoes,float * valor_em_caixa);

void mostrarComissao(Comissao *comissoes);

void mostrarTodasComissoes(Comissao *comissoes);

void pagarComissao(Comissao **comissoes,float * valor_em_caixa);
//Model Estoque
int getTamanhoEstoque();

void setTamanhoEstoque();

void editTamanhoEstoques(int tamanho);

Estoque *migraDadosEstoque();

void setEstoque(Estoque *estoque);

Estoque *getEstoque();

Estoque *ler_arquivo_txt_estoque(FILE *buffer);

void escrever_arquivo_txt_estoque(FILE *buffer, Estoque *estoques);

Estoque *ler_arquivo_bin_estoque(FILE *buffer);

void escrever_arquivo_bin_estoque(FILE *buffer, Estoque *estoques);

// Controller Estoque
Estoque *readEstoque();

int encontraPecaNome(Peca *pecas, char *descricao, int idFornecedor);

int createEstoque(Estoque **estoques, Estoque *estoque, Oficina *oficina, Peca **pecas, Fornecedor **fornecedores,
                  Transacao **transacoes,
                  ContasPagar **contas_pagar, ContasReceber **contas_receber, float *valor_em_caixa);

int buscaNovoIDEstoque(Estoque *estoques);

int showEstoque(Estoque *estoques, int codigo);

// View Estoque
void novaPecaEstoque(Peca *pecas, Peca *novaPecas);

void registrarCompra(Estoque **estoques, Oficina *oficina, Peca **pecas, Fornecedor **fornecedores,
                     Transacao **transacoes,
                     ContasPagar **contas_pagar, ContasReceber **contas_receber, float *valor_em_caixa);

void menuEstoque(Estoque **estoques, Oficina *oficina, Peca **pecas, Fornecedor **fornecedores, Transacao **transacoes,
                 ContasPagar **contas_pagar, ContasReceber **contas_receber, float *valor_em_caixa);

void mostrarEstoque(Estoque *estoques);

void mostrarTodosEstoques(Estoque *estoques);

void listarEstoque(Peca *pecas);

// Model transações
Transacao *migraDadosTransacao(); // Função que migra dados entre diferentes tipos de arquivo
Transacao *getTransacoes(); // Função que retorna a lista de transações
void setTransacoes(Transacao *transacoes); // Função que atualiza a lista de transações
int getTamanhoTransacoes(); // Função que retorna o número de transações cadastradas
void setTamanhoTransacoes(); // Função que incrementa o número de transações cadastradas
void editTamanhoTransacoes(int tamanho);
void removeTamanhoTransacoes(); // Função que decrementa 1 unidade a quantidade de transações
void escrever_arquivo_txt_transacao(FILE *buffer, Transacao *transacoes); // Função para escrever no arquivo txt
Transacao *ler_arquivo_txt_transacao(FILE *buffer); // Função que lê os dados de uma transação do arquivo txt
Transacao *ler_arquivo_bin_transacao(FILE *buffer); // Função que lê os dados de uma transação do arquivo binário
void escrever_arquivo_bin_transacao(FILE *buffer, Transacao *transacoes); // Função para escrever no arquivo binário

// Controller transações
int createTransacao(Transacao **transacoes, Transacao *transacao, float *valor_em_caixa, ContasPagar **contas_pagar,
                    ContasReceber **contas_receber, int qtd_parcelas);

Transacao *readTransacoes(); // Função que retorna todas as transações cadastradas
int showTransacao(Transacao *transacoes, int codigo); // Função que mostra os dados de uma transação baseada no código
int deleteTransacao(Transacao *transacoes, int codigo); // Função que exclui uma transação com base no código
int buscaNovoIDTransacao(Transacao *transacoes); // Função que encontra um ID válido para uma nova transação
int saveFinanceiroCSV(void **filtro, int tamT, int tamCP, int CR);

// Função que salva os dados financeiros em um arquivo CSV
void **filterIntervaloDatas(Transacao *transacoes, ContasPagar *contas_pagar, ContasReceber *contas_receber,
                            time_t dataI, time_t dataF, int *TamanhoT, int *TamanhoCP,
                            int *TamanhoCR); // Função que filtra as transações por intervalo de datas

// View transação
int novaTransacao(Transacao **transacoes, int tipoTransacao, float valor_Total, ContasPagar **contas_pagar,
                  ContasReceber **contas_receber, float *valor_em_caixa);

void menuTransacoes(Transacao **transacoes); // Menu que exibe as opções de interação com as transações
void mostrarTransacao(Transacao *transacoes); // Função que exibe os dados de uma transação
void apagarTransacao(Transacao *transacoes); // Função que apaga os dados de uma transação
void mostrarTodasTransacoes(Transacao *transacoes); // Função que exibe todas as transações cadastradas
void filtrarFinanceiro(Transacao *transacoes, ContasPagar *contas_pagar, ContasReceber *contas_receber);

// Função que filtra as transações financeiras
void printTransacoes(Transacao *transacoes, int tam); // Função que exibe as informações de todas as transações

// Model contas a receber

int getTamanhoContasReceber();

void setTamanhoContasReceber();

void editTamanhoContasReceber(int tamanho);

void removeTamanhoContasReceber();

ContasReceber *migraDadosContasReceber();

void setContasReceber(ContasReceber *contas);

ContasReceber *getContasReceber();

ContasReceber *ler_arquivo_txt_contasReceber(FILE *buffer);

void escrever_arquivo_txt_contasReceber(FILE *buffer, ContasReceber *contas);

ContasReceber *ler_arquivo_bin_contasReceber(FILE *buffer);

void escrever_arquivo_bin_contasReceber(FILE *buffer, ContasReceber *contas);

// Controller contas a receber

ContasReceber *readContasReceber();

int createContasReceber(ContasReceber **contas, int codTransacao, time_t dataPagamento, float valor);

int showContasReceber(ContasReceber *contas, int codigo);

int deleteContasReceber(ContasReceber *contas, int codigo);

int contaRecebida(ContasReceber *contas, int codigo, float *valor_em_caixa, time_t dataPagamento);

int buscaNovoIDContasReceber(ContasReceber *contas);

int temContasReceber(ContasReceber *contas);

int temContasRecebidas(ContasReceber *contas);

int saveContasReceberCSV(ContasReceber *contas, int tamanho); // Função que salva as contas a receber em um arquivo CSV
ContasReceber *filterContasReceberIntervaloDatas(ContasReceber *contas_receber, time_t dataInicio, time_t dataFim,
                                                 int *Tamanho);

// Função que filtra as contas a receber por intervalo de datas
// View contas a recebar

void receberConta(ContasReceber *contas, float *valor_em_caixa);

void mostrarTodasContasReceber(ContasReceber *contas);

void menuContasReceber(ContasReceber **contas, float *valor_em_caixa);

void mostrarContasReceber(ContasReceber *contas);

void mostrarContasRecebidas(ContasReceber *contas);

void mostrarTodasContasRecebidas(ContasReceber *contas);

void mostrarTodasContasReceber(ContasReceber *contas);

void apagarContaReceber(ContasReceber *contas);

void mostrarTodasContaReceber(ContasReceber *contas);

void printContasReceber(ContasReceber *contas_receber, int tam);

// Função que exibe as informações de todas as contas a receber
void filtrarContasReceber(ContasReceber *contas_receber); // Função que filtra as contas a receber

// Model contas a pagar

int getTamanhoContasPagar();

void setTamanhoContasPagar();

void removeTamanhoContasPagar();

void editTamanhoContasPagar(int tamanho);

ContasPagar *migraDadosContasPagar();

void setContasPagar(ContasPagar *contas);

ContasPagar *getContasPagar();

ContasPagar *ler_arquivo_txt_contasPagar(FILE *buffer);

void escrever_arquivo_txt_contasPagar(FILE *buffer, ContasPagar *contas);

ContasPagar *ler_arquivo_bin_contasPagar(FILE *buffer);

void escrever_arquivo_bin_contasPagar(FILE *buffer, ContasPagar *contas);

// Controller contas a pagar

ContasPagar *readContasPagar();

int createContasPagar(ContasPagar **contas, int codTransacao, time_t dataPagamento);

int showContasPagar(ContasPagar *contas, int codigo);

int deleteContasPagar(ContasPagar *contas, int codigo);

int contaPaga(ContasPagar *contas, int codigo, Transacao *transacoes, float *valor_em_caixa);

int buscaNovoIDContasPagar(ContasPagar *contas);

int temContasPagar(ContasPagar *contas);

int temContasPagas(ContasPagar *contas);

int saveContasPagarCSV(ContasPagar *contas_pagar, int tamanho); // Função que salva as contas a pagar em um arquivo CSV
ContasPagar *filterContasPagarIntervaloDatas(ContasPagar *contas, time_t dataInicio, time_t dataFim, int *Tamanho);

// Função que filtra as contas a pagar por intervalo de datas
// View contas a pagar

void menuContasPagar(ContasPagar **contas, Transacao *transacoes, float *valor_em_caixa);

void mostrarContaPagar(ContasPagar *contas, Transacao *transacoes);

void mostrarContasPaga(ContasPagar *contas, Transacao *transacoes);

void mostrarTodasContasPagar(ContasPagar *contas, Transacao *transacoes);

void apagarContaPagar(ContasPagar *contas, Transacao *transacoes);

void mostrarTodasContas(ContasPagar *contas, Transacao *transacoes);

void mostrarTodasContasPagas(ContasPagar *contas, Transacao *transacoes);

void pagarConta(ContasPagar *contas, Transacao *transacoes, float *valor_em_caixa);

void printContasPagar(ContasPagar *contas_pagar, int tam); // Função que exibe as informações de todas as contas a pagar
void filtrarContasPagar(ContasPagar *contas_pagar); // Função que filtra as contas a pagar

#endif //MODULO2_H
