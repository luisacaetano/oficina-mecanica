
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/oficina.h"
#include "../bibliotecas/peca.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/Modulo2.h"

// Variável global para rastrear o número de estoques cadastrados
int qtdEstoque = 0;

// Função para retornar o tamanho atual do vetor de estoques
int getTamanhoEstoque() {
    return qtdEstoque;
}

// Incrementa o contador global de estoques
void setTamanhoEstoque() {
    qtdEstoque++;
}

void editTamanhoEstoques(int tamanho) {
    qtdEstoque = tamanho;
}
Estoque *migraDadosEstoque() {
    Estoque *estoque = NULL;
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        // Caso o tipo de arquivo seja TXT
        buffer = fopen("../bd/estoque.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para bin
            estoque = getEstoque();
            setTipoArquivo(TXT); // volta para o arquivo txt

            setEstoque(estoque); // escreve no txt
            remove("../bd/estoque.bin");
            return NULL;
        }
    }
    if (getTipoArquivo() == BIN) {
        // Caso o tipo de arquivo seja BIN
        buffer = fopen("../bd/estoque.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            estoque = getEstoque();
            setTipoArquivo(BIN);
            setEstoque(estoque);
            remove("../bd/estoque.txt");
            return NULL;
        }
    }
    if (getTipoArquivo() == MEM) {
        // Caso o tipo de armazenamento seja MEM
        buffer = fopen("../bd/estoque.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT); // muda o tipo de arquivo para TXT
            estoque = getEstoque();
            setTipoArquivo(MEM);
            remove("../bd/estoque.txt");
            return estoque;
        }

        buffer = fopen("../bd/estoque.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para BIN
            estoque = getEstoque();
            setTipoArquivo(MEM);
            remove("../bd/estoque.bin");
            return estoque;
        }
    }
    return NULL;
}
/**
 * Salva os dados de estoque no formato especificado (TXT ou BIN).
 */
void setEstoque(Estoque *estoque) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/estoque.txt", "w"); // Abre o arquivo para escrita de texto
        if (buffer != NULL) {
            escrever_arquivo_txt_estoque(buffer, estoque);
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/estoque.bin", "wb"); // Abre o arquivo para escrita binária
        if (buffer != NULL) {
            escrever_arquivo_bin_estoque(buffer, estoque);
            fclose(buffer);
        }
    }
}

