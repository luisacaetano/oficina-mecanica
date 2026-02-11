#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/cliente.h"

void menuCliente(Cliente **clientes) {
    if (getTipoArquivo() != 3) {
        *clientes = readClientes();
    }
    int opcao = -1;
    while (opcao != 0) {
        printf("\t==== MENU CLIENTE ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
            "\t\t\t1- CADASTRAR NOVO CLIENTE\n"
            "\t\t\t2- VER CLIENTE\n"
            "\t\t\t3- EDITAR CLIENTE\n"
            "\t\t\t4- EXCLUIR CLIENTE\n"
            "\t\t\t0- SAIR\n"
            "=>");
        switch (opcao) {
            case 1:
                novoCliente(clientes);
                if (getTipoArquivo() != MEM) *clientes = getClientes();
                opcao = -1;
                break;

            case 2:
                mostrarCliente(*clientes);
                opcao = -1;
                break;
            case 3:
                editarCLiente(*clientes);
                opcao = -1;
                break;
            case 4:
                apagarCliente(*clientes);
                opcao = -1;
                break;
        }
    }
    //free(clientes);
    //clientes = NULL;
}

void novoCliente(Cliente **clientes) {
    Cliente *cliente = malloc(sizeof(Cliente));
    strcpy(cliente->nome, lerString("Digite o nome do cliente: "));
    strcpy(cliente->cpf_cnpj, lerString("Digite o CPF/CNPJ do cliente: "));
    strcpy(cliente->endereco, lerString("Digite o endereço completo do cliente: "));
    strcpy(cliente->telefone, lerString("Digite o telefone do cliente: "));
    strcpy(cliente->email, lerString("Digite o email do cliente: "));
    cliente->ativo = 1; // Define o cliente como ativo
    if (createCliente(clientes, cliente) != FALSE) {
        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Erro no cadastro do cliente!\n");
    }

    free(cliente); // Libera a memória do cliente após o cadastro
}


void mostrarCliente(Cliente *clientes) {
    if (getTamanhoClientes() == 0) {
        printf("Nenhum cliente cadastrado até o momento\n");
        return;
    }
    printf("\tCLIENTES:\n");
    mostrarTodosClientes(clientes);

    int codigo = lerInt("Digite o código do cliente que você deseja ver: ");
    int posicao = showCliente(clientes, codigo);

    if (posicao != FALSE) {
        printf("Código: %d\n"
               "Nome: %s\n"
               "Endereço: %s\n"
               "CPF/CNPJ: %s\n"
               "Telefone: %s\n"
               "E-mail: %s\n",
               clientes[posicao].codigo,
               clientes[posicao].nome,
               clientes[posicao].endereco,
               clientes[posicao].cpf_cnpj,
               clientes[posicao].telefone,
               clientes[posicao].email
        ); // Exibe se o cliente está ativo
    } else {
        printf("Cliente não encontrado!\n");
    }
}

void editarCLiente(Cliente *clientes) {
    if (getTamanhoClientes() == 0) {
        printf("Nenhum cliente cadastrado\n");
        return;
    }

    Cliente *cliente = malloc(sizeof(Cliente));
    if (cliente == NULL) {
        printf("Erro ao alocar memória para a edição do cliente\n");
        return;
    }

    printf("\tCLIENTES:\n");
    mostrarTodosClientes(clientes);

    cliente->codigo = lerInt("Digite o código do cliente que você deseja editar: ");

    // Procurar o cliente
    int posicao = showCliente(clientes, cliente->codigo);

    if (posicao == FALSE) {
        printf("Cliente não encontrado.\n");
        free(cliente);
        return;
    }

    // Edição do campo `nome`
    printf("O nome do cliente é: %s\n", clientes[posicao].nome);
    int opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(cliente->nome, lerString("Digite o novo nome do cliente: "));
    } else {
        strcpy(cliente->nome, clientes[posicao].nome);
    }

    // Edição do campo `cpf_cnpj`
    printf("O CPF/CNPJ do cliente é: %s\n", clientes[posicao].cpf_cnpj);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(cliente->cpf_cnpj, lerString("Digite o novo CPF/CNPJ do cliente: "));
    } else {
        strcpy(cliente->cpf_cnpj, clientes[posicao].cpf_cnpj);
    }

    // Edição do campo `endereco`
    printf("O endereço do cliente é: %s\n", clientes[posicao].endereco);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(cliente->endereco, lerString("Digite o novo endereço completo do cliente: "));
    } else {
        strcpy(cliente->endereco, clientes[posicao].endereco);
    }

    // Edição do campo `telefone`
    printf("O telefone do cliente é: %s\n", clientes[posicao].telefone);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(cliente->telefone, lerString("Digite o novo telefone do cliente: "));
    } else {
        strcpy(cliente->telefone, clientes[posicao].telefone);
    }

    // Edição do campo `email`
    printf("O email do cliente é: %s\n", clientes[posicao].email);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(cliente->email, lerString("Digite o novo email do cliente: "));
    } else {
        strcpy(cliente->email, clientes[posicao].email);
    }

    if (updateCliente(clientes, cliente) == FALSE) {
        printf("Erro na edição dos dados do cliente\n");
    } else {
        printf("Cliente editado com sucesso!\n");
    }

    free(cliente); // Libera a memória alocada
}

