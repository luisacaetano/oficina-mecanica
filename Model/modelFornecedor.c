#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/fornecedor.h"
#include "../bibliotecas/utils.h"

// Variável global para armazenar a quantidade de fornecedores
int qtdFornecedores = 0;

// Função para obter a quantidade atual de fornecedores
int getTamanhoFornecedores() {
    return qtdFornecedores;
}

// Função para incrementar a quantidade de fornecedores
void setTamanhoFornecedores() {
    qtdFornecedores++;
}
void editTamanhoFornecedores(int tamanho) {
    qtdFornecedores = tamanho;
}

// Função para migrar os dados dos fornecedores entre formatos de arquivos
Fornecedor *migraDadosFornecedor() {
    Fornecedor *fornecedores = NULL;
    FILE *buffer;

    // Migração de binário para texto
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/fornecedores.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN);
            fornecedores = getFornecedores();
            setTipoArquivo(TXT);
            setFornecedores(fornecedores);
            remove("../bd/fornecedores.bin");
            return NULL;
        }
    }

    // Migração de texto para binário
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/fornecedores.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            fornecedores = getFornecedores();
            setTipoArquivo(BIN);
            setFornecedores(fornecedores);
            remove("../bd/fornecedores.txt");
            return NULL;
        }
    }

    // Migração de arquivos para memória
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/fornecedores.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            fornecedores = getFornecedores();
            setTipoArquivo(MEM);
            remove("../bd/fornecedores.txt");
            return fornecedores;
        }

        buffer = fopen("../bd/fornecedores.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN);
            fornecedores = getFornecedores();
            setTipoArquivo(MEM);
            remove("../bd/fornecedores.bin");
            return fornecedores;
        }
    }
    return NULL;
}

// Função para salvar fornecedores no formato especificado
void setFornecedores(Fornecedor *fornecedores) {
    FILE *buffer;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/fornecedores.txt", "w");
        if (buffer != NULL) {
            escrever_arquivo_txt_fornecedor(buffer, fornecedores);
            fclose(buffer);
            return;
        }
    }

    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/fornecedores.bin", "wb");
        if (buffer != NULL) {
            escrever_arquivo_bin_fornecedor(buffer, fornecedores);
            fclose(buffer);
        }
    }
}

// Função para ler fornecedores no formato especificado
Fornecedor *getFornecedores() {
    FILE *buffer;
    Fornecedor *fornecedores = NULL;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/fornecedores.txt", "r");
        if (buffer == NULL) {

            return NULL;
        }
        fornecedores = ler_arquivo_txt_fornecedor(buffer);
    }

    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/fornecedores.bin", "rb");
        if (buffer == NULL) {

            return NULL;
        }
        fornecedores = ler_arquivo_bin_fornecedor(buffer);
    }

    if (getTipoArquivo() == MEM) {
        return NULL;
    }

    return fornecedores;
}

// Função para ler fornecedores de um arquivo TXT
Fornecedor *ler_arquivo_txt_fornecedor(FILE *buffer) {
    int numFornecedores = 0;
    Fornecedor *fornecedores = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            fornecedores = malloc(sizeof(Fornecedor) * (numFornecedores + 1));
            isPrimeiro = FALSE;
        } else {
            fornecedores = realloc(fornecedores, (numFornecedores + 1) * sizeof(Fornecedor));
        }
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE){
            if(equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    fornecedores[numFornecedores].codigo = atoi(removeTags(Linha));
                    break;
                case 1:
                    strcpy(fornecedores[numFornecedores].nome_fantasia, removeTags(Linha));
                    break;
                case 2:
                    strcpy(fornecedores[numFornecedores].razao_social, removeTags(Linha));
                    break;
                case 3:
                    fornecedores[numFornecedores].incricao_estadual = atoi(removeTags(Linha));
                    break;
                case 4:
                    strcpy(fornecedores[numFornecedores].cnpj, removeTags(Linha));
                    break;
                case 5:
                    strcpy(fornecedores[numFornecedores].endereco, removeTags(Linha));
                    break;
                case 6:
                    strcpy(fornecedores[numFornecedores].telefone, removeTags(Linha));
                    break;
                case 7:
                    strcpy(fornecedores[numFornecedores].email, removeTags(Linha));
                    break;
                case 8:
                    fornecedores[numFornecedores].ativo = atoi(removeTags(Linha));
                    i = -1;
                    numFornecedores++;
                    break;
            }
            i++;
        }
    }
    qtdFornecedores = numFornecedores;
    return fornecedores;
}

// Função para escrever fornecedores em um arquivo TXT
void escrever_arquivo_txt_fornecedor(FILE *buffer, Fornecedor *fornecedores) {
    for (int i = 0; i < getTamanhoFornecedores(); i++) {
        fprintf(buffer,
                "<registro>\n"
                "<codigo>%d</codigo>\n"
                "<nome_fantasia>%s</nome_fantasia>\n"
                "<razao_social>%s</razao_social>\n"
                "<incricao_estadual>%d</incricao_estadual>\n"
                "<cnpj>%s</cnpj>\n"
                "<endereco>%s</endereco>\n"
                "<telefone>%s</telefone>\n"
                "<email>%s</email>\n"
                "<ativo>%d</ativo>\n"
                "</registro>\n",
                fornecedores[i].codigo,
                fornecedores[i].nome_fantasia,
                fornecedores[i].razao_social,
                fornecedores[i].incricao_estadual,
                fornecedores[i].cnpj,
                fornecedores[i].endereco,
                fornecedores[i].telefone,
                fornecedores[i].email,
                fornecedores[i].ativo);
    }
}

// Função para ler fornecedores de um arquivo BIN
Fornecedor *ler_arquivo_bin_fornecedor(FILE *buffer) {
    qtdFornecedores = 0; // Inicializa a quantidade de fornecedores
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdFornecedores = (int) ftell(buffer) / sizeof(Fornecedor); // Calcula a quantidade de fornecedores
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    Fornecedor *fornecedores = malloc(sizeof(Fornecedor) * qtdFornecedores); // Aloca a quantidade exata de fornecedores

    if (fornecedores == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para os fornecedores.\n");
        return NULL;
    }

    fread(fornecedores, sizeof(Fornecedor), qtdFornecedores, buffer); // Lê todos os fornecedores do arquivo e insere no vetor de fornecedores
    return fornecedores;
}


// Função para escrever fornecedores em um arquivo BIN
void escrever_arquivo_bin_fornecedor(FILE *buffer, Fornecedor *fornecedores) {
    for (int i = 0; i < getTamanhoFornecedores(); i++) {
        fwrite(&fornecedores[i], sizeof(Fornecedor), 1, buffer);
    }
}
