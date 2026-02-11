
#ifndef CLIENTE_H
#define CLIENTE_H

// Definição de Cliente
typedef struct {
    int codigo;
    char nome[100];
    char cpf_cnpj[20];
    char endereco[150];
    char telefone[15];
    char email[100];
    int ativo;
} Cliente;
// Model cliente
int getTamanhoClientes();

void setTamanhoClientes();

Cliente* migraDadosCliente();

void setClientes(Cliente *cliente);

Cliente* getClientes();

Cliente *ler_arquivo_txt_cliente(FILE *buffer);
void escrever_arquivo_txt_cliente(FILE *buffer, Cliente *clientes);
Cliente *ler_arquivo_bin_cliente(FILE *buffer);
void *escrever_arquivo_bin_cliente(FILE *buffer, Cliente *clientes);
void editTamanhoClientes(int tamanho);
//Controller cliente
Cliente* readClientes();

int createCliente(Cliente ** clientes, Cliente *cliente);

int showCliente(Cliente * clientes, int id);

int  updateCliente(Cliente *clientes,Cliente * cliente);

int deleteCliente(Cliente* clientes, int id);

int buscaNovoIDCliente(Cliente * clientes);

Cliente *filterClienteNome(Cliente *clientes, char *nome, int *tamanho);

int saveClienteCSV(Cliente * clientes, int tamanho);

//View cliente

void menuCliente(Cliente **clientes);

void novoCliente(Cliente **clientes);

void mostrarCliente(Cliente *clientes);

void apagarCliente(Cliente *clientes);

void editarCLiente(Cliente * clientes);

void mostrarTodosClientes(Cliente * clientes);

void filtrarClienteIDNome(Cliente *clientes);

#endif //CLIENTE_H