void apagarCliente(Cliente *clientes) {
    if (getTamanhoClientes() == 0) {
        printf("Nenhum cliente cadastrado\n");
        return;
    }
    printf("\tCLIENTES:\n");
    mostrarTodosClientes(clientes);
    int codigo = lerInt("Digite o código do cliente que você deseja apagar: ");
    if (deleteCliente(clientes, codigo) == TRUE) {
        printf("Cliente apagado com sucesso\n");
    } else {
        printf("Cliente não existe\n");
    }
}

void mostrarTodosClientes(Cliente *clientes) {
    for (int i = 0; i < getTamanhoClientes(); i++) {
        if (clientes[i].ativo != FALSE)
            printf("Cliente: %s Codigo: %d\n", clientes[i].nome, clientes[i].codigo);
    }
}

void filtrarClienteIDNome(Cliente *clientes) {
    printf("\t==== FILTRO DE CLIENTE ====\n");
    int opcao = lerInt("Você seja filtrar por Codigo ou por nome?\n"
        "1 - Codigo\n"
        "2 - Nome\n"
        "=>");
    if (opcao == 1) {
        int id = lerInt("Qual o Codigo do cliente que você deseja filtrar?\n=>");
        int posicao = showCliente(clientes, id);
        if (posicao != FALSE) {
            printf("Código: %d\n"
                   "Nome: %s\n"
                   "Endereço: %s\n"
                   "CPF/CNPJ: %s\n"
                   "Telefone: %s\n"
                   "E-mail: %s\n",
                   clientes[posicao].codigo,
                   clientes[posicao].nome,
                   clientes[posicao].endereco,
                   clientes[posicao].cpf_cnpj,
                   clientes[posicao].telefone,
                   clientes[posicao].email
            ); // Exibe se o cliente está ativo
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveClienteCSV(&clientes[posicao], 1);
            }
        } else {
            printf("Codigo não encontrado\n");
        }
    } else if (opcao == 2) {
        char *nome = lerString("Qual o nome do cliente que você deseja filtrar?\n=>");
        int tamanho = 0;
        Cliente *clientesFiltrados = filterClienteNome(clientes, nome, &tamanho);
        if (clientesFiltrados != NULL) {
            for (int i = 0; i < tamanho; i++) {
                printf("Código: %d\n"
                       "Nome: %s\n"
                       "Endereço: %s\n"
                       "CPF/CNPJ: %s\n"
                       "Telefone: %s\n"
                       "E-mail: %s\n",
                       clientesFiltrados[i].codigo,
                       clientesFiltrados[i].nome,
                       clientesFiltrados[i].endereco,
                       clientesFiltrados[i].cpf_cnpj,
                       clientesFiltrados[i].telefone,
                       clientesFiltrados[i].email
                ); // Exibe se o cliente está ativo
            }
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveClienteCSV(clientesFiltrados, tamanho);
            }
        } else {
            printf("Nome não encontrado\n");
        }
    }
    else {
        printf("Opção inválida\n");
    }
}
