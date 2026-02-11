#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/modulo2.h"

// Variável global para rastrear o número de ordem de servicos cadastrado
int qtdOrdemServico = 0;

// Função para retornar o tamanho atual do vetor de ordem de servico
int getTamanhoOrdemServico() {
    return qtdOrdemServico;
}

// Incrementa o contador global de ordem de servico
void setTamanhoOrdemServico() {
    qtdOrdemServico++;
}
void editTamanhoOrdemServicos(int tamanho) {
    qtdOrdemServico = tamanho;
}
OrdemServico *migraDadosOrdemServico() {
    OrdemServico *ordem_servico = NULL;
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        // Caso o tipo de arquivo seja TXT
        buffer = fopen("../bd/ordemServico.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para bin
            ordem_servico = getOrdemServico();
            setTipoArquivo(TXT); // volta para o arquivo txt

            setOrdemServico(ordem_servico); // escreve no txt
            remove("../bd/ordemServico.bin");
            return NULL;
        }
    }
    if (getTipoArquivo() == BIN) {
        // Caso o tipo de arquivo seja BIN
        buffer = fopen("../bd/ordemServico.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            ordem_servico = getOrdemServico();
            setTipoArquivo(BIN);
            setOrdemServico(ordem_servico);
            remove("../bd/ordemServico.txt");
            return NULL;
        }
    }
    if (getTipoArquivo() == MEM) {
        // Caso o tipo de armazenamento seja MEM
        buffer = fopen("../bd/ordemServico.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT); // muda o tipo de arquivo para TXT
            ordem_servico = getOrdemServico();
            setTipoArquivo(MEM);
            remove("../bd/ordemServico.txt");
            return ordem_servico;
        }

        buffer = fopen("../bd/ordemServico.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para BIN
            ordem_servico = getOrdemServico();
            setTipoArquivo(MEM);
            remove("../bd/ordemServico.bin");
            return ordem_servico;
        }
    }
    return NULL;
}

/**
 * Salva os dados de serviços no formato especificado (TXT ou BIN).
 */
void setOrdemServico(OrdemServico *ordem_servico) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/ordemServico.txt", "w"); // Abre o arquivo para escrita de texto
        if (buffer != NULL) {
            escrever_arquivo_txt_ordem_servico(buffer, ordem_servico);
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/ordemServico.bin", "wb"); // Abre o arquivo para escrita binária
        if (buffer != NULL) {
         escrever_arquivo_bin_ordem_servico(buffer, ordem_servico);
            fclose(buffer);
        }
    }
}

/**
 * Lê os dados de serviços do formato especificado (TXT ou BIN).
 * Retorna um ponteiro para o array de clientes.
 */
OrdemServico *getOrdemServico() {
    FILE *buffer;
    OrdemServico *ordem_servico = NULL;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/ordemServico.txt", "r");
        if (buffer == NULL) {
            printf("Erro ao abrir o arquivo de ordem de serviço.\n");
            return NULL;
        }
        int tamanho = 0;
        ordem_servico = ler_arquivo_txt_ordem_servico(buffer);
    }

    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/ordemServico.bin", "rb");
        if (buffer == NULL) {
            printf("Erro ao abrir o arquivo binário de ordem de serviço.\n");
            return NULL;
        }
//       ordem_servico = ler_arquivo_bin_ordemServico(buffer);
    }

    if (getTipoArquivo() == MEM) {
        return NULL; // Não há dados em memória, retornar NULL
    }
    return ordem_servico;
}


