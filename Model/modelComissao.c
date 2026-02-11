#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/Modulo2.h"

int qtdComissoes = 0;

// Função para obter a quantidade de comissões
int getTamanhoComissoes() {
    return qtdComissoes;
}

// Função para incrementar a quantidade de comissões
void setTamanhoComissoes() {
    qtdComissoes++;
}

// Função para editar a quantidade de comissões
void editTamanhoComissoes(int tamanho) {
    qtdComissoes = tamanho;
}
// Função para migrar os dados da comissão entre arquivos de texto, binário e memória
Comissao *migraDadosComissao() {
    Comissao *comisoes = NULL;
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        // se o usuário deseja gravar os dados no txt
        buffer = fopen("../bd/comisoes.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para bin
            comisoes = getComissoes();
            setTipoArquivo(TXT); // volta para o arquivo txt

            setComissoes(comisoes); // escreve no txt
            free(buffer);
            remove("../bd/comisoes.bin");
            return NULL;
        }
        fclose(buffer);
        free(buffer);
    }
    if (getTipoArquivo() == BIN) {
        // se o usuário deseja gravar em binário
        buffer = fopen("../bd/comisoes.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT);
            comisoes = getComissoes();
            setTipoArquivo(BIN);
            setComissoes(comisoes);
            remove("../bd/comisoes.txt");
            return NULL;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/comisoes.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            // se antes ele tiver gravado os dados em txt
            setTipoArquivo(TXT); // muda o tipo de arquivo para TXT
            comisoes = getComissoes();
            setTipoArquivo(MEM);
            remove("../bd/comisoes.txt");
            return comisoes;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/comisoes.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            // se antes ele tiver gravado os dados em binário
            setTipoArquivo(BIN); // muda o tipo de arquivo para BIN
            comisoes = getComissoes();
            setTipoArquivo(MEM);
            remove("../bd/comisoes.bin");
            return comisoes;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
}
void setComissoes(Comissao *comisoes) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/comisoes.txt", "w"); // Abre o arquivo corretamente
        if (buffer != NULL) {
            escrever_arquivo_txt_comissao(buffer, comisoes);
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/comisoes.bin", "wb"); // Abre o arquivo corretamente
        if (buffer != NULL) {
            escrever_arquivo_bin_comisao(buffer, comisoes);
            fclose(buffer);
        }
    }
}
Comissao *getComissoes() {
    FILE *buffer;
    Comissao *comisoes = NULL;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/comisoes.txt", "r"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        comisoes = ler_arquivo_txt_comissao(buffer);
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/comisoes.bin", "rb"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        comisoes = ler_arquivo_bin_comissao(buffer);
    }
    if (getTipoArquivo() == MEM) {
        return NULL;
    }
    return comisoes;
}
Comissao *ler_arquivo_txt_comissao(FILE *buffer) {
    int numComisoes = 0;
    Comissao *comisoes = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            comisoes = malloc(sizeof(Comissao) * (numComisoes + 1));
            isPrimeiro = FALSE;
        } else if (isPrimeiro == FALSE) {
            comisoes = realloc(comisoes, (numComisoes + 1) * sizeof(Comissao));
        }

        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE &&
            equalsString(filtrarSoATag(Linha), "</registro>") != TRUE) {

            if (equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;

            switch (i) {
                case 0:
                    comisoes[numComisoes].codigo = atoi(removeTags(Linha));
                i++;
                break;
                case 1:
                    comisoes[numComisoes].CodigoOrdem = atoi(removeTags(Linha));
                i++;
                break;
                case 2:
                    comisoes[numComisoes].valorComisao = atof(removeTags(Linha));
                i++;
                break;
                case 3:
                    comisoes[numComisoes].tempoGasto = atof(removeTags(Linha));
                i++;
                break;
                case 4:
                    comisoes[numComisoes].codigoFuncionario = atoi(removeTags(Linha));
                i++;
                break;
                case 5:
                    comisoes[numComisoes].pago = atoi(removeTags(Linha));
                i = 0; // Reinicia para ler a próxima comissão
                numComisoes++;
                comisoes = realloc(comisoes, (numComisoes + 1) * sizeof(Comissao));
                break;
            }
            }
    }
    qtdComissoes = numComisoes;
    return comisoes;
}
void escrever_arquivo_txt_comissao(FILE *buffer, Comissao *comisoes) {
    if (getTamanhoComissoes() == 0) setTamanhoComissoes(); // se acaso não tiver nenhuma comissão cadastrada

    for (int i = 0; i < getTamanhoComissoes(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<CodigoOrdem>%d</CodigoOrdem>\n"
                                 "<valorComisao>%.2f</valorComisao>\n"
                                 "<tempoGasto>%f</tempoGasto>\n"
                                 "<codigoFuncionario>%d</codigoFuncionario>\n"
                                 "<pago>%d</pago>\n"
                                 "</registro>\n",
                                 comisoes[i].codigo,
                                 comisoes[i].CodigoOrdem,
                                 comisoes[i].valorComisao,
                                 comisoes[i].tempoGasto,
                                 comisoes[i].codigoFuncionario,
                                 comisoes[i].pago
        );
        if (escrevendo < 0) {
            return;
        }
    }
}
Comissao *ler_arquivo_bin_comissao(FILE *buffer) {
    qtdComissoes = 0;
    fseek(buffer, 0, SEEK_END);  // Move o ponteiro até o fim do arquivo
    qtdComissoes = (int) ftell(buffer) / sizeof(Comissao);  // Pega a quantidade de comissões
    fseek(buffer, 0, SEEK_SET);  // Volta o ponteiro para o início do arquivo
    Comissao *comisoes = malloc(sizeof(Comissao) * qtdComissoes);  // Aloca a quantidade exata de comissões

    if (comisoes == NULL) {  // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para as comissões.\n");
        return NULL;
    }

    fread(comisoes, sizeof(Comissao), qtdComissoes, buffer);  // Lê todas as comissões do arquivo e insere no vetor de comissões
    return comisoes;
}
void *escrever_arquivo_bin_comisao(FILE *buffer, Comissao *comisoes) {
    for (int i = 0; i < getTamanhoComissoes(); i++) {
        if (fwrite(&comisoes[i], sizeof(Comissao), 1, buffer)) {
            // Sucesso ao escrever
        }
    }
}

