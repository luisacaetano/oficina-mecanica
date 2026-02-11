#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/peca.h"
#include "../bibliotecas/utils.h"

// Variável global para armazenar o número de peças
int qtdPecas = 0;

// Função para obter o tamanho atual das peças
int getTamanhoPecas() {
    return qtdPecas; // Retorna a quantidade de peças
}

// Função para incrementar o número de peças
void setTamanhoPecas() {
    qtdPecas++; // Incrementa a quantidade de peças
}
// Função para atualizar manualmente a quantidade de peças
void editTamanhoPecas(int tamanho) {
    qtdPecas = tamanho; // Atualiza a quantidade de peças
}

// Função para migrar os dados das peças entre os formatos de arquivo
Peca *migraDadosPeca() {
    Peca *pecas = NULL;
    FILE *buffer;

    // Se o tipo de arquivo for TXT, tenta migrar para BIN
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/pecas.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // Muda o tipo de arquivo para bin
            pecas = getPecas(); // Obtém as peças do arquivo binário
            setTipoArquivo(TXT); // Volta para o arquivo txt

            setPecas(pecas); // Escreve as peças no arquivo txt
            remove("../bd/pecas.bin"); // Remove o arquivo binário
            return NULL;
        }
    }

    // Se o tipo de arquivo for BIN, tenta migrar para TXT
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/pecas.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT); // Muda o tipo de arquivo para TXT
            pecas = getPecas(); // Obtém as peças do arquivo TXT
            setTipoArquivo(BIN); // Muda de volta para BIN
            setPecas(pecas); // Escreve as peças no arquivo BIN
            remove("../bd/pecas.txt"); // Remove o arquivo TXT
            return NULL;
        }
    }

    // Se o tipo de arquivo for MEM (memória), tenta migrar de TXT ou BIN
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/pecas.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT); // Muda o tipo de arquivo para TXT
            pecas = getPecas(); // Obtém as peças da memória
            setTipoArquivo(MEM); // Muda de volta para MEM
            remove("../bd/pecas.txt"); // Remove o arquivo TXT
            return pecas;
        }

        buffer = fopen("../bd/pecas.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // Muda o tipo de arquivo para BIN
            pecas = getPecas(); // Obtém as peças do arquivo BIN
            setTipoArquivo(MEM); // Muda de volta para MEM
            remove("../bd/pecas.bin"); // Remove o arquivo BIN
            return pecas;
        }
    }

    return NULL; // Se não encontrou arquivos, retorna NULL
}

// Função para salvar as peças em um arquivo
void setPecas(Peca *pecas) {
    FILE *buffer;

    // Se o tipo de arquivo for TXT, escreve as peças em um arquivo de texto
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/pecas.txt", "w"); // Abre o arquivo para escrita de texto
        if (buffer != NULL) {
            escrever_arquivo_txt_peca(buffer, pecas); // Chama a função para escrever no formato TXT
            fclose(buffer); // Fecha o arquivo após escrever
            return;
        }
    }

    // Se o tipo de arquivo for BIN, escreve as peças em um arquivo binário
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/pecas.bin", "wb"); // Abre o arquivo para escrita binária
        if (buffer != NULL) {
            escrever_arquivo_bin_peca(buffer, pecas); // Chama a função para escrever no formato BIN
            fclose(buffer); // Fecha o arquivo após escrever
        }
    }
}

// Função para obter as peças de um arquivo
Peca *getPecas() {
    FILE *buffer;
    Peca *pecas = NULL;

    // Se o tipo de arquivo for TXT, lê as peças de um arquivo de texto
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/pecas.txt", "r"); // Abre o arquivo para leitura de texto
        if (buffer == NULL) {

            return NULL;
        }
        pecas = ler_arquivo_txt_peca(buffer); // Lê as peças do arquivo de texto
    }

    // Se o tipo de arquivo for BIN, lê as peças de um arquivo binário
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/pecas.bin", "rb"); // Abre o arquivo para leitura binária
        if (buffer == NULL) {

            return NULL;
        }
        pecas = ler_arquivo_bin_peca(buffer); // Lê as peças do arquivo binário
    }

    // Se o tipo de arquivo for MEM, não faz nada e retorna NULL
    if (getTipoArquivo() == MEM) {
        return NULL;
    }

    return pecas; // Retorna as peças lidas
}

