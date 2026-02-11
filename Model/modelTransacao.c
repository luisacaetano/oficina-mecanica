#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/utils.h" // Biblioteca utilitária
#include "../bibliotecas/modulo2.h"
// Variável global para armazenar a quantidade de transações
int qtdTransacoes = 0;

// Função para obter a quantidade atual de transações
int getTamanhoTransacoes() {
    return qtdTransacoes;
}

// Função para incrementar a quantidade de transações
void setTamanhoTransacoes() {
    qtdTransacoes++;
}
//Função que decrementa 1 unidade a quantidade de transações
void removeTamanhoTransacoes() {
    qtdTransacoes--;
}

void editTamanhoTransacoes(int tamanho) {
    qtdTransacoes = tamanho;
}

// Função para migrar dados de transações entre os formatos de arquivo
Transacao *migraDadosTransacao() {
    Transacao *transacoes = NULL;
    FILE *buffer;

    // Caso o formato de arquivo seja TXT
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/transacoes.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Troca para formato BIN
            transacoes = getTransacoes(); // Lê as transações
            setTipoArquivo(TXT); // Retorna para formato TXT

            setTransacoes(transacoes); // Escreve no TXT
            free(buffer);
            remove("../bd/transacoes.bin"); // Remove o arquivo BIN
            return NULL;
        }
        fclose(buffer);
        free(buffer);
    }

    // Caso o formato de arquivo seja BIN
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/transacoes.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Troca para formato TXT
            transacoes = getTransacoes(); // Lê as transações
            setTipoArquivo(BIN); // Retorna para formato BIN
            setTransacoes(transacoes); // Escreve no BIN
            remove("../bd/transacoes.txt"); // Remove o arquivo TXT
            return NULL;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }

    // Caso o formato de arquivo seja MEM
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/transacoes.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Troca para formato TXT
            transacoes = getTransacoes(); // Lê as transações
            setTipoArquivo(MEM); // Retorna para formato MEM
            remove("../bd/transacoes.txt"); // Remove o arquivo TXT
            return transacoes;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/transacoes.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Troca para formato BIN
            transacoes = getTransacoes(); // Lê as transações
            setTipoArquivo(MEM); // Retorna para formato MEM
            remove("../bd/transacoes.bin"); // Remove o arquivo BIN
            return transacoes;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    return NULL;
}

// Função para gravar as transações no arquivo especificado (TXT ou BIN)
void setTransacoes(Transacao *transacoes) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/transacoes.txt", "w"); // Abre o arquivo TXT
        if (buffer != NULL) {
            escrever_arquivo_txt_transacao(buffer, transacoes); // Escreve no TXT
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/transacoes.bin", "wb"); // Abre o arquivo BIN
        if (buffer != NULL) {
            escrever_arquivo_bin_transacao(buffer, transacoes); // Escreve no BIN
            fclose(buffer);
        }
    }
}

// Função para ler as transações do arquivo especificado (TXT ou BIN)
Transacao *getTransacoes() {
    FILE *buffer;
    Transacao *transacoes = NULL;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/transacoes.txt", "r"); // Abre o arquivo TXT
        if (buffer == NULL) {
            return NULL;
        }
        transacoes = ler_arquivo_txt_transacao(buffer); // Lê do TXT
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/transacoes.bin", "rb"); // Abre o arquivo BIN
        if (buffer == NULL) {
            return NULL;
        }
        transacoes = ler_arquivo_bin_transacao(buffer); // Lê do BIN
    }
    if (getTipoArquivo() == MEM) {
        return NULL; // Retorna NULL se o formato for MEM
    }
    return transacoes;
}

// Função para ler transações de um arquivo TXT
Transacao *ler_arquivo_txt_transacao(FILE *buffer) {
    int numTransacoes = 0;
    Transacao *transacoes = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            transacoes = malloc(sizeof(Transacao) * (numTransacoes + 1));
            isPrimeiro = FALSE;
        } else transacoes = realloc(transacoes, (numTransacoes + 1) * sizeof(Transacao));
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(
                filtrarSoATag(Linha), "</registro>") != TRUE) {
            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    transacoes[numTransacoes].codigo = atoi(removeTags(Linha));
                i++;
                break;
                case 1:
                    transacoes[numTransacoes].formaPagamento = atoi(removeTags(Linha));
                i++;
                break;
                case 2:
                    transacoes[numTransacoes].valorTotal = atof(removeTags(Linha));
                i++;
                break;
                case 3:
                    transacoes[numTransacoes].tipoTransacao = atoi(removeTags(Linha));
                i++;
                break;
                case 4:
                    transacoes[numTransacoes].dataTransacao = atol(removeTags(Linha));
                i = 0; // Reinicia para ler a próxima transação
                numTransacoes++; // atualiza a quantidade de transações
                    break;
            }
        }
    }
    qtdTransacoes = numTransacoes;
    return transacoes;
}

// Função para escrever transações em um arquivo TXT
void escrever_arquivo_txt_transacao(FILE *buffer, Transacao *transacoes) {
    if (getTamanhoTransacoes() == 0) setTamanhoTransacoes(); // Verifica se há transações cadastradas

    for (int i = 0; i < getTamanhoTransacoes(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<formaPagamento>%d</formaPagamento>\n"
                                 "<valorTotal>%.2f</valorTotal>\n"
                                 "<tipoTransacao>%d</tipoTransacao>\n"
                                 "<dataTransacao>%ld</dataTransacao>\n"
                                 "</registro>\n",
                                 transacoes[i].codigo,
                                 transacoes[i].formaPagamento,
                                 transacoes[i].valorTotal,
                                 transacoes[i].tipoTransacao,
                                 transacoes[i].dataTransacao
        );
        if (escrevendo < 0) {
            return;
        }
    }
}

// Função para ler transações de um arquivo BIN
Transacao *ler_arquivo_bin_transacao(FILE *buffer) {
    qtdTransacoes = 0; // Inicializa a quantidade de transações
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdTransacoes = (int) ftell(buffer) / sizeof(Transacao); // Calcula a quantidade de transações
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    Transacao *transacoes = malloc(sizeof(Transacao) * qtdTransacoes); // Aloca a quantidade exata de transações

    if (transacoes == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para as transações.\n");
        return NULL;
    }

    fread(transacoes, sizeof(Transacao), qtdTransacoes, buffer); // Lê todas as transações do arquivo e insere no vetor de transações
    return transacoes; // Retorna o vetor de transações
}

// Função para escrever transações em um arquivo BIN
void escrever_arquivo_bin_transacao(FILE *buffer, Transacao *transacoes) {
    for (int i = 0; i < getTamanhoTransacoes(); i++) {
        if (fwrite(&transacoes[i], sizeof(Transacao), 1, buffer)) {
            // Gravação bem-sucedida
        }
    }
}
