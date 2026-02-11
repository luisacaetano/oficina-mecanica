#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/oficina.h"

// Variável global indicando se a oficina está ativa
int oficina = FALSE;

// Função para verificar se há uma oficina registrada (retorna o status)
int hasOficina() {
    return oficina;  // Retorna o status da oficina (TRUE ou FALSE)
}

// Função para ativar a oficina (marca a oficina como ativa)
void addOficina() {
    oficina = TRUE;  // Marca a oficina como ativa
}

// Função para remover a oficina (marca a oficina como inativa)
int removeOficina() {
    oficina = FALSE;  // Marca a oficina como inativa
    return FALSE;  // Retorna FALSE indicando que a oficina foi removida
}

// Função para migrar os dados da oficina entre arquivos de texto, binário e memória
Oficina *migraDadosOficina() {
    Oficina *oficina = NULL;
    FILE *buffer;

    // Caso o tipo de arquivo seja TXT, converte para BIN
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/oficina.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN);
            oficina = getOficina();
            setTipoArquivo(TXT);
            setOficina(oficina);
            remove("../bd/oficina.bin");  // Remove o arquivo binário
            return NULL;
        }
    }
    // Caso o tipo de arquivo seja BIN, converte para TXT
    else if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/oficina.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            oficina = getOficina();
            setTipoArquivo(BIN);
            setOficina(oficina);
            remove("../bd/oficina.txt");  // Remove o arquivo de texto
            return NULL;
        }
    }
    // Caso o tipo de arquivo seja MEM, verifica arquivos TXT e BIN
    else if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/oficina.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(TXT);
            oficina = getOficina();
            setTipoArquivo(MEM);
            remove("../bd/oficina.txt");  // Remove o arquivo de texto
            addOficina();  // Marca a oficina como ativa
            return oficina;
        }

        buffer = fopen("../bd/oficina.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            setTipoArquivo(BIN);
            oficina = getOficina();
            setTipoArquivo(MEM);
            remove("../bd/oficina.bin");  // Remove o arquivo binário
            addOficina();  // Marca a oficina como ativa
            return oficina;
        }
    }

    buffer = NULL;
    free(buffer);  // Libera a memória de buffer
    return NULL;  // Retorna NULL se não houver dados para migrar
}

// Função para salvar a oficina em um arquivo, dependendo do tipo de arquivo (TXT ou BIN)
void setOficina(Oficina *oficina) {
    FILE *buffer;

    // Se o tipo de arquivo for TXT, grava em formato texto
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/oficina.txt", "w");  // Abre o arquivo de texto para escrita
        if (buffer != NULL) {
            escrever_arquivo_txt_oficina(buffer, oficina);  // Escreve a oficina no arquivo texto
            fclose(buffer);  // Fecha o arquivo
            return;
        }
    }
    // Se o tipo de arquivo for BIN, grava em formato binário
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/oficina.bin", "wb");  // Abre o arquivo binário para escrita
        if (buffer != NULL) {
            escrever_arquivo_bin_oficina(buffer, oficina);  // Escreve a oficina no arquivo binário
            fclose(buffer);  // Fecha o arquivo
        }
    }
}

// Função para obter a oficina, dependendo do tipo de arquivo (TXT, BIN ou MEM)
Oficina *getOficina() {
    FILE *buffer;
    Oficina *oficina = NULL;

    // Se o tipo de arquivo for TXT, lê os dados do arquivo de texto
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/oficina.txt", "r");  // Abre o arquivo de texto para leitura
        if (buffer == NULL) {
            free(oficina);  // Libera memória caso ocorra erro
            return NULL;
        }
        oficina = ler_arquivo_txt_oficina(buffer);  // Lê a oficina do arquivo de texto
    }
    // Se o tipo de arquivo for BIN, lê os dados do arquivo binário
    else if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/oficina.bin", "rb");  // Abre o arquivo binário para leitura
        if (buffer == NULL) {
            free(oficina);  // Libera memória caso ocorra erro
            return NULL;
        }
        oficina = ler_arquivo_bin_oficina(buffer);  // Lê a oficina do arquivo binário
    }
    // Se o tipo de arquivo for MEM, não há dados para ler
    else if (getTipoArquivo() == MEM) {
        return NULL;  // Retorna NULL, pois não há dados na memória
    }

    fclose(buffer);  // Fecha o arquivo após leitura
    buffer = NULL;
    return oficina;  // Retorna o ponteiro para a oficina lida
}