/**
* Lê os serviços de um arquivo TXT e retorna um array dinâmico de `Serviço`.
*/
// Função para ler o arquivo de ordens de serviço em TXT
OrdemServico *ler_arquivo_txt_ordem_servico(FILE *buffer) {
    int numOrdemServico = 0;
    OrdemServico *ordensServico = NULL;
    char Linha[200];
    int i = 0;
    int isPrimeiro = 1;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro) {
            ordensServico = malloc(sizeof(OrdemServico) * (numOrdemServico + 1));
            if (ordensServico == NULL) {
                printf("Erro ao alocar memória.\n");
                return NULL;
            }
            isPrimeiro = 0;
        } else {
            ordensServico = realloc(ordensServico, (numOrdemServico + 1) * sizeof(OrdemServico));
            if (ordensServico == NULL) {
                printf("Erro ao realocar memória.\n");
                return NULL;
            }
        }

        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE) {
            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;

            switch (i) {
                case 0:
                    ordensServico[numOrdemServico].codigo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 1:
                    strcpy(ordensServico[numOrdemServico].descricao, removeTags(Linha));
                    i++;
                    break;
                case 2:
                    ordensServico[numOrdemServico].codigoCliente = atoi(removeTags(Linha));
                    i++;
                    break;
                case 3:
                    ordensServico[numOrdemServico].codigoAgendamento = atoi(removeTags(Linha));
                    i++;
                    break;
                case 4:
                    ordensServico[numOrdemServico].codigoVeiculo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 5:
                    ordensServico[numOrdemServico].codigoFuncionario = atoi(removeTags(Linha));
                    i++;
                    break;
                case 6:
                    ordensServico[numOrdemServico].codigoTransacao = atoi(removeTags(Linha));
                    i++;
                    break;
                case 7:
                    ordensServico[numOrdemServico].quantidadeServicos = atoi(removeTags(Linha));
                    i++;
                    break;
                case 8:
                    char *dado = removeTags(Linha);
                    ordensServico[numOrdemServico].codigoServicos = separaVetor(dado, ordensServico[numOrdemServico].quantidadeServicos);
                    i++;
                    break;
                case 9:
                    ordensServico[numOrdemServico].quantidadePecas = atoi(removeTags(Linha));
                    i++;
                    break;
                case 10:
                    dado = removeTags(Linha);
                    ordensServico[numOrdemServico].codigosPecas = separaVetor(dado, ordensServico[numOrdemServico].quantidadePecas);
                    i++;
                    break;
                case 11:
                    ordensServico[numOrdemServico].dataHoraInicio = atoi(removeTags(Linha));
                    i++;
                    break;
                case 12:
                    ordensServico[numOrdemServico].dataHoraFim = atoi(removeTags(Linha));
                    i++;
                    break;
                case 13:
                    ordensServico[numOrdemServico].valorTotal = atof(removeTags(Linha));
                    i++;
                    break;
                case 14:
                    ordensServico[numOrdemServico].feito = atoi(removeTags(Linha));
                    i++;
                    break;
                case 15:
                    ordensServico[numOrdemServico].ativo = atoi(removeTags(Linha));
                    i = 0;
                    numOrdemServico++;
                    break;
            }
        }
    }

    qtdOrdemServico = numOrdemServico;
    return ordensServico;
}

void escrever_arquivo_txt_ordem_servico(FILE *buffer, OrdemServico *ordensServico) {
    if (getTamanhoOrdemServico() == 0) setTamanhoOrdemServico();

    for (int i = 0; i < getTamanhoOrdemServico(); i++) {
        fprintf(buffer,
                "<registro>\n"
                "<codigo>%d</codigo>\n"
                "<descricao>%s</descricao>\n"
                "<codigoCliente>%d</codigoCliente>\n"
                "<codigoAgendamento>%d</codigoAgendamento>\n"
                "<codigoVeiculo>%d</codigoVeiculo>\n"
                "<codigoFuncionario>%d</codigoFuncionario>\n"
                "<codigoTransacao>%d</codigoTransacao>\n"
                "<quantidadeServicos>%d</quantidadeServicos>\n",
                ordensServico[i].codigo,
                ordensServico[i].descricao,
                ordensServico[i].codigoCliente,
                ordensServico[i].codigoAgendamento,
                ordensServico[i].codigoVeiculo,
                ordensServico[i].codigoFuncionario,
                ordensServico[i].codigoTransacao,
                ordensServico[i].quantidadeServicos
        );

        // Escreve os códigos dos serviços como uma lista separada por vírgulas
        fprintf(buffer, "<codigosServicos>");
        for (int j = 0; j < ordensServico[i].quantidadeServicos; j++) {
            fprintf(buffer, "%d,", ordensServico[i].codigoServicos[j]);
        }
        fprintf(buffer, "</codigosServicos>\n");

        // Escreve a quantidade de peças e os códigos das peças
        fprintf(buffer,
                "<quantidadePecas>%d</quantidadePecas>\n",
                ordensServico[i].quantidadePecas);

        fprintf(buffer, "<codigosPecas>");
        for (int j = 0; j < ordensServico[i].quantidadePecas; j++) {
            fprintf(buffer, "%d,", ordensServico[i].codigosPecas[j]);
        }
        fprintf(buffer, "</codigosPecas>\n");

        // Escreve os outros dados da ordem de serviço
        fprintf(buffer,
                "<dataHoraInicio>%d</dataHoraInicio>\n"
                "<dataHoraFim>%d</dataHoraFim>\n"
                "<valorTotal>%.2f</valorTotal>\n"
                "<feito>%d</feito>\n"
                "<ativo>%d</ativo>\n"
                "</registro>\n",
                ordensServico[i].dataHoraInicio,
                ordensServico[i].dataHoraFim,
                ordensServico[i].valorTotal,
                ordensServico[i].feito,
                ordensServico[i].ativo);
    }
}

