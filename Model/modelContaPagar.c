#include <stdio.h>
#include <stdlib.h>
#include "../bibliotecas/utils.h" // Biblioteca utilitária
#include "../bibliotecas/modulo2.h"

int qtdContasPagar = 0;

// Função para obter a quantidade atual de contas a pagar
int getTamanhoContasPagar() {
    return qtdContasPagar;
}

// Função para incrementar a quantidade de contas a pagar
void setTamanhoContasPagar() {
    qtdContasPagar++;
}

void removeTamanhoContasPagar() {
    qtdContasPagar--;
}
void editTamanhoContasPagar(int tamanho) {
    qtdContasPagar = tamanho;
}

ContasPagar *migraDadosContasPagar() {
    ContasPagar *contas = NULL;
    FILE *buffer;

    // Caso o formato de arquivo seja TXT
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/contasPagar.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Troca para formato BIN
            contas = getContasPagar(); // Lê as contas a pagar
            setTipoArquivo(TXT); // Retorna para formato TXT

            setContasPagar(contas); // Escreve no TXT
            free(buffer);
            remove("../bd/contasPagar.bin"); // Remove o arquivo BIN
            return NULL;
        }
        fclose(buffer);
        free(buffer);
    }

    // Caso o formato de arquivo seja BIN
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/contasPagar.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Troca para formato TXT
            contas = getContasPagar(); // Lê as contas a pagar
            setTipoArquivo(BIN); // Retorna para formato BIN
            setContasPagar(contas); // Escreve no BIN
            remove("../bd/contasPagar.txt"); // Remove o arquivo TXT
            return NULL;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }

    // Caso o formato de arquivo seja MEM
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/contasPagar.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Troca para formato TXT
            contas = getContasPagar(); // Lê as contas a pagar
            setTipoArquivo(MEM); // Retorna para formato MEM
            remove("../bd/contasPagar.txt"); // Remove o arquivo TXT
            return contas;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/contasPagar.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Troca para formato BIN
            contas = getContasPagar(); // Lê as contas a pagar
            setTipoArquivo(MEM); // Retorna para formato MEM
            remove("../bd/contasPagar.bin"); // Remove o arquivo BIN
            return contas;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    return NULL;
}

// Função para gravar as contas a pagar no arquivo especificado (TXT ou BIN)
void setContasPagar(ContasPagar *contas) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/contasPagar.txt", "w"); // Abre o arquivo TXT
        if (buffer != NULL) {
            escrever_arquivo_txt_contasPagar(buffer, contas); // Escreve no TXT
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/contasPagar.bin", "wb"); // Abre o arquivo BIN
        if (buffer != NULL) {
            escrever_arquivo_bin_contasPagar(buffer, contas); // Escreve no BIN
            fclose(buffer);
        }
    }
}

// Função para ler as contas a pagar do arquivo especificado (TXT ou BIN)
ContasPagar *getContasPagar() {
    FILE *buffer;
    ContasPagar *contas = NULL;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/contasPagar.txt", "r"); // Abre o arquivo TXT
        if (buffer == NULL) {
            return NULL;
        }
        contas = ler_arquivo_txt_contasPagar(buffer); // Lê do TXT
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/contasPagar.bin", "rb"); // Abre o arquivo BIN
        if (buffer == NULL) {
            return NULL;
        }
        contas = ler_arquivo_bin_contasPagar(buffer); // Lê do BIN
    }
    if (getTipoArquivo() == MEM) {
        return NULL; // Retorna NULL se o formato for MEM
    }
    return contas;
}

// Função para ler contas a pagar de um arquivo TXT
ContasPagar *ler_arquivo_txt_contasPagar(FILE *buffer) {
    int numContas = 0;
    ContasPagar *contas = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            contas = malloc(sizeof(ContasPagar) * (numContas + 1));
            isPrimeiro = FALSE;
        }else
            contas = realloc(contas, (numContas + 1) * sizeof(ContasPagar));
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(
                filtrarSoATag(Linha), "</registro>") != TRUE) {
            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    contas[numContas].codigo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 1:
                    contas[numContas].codTransacao = atoi(removeTags(Linha));
                    i++;
                    break;
                case 2:
                    contas[numContas].dataPagamento = atol(removeTags(Linha));
                    i++;
                    break;
                case 3:
                    contas[numContas].dataEfeituacaoPagamento = atol(removeTags(Linha));
                    i++;
                    break;
                case 4:
                    contas[numContas].pago = atoi(removeTags(Linha));
                    i = 0; // Reinicia para ler a próxima conta
                    numContas++;
                    break;
            }
        }
    }
    qtdContasPagar = numContas;
    return contas;
}

// Função para escrever contas a pagar em um arquivo TXT
void escrever_arquivo_txt_contasPagar(FILE *buffer, ContasPagar *contas) {
    if (getTamanhoContasPagar() == 0) setTamanhoContasPagar(); // Verifica se há contas cadastradas

    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<codTransacao>%d</codTransacao>\n"
                                 "<dataPagamento>%ld</dataPagamento>\n"
                                 "<dataEfeituacaoPagamento>%ld</dataEfeituacaoPagamento>\n"
                                 "<pago>%d</pago>\n"
                                 "</registro>\n",
                                 contas[i].codigo,
                                 contas[i].codTransacao,
                                 contas[i].dataPagamento,
                                 contas[i].dataEfeituacaoPagamento,
                                 contas[i].pago
        );
        if (escrevendo < 0) {
            return; // Interrompe caso ocorra um erro ao escrever
        }
    }
}

// Função para ler contas a pagar de um arquivo BIN
ContasPagar *ler_arquivo_bin_contasPagar(FILE *buffer) {
    qtdContasPagar = 0; // Inicializa a quantidade de contas a pagar
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdContasPagar = (int) ftell(buffer) / sizeof(ContasPagar); // Calcula a quantidade de registros
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    ContasPagar *contas = malloc(sizeof(ContasPagar) * qtdContasPagar); // Aloca a quantidade exata de registros

    if (contas == NULL) {
        // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para as contas a pagar.\n");
        return NULL;
    }

    fread(contas, sizeof(ContasPagar), qtdContasPagar, buffer); // Lê todas as contas do arquivo e insere no vetor
    return contas; // Retorna o vetor de contas
}

// Função para escrever contas a pagar em um arquivo BIN
void escrever_arquivo_bin_contasPagar(FILE *buffer, ContasPagar *contas) {
    for (int i = 0; i < getTamanhoContasPagar(); i++) {
        if (fwrite(&contas[i], sizeof(ContasPagar), 1, buffer)) {
            // Gravação bem-sucedida
        }
    }
}
