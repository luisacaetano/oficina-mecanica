#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/funcionario.h"

// Função que exibe o menu de opções para o usuário e realiza as ações com base na escolha
void menuFuncionario(Funcionario ** funcionarios) {
    if (getTipoArquivo() != MEM) {
        *funcionarios = readFuncionarios();
    }

    int opcao = -1;
    while (opcao != 0) {
        // Exibe o menu de opções para o usuário
        printf("\t==== MENU FUNCIONÁRIO ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
                    "\t\t\t1- CADASTRAR NOVO FUNCIONÁRIO\n"
                    "\t\t\t2- VER FUNCIONÁRIOS\n"
                    "\t\t\t3- EDITAR FUNCIONÁRIO\n"
                    "\t\t\t4- EXCLUIR FUNCIONÁRIO\n"
                    "\t\t\t0- SAIR\n"
                    "=>");

        switch (opcao) {
            case 1:
                novoFuncionario(funcionarios);
                if (getTipoArquivo() != MEM) *funcionarios = getFuncionarios();
                break;
            case 2:
                mostrarFuncionario(*funcionarios);
                break;
            case 3:
                editarFuncionario(*funcionarios);
                break;
            case 4:
                apagarFuncionario(*funcionarios);
                break;
        }
    }
}

// Função para cadastrar um novo funcionário
void novoFuncionario(Funcionario **funcionarios) {
    Funcionario *funcionario = malloc(sizeof(Funcionario));
    // Solicita os dados do novo funcionário
    strcpy(funcionario->nome, lerString("Digite o nome completo do funcionário: "));
    strcpy(funcionario->cpf, lerString("Digite o cpf do funcionário: "));
    strcpy(funcionario->endereco, lerString("Digite o endereço do funcionário: "));
    strcpy(funcionario->telefone, lerString("Digite o telefone do funcionário: "));
    strcpy(funcionario->cargo, lerString("Digite o cargo do funcionário: "));
    funcionario->salario = atoi(lerString("Digite o salário do funcionário: "));
    funcionario->ativo = 1; // Define o funcionario como ativo
    // Tenta cadastrar o novo funcionário
    if (createFuncionario(funcionarios, funcionario) != FALSE) {
        printf("Funcionario cadastrado com sucesso!\n");
    } else {
        printf("Erro no cadastro do funcionario!\n");
    }

    free(funcionario); // Libera a memória do funcionario após o cadastro
}
// Função para exibir os funcionários cadastrados
void mostrarFuncionario(Funcionario *funcionarios) {
    if (getTamanhoFuncionarios() == 0) {
        printf("Nenhum funcionário cadastrado até o momento\n");
        return;
    }

    printf("\tFUNCIONARIOS:\n");
    mostrarTodosFuncionarios(funcionarios);

    int codigo = lerInt("Digite o código do funcionário que você deseja ver: ");
    int posicao = showFuncionario(funcionarios, codigo);

    if (posicao != FALSE) {
        // Exibe os detalhes do funcionário
        printf("Código: %d\n"
               "Nome Completo: %s\n"
               "CPF: %s\n"
               "Endereço: %s\n"
               "Telefone: %s\n"
               "Cargo: %s\n"
               "Salário: %f\n",
               funcionarios[posicao].codigo,
               funcionarios[posicao].nome,
               funcionarios[posicao].cpf,
               funcionarios[posicao].endereco,
               funcionarios[posicao].telefone,
               funcionarios[posicao].cargo,
               funcionarios[posicao].salario
        ); // Exibe se o funcionário está ativo
    }
}
// Função para editar os dados de um funcionário
void editarFuncionario(Funcionario *funcionarios) {
    if (getTamanhoFuncionarios() == 0) {
        printf("Nenhum funcionário cadastrado\n");
        return;
    }

    Funcionario *funcionario = malloc(sizeof(Funcionario));// Aloca memória para armazenar as modificações

    if (funcionario == NULL) {
        printf("Erro ao alocar memória para a edição do funcionário\n");
        return;
    }

    printf("\tFUNCIONÁRIOS:\n");
    mostrarTodosFuncionarios(funcionarios);

    funcionario->codigo = lerInt("Digite o código do funcionário que você deseja editar: ");

    int posicao = showFuncionario(funcionarios, funcionario->codigo);

    if (posicao == FALSE) {
        printf("Funcionário não encontrado.\n");
        free(funcionario);
        return;
    }

    printf("O nome do funcionário é: %s\n", funcionarios[posicao].nome);
    int opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(funcionario->nome, lerString("Digite o novo nome do funcionário: "));
    } else {
        strcpy(funcionario->nome, funcionarios[posicao].nome);
    }

    printf("O CPF do funcionário é: %s\n", funcionarios[posicao].cpf);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(funcionario->cpf, lerString("Digite o novo CPF do funcionário: "));
    } else {
        strcpy(funcionario->cpf, funcionarios[posicao].cpf);
    }

    printf("O endereço do funcionário é: %s\n", funcionarios[posicao].endereco);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(funcionario->endereco, lerString("Digite o novo endereço do funcionário: "));
    } else {
        strcpy(funcionario->endereco, funcionarios[posicao].endereco);
    }

    printf("O telefone do funcionário é: %s\n", funcionarios[posicao].telefone);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(funcionario->telefone, lerString("Digite o novo telefone do funcionário: "));
    } else {
        strcpy(funcionario->telefone, funcionarios[posicao].telefone);
    }

    printf("O cargo do funcionário é: %s\n", funcionarios[posicao].cargo);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        strcpy(funcionario->cargo, lerString("Digite o novo cargo do funcionário: "));
    } else {
        strcpy(funcionario->cargo, funcionarios[posicao].cargo);
    }

    printf("O salário do funcionário é: %.2f\n", funcionarios[posicao].salario);
    opcao = lerInt("Deseja editar? (1 - Sim, 0 - Não): ");
    if (opcao == TRUE) {
        funcionario->salario = lerFloat("Digite o novo salário do funcionário: ");
    } else {
        funcionario->salario = funcionarios[posicao].salario;
    }

    if (updateFuncionario(funcionarios, funcionario) == FALSE) {
        printf("Erro na edição dos dados do funcionário\n");
    } else {
        printf("Funcionário editado com sucesso!\n");
    }

    free(funcionario);
}
// Função para apagar um funcionário do sistema
void apagarFuncionario(Funcionario *funcionarios) {
    if (getTamanhoFuncionarios() == 0) {
        printf("Nenhum funcionário cadastrado\n");
        return;
    }
    printf("\tFUNCIONÁRIOS:\n");
    mostrarTodosFuncionarios(funcionarios);

    int codigo = lerInt("Digite o código do funcionário que você deseja apagar: ");
    if (deleteFuncionario(funcionarios, codigo) == TRUE) {// Exclui o funcionário
        printf("Funcionário apagado com sucesso\n");
    } else {
        printf("Funcionário não existe\n");
    }
}
// Função para exibir todos os funcionários cadastrados
void mostrarTodosFuncionarios(Funcionario *funcionarios) {
    for (int i = 0; i < getTamanhoFuncionarios(); i++) {
        if (funcionarios[i].ativo != FALSE)
            printf("Fornecedor: %s Codigo: %d\n", funcionarios[i].nome, funcionarios[i].codigo);
    }
}

