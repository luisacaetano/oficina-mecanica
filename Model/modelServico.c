#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/servico.h"
#include "../bibliotecas/utils.h"
// Variável global para rastrear o número de serviços cadastrado
int qtdServicos = 0;
// Função para retornar o tamanho atual do vetor de serviços
int getTamanhoServicos() {
    return qtdServicos;
}

// Incrementa o contador global de serviços
void setTamanhoServicos() {
    qtdServicos++;
}
// Função para atualizar manualmente o tamanho do vetor de serviços
void editTamanhoServicos(int tamanho) {
    qtdServicos = tamanho;
}

/**
 * Função para migrar dados de serviços entre diferentes formatos de armazenamento (TXT, BIN ou MEM).
 * Retorna um ponteiro para os dados migrados ou NULL se não for necessário migrar.
 */
Servico *migraDadosServicos() {
    Servico *servicos = NULL;
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        // Caso o tipo de arquivo seja TXT
        buffer = fopen("../bd/servicos.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para bin
            servicos = getServicos();
            setTipoArquivo(TXT); // volta para o arquivo txt

            setServicos(servicos); // escreve no txt
            remove("../bd/servicos.bin");
            return NULL;
        }
    }
    if (getTipoArquivo() == BIN) {
        // Caso o tipo de arquivo seja BIN
        buffer = fopen("../bd/servicos.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            servicos = getServicos();
            setTipoArquivo(BIN);
            setServicos(servicos);
            remove("../bd/servicos.txt");
            return NULL;
        }
    }
    if (getTipoArquivo() == MEM) {
        // Caso o tipo de armazenamento seja MEM
        buffer = fopen("../bd/servicos.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT); // muda o tipo de arquivo para TXT
            servicos = getServicos();
            setTipoArquivo(MEM);
            remove("../bd/servicos.txt");
            return servicos;
        }

        buffer = fopen("../bd/servicos.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para BIN
            servicos = getServicos();
            setTipoArquivo(MEM);
            remove("../bd/servicos.bin");
            return servicos;
        }
    }
    return NULL;
}

/**
 * Salva os dados de serviços no formato especificado (TXT ou BIN).
 */
void setServicos(Servico *servicos) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/servicos.txt", "w"); // Abre o arquivo para escrita de texto
        if (buffer != NULL) {
            escrever_arquivo_txt_servico(buffer, servicos);
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/servicos.bin", "wb"); // Abre o arquivo para escrita binária
        if (buffer != NULL) {
            escrever_arquivo_bin_servico(buffer, servicos);
            fclose(buffer);
        }
    }
}

/**
 * Lê os dados de serviços do formato especificado (TXT ou BIN).
 * Retorna um ponteiro para o array de clientes.
 */
Servico *getServicos() {
    FILE *buffer;
    Servico *servicos = NULL;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/servicos.txt", "r"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        servicos = ler_arquivo_txt_servico(buffer);
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/servicos.bin", "rb"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        servicos = ler_arquivo_bin_servico(buffer);
    }
    if (getTipoArquivo() == MEM) {
        return NULL;
    }
    return servicos;
}

/**
 * Lê os serviços de um arquivo TXT e retorna um array dinâmico de `Serviço`.
 */
Servico *ler_arquivo_txt_servico(FILE *buffer) {
    int numServicos = 0; // armazena a cada interação o numero de serviços lidos do arquivo
    Servico *servicos = NULL;
    char Linha[100]; // variavel para receber os dados lidos do arquivo
    int i = 0; // contador para calcular quantos dados foram lidos de casa serviço
    int isPrimeiro = TRUE; // usado para dizer se é a primeira iteração do loop

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        // lê o arquivo linha a linha
        if (isPrimeiro == TRUE) {
            // se for a primeira iteração, cria uma posição de memoria
            servicos = malloc(sizeof(Servico) * (numServicos + 1));
            isPrimeiro = FALSE;
        } else {
            //se não so realoca memeoria para o ponteiro
            servicos = realloc(servicos, (numServicos + 1) * sizeof(Servico));
        }
        // ignora as tegs de registro
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE){
            if(equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break; 
            switch (i) {
                case 0:
                    servicos[numServicos].codigo = atoi(removeTags(Linha));
                    i++;
                    break;
                case 1:
                    strcpy(servicos[numServicos].descricao, removeTags(Linha));
                    i++;
                    break;
                case 2:
                    servicos[numServicos].preco = atof(removeTags(Linha));
                    i++;
                    break;
                case 3:
                    servicos[numServicos].comicao = atof(removeTags(Linha));
                    i++;
                    break;
                case 4:
                    servicos[numServicos].ativo = atoi(removeTags(Linha));
                    i = 0; // Reinicia para ler o próximo serviço
                    numServicos++; // aumenta a qtd de serviços lidos
                    break;
            }
        }
    }
    qtdServicos = numServicos; // Atualiza a quantidade de serviços
    return servicos;
}

/**
 * Escreve os dados de serviço em um arquivo TXT.
 */
void escrever_arquivo_txt_servico(FILE *buffer, Servico *servicos) {
    if (getTamanhoServicos() == 0) setTamanhoServicos(); // caso não tenha nenhum serviço cadastrado

    for (int i = 0; i < getTamanhoServicos(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<descricao>%s</descricao>\n"
                                 "<preco>%.2f</preco>\n"
                                 "<comicao>%.2f</comicao>\n"
                                 "<ativo>%d</ativo>\n"
                                 "</registro>\n",
                                 servicos[i].codigo,
                                 servicos[i].descricao,
                                 servicos[i].preco,
                                 servicos[i].comicao,
                                 servicos[i].ativo
        );
        if (escrevendo < 0) {
            printf("Erro ao escrever no arquivo.\n");
            return;
        }
    }
}

/**
 * Lê os serviço de um arquivo BIN e retorna um array dinâmico de `Serviço`.
 */
Servico *ler_arquivo_bin_servico(FILE *buffer) {
    qtdServicos = 0; // Inicializa a quantidade de serviços
    fseek(buffer, 0, SEEK_END); // Move o ponteiro até o fim do arquivo
    qtdServicos = (int) ftell(buffer) / sizeof(Servico); // Calcula a quantidade de serviços
    fseek(buffer, 0, SEEK_SET); // Volta o ponteiro para o início do arquivo

    Servico *servicos = malloc(sizeof(Servico) * qtdServicos); // Aloca a quantidade exata de serviços

    if (servicos == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para os serviços.\n");
        return NULL;
    }

    fread(servicos, sizeof(Servico), qtdServicos, buffer); // Lê todos os serviços do arquivo e insere no vetor de serviços
    return servicos; // Retorna o vetor de serviços
}


/**
 * Escreve os dados de serviços em um arquivo BIN.
 */
void escrever_arquivo_bin_servico(FILE *buffer, Servico *servicos) {
    for (int i = 0; i < getTamanhoServicos(); i++) {
        if (fwrite(&servicos[i], sizeof(Servico), 1, buffer) != 1) {
            printf("Erro ao escrever no arquivo binário\n");
            break;
        }
    }
}
