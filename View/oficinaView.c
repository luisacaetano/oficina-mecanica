#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/oficina.h"
#include "../bibliotecas/utils.h"

// Função que exibe o menu para o usuário e permite gerenciar os dados da oficina
void menuOficina(Oficina **oficina) {
    // Se o tipo de arquivo não for MEM (memória), lê os dados da oficina
    if (getTipoArquivo() != MEM) {
        *oficina = readOficina();
    }

    int opcao = -1; // Inicializa a variável de opção
    while (opcao != 0) { // O loop continua até o usuário escolher sair

        // Exibe as opções de menu e lê a escolha do usuário
        printf("\t==== MENU OFICINA ====\n");
        opcao = lerInt("DIGITE A OPÇÃO DESEJADA:\n"
                    "\t\t\t1- CADASTRAR NOVA OFICINA\n"
                    "\t\t\t2- VER OFICINA\n"
                    "\t\t\t3- EDITAR OFICINA\n"
                    "\t\t\t4- EXCLUIR OFICINA\n"
                    "\t\t\t0- SAIR\n"
                    "=>");


        // Executa a ação de acordo com a opção escolhida
        switch (opcao) {
            case 1:
                novaOficina(*oficina); // Cadastra uma nova oficina
                if (getTipoArquivo() != MEM) *oficina = readOficina(); // Atualiza os dados da oficina
                opcao = -1;
                break;

            case 2:
                mostrarOficina(*oficina); // Exibe os dados da oficina
                opcao = -1;
                break;

            case 3:
                editarOficina(*oficina); // Edita os dados da oficina
                opcao = -1;
                break;

            case 4:
                apagarOficina(oficina); // Exclui os dados da oficina
                opcao = -1;
                break;
        }
    }
}

// Função para cadastrar uma nova oficina
void novaOficina(Oficina *oficina) {
    // Aloca memória para a nova oficina
    Oficina *novaOficina = malloc(sizeof(Oficina));
    if (novaOficina == NULL) {
        printf("Erro ao alocar memória para a nova oficina!\n");
        return;
    }

    // Solicita ao usuário os dados da nova oficina
    strcpy(novaOficina->nome, lerString("Digite o nome da oficina: "));
    strcpy(novaOficina->cpf_cnpj, lerString("Digite o CPF ou CNPJ da oficina: "));
    strcpy(novaOficina->endereco, lerString("Digite o endereço completo da oficina: "));
    strcpy(novaOficina->telefone, lerString("Digite o telefone da oficina: "));
    strcpy(novaOficina->email, lerString("Digite o email da oficina: "));
    novaOficina->porcentagem_lucro = lerFloat("Digite a porcentagem de lucro da oficina: ");

    // Chama a função para criar a oficina e verifica o sucesso
    if (createOficina(&oficina, novaOficina) != FALSE) {
        printf("Oficina cadastrada com sucesso!\n");
    } else {
        printf("Erro no cadastro da oficina!\n");
    }

    free(novaOficina); // Libera a memória da nova oficina após o cadastro
}

// Função para exibir os dados da oficina
void mostrarOficina(Oficina *oficina) {
    if (hasOficina() != FALSE) { // Verifica se existe uma oficina cadastrada
        // Exibe os dados da oficina
        printf("Nome: %s\n"
               "CPF/CNPJ: %s\n"
               "Endereço: %s\n"
               "Telefone: %s\n"
               "Email: %s\n"
               "Porcentagem de Lucro: %.2f\n",
               oficina->nome,
               oficina->cpf_cnpj,
               oficina->endereco,
               oficina->telefone,
               oficina->email,
               oficina->porcentagem_lucro);
    } else {
        printf("Dados da oficina não cadastrados\n"); // Caso não haja oficina cadastrada
    }
}

// Função para editar os dados da oficina
void editarOficina(Oficina *oficina) {
    if (oficina == NULL) { // Se não houver oficina, exibe uma mensagem de erro
        printf("Nenhuma oficina cadastrada\n");
        return;
    }

    // Aloca memória para armazenar os novos dados da oficina
    Oficina *novaOficina = malloc(sizeof(Oficina));
    if (novaOficina == NULL) {
        printf("Erro ao alocar memória para a edição da oficina\n");
        return;
    }

    // Pergunta ao usuário se deseja editar os dados da oficina e atualiza os campos conforme necessário
    printf("O nome da Oficina é: %s\n", oficina->nome);
    if (lerInt("Deseja editar? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaOficina->nome, lerString("Digite o novo nome da oficina: "));
    } else {
        strcpy(novaOficina->nome, oficina->nome);
    }

    printf("O CPF/CNPJ atual da Oficina é: %s\n", oficina->cpf_cnpj);
    if (lerInt("Deseja editar o CPF/CNPJ da oficina? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaOficina->cpf_cnpj, lerString("Digite o novo CPF/CNPJ da oficina: "));
    } else {
        strcpy(novaOficina->cpf_cnpj, oficina->cpf_cnpj);
    }

    printf("O endereço atual da Oficina é: %s\n", oficina->endereco);
    if (lerInt("Deseja editar o endereço da oficina? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaOficina->endereco, lerString("Digite o novo endereço da oficina: "));
    } else {
        strcpy(novaOficina->endereco, oficina->endereco);
    }

    printf("O telefone atual da Oficina é: %s\n", oficina->telefone);
    if (lerInt("Deseja editar o telefone da oficina? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaOficina->telefone, lerString("Digite o novo telefone da oficina: "));
    } else {
        strcpy(novaOficina->telefone, oficina->telefone);
    }

    printf("O email atual da Oficina é: %s\n", oficina->email);
    if (lerInt("Deseja editar o email da oficina? (1 - Sim, 0 - Não): ") == TRUE) {
        strcpy(novaOficina->email, lerString("Digite o novo email da oficina: "));
    } else {
        strcpy(novaOficina->email, oficina->email);
    }

    printf("A porcentagem atual da Oficina é: %.2f\n", oficina->porcentagem_lucro);
    if (lerInt("Deseja editar a porcentagem de lucro da oficina? (1 - Sim, 0 - Não): ") == TRUE) {
        novaOficina->porcentagem_lucro = lerFloat("Digite a nova porcentagem de lucro da oficina: ");
    } else {
        novaOficina->porcentagem_lucro = oficina->porcentagem_lucro;
    }

    // Chama a função para atualizar a oficina e verifica o sucesso
    if (updateOficina(&oficina, novaOficina)) {
        printf("Erro na edição dos dados da oficina!\n");
    } else {
        printf("Dados da oficina editados com sucesso!\n");
        *oficina = *novaOficina; // Atualiza os dados da oficina original
    }

    free(novaOficina); // Libera a memória da oficina temporária
}

// Função para excluir a oficina
void apagarOficina(Oficina **oficina) {
    *oficina = deleteOficina(); // Chama a função para deletar a oficina
    if (*oficina != NULL) {
        printf("Erro ao deletar dados da oficina!\n");
    } else {
        printf("Dados apagados com sucesso!\n"); // Exibe sucesso na exclusão
    }
}
