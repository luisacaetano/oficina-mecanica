#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/funcionario.h"

int qtdFuncionarios = 0;

// Função para retornar a quantidade de funcionários cadastrados
int getTamanhoFuncionarios() {
    return qtdFuncionarios;
}

// Função para incrementar a quantidade de funcionários cadastrados
void setTamanhoFuncionarios() {
    qtdFuncionarios++;
}
// Função para atualizar manualmente a quantidade de funcionários
void editTamanhoFuncionarios(int tamanho) {
    qtdFuncionarios = tamanho;
}

// Função para migrar dados de funcionários entre diferentes formatos de arquivo (TXT, BIN, MEM)
Funcionario *migraDadosFuncionario() {
    Funcionario *funcionarios = NULL;
    FILE *buffer;

    // Verifica o tipo de arquivo e realiza a migração
    if (getTipoArquivo() == TXT) {
        // Se o usuário deseja gravar os dados no arquivo TXT
        buffer = fopen("../bd/funcionarios.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Muda o tipo de arquivo para BIN
            funcionarios = getFuncionarios(); // Obtém os funcionários
            setTipoArquivo(TXT); // Retorna para o arquivo TXT
            setFuncionarios(funcionarios); // Escreve os dados no arquivo TXT
            free(buffer);
            remove("../bd/funcionarios.bin"); // Remove o arquivo BIN
            return NULL;
        }
        fclose(buffer);
        free(buffer);
    }
    if (getTipoArquivo() == BIN) {
        // Se o usuário deseja gravar em arquivo BIN
        buffer = fopen("../bd/funcionarios.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Muda o tipo de arquivo para TXT
            funcionarios = getFuncionarios(); // Obtém os funcionários
            setTipoArquivo(BIN); // Retorna para o arquivo BIN
            setFuncionarios(funcionarios); // Escreve os dados no arquivo BIN
            remove("../bd/funcionarios.txt"); // Remove o arquivo TXT
            return NULL;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/funcionarios.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            // Se o usuário gravou os dados em TXT anteriormente
            setTipoArquivo(TXT); // Muda para o arquivo TXT
            funcionarios = getFuncionarios(); // Obtém os funcionários
            setTipoArquivo(MEM); // Muda para o tipo de arquivo MEM
            remove("../bd/funcionarios.txt"); // Remove o arquivo TXT
            return funcionarios;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/funcionarios.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            // Se o usuário gravou os dados em BIN anteriormente
            setTipoArquivo(BIN); // Muda para o arquivo BIN
            funcionarios = getFuncionarios(); // Obtém os funcionários
            setTipoArquivo(MEM); // Muda para o tipo de arquivo MEM
            remove("../bd/funcionarios.bin"); // Remove o arquivo BIN
            return funcionarios;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    return NULL;
}

// Função para gravar os dados de funcionários no arquivo conforme o tipo de arquivo (TXT ou BIN)
void setFuncionarios(Funcionario *funcionarios) {
    FILE *buffer;

    // Verifica o tipo de arquivo e grava os dados correspondentes
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/funcionarios.txt", "w"); // Abre o arquivo TXT
        if (buffer != NULL) {
            escrever_arquivo_txt_funcionario(buffer, funcionarios); // Escreve no arquivo TXT
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/funcionarios.bin", "wb"); // Abre o arquivo BIN
        if (buffer != NULL) {
            escrever_arquivo_bin_funcionario(buffer, funcionarios); // Escreve no arquivo BIN
            fclose(buffer);
        }
    }
}

// Função para obter os funcionários do arquivo conforme o tipo de arquivo (TXT ou BIN)
Funcionario *getFuncionarios() {
    FILE *buffer;
    Funcionario *funcionarios = NULL;

    // Lê os dados do arquivo TXT
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/funcionarios.txt", "r"); // Abre o arquivo TXT
        if (buffer == NULL) {
            return NULL;
        }
        funcionarios = ler_arquivo_txt_funcionario(buffer); // Lê os funcionários do arquivo TXT
    }
    // Lê os dados do arquivo BIN
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/funcionarios.bin", "rb"); // Abre o arquivo BIN
        if (buffer == NULL) {
            return NULL;
        }
        funcionarios = ler_arquivo_bin_funcionario(buffer); // Lê os funcionários do arquivo BIN
    }
    // Caso o tipo de arquivo seja MEM, retorna NULL
    if (getTipoArquivo() == MEM) {
        return NULL;
    }
    return funcionarios;
}