Estoque *getEstoque() {
    FILE *buffer;
    Estoque *estoque = NULL;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/estoque.txt", "r");
        if (buffer == NULL) {
            printf("Erro ao abrir o arquivo de estoque.\n");
            return NULL;
        }
        int tamanho = 0;
        estoque = ler_arquivo_txt_estoque(buffer);
    }

    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/estoque.bin", "rb");
        if (buffer == NULL) {
            printf("Erro ao abrir o arquivo binário de estoque.\n");
            return NULL;
        }
         estoque = ler_arquivo_bin_estoque(buffer);
    }

    if (getTipoArquivo() == MEM) {
        return NULL; // Não há dados em memória, retornar NULL
    }
    return estoque;
}
// Função para ler o arquivo de estoque em TXT
Estoque *ler_arquivo_txt_estoque(FILE *buffer) {
    int numEstoque = 0;
    Estoque *estoques = NULL;
    char Linha[200];
    int i = 0;
    int isPrimeiro = 1;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro) {
            estoques = malloc(sizeof(Estoque) * (numEstoque + 1));
            if (estoques == NULL) {
                printf("Erro ao alocar memória.\n");
                return NULL;
            }
            isPrimeiro = 0;
        } else {
            estoques = realloc(estoques, (numEstoque + 1) * sizeof(Estoque));
            if (estoques == NULL) {
                printf("Erro ao realocar memória.\n");
                return NULL;
            }
        }

        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE) {
            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;

            switch (i) {
                case 0:
                    estoques[numEstoque].codigo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 1:
                    estoques[numEstoque].quantidadePecas = atoi(removeTags(Linha));
                    i++;
                    break;
                case 2:
                    char *dado = removeTags(Linha);
                    estoques[numEstoque].codigoPecas = separaVetor(dado, estoques[numEstoque].quantidadePecas);
                    i++;
                    break;
                case 3:
                    estoques[numEstoque].codigoFornecedor = atoi(removeTags(Linha));
                    i++;
                    break;
                case 4:
                    estoques[numEstoque].imposto = atof(removeTags(Linha));
                    i++;
                    break;
                case 5:
                    estoques[numEstoque].frete = atof(removeTags(Linha));
                    i++;
                    break;
                case 6:
                    estoques[numEstoque].valorTotal = atof(removeTags(Linha));
                    i++;
                    break;
                case 7:
                    estoques[numEstoque].codigoTransacao = atoi(removeTags(Linha));
                    i = 0;
                    numEstoque++;
                    break;
            }
        }
    }

    qtdEstoque = numEstoque;
    return estoques;
}
void escrever_arquivo_txt_estoque(FILE *buffer, Estoque *estoques) {
    if (getTamanhoEstoque() == 0) setTamanhoEstoque();

    for (int i = 0; i < getTamanhoEstoque(); i++) {
        fprintf(buffer,
                "<registro>\n"
                "<codigo>%d</codigo>\n"
                "<quantidadePecas>%d</quantidadePecas>\n",
                estoques[i].codigo,
                estoques[i].quantidadePecas
        );

        // Escreve os códigos das peças como uma lista separada por vírgulas
        fprintf(buffer, "<codigoPecas>");
        for (int j = 0; j < estoques[i].quantidadePecas; j++) {
            fprintf(buffer, "%d,", estoques[i].codigoPecas[j]);
        }
        fprintf(buffer, "</codigoPecas>\n");

        // Escreve os outros dados do estoque
        fprintf(buffer,
                "<codigoFornecedor>%d</codigoFornecedor>\n"
                "<imposto>%.2f</imposto>\n"
                "<frete>%.2f</frete>\n"
                "<valorTotal>%.2f</valorTotal>\n"
                "<codigoTransacao>%d</codigoTransacao>\n"
                "</registro>\n",
                estoques[i].codigoFornecedor,
                estoques[i].imposto,
                estoques[i].frete,
                estoques[i].valorTotal,
                estoques[i].codigoTransacao);
    }
}
Estoque *ler_arquivo_bin_estoque(FILE *buffer) {
    if (buffer == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    int qtdEstoques;
    fread(&qtdEstoques, sizeof(int), 1, buffer); // Lê a quantidade de estoques

    Estoque *estoques = malloc(sizeof(Estoque) * qtdEstoques);
    if (estoques == NULL) {
        printf("Erro ao alocar memória para estoques.\n");
        return NULL;
    }

    for (int i = 0; i < qtdEstoques; i++) {
        fread(&estoques[i].codigo, sizeof(int), 1, buffer);
        fread(&estoques[i].quantidadePecas, sizeof(int), 1, buffer);
        fread(&estoques[i].codigoFornecedor, sizeof(int), 1, buffer);
        fread(&estoques[i].imposto, sizeof(float), 1, buffer);
        fread(&estoques[i].frete, sizeof(float), 1, buffer);
        fread(&estoques[i].valorTotal, sizeof(float), 1, buffer);
        fread(&estoques[i].codigoTransacao, sizeof(int), 1, buffer);

        // Aloca memória para os códigos das peças e lê os dados
        if (estoques[i].quantidadePecas > 0) {
            estoques[i].codigoPecas = malloc(sizeof(int) * estoques[i].quantidadePecas);
            if (estoques[i].codigoPecas == NULL) {
                printf("Erro ao alocar memória para os códigos de peças.\n");
                free(estoques); // Libera a memória alocada antes de retornar
                return NULL;
            }
            fread(estoques[i].codigoPecas, sizeof(int), estoques[i].quantidadePecas, buffer);
        } else {
            estoques[i].codigoPecas = NULL;
        }
    }

    return estoques;
}


void escrever_arquivo_bin_estoque(FILE *buffer, Estoque *estoques) {
    int qtdEstoques = getTamanhoEstoque();
    fwrite(&qtdEstoques, sizeof(int), 1, buffer); // Armazena a quantidade de estoques

    for (int i = 0; i < qtdEstoques; i++) {
        // Grava os campos primitivos da struct (exceto os ponteiros)
        fwrite(&estoques[i].codigo, sizeof(int), 1, buffer);
        fwrite(&estoques[i].quantidadePecas, sizeof(int), 1, buffer);
        fwrite(&estoques[i].codigoFornecedor, sizeof(int), 1, buffer);
        fwrite(&estoques[i].imposto, sizeof(float), 1, buffer);
        fwrite(&estoques[i].frete, sizeof(float), 1, buffer);
        fwrite(&estoques[i].valorTotal, sizeof(float), 1, buffer);
        fwrite(&estoques[i].codigoTransacao, sizeof(int), 1, buffer);

        // Grava os códigos das peças, se houver
        if (estoques[i].quantidadePecas > 0) {
            fwrite(estoques[i].codigoPecas, sizeof(int), estoques[i].quantidadePecas, buffer);
        }
    }
}