// Função para ler as peças de um arquivo de texto
Peca *ler_arquivo_txt_peca(FILE *buffer) {
    int numPecas = 0;
    Peca *pecas = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    // Lê cada linha do arquivo
    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            pecas = malloc(sizeof(Peca) * (numPecas + 1)); // Aloca memória para a primeira peça
            isPrimeiro = FALSE;
        } else {
            pecas = realloc(pecas, (numPecas + 1) * sizeof(Peca)); // Realoca memória para mais peças
        }

        // Verifica se a linha não contém tags de registro
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE){
            if(equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    pecas[numPecas].codigo = atoi(removeTags(Linha)); // Lê o código da peça
                    i++;
                    break;
                case 1:
                    strcpy(pecas[numPecas].descricao, removeTags(Linha)); // Lê a descrição da peça
                    i++;
                    break;
                case 2:
                    strcpy(pecas[numPecas].fabricante, removeTags(Linha)); // Lê o fabricante da peça
                    i++;
                    break;
                case 3:
                    pecas[numPecas].fornecedor = atoi(removeTags(Linha)); // Lê o fornecedor da peça
                    i++;
                    break;
                case 4:
                    pecas[numPecas].preco_custo = atof(removeTags(Linha)); // Lê o preço de custo da peça
                    i++;
                    break;
                case 5:
                    pecas[numPecas].preco_venda = atof(removeTags(Linha)); // Lê o preço de venda da peça
                    i++;
                    break;
                case 6:
                    pecas[numPecas].estoque = atoi(removeTags(Linha)); // Lê o estoque da peça
                    i++;
                    break;
                case 7:
                    pecas[numPecas].estoque_min = atoi(removeTags(Linha)); // Lê o estoque mínimo da peça
                    i++;
                    break;
                case 8:
                    pecas[numPecas].ativo = atoi(removeTags(Linha)); // Lê o estado ativo da peça
                    i = 0; // Reinicia o contador para a próxima peça
                    numPecas++;
                    pecas = realloc(pecas, (numPecas + 1) * sizeof(Peca)); // Realoca a memória para mais peças
                    break;
            }
        }
    }
    qtdPecas = numPecas; // Atualiza o número total de peças
    return pecas; // Retorna as peças lidas do arquivo
}

// Função para escrever as peças em um arquivo de texto
void escrever_arquivo_txt_peca(FILE *buffer, Peca *pecas) {
    if (getTamanhoPecas() == 0) setTamanhoPecas(); // Se não houver peças cadastradas, incrementa o número
    for (int i = 0; i < getTamanhoPecas(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<descricao>%s</descricao>\n"
                                 "<fabricante>%s</fabricante>\n"
                                 "<fornecedor>%d</fornecedor>\n"
                                 "<preco_custo>%.2f</preco_custo>\n"
                                 "<preco_venda>%.2f</preco_venda>\n"
                                 "<estoque>%d</estoque>\n"
                                 "<estoque_min>%d</estoque_min>\n"
                                 "<ativo>%d</ativo>\n"
                                 "</registro>\n",
                                 pecas[i].codigo,
                                 pecas[i].descricao,
                                 pecas[i].fabricante,
                                 pecas[i].fornecedor,
                                 pecas[i].preco_custo,
                                 pecas[i].preco_venda,
                                 pecas[i].estoque,
                                 pecas[i].estoque_min,
                                 pecas[i].ativo
        );
        if (escrevendo < 0) {
            return; // Se houver erro na escrita, sai da função
        }
    }
}

// Função para ler as peças de um arquivo binário
Peca *ler_arquivo_bin_peca(FILE *buffer) {
    qtdPecas = 0; // Inicializa a quantidade de peças
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdPecas = (int) ftell(buffer) / sizeof(Peca); // Calcula a quantidade de peças
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    Peca *pecas = malloc(sizeof(Peca) * qtdPecas); // Aloca a quantidade exata de peças

    if (pecas == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para as peças.\n");
        return NULL;
    }

    fread(pecas, sizeof(Peca), qtdPecas, buffer); // Lê todas as peças do arquivo e insere no vetor de peças
    return pecas; // Retorna o vetor de peças
}


// Função para escrever as peças em um arquivo binário
void escrever_arquivo_bin_peca(FILE *buffer, Peca *pecas) {
    for (int i = 0; i < getTamanhoPecas(); i++) {
        if (fwrite(&pecas[i], sizeof(Peca), 1, buffer)) { // Escreve cada peça no arquivo binário
        }
    }
}
