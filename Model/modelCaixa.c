#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/caixa.h"
#include "../bibliotecas/utils.h"

// Função responsável por migrar os dados do caixa entre arquivos TXT, BIN ou memória
float migraDadosCaixa() {
    FILE *buffer;
    float valor_em_caixa = 0;

    if (getTipoArquivo() == TXT) {
        // Verifica se existe um arquivo binário para migrar para TXT
        buffer = fopen("../bd/caixa.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // Muda para o formato BIN para leitura
            valor_em_caixa = getCaixa();
            setTipoArquivo(TXT); // Volta para o formato TXT para escrita
            setCaixa(valor_em_caixa); // Grava no TXT
            remove("../bd/caixa.bin");
            return 0;
        }
        fclose(buffer);
        free(buffer);
    }

    if (getTipoArquivo() == BIN) {
        // Verifica se existe um arquivo TXT para migrar para BIN
        buffer = fopen("../bd/caixa.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT); // Muda para o formato TXT para leitura
            valor_em_caixa = getCaixa();
            setTipoArquivo(BIN); // Volta para o formato BIN para escrita
            setCaixa(valor_em_caixa); // Grava no BIN
            remove("../bd/caixa.txt");
            return 0;
        }
        fclose(buffer);
        free(buffer);
    }

    if (getTipoArquivo() == MEM) {
        // Lida com a migração de dados da memória para arquivos
        buffer = fopen("../bd/caixa.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN);
            valor_em_caixa = getCaixa();
            setTipoArquivo(MEM);
            remove("../bd/caixa.bin");
            return valor_em_caixa;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/caixa.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT);
            valor_em_caixa = getCaixa();
            setTipoArquivo(MEM);
            remove("../bd/caixa.txt");
            return valor_em_caixa;
        }
        fclose(buffer);
        free(buffer);
    }
    return 0;
}

// Função que define o valor atual do caixa, salvando-o em arquivo
void setCaixa(float valor_em_caixa) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/caixa.txt", "w");
        if (buffer != NULL) {
            escrever_arquivo_txt_caixa(buffer, valor_em_caixa);
            fclose(buffer);
        }
    } else if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/caixa.bin", "wb");
        if (buffer != NULL) {
            escrever_arquivo_bin_caixa(buffer, valor_em_caixa);
            fclose(buffer);
        }
    }
}

// Função que recupera o valor atual do caixa a partir do arquivo selecionado
float getCaixa() {
    FILE *buffer;

    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/caixa.txt", "r");
        if (buffer == NULL) return 0;
        return ler_arquivo_txt_caixa(buffer);
    } else if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/caixa.bin", "rb");
        if (buffer == NULL) return 0;
        return ler_arquivo_bin_caixa(buffer);
    } else if (getTipoArquivo() == MEM) {
        return 0;
    }
    return 0;
}

// Função que lê o valor do caixa de um arquivo TXT
float ler_arquivo_txt_caixa(FILE *buffer) {
    char Linha[100];
    float valor_em_caixa = 0;
    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(
                filtrarSoATag(Linha), "</registro>") != TRUE) {
            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            valor_em_caixa = atof(removeTags(Linha));
        }
    }
    fclose(buffer);
    return valor_em_caixa;
}

// Função que escreve o valor do caixa em um arquivo TXT
void escrever_arquivo_txt_caixa(FILE *buffer, float valor_em_caixa) {
    fprintf(buffer,
            "<registro>\n"
            "  <valor_em_caixa>%.2f</valor_em_caixa>\n"
            "</registro>\n",
            valor_em_caixa);
}

// Função que lê o valor do caixa de um arquivo BIN
float ler_arquivo_bin_caixa(FILE *buffer) {
    float valor_em_caixa = 0;
    if (fread(&valor_em_caixa, sizeof(float), 1, buffer) != 1) return 0;
    fclose(buffer);
    return valor_em_caixa;
}

// Função que escreve o valor do caixa em um arquivo BIN
void escrever_arquivo_bin_caixa(FILE *buffer, float valor_em_caixa) {
    fwrite(&valor_em_caixa, sizeof(float), 1, buffer);
    fclose(buffer);
}