// Função para ler dados de funcionários a partir de um arquivo TXT
Funcionario *ler_arquivo_txt_funcionario(FILE *buffer) {
    int numFuncionarios = 0;
    Funcionario *funcionarios = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    // Lê o arquivo linha por linha
    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            funcionarios = malloc(sizeof(Funcionario) * (numFuncionarios + 1)); // Aloca memória para o primeiro funcionário
            isPrimeiro = FALSE;
        }else {
            funcionarios = realloc(funcionarios, (numFuncionarios + 1) * sizeof(Funcionario)); // Realoca memória para cada novo funcionário
        }
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE){
            if(equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    funcionarios[numFuncionarios].codigo = atoi(removeTags(Linha)); // Lê o código do funcionário
                    i++;
                    break;
                case 1:
                    strcpy(funcionarios[numFuncionarios].nome, removeTags(Linha)); // Lê o nome do funcionário
                    i++;
                    break;
                case 2:
                    strcpy(funcionarios[numFuncionarios].cpf, removeTags(Linha)); // Lê o CPF do funcionário
                    i++;
                    break;
                case 3:
                    strcpy(funcionarios[numFuncionarios].endereco, removeTags(Linha)); // Lê o endereço do funcionário
                    i++;
                    break;
                case 4:
                    strcpy(funcionarios[numFuncionarios].telefone, removeTags(Linha)); // Lê o telefone do funcionário
                    i++;
                    break;
                case 5:
                    strcpy(funcionarios[numFuncionarios].cargo, removeTags(Linha)); // Lê o cargo do funcionário
                    i++;
                    break;
                case 6:
                    funcionarios[numFuncionarios].salario = atof(removeTags(Linha)); // Lê o salário do funcionário
                    i++;
                    break;
                case 7:
                    funcionarios[numFuncionarios].ativo = atoi(removeTags(Linha)); // Lê o status de ativo do funcionário
                    i = 0; // Reinicia para o próximo funcionário
                    numFuncionarios++;
                    funcionarios = realloc(funcionarios, (numFuncionarios + 1) * sizeof(Funcionario)); // Realoca memória para o próximo funcionário
                    break;
            }
        }
    }
    qtdFuncionarios = numFuncionarios; // Atualiza a quantidade de funcionários
    return funcionarios;
}

// Função para escrever dados de funcionários no arquivo TXT
void escrever_arquivo_txt_funcionario(FILE *buffer, Funcionario *funcionarios) {
    if (getTamanhoFuncionarios() == 0) setTamanhoFuncionarios(); // Se não houver funcionários, inicializa a quantidade

    // Escreve cada funcionário no arquivo TXT
    for (int i = 0; i < getTamanhoFuncionarios(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<nome>%s</nome>\n"
                                 "<cpf>%s</cpf>\n"
                                 "<endereco>%s</endereco>\n"
                                 "<telefone>%s</telefone>\n"
                                 "<cargo>%s</cargo>\n"
                                 "<salario>%f</salario>\n"
                                 "<ativo>%d</ativo>\n"
                                 "</registro>\n",
                                 funcionarios[i].codigo,
                                 funcionarios[i].nome,
                                 funcionarios[i].cpf,
                                 funcionarios[i].endereco,
                                 funcionarios[i].telefone,
                                 funcionarios[i].cargo,
                                 funcionarios[i].salario,
                                 funcionarios[i].ativo
        );
        if (escrevendo < 0) {
            return;
        }
    }
}

// Função para ler dados de funcionários de um arquivo BIN
Funcionario *ler_arquivo_bin_funcionario(FILE *buffer) {
    qtdFuncionarios = 0; // Inicializa a quantidade de funcionários
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdFuncionarios = (int) ftell(buffer) / sizeof(Funcionario); // Calcula a quantidade de funcionários
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    Funcionario *funcionarios = malloc(sizeof(Funcionario) * qtdFuncionarios); // Aloca a quantidade exata de funcionários

    if (funcionarios == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para os funcionários.\n");
        return NULL;
    }

    fread(funcionarios, sizeof(Funcionario), qtdFuncionarios, buffer); // Lê todos os funcionários do arquivo e insere no vetor de funcionários
    return funcionarios;
}


// Função para escrever dados de funcionários no arquivo BIN
void escrever_arquivo_bin_funcionario(FILE *buffer, Funcionario *funcionarios) {
    for (int i = 0; i < getTamanhoFuncionarios(); i++) {
        fwrite(&funcionarios[i], sizeof(Funcionario), 1, buffer); // Escreve os dados dos funcionários no arquivo BIN
    }
}
