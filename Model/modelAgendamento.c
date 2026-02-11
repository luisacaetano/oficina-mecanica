#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"

int qtdAgendamentos = 0;

// Função para obter a quantidade de agendamentos
int getTamanhoAgendamentos() {
    return qtdAgendamentos;
}

// Função para incrementar a quantidade de agendamentos
void setTamanhoAgendamentos() {
    qtdAgendamentos++;
}
void editTamanhoAgendamentos(int tamanho) {
    qtdAgendamentos = tamanho;
}

// Função para migrar dados de agendamentos de TXT para BIN ou MEM
Agendamento *migraDadosAgendamento() {
    Agendamento *agendamentos = NULL;
    FILE *buffer;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/agendamentos.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // Muda o tipo de arquivo para binário
            agendamentos = getAgendamentos();
            if (agendamentos != NULL) {
                setTipoArquivo(TXT); // Volta para o arquivo TXT
                setAgendamentos(agendamentos); // Escreve no arquivo TXT
            }
            remove("../bd/agendamentos.bin");
            return NULL;
        }
    }

    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/agendamentos.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            agendamentos = getAgendamentos();
            if (agendamentos != NULL) {
                setTipoArquivo(BIN);
                setAgendamentos(agendamentos);
            }
            remove("../bd/agendamentos.txt");
            return NULL;
        }
    }

    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/agendamentos.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            agendamentos = getAgendamentos();
            setTipoArquivo(MEM);
            remove("../bd/agendamentos.txt");
            return agendamentos;
        }

        buffer = fopen("../bd/agendamentos.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN);
            agendamentos = getAgendamentos();
            setTipoArquivo(MEM);
            remove("../bd/agendamentos.bin");
            return agendamentos;
        }
    }

    return NULL;
}

// Função para salvar os agendamentos em um arquivo
void setAgendamentos(Agendamento *agendamentos) {
    FILE *buffer;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/agendamentos.txt", "w");
        if (buffer != NULL) {
            escrever_arquivo_txt_agendamento(buffer, agendamentos);
            fclose(buffer);
        }
    } else if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/agendamentos.bin", "wb");
        if (buffer != NULL) {
            escrever_arquivo_bin_agendamento(buffer, agendamentos);
            fclose(buffer);
        }
    }
}

// Função para obter os agendamentos do arquivo
Agendamento *getAgendamentos() {
    FILE *buffer;
    Agendamento *agendamentos = NULL;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/agendamentos.txt", "r"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        agendamentos = ler_arquivo_txt_agendamento(buffer);
        fclose(buffer); // Certifique-se de fechar o arquivo após a leitura
    } else if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/agendamentos.bin", "rb"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        agendamentos = ler_arquivo_bin_agendamento(buffer);
        fclose(buffer); // Certifique-se de fechar o arquivo após a leitura
    }

    return agendamentos;
}

// Função para ler o arquivo de agendamentos em TXT
Agendamento *ler_arquivo_txt_agendamento(FILE *buffer) {
    int numAgendamento = 0;
    Agendamento *agendamentos = NULL;
    char Linha[200];
    int i = 0;
    int isPrimeiro = 1;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro) {
            agendamentos = malloc(sizeof(Agendamento) * (numAgendamento + 1));
            if (agendamentos == NULL) {
                printf("Erro ao alocar memória.\n");
                return NULL;
            }
            isPrimeiro = 0;
        } else {
            agendamentos = realloc(agendamentos, (numAgendamento + 1) * sizeof(Agendamento));
            if (agendamentos == NULL) {
                printf("Erro ao realocar memória.\n");
                return NULL;
            }
        }

        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(
                filtrarSoATag(Linha), "</registro>") != TRUE) {
            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    agendamentos[numAgendamento].codigo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 1:
                    agendamentos[numAgendamento].quantidadeServicos = atoi(removeTags(Linha));
                    i++;
                    break;
                case 2:
                    char *dado = removeTags(Linha);
                    agendamentos[numAgendamento].codigosServicos = separaVetor(
                        dado, agendamentos[numAgendamento].quantidadeServicos);
                    i++;
                    break;
                case 3:
                    agendamentos[numAgendamento].codigoFuncionario = atoi(removeTags(Linha));
                    i++;
                    break;
                case 4:
                    agendamentos[numAgendamento].codigoCliente = atoi(removeTags(Linha));
                    i++;
                    break;
                case 5:
                    agendamentos[numAgendamento].codigoVeiculo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 6:
                    agendamentos[numAgendamento].dataHora = atol(removeTags(Linha));
                    i++;
                    break;
                case 7:
                    agendamentos[numAgendamento].valorPrevisto = atof(removeTags(Linha));
                    i++;
                    break;
                case 8:
                    agendamentos[numAgendamento].ativo = atoi(removeTags(Linha));
                    i = 0;
                    numAgendamento++;
                    break;
            }
        }
    }
    qtdAgendamentos = numAgendamento;
    return agendamentos;
}

