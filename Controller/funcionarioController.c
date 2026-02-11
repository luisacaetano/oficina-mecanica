#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/funcionario.h"
#include "../bibliotecas/utils.h"
// Função para ler a funcionario
Funcionario *readFuncionarios() {
    return getFuncionarios();;
}

// Função para criar e adicionar um novo funcionário à lista
int createFuncionario(Funcionario **funcionarios, Funcionario *funcionario) {
    int tamanhoAtual = getTamanhoFuncionarios();

    // Realoca a memória para acomodar um novo Funcionario
    Funcionario *novoFuncionarios = realloc(*funcionarios, (tamanhoAtual + 1) * sizeof(Funcionario));
    if (novoFuncionarios == NULL) {
        printf("Erro ao alocar memória para Funcionários.\n");
        return 0; // Retorna 0 indicando falha
    }

    *funcionarios = novoFuncionarios; // Atualiza o ponteiro de Funcionario com o novo endereço
    int index = tamanhoAtual; // Novo índice é o tamanho atual

    // Preenche os dados do novo Funcionario no array
    (*funcionarios)[index].codigo = buscaNovoIDFuncionario(*funcionarios);
    strcpy((*funcionarios)[index].nome, funcionario->nome);
    strcpy((*funcionarios)[index].cpf, funcionario->cpf);
    strcpy((*funcionarios)[index].endereco, funcionario->endereco);
    strcpy((*funcionarios)[index].telefone, funcionario->telefone);
    strcpy((*funcionarios)[index].cargo, funcionario->cargo);
    (*funcionarios)[index].salario = funcionario->salario;
    (*funcionarios)[index].ativo = 1; // Funcionário ativo ao ser criado

    setTamanhoFuncionarios(); // Atualiza o tamanho dos Funcionarios

    // Salva Funcionarios se o tipo de arquivo não for memória
    if (getTipoArquivo() != MEM) setFuncionarios(*funcionarios);
    return TRUE;
}

// Função para exibir os dados de um funcionário com base no código
int showFuncionario(Funcionario *funcionarios, int codigo) {
    if (funcionarios == NULL) return FALSE;

    int posicao = 0;
    int tamanho = getTamanhoFuncionarios();

    // Procura o Funcionario com o código especificado dentro do limite do array
    while (posicao < tamanho && funcionarios[posicao].codigo != codigo) {
        posicao++;
    }

    // Verifica se o Funcionario foi encontrado e se está ativo
    if (posicao == tamanho || funcionarios[posicao].ativo == FALSE)
        return FALSE;

    return posicao;
}

// Função para atualizar os dados de um funcionário
int updateFuncionario(Funcionario *funcionarios, Funcionario *funcionario) {
    int posicao = showFuncionario(funcionarios, funcionario->codigo);

    if (posicao == FALSE) return FALSE;

    // Atualiza os campos necessários
    funcionarios[posicao].codigo = funcionario->codigo;
    strcpy(funcionarios[posicao].nome, funcionario->nome);
    strcpy(funcionarios[posicao].cpf, funcionario->cpf);
    strcpy(funcionarios[posicao].endereco, funcionario->endereco);
    strcpy(funcionarios[posicao].telefone, funcionario->telefone);
    strcpy(funcionarios[posicao].cargo, funcionario->cargo);
    funcionarios[posicao].salario = funcionario->salario;
    funcionarios[posicao].ativo = funcionario->ativo; // Atualiza o campo ativo

    if (getTipoArquivo() != MEM) setFuncionarios(funcionarios);
    return TRUE;
}

// Função para deletar (desativar) um funcionário
int deleteFuncionario(Funcionario *funcionarios, int codigo) {
    int posicao = showFuncionario(funcionarios, codigo);

    if (posicao == FALSE) return FALSE; // Verifica se o Funcionario existe e está ativo

    funcionarios[posicao].ativo = FALSE; // Marca o Funcionario como inativo
    if (getTipoArquivo() != MEM) setFuncionarios(funcionarios);
    return TRUE;
}

// Função para buscar um novo ID único para um funcionário
int buscaNovoIDFuncionario(Funcionario *funcionarios) {
    int maior = 1; // Inicializa o maior ID com 1
    if (getTamanhoFuncionarios() == 0) return maior; // Se não há funcionários, retorna 1

    // Percorre a lista para encontrar o maior ID atual
    for (int i = 0; i < getTamanhoFuncionarios(); i++) {
        if (maior <= funcionarios[i].codigo) {
            maior = funcionarios[i].codigo + 1; // Atualiza o maior ID
        }
    }
    return maior; // Retorna o novo ID
}

int saveFuncionarioCSV(Funcionario *funcionario, int tamanho) {
    char **string = malloc(sizeof(char) * tamanho+1);
    string[0] = "codigo;nome;cpf;endereco;telefone;cargo;salario";
    for (int i = 1; i < tamanho+1; i++) {
        string[i] = malloc(sizeof(char) * 150);
        string[i] = transformaString(&funcionario[i-1].codigo, 'i');
        string[i] = concatenarStringPontoEVirgula(string[i], funcionario[i-1].nome);
        string[i] = concatenarStringPontoEVirgula(string[i], funcionario[i-1].cpf);
        string[i] = concatenarStringPontoEVirgula(string[i], funcionario[i-1].endereco);
        string[i] = concatenarStringPontoEVirgula(string[i], funcionario[i-1].telefone);
        string[i] = concatenarStringPontoEVirgula(string[i], funcionario[i-1].cargo);
        string[i] = concatenarStringPontoEVirgula(string[i], transformaString(&funcionario[i-1].salario, 'f'));
    }
    escreverCSV(string, "funcionario", tamanho+1);
    return TRUE;
}

Funcionario *filterFuncionarioNome(Funcionario *funcionario, char *nome, int *tamanho) {
    Funcionario *funcionarioFiltrados = malloc(sizeof(Funcionario));

    int tamanhoTotal = getTamanhoFuncionarios();

    for (int i = 0; i < tamanhoTotal; i++) {
        if (equalsString(funcionario[i].nome, nome) == TRUE) {
            if (funcionario[i].ativo == TRUE) {
                *(tamanho) = *(tamanho) + 1;
                funcionarioFiltrados = realloc(funcionarioFiltrados, (*tamanho) * sizeof(Funcionario));
                if (funcionarioFiltrados == NULL) {
                    printf("Erro ao alocar memória para funcionarios filtrados.\n");
                    return NULL;
                }
                funcionarioFiltrados[*tamanho - 1] = funcionario[i];
            }
        }
    }
    if (*tamanho == 0) return NULL;

    return funcionarioFiltrados;
}