// Função para ler o arquivo binário de ordem de servicos
OrdemServico *ler_arquivo_bin_ordem_servico(FILE *buffer) {
    if (buffer == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    int qtdOrdens;
    fread(&qtdOrdens, sizeof(int), 1, buffer); // Lê a quantidade de ordens

    OrdemServico *ordensServico = malloc(sizeof(OrdemServico) * qtdOrdens);
    if (ordensServico == NULL) {
        printf("Erro ao alocar memória para ordens de serviço.\n");
        return NULL;
    }

    for (int i = 0; i < qtdOrdens; i++) {
        fread(&ordensServico[i].codigo, sizeof(int), 1, buffer);
        fread(ordensServico[i].descricao, sizeof(char), 100, buffer);
        fread(&ordensServico[i].codigoCliente, sizeof(int), 1, buffer);
        fread(&ordensServico[i].codigoAgendamento, sizeof(int), 1, buffer);
        fread(&ordensServico[i].codigoVeiculo, sizeof(int), 1, buffer);
        fread(&ordensServico[i].codigoFuncionario, sizeof(int), 1, buffer);
        fread(&ordensServico[i].codigoTransacao, sizeof(int), 1, buffer);
        fread(&ordensServico[i].quantidadeServicos, sizeof(int), 1, buffer);
        fread(&ordensServico[i].quantidadePecas, sizeof(int), 1, buffer);
        fread(&ordensServico[i].dataHoraInicio, sizeof(int), 1, buffer);
        fread(&ordensServico[i].dataHoraFim, sizeof(int), 1, buffer);
        fread(&ordensServico[i].valorTotal, sizeof(float), 1, buffer);
        fread(&ordensServico[i].feito, sizeof(int), 1, buffer);
        fread(&ordensServico[i].ativo, sizeof(int), 1, buffer);

        // Aloca memória para os serviços e lê os dados
        if (ordensServico[i].quantidadeServicos > 0) {
            ordensServico[i].codigoServicos = malloc(sizeof(int) * ordensServico[i].quantidadeServicos);
            if (ordensServico[i].codigoServicos == NULL) {
                printf("Erro ao alocar memória para os códigos de serviços.\n");
                free(ordensServico); // Libera a memória alocada antes de retornar
                return NULL;
            }
            fread(ordensServico[i].codigoServicos, sizeof(int), ordensServico[i].quantidadeServicos, buffer);
        } else {
            ordensServico[i].codigoServicos = NULL;
        }

        // Aloca memória para as peças e lê os dados
        if (ordensServico[i].quantidadePecas > 0) {
            ordensServico[i].codigosPecas = malloc(sizeof(int) * ordensServico[i].quantidadePecas);
            if (ordensServico[i].codigosPecas == NULL) {
                printf("Erro ao alocar memória para os códigos de peças.\n");
                free(ordensServico[i].codigoServicos);
                free(ordensServico);
                return NULL;
            }
            fread(ordensServico[i].codigosPecas, sizeof(int), ordensServico[i].quantidadePecas, buffer);
        } else {
            ordensServico[i].codigosPecas = NULL;
        }
    }

    return ordensServico;
}


void escrever_arquivo_bin_ordem_servico(FILE *buffer, OrdemServico *ordensServico) {
    int qtdOrdens = getTamanhoOrdemServico();
    fwrite(&qtdOrdens, sizeof(int), 1, buffer); // Armazena a quantidade de ordens

    for (int i = 0; i < qtdOrdens; i++) {
        // Grava os campos primitivos da struct (exceto os ponteiros)
        fwrite(&ordensServico[i].codigo, sizeof(int), 1, buffer);
        fwrite(ordensServico[i].descricao, sizeof(char), 100, buffer);
        fwrite(&ordensServico[i].codigoCliente, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].codigoAgendamento, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].codigoVeiculo, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].codigoFuncionario, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].codigoTransacao, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].quantidadeServicos, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].quantidadePecas, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].dataHoraInicio, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].dataHoraFim, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].valorTotal, sizeof(float), 1, buffer);
        fwrite(&ordensServico[i].feito, sizeof(int), 1, buffer);
        fwrite(&ordensServico[i].ativo, sizeof(int), 1, buffer);

        // Grava os códigos dos serviços, se houver
        if (ordensServico[i].quantidadeServicos > 0) {
            fwrite(ordensServico[i].codigoServicos, sizeof(int), ordensServico[i].quantidadeServicos, buffer);
        }

        // Grava os códigos das peças, se houver
        if (ordensServico[i].quantidadePecas > 0) {
            fwrite(ordensServico[i].codigosPecas, sizeof(int), ordensServico[i].quantidadePecas, buffer);
        }
    }
}