// Função para ler os dados da oficina a partir de um arquivo de texto
Oficina *ler_arquivo_txt_oficina(FILE *buffer) {
    int i = 0;
    int isPrimeiro = TRUE;
    Oficina *oficina = NULL;
    char linha[200];

    // Lê linha por linha do arquivo
    while (fgets(linha, sizeof(linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            oficina = malloc(sizeof(Oficina));  // Aloca memória para a oficina
            if (oficina == NULL) {
                printf("Erro ao alocar memória para oficina\n");
                return NULL;
            }
            isPrimeiro = FALSE;
        }

        // Processa cada linha, removendo as tags e atribuindo os valores aos campos da oficina
        if (equalsString(filtrarSoATag(linha), "<registro>") != TRUE && equalsString(filtrarSoATag(linha), "</registro>") != TRUE){
            if(equalsString(filtrarSoATag(linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    strcpy(oficina->nome, removeTags(linha));  // Atribui o nome da oficina
                    i++;
                    break;
                case 1:
                    strcpy(oficina->cpf_cnpj, removeTags(linha));  // Atribui o CPF ou CNPJ
                    i++;
                    break;
                case 2:
                    strcpy(oficina->endereco, removeTags(linha));  // Atribui o endereço
                    i++;
                    break;
                case 3:
                    strcpy(oficina->telefone, removeTags(linha));  // Atribui o telefone
                    i++;
                    break;
                case 4:
                    strcpy(oficina->email, removeTags(linha));  // Atribui o email
                    i++;
                    break;
                case 5:
                    oficina->porcentagem_lucro = atof(removeTags(linha));  // Atribui a porcentagem de lucro
                    addOficina();  // Marca a oficina como ativa
                    i = 0;  // Reseta o contador
                    break;
            }
        }
    }

    return oficina;  // Retorna o ponteiro para a oficina lida
}

// Função para escrever os dados da oficina em um arquivo de texto
void escrever_arquivo_txt_oficina(FILE *buffer, Oficina *oficina) {
    fprintf(buffer,
            "<registro>\n"
            "<nome>%s</nome>\n"
            "<cpf_cnpj>%s</cpf_cnpj>\n"
            "<endereco>%s</endereco>\n"
            "<telefone>%s</telefone>\n"
            "<email>%s</email>\n"
            "<porcentagem_lucro>%.2f</porcentagem_lucro>\n"
            "</registro>\n",
            oficina->nome,
            oficina->cpf_cnpj,
            oficina->endereco,
            oficina->telefone,
            oficina->email,
            oficina->porcentagem_lucro);
    addOficina();  // Marca a oficina como ativa
}

// Função para ler os dados da oficina a partir de um arquivo binário
Oficina *ler_arquivo_bin_oficina(FILE *buffer) {
    Oficina *oficina = malloc(sizeof(Oficina));  // Aloca memória para a oficina
    if (oficina == NULL) {
        printf("Erro ao alocar memória para oficina\n");
        return NULL;
    }

    if (fread(oficina, sizeof(Oficina), 1, buffer) != 1) {  // Lê os dados da oficina do arquivo binário
        free(oficina);  // Libera memória se ocorrer erro
        return NULL;
    }

    return oficina;  // Retorna o ponteiro para a oficina lida
}

// Função para escrever os dados da oficina em um arquivo binário
void escrever_arquivo_bin_oficina(FILE *buffer, Oficina *oficina) {
    fwrite(oficina, sizeof(Oficina), 1, buffer);  // Escreve os dados da oficina no arquivo binário
    addOficina();  // Marca a oficina como ativa
}

// Função para excluir o arquivo da oficina, dependendo do tipo de arquivo (TXT ou BIN)
void delete_arq_oficina() {
    if (getTipoArquivo() == TXT) {
        remove("../bd/oficina.txt");  // Remove o arquivo de texto
        fopen("../bd/oficina.txt", "w");  // Recria o arquivo de texto vazio
    }
    if (getTipoArquivo() == BIN) {
        remove("../bd/oficina.bin");  // Remove o arquivo binário
        fopen("../bd/oficina.bin", "bw");  // Recria o arquivo binário vazio
    }
}
