#include <stdio.h>
#include <stdlib.h>
#include "../bibliotecas/utils.h" // Biblioteca utilitária
#include "../bibliotecas/modulo2.h"
int qtdContasReceber = 0;

// Função para obter a quantidade atual de contas a receber
int getTamanhoContasReceber() {
    return qtdContasReceber;
}

// Função para incrementar a quantidade de contas a receber
void setTamanhoContasReceber() {
    qtdContasReceber++;
}

// Função para decrementar a quantidade de contas a receber
void removeTamanhoContasReceber() {
    qtdContasReceber--;
}
void editTamanhoContasReceber(int tamanho) {
    qtdContasReceber = tamanho;
}
ContasReceber *migraDadosContasReceber() {
    ContasReceber *contas = NULL;
    FILE *buffer;

    // Caso o formato de arquivo seja TXT
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/contasReceber.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Troca para formato BIN
            contas = getContasReceber(); // Lê as contas a receber
            setTipoArquivo(TXT); // Retorna para formato TXT

            setContasReceber(contas); // Escreve no TXT
            free(buffer);
            remove("../bd/contasReceber.bin"); // Remove o arquivo BIN
            return NULL;
        }
        fclose(buffer);
        free(buffer);
    }

    // Caso o formato de arquivo seja BIN
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/contasReceber.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Troca para formato TXT
            contas = getContasReceber(); // Lê as contas a receber
            setTipoArquivo(BIN); // Retorna para formato BIN
            setContasReceber(contas); // Escreve no BIN
            remove("../bd/contasReceber.txt"); // Remove o arquivo TXT
            return NULL;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }

    // Caso o formato de arquivo seja MEM
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/contasReceber.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Troca para formato TXT
            contas = getContasReceber(); // Lê as contas a receber
            setTipoArquivo(MEM); // Retorna para formato MEM
            remove("../bd/contasReceber.txt"); // Remove o arquivo TXT
            return contas;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/contasReceber.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Troca para formato BIN
            contas = getContasReceber(); // Lê as contas a receber
            setTipoArquivo(MEM); // Retorna para formato MEM
            remove("../bd/contasReceber.bin"); // Remove o arquivo BIN
            return contas;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    return NULL;
}

// Função para gravar as contas a receber no arquivo especificado (TXT ou BIN)
void setContasReceber(ContasReceber *contas) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/contasReceber.txt", "w"); // Abre o arquivo TXT
        if (buffer != NULL) {
            escrever_arquivo_txt_contasReceber(buffer, contas); // Escreve no TXT
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/contasReceber.bin", "wb"); // Abre o arquivo BIN
        if (buffer != NULL) {
            escrever_arquivo_bin_contasReceber(buffer, contas); // Escreve no BIN
            fclose(buffer);
        }
    }
}

// Função para ler as contas a receber do arquivo especificado (TXT ou BIN)
ContasReceber *getContasReceber() {
    FILE *buffer;
    ContasReceber *contas = NULL;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/contasReceber.txt", "r"); // Abre o arquivo TXT
        if (buffer == NULL) {
            return NULL;
        }
        contas = ler_arquivo_txt_contasReceber(buffer); // Lê do TXT
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/contasReceber.bin", "rb"); // Abre o arquivo BIN
        if (buffer == NULL) {
            return NULL;
        }
        contas = ler_arquivo_bin_contasReceber(buffer); // Lê do BIN
    }
    if (getTipoArquivo() == MEM) {
        return NULL; // Retorna NULL se o formato for MEM
    }
    return contas;
}

// Função para ler contas a receber de um arquivo TXT
ContasReceber *ler_arquivo_txt_contasReceber(FILE *buffer) {
    int numContas = 0;
    ContasReceber *contas = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            contas = malloc(sizeof(ContasReceber) * (numContas + 1));
            isPrimeiro = FALSE;
        }else contas = realloc(contas, (numContas + 1) * sizeof(ContasReceber));
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
                    contas[numContas].valor = atof(removeTags(Linha)); // Lê o valor da conta
                    i++;
                    break;
                case 3:
                    contas[numContas].dataPagamento = atol(removeTags(Linha)); // Lê a data de pagamento
                    i++;
                    break;
                case 4:
                    contas[numContas].recebimento = atol(removeTags(Linha)); // Lê a data que pagou
                    i++;
                    break;
                case 5:
                    contas[numContas].pago = atoi(removeTags(Linha)); // Lê o status de pagamento
                    i = 0; // Reinicia para ler a próxima conta
                    numContas++;
                    break;
            }
        }
    }
    qtdContasReceber = numContas; // Atualiza a quantidade de contas a receber
    return contas;
}

// Função para escrever contas a receber em um arquivo TXT
void escrever_arquivo_txt_contasReceber(FILE *buffer, ContasReceber *contas) {
    if (getTamanhoContasReceber() == 0) setTamanhoContasReceber(); // Verifica se há contas cadastradas

    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n" // Código da conta a receber
                                 "<codTransacao>%d</codTransacao>\n" // Código da transação associada
                                 "<valor>%.2f</valor>\n" // Valor da conta a receber
                                 "<dataPagamento>%ld</dataPagamento>\n" // Data de pagamento
                                 "<dataQuePagou>%ld</dataQuePagou>\n" // Data que pagou
                                 "<pago>%d</pago>\n" // Status de pagamento
                                 "</registro>\n",
                                 contas[i].codigo,
                                 contas[i].codTransacao,
                                 contas[i].valor,
                                 contas[i].dataPagamento,
                                 contas[i].recebimento,
                                 contas[i].pago
        );
        if (escrevendo < 0) {
            return; // Interrompe caso ocorra um erro ao escrever
        }
    }
}

// Função para ler contas a receber de um arquivo BIN
ContasReceber *ler_arquivo_bin_contasReceber(FILE *buffer) {
    qtdContasReceber = 0; // Inicializa a quantidade de contas a receber
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdContasReceber = (int) ftell(buffer) / sizeof(ContasReceber); // Calcula a quantidade de registros
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    ContasReceber *contas = malloc(sizeof(ContasReceber) * qtdContasReceber); // Aloca a quantidade exata de registros

    if (contas == NULL) {
        // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para as contas a receber.\n");
        return NULL;
    }

    fread(contas, sizeof(ContasReceber), qtdContasReceber, buffer); // Lê todas as contas do arquivo e insere no vetor
    return contas; // Retorna o vetor de contas
}

// Função para escrever contas a receber em um arquivo BIN
void escrever_arquivo_bin_contasReceber(FILE *buffer, ContasReceber *contas) {
    for (int i = 0; i < getTamanhoContasReceber(); i++) {
        if (fwrite(&contas[i], sizeof(ContasReceber), 1, buffer)) {
            // Gravação bem-sucedida
        }
    }
}