// Função para escrever o arquivo de agendamentos em TXT
void escrever_arquivo_txt_agendamento(FILE *buffer, Agendamento *agendamentos) {
    if (getTamanhoAgendamentos() == 0) setTamanhoAgendamentos();

    for (int i = 0; i < getTamanhoAgendamentos(); i++) {
        fprintf(buffer,
                "<registro>\n"
                "<codigo>%d</codigo>\n"
                "<quantidadeServicos>%d</quantidadeServicos>\n",
                agendamentos[i].codigo,
                agendamentos[i].quantidadeServicos
        );

        // Escreve os códigos dos serviços como uma lista separada por vírgulas
        fprintf(buffer, "<codigosServicos>");
        for (int j = 0; j < agendamentos[i].quantidadeServicos; j++) {
            fprintf(buffer, "%d,", agendamentos[i].codigosServicos[j]);
        }
        fprintf(buffer, "</codigosServicos>\n");

        fprintf(buffer,
                "<codigoFuncionario>%d</codigoFuncionario>\n"
                "<codigoCliente>%d</codigoCliente>\n"
                "<codigoVeiculo>%d</codigoVeiculo>\n"
                "<dataHora>%ld</dataHora>\n"
                "<valorPrevisto>%.2f</valorPrevisto>\n"
                "<ativo>%d</ativo>\n"
                "</registro>\n",
                agendamentos[i].codigoFuncionario,
                agendamentos[i].codigoCliente,
                agendamentos[i].codigoVeiculo,
                agendamentos[i].dataHora,
                agendamentos[i].valorPrevisto,
                agendamentos[i].ativo);
    }
}

// Função para ler o arquivo binário de agendamentos
Agendamento *ler_arquivo_bin_agendamento(FILE *buffer) {
    int qtdAgendamentos = getTamanhoAgendamentos();
    if (buffer == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    fread(qtdAgendamentos, sizeof(int), 1, buffer); // Lê a quantidade de agendamentos

    Agendamento *agendamentos = malloc(sizeof(Agendamento) * (qtdAgendamentos));
    if (agendamentos == NULL) {
        printf("Erro ao alocar memória para agendamentos.\n");
        return NULL;
    }

    for (int i = 0; i < qtdAgendamentos; i++) {
        fread(&agendamentos[i].codigo, sizeof(int), 1, buffer);
        fread(&agendamentos[i].quantidadeServicos, sizeof(int), 1, buffer);
        fread(&agendamentos[i].codigoFuncionario, sizeof(int), 1, buffer);
        fread(&agendamentos[i].codigoCliente, sizeof(int), 1, buffer);
        fread(&agendamentos[i].codigoVeiculo, sizeof(int), 1, buffer);
        fread(&agendamentos[i].dataHora, sizeof(time_t), 1, buffer);
        fread(&agendamentos[i].valorPrevisto, sizeof(float), 1, buffer);
        fread(&agendamentos[i].ativo, sizeof(int), 1, buffer);

        // Aloca memória para os serviços e lê os dados
        if (agendamentos[i].quantidadeServicos > 0) {
            agendamentos[i].codigosServicos = malloc(sizeof(int) * agendamentos[i].quantidadeServicos);
            if (agendamentos[i].codigosServicos == NULL) {
                printf("Erro ao alocar memória para os códigos de serviços.\n");
                free(agendamentos); // Libera a memória alocada antes de retornar
                return NULL;
            }
            fread(agendamentos[i].codigosServicos, sizeof(int), agendamentos[i].quantidadeServicos, buffer);
        } else {
            agendamentos[i].codigosServicos = NULL;
        }
    }

    return agendamentos;
}


// Função para escrever o arquivo binário de agendamentos
void escrever_arquivo_bin_agendamento(FILE *buffer, Agendamento *agendamentos) {
    int qtdAgendamentos = getTamanhoAgendamentos();
    fwrite(&qtdAgendamentos, sizeof(int), 1, buffer); // Grava a quantidade de agendamentos

    for (int i = 0; i < qtdAgendamentos; i++) {
        // Grava os campos primitivos da struct (exceto os ponteiros)
        fwrite(&agendamentos[i].codigo, sizeof(int), 1, buffer);
        fwrite(&agendamentos[i].quantidadeServicos, sizeof(int), 1, buffer);
        fwrite(&agendamentos[i].codigoFuncionario, sizeof(int), 1, buffer);
        fwrite(&agendamentos[i].codigoCliente, sizeof(int), 1, buffer);
        fwrite(&agendamentos[i].codigoVeiculo, sizeof(int), 1, buffer);
        fwrite(&agendamentos[i].dataHora, sizeof(time_t), 1, buffer);
        fwrite(&agendamentos[i].valorPrevisto, sizeof(float), 1, buffer);
        fwrite(&agendamentos[i].ativo, sizeof(int), 1, buffer);

        // Grava os códigos dos serviços, se houver
        if (agendamentos[i].quantidadeServicos > 0) {
            fwrite(agendamentos[i].codigosServicos, sizeof(int), agendamentos[i].quantidadeServicos, buffer);
        }
    }
}

