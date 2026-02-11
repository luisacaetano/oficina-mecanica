#ifndef FORNECEDOR_H
#define FORNECEDOR_H

// Estrutura para armazenar os dados de um fornecedor
typedef struct {
    int codigo;                  // Código único do fornecedor
    char nome_fantasia[50];      // Nome fantasia do fornecedor
    char razao_social[50];       // Razão social do fornecedor
    int incricao_estadual;       // Inscrição estadual do fornecedor
    char cnpj[19];               // CNPJ do fornecedor
    char endereco[60];           // Endereço do fornecedor
    char telefone[20];           // Telefone do fornecedor
    char email[50];              // E-mail do fornecedor
    int ativo;                   // Status do fornecedor (ativo ou inativo)
} Fornecedor;

// Model fornecedor

Fornecedor *migraDadosFornecedor();                    // Função que migra dados entre diferentes tipos de arquivo
Fornecedor* getFornecedores();                        // Função que retorna a lista de fornecedores
void setFornecedores(Fornecedor *fornecedores);       // Função que atualiza a lista de fornecedores
int getTamanhoFornecedores();                         // Função que retorna o número de fornecedores cadastrados
void setTamanhoFornecedores();                        // Função que incrementa o número de fornecedores cadastrados
void editTamanhoFornecedores(int tamanho);            // Função que atualiza o número de fornecedores cadastrados
void escrever_arquivo_txt_fornecedor(FILE *buffer, Fornecedor *fornecedores);  // Função para escrever no arquivo txt
Fornecedor *ler_arquivo_txt_fornecedor(FILE *buffer);  // Função que lê os dados de um fornecedor do arquivo txt
Fornecedor *ler_arquivo_bin_fornecedor(FILE *buffer);  // Função que lê os dados de um fornecedor do arquivo binário
void escrever_arquivo_bin_fornecedor(FILE *buffer, Fornecedor *fornecedores); // Função para escrever no arquivo binário

// Controller fornecedores

Fornecedor* readFornecedores();                       // Função que retorna todos os fornecedores cadastrados
int createFornecedor(Fornecedor **fornecedores, Fornecedor *fornecedor);  // Função para criar um novo fornecedor
int showFornecedor(Fornecedor *fornecedores, int codigo);  // Função que mostra os dados de um fornecedor baseado no código
int updateFornecedor(Fornecedor *fornecedores, Fornecedor *fornecedor);  // Função que atualiza os dados de um fornecedor
int deleteFornecedor(Fornecedor* fornecedores, int codigo);  // Função que exclui um fornecedor com base no código
int buscaNovoIDFornecedor(Fornecedor * fornecedores);  // Função que encontra um ID válido para um novo fornecedor
Fornecedor * filterFornecedorNomeFantasia(Fornecedor *fornecedores, char * nome, int *tamanho);
int saveFornecedorCSV(Fornecedor * fornecedor, int tamanho);  // Função que salva os dados de um fornecedor em um arquivo CSV
// View fornecedor

void menuFornecedor(Fornecedor **fornecedores);       // Menu que exibe as opções de interação com os fornecedores
int novoFornecedor(Fornecedor **fornecedores);      // Função para cadastrar um novo fornecedor
void mostrarFornecedor(Fornecedor *fornecedores);    // Função que exibe os dados de um fornecedor
void editarFornecedor(Fornecedor *fornecedores);     // Função que permite editar os dados de um fornecedor
void apagarFornecedor(Fornecedor *fornecedores);     // Função que apaga os dados de um fornecedor
void mostrarTodosFornecedores(Fornecedor * fornecedores);  // Função que exibe todos os fornecedores cadastrados
void filtrarFornecedorIDNomeFantasia(Fornecedor *fornecedores);  // Função que filtra os fornecedores por ID ou nome fantasia
#endif //FORNECEDOR_H
