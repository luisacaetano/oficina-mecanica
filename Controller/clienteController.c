#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/cliente.h"
#include "../bibliotecas/utils.h"

// Função para ler os clientes existentes
Cliente* readClientes() {
    Cliente *clientes = NULL;   // Inicializa o ponteiro de clientes
    clientes = getClientes();   // Obtém a lista de clientes de um arquivo ou memória
    return clientes;            // Retorna o ponteiro para os clientes
}

// Função para criar um novo cliente
int createCliente(Cliente **clientes, Cliente *cliente) {
    int tamanhoAtual = getTamanhoClientes(); // Obtém o tamanho atual da lista de clientes

    // Realoca a memória para incluir o novo cliente
    Cliente *novoClientes = realloc(*clientes, (tamanhoAtual + 1) * sizeof(Cliente));
    if (novoClientes == NULL) {
        printf("Erro ao alocar memória para clientes.\n");
        return FALSE; // Retorna -1 em caso de falha na alocação
    }

    *clientes = novoClientes; // Atualiza o ponteiro para o novo bloco de memória
    int index = tamanhoAtual; // O índice do novo cliente é igual ao tamanho atual da lista

    // Preenche os dados do novo cliente
    (*clientes)[index].codigo = buscaNovoIDCliente(*clientes);
    strcpy((*clientes)[index].nome, cliente->nome);
    strcpy((*clientes)[index].cpf_cnpj, cliente->cpf_cnpj);
    strcpy((*clientes)[index].endereco, cliente->endereco);
    strcpy((*clientes)[index].telefone, cliente->telefone);
    strcpy((*clientes)[index].email, cliente->email);
    (*clientes)[index].ativo = 1;  // Marca o cliente como ativo

    setTamanhoClientes(); // Atualiza o tamanho total da lista de clientes

    // Salva os dados no arquivo se o modo de armazenamento não for somente memória
    if (getTipoArquivo() != MEM) setClientes(*clientes);

    return TRUE;
}

// Função para exibir um cliente com base no código
int showCliente(Cliente *clientes, int codigo) {
    if (clientes == NULL) return FALSE; // Retorna FALSE se a lista de clientes for vazia

    int posicao = 0;
    int tamanho = getTamanhoClientes(); // Obtém o tamanho atual da lista de clientes

    // Procura pelo cliente com o código especificado
    while (posicao < tamanho && clientes[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se o cliente foi encontrado e se está ativo
    if (posicao == tamanho || clientes[posicao].ativo == FALSE)
        return FALSE;

    return posicao; // Retorna a posição do cliente na lista
}

// Função para atualizar os dados de um cliente existente
int updateCliente(Cliente *clientes, Cliente *cliente) {
    int posicao = showCliente(clientes, cliente->codigo); // Busca o cliente pelo código

    if (posicao == FALSE) return FALSE; // Retorna FALSE se o cliente não for encontrado ou estiver inativo

    // Atualiza os campos do cliente
    clientes[posicao].codigo = cliente->codigo;
    strcpy(clientes[posicao].nome, cliente->nome);
    strcpy(clientes[posicao].cpf_cnpj, cliente->cpf_cnpj);
    strcpy(clientes[posicao].endereco, cliente->endereco);
    strcpy(clientes[posicao].telefone, cliente->telefone);
    strcpy(clientes[posicao].email, cliente->email);
    clientes[posicao].ativo = cliente->ativo; // Atualiza o status de ativo

    // Salva os dados no arquivo se o modo de armazenamento não for somente memória
    if (getTipoArquivo() != MEM) setClientes(clientes);
    return TRUE;
}

// Função para deletar (ou desativar) um cliente
int deleteCliente(Cliente* clientes, int codigo) {
    int posicao = showCliente(clientes, codigo); // Busca o cliente pelo código

    if (posicao == FALSE) return FALSE; // Retorna FALSE se o cliente não for encontrado ou já estiver inativo

    clientes[posicao].ativo = FALSE; // Marca o cliente como inativo
    if (getTipoArquivo() != MEM) setClientes(clientes); // Salva as alterações se necessário
    return TRUE;
}

// Função para gerar um novo ID exclusivo para um cliente
int buscaNovoIDCliente(Cliente *clientes) {
    int maior = 1; // Inicializa o maior ID como 1
    if (getTamanhoClientes() == 0) return maior; // Retorna 1 se não houver clientes

    // Itera sobre a lista de clientes para encontrar o maior ID
    for (int i = 0; i < getTamanhoClientes(); i++) {
        if (maior <= clientes[i].codigo) {
            maior = clientes[i].codigo + 1; // Define o próximo ID como maior + 1
        }
    }
    return maior; // Retorna o novo ID
}

int saveClienteCSV(Cliente * clientes, int tamanho) {
    char ** string = malloc(sizeof(char) * tamanho+1);
    string[0] = "codigo;nome;cpf_cnpj;telefone;email;endereco";
    for(int i = 1; i< tamanho+1; i++){
        string[i] = malloc(sizeof(char) * 150);
        string[i] = transformaString(&clientes[i-1].codigo, 'i');
        string[i] = concatenarStringPontoEVirgula(string[i], clientes[i-1].nome);
        string[i] = concatenarStringPontoEVirgula(string[i], clientes[i-1].cpf_cnpj);
        string[i] = concatenarStringPontoEVirgula(string[i], clientes[i-1].telefone);
        string[i] = concatenarStringPontoEVirgula(string[i], clientes[i-1].email);
        string[i] = concatenarStringPontoEVirgula(string[i], clientes[i-1].endereco);
    }
    escreverCSV(string, "cliente", tamanho+1);
    return TRUE;
}
Cliente * filterClienteNome(Cliente *clientes, char * nome, int *tamanho){
    Cliente * clientesFiltrados = malloc(sizeof(Cliente));
    int tamanhoTotal = getTamanhoClientes();

    for(int i = 0; i < tamanhoTotal; i++){
        if(equalsString(clientes[i].nome, nome)== TRUE){
            if(clientes[i].ativo == TRUE) {
                *(tamanho) = *(tamanho) + 1;
                clientesFiltrados = realloc(clientesFiltrados, (*tamanho) * sizeof(Cliente));
                if(clientesFiltrados == NULL){
                    printf("Erro ao alocar memória para clientes filtrados.\n");
                    return NULL;
                }
                clientesFiltrados[*tamanho-1] = clientes[i];
            }
        }

    }
    if (*tamanho == 0) return NULL;

    return clientesFiltrados;
}