void filtrarFuncionarioIDNome(Funcionario *funcionarios) {
    printf("\t==== FILTRO DE FUNCIONARIO ====\n");
    int opcao = lerInt("Você seja filtrar por Codigo ou por nome?\n"
        "1 - Codigo\n"
        "2 - Nome\n"
        "=>");
    if (opcao == 1) {
        int id = lerInt("Qual o Codigo do fornecedor que você deseja filtrar?\n=>");
        int posicao = showFuncionario(funcionarios, id);
        if (posicao != FALSE) {
            printf("Código: %d\n"
                   "Nome: %s\n"
                   "CPF: %s\n"
                   "Endereço: %s\n"
                   "Telefone: %s\n"
                   "Cargo: %s\n"
                   "Salario: %f\n",
                   funcionarios[posicao].codigo,
                   funcionarios[posicao].nome,
                   funcionarios[posicao].cpf,
                   funcionarios[posicao].endereco,
                   funcionarios[posicao].telefone,
                   funcionarios[posicao].cargo,
                     funcionarios[posicao].salario
            ); // Exibe se o cliente está ativo
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveFuncionarioCSV(&funcionarios[posicao], 1);
            }
        } else {
            printf("Codigo não encontrado\n");
        }
    } else if (opcao == 2) {
        char *nome = lerString("Qual o nome do funcionario que você deseja filtrar?\n=>");
        int tamanho = 0;
        Funcionario *funcionarioFiltrados = filterFuncionarioNome(funcionarios, nome, &tamanho);
        if (funcionarioFiltrados != NULL) {
            for (int i = 0; i < tamanho; i++) {
                printf("Código: %d\n"
                   "Nome: %s\n"
                   "CPF: %s\n"
                   "Endereço: %s\n"
                   "Telefone: %s\n"
                   "Cargo: %s\n"
                   "Salario: %f\n",
                   funcionarioFiltrados[i].codigo,
                   funcionarioFiltrados[i].nome,
                   funcionarioFiltrados[i].cpf,
                   funcionarioFiltrados[i].endereco,
                   funcionarioFiltrados[i].telefone,
                   funcionarioFiltrados[i].cargo,
                     funcionarioFiltrados[i].salario
            );// Exibe se o cliente está ativo
            }
            if (lerInt("Deseja salvar no CSV? 1 - Sim, 0 - Não\n=>") == 1) {
                saveFuncionarioCSV(funcionarioFiltrados, tamanho);
            }
        } else {
            printf("Nome não encontrado\n");
        }
    }
    else {
        printf("Opção inválida\n");
    }
}

