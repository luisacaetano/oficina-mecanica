#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

typedef struct {
    int codigo;                // Código identificador do funcionário
    char nome[100];            // Nome completo do funcionário
    char cpf[11];              // CPF do funcionário (11 dígitos, sem formatação)
    char endereco[150];        // Endereço residencial do funcionário
    char telefone[15];         // Telefone de contato do funcionário
    char cargo[100];           // Cargo ocupado pelo funcionário
    float salario;             // Salário do funcionário
    int ativo;                 // Indicador de ativo (1 para ativo, -1 para inativo)
} Funcionario;

// Model Funcionario

int getTamanhoFuncionarios();                          // Retorna a quantidade de funcionários cadastrados
void setTamanhoFuncionarios();                         // Incrementa a quantidade de funcionários cadastrados
void editTamanhoFuncionarios(int tamanho);             // Atualiza a quantidade de funcionários cadastrados
Funcionario* migraDadosFuncionario();                 // Migra os dados dos funcionários entre diferentes formatos de arquivo
void setFuncionarios(Funcionario *funcionario);        // Atualiza a lista de funcionários cadastrados
Funcionario* getFuncionarios();                       // Retorna todos os funcionários cadastrados
Funcionario* ler_arquivo_txt_funcionario(FILE *buffer); // Lê os dados de funcionários de um arquivo de texto
void escrever_arquivo_txt_funcionario(FILE *buffer, Funcionario *funcionarios); // Escreve os dados de funcionários em um arquivo de texto
Funcionario* ler_arquivo_bin_funcionario(FILE *buffer); // Lê os dados de funcionários de um arquivo binário
void escrever_arquivo_bin_funcionario(FILE *buffer, Funcionario *funcionarios); // Escreve os dados de funcionários em um arquivo binário

// Controller Funcionario

Funcionario* readFuncionarios();                      // Retorna a lista de funcionários cadastrados
int createFuncionario(Funcionario **funcionarios, Funcionario *funcionario); // Cadastra um novo funcionário
int showFuncionario(Funcionario *funcionarios, int id); // Exibe as informações de um funcionário pelo ID
int updateFuncionario(Funcionario *funcionarios, Funcionario *funcionario); // Atualiza os dados de um funcionário
int deleteFuncionario(Funcionario *funcionarios, int id); // Remove um funcionário pelo ID
int saveFuncionarioCSV(Funcionario * funcionario, int tamanho); // Salva os dados de um funcionário em um arquivo CSV
Funcionario * filterFuncionarioNome(Funcionario *funcionario, char * nome, int *tamanho);
// View Funcionario

void menuFuncionario(Funcionario **funcionarios);     // Exibe o menu para manipulação dos funcionários
void novoFuncionario(Funcionario **funcionarios);     // Interage com o usuário para cadastrar um novo funcionário
void mostrarFuncionario(Funcionario *funcionarios);   // Mostra as informações de um funcionário específico
void apagarFuncionario(Funcionario *funcionarios);    // Remove um funcionário
void editarFuncionario(Funcionario *funcionarios);    // Permite editar as informações de um funcionário
int buscaNovoIDFuncionario(Funcionario *funcionarios); // Busca um novo ID válido para cadastro de um funcionário
void mostrarTodosFuncionarios(Funcionario *funcionarios); // Exibe todos os funcionários cadastrados
void filtrarFuncionarioIDNome(Funcionario *funcionarios); // Filtra os funcionários por ID ou nome
#endif // FUNCIONARIO_H
