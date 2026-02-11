#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/utils.h"
#include "../bibliotecas/cliente.h"


int qtdClientes = 0;
// Função para cadastrar cliente no arquivo
int getTamanhoClientes() {
    return qtdClientes;
}

void setTamanhoClientes() {
    qtdClientes++;
}
void editTamanhoClientes(int tamanho) {
    qtdClientes = tamanho;
}
// Função para migrar os dados do cliente entre arquivos de texto, binário e memória
Cliente *migraDadosCliente() {
    Cliente *clientes = NULL;
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        // se o usuario deseja gravar os dados no txt
        buffer = fopen("../bd/clientes.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(BIN); // muda o tipo de arquivo para bin
            clientes = getClientes();
            setTipoArquivo(TXT);// volta para o arquivo txt

            setClientes(clientes);// escreve no txt
            free(buffer);
            remove("../bd/clientes.bin");
            return NULL;
        }
        fclose(buffer);
        free(buffer);
    }
    if (getTipoArquivo() == BIN) {
        //se o usuario deseja gravar em binario
        buffer = fopen("../bd/clientes.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            setTipoArquivo(TXT);
            clientes = getClientes();
            setTipoArquivo(BIN);
            setClientes(clientes);
            remove("../bd/clientes.txt");
            return NULL;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
    if (getTipoArquivo() == MEM) {
        buffer = fopen("../bd/clientes.txt", "r");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            //se antes ele tiver gravado os dados em txt
            setTipoArquivo(TXT); // muda o tipo de arquivo para
            clientes = getClientes();
            setTipoArquivo(MEM);
            remove("../bd/clientes.txt");
            return clientes;
        }
        fclose(buffer);
        free(buffer);

        buffer = fopen("../bd/clientes.bin", "rb");
        if (buffer != NULL) {
            fclose(buffer);
            free(buffer);
            //se antes ele tiver gravado os dados em binario
            setTipoArquivo(BIN); // muda o tipo de arquivo para
            clientes = getClientes();
            setTipoArquivo(MEM);
            remove("../bd/clientes.bin");
            return clientes;
        }
        fclose(buffer);
        free(buffer);
        return NULL;
    }
}
void setClientes(Cliente *clientes) {
    FILE *buffer;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/clientes.txt", "w"); // Abre o arquivo corretamente
        if (buffer != NULL) {
            escrever_arquivo_txt_cliente(buffer, clientes);
            fclose(buffer);
            return;
        }
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/clientes.bin", "wb"); // Abre o arquivo corretamente
        if (buffer != NULL) {
            escrever_arquivo_bin_cliente(buffer, clientes);
            fclose(buffer);
        }
    }
}

Cliente *getClientes() {
    FILE *buffer;
    Cliente *clientes = NULL;
    if (getTipoArquivo() == TXT) {
        buffer = fopen("../bd/clientes.txt", "r"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        clientes = ler_arquivo_txt_cliente(buffer);
    }
    if (getTipoArquivo() == BIN) {
        buffer = fopen("../bd/clientes.bin", "rb"); // Abre o arquivo corretamente
        if (buffer == NULL) {
            return NULL;
        }
        clientes = ler_arquivo_bin_cliente(buffer);
    }
    if (getTipoArquivo() == MEM) {
        return NULL;
    }
    return clientes;
}
Cliente *ler_arquivo_txt_cliente(FILE *buffer) {
    int numClientes = 0;
    Cliente *clientes = NULL;
    char Linha[100];
    int i = 0;
    int isPrimeiro = TRUE;

    while (fgets(Linha, sizeof(Linha), buffer) != NULL) {
        if (isPrimeiro == TRUE) {
            clientes = malloc(sizeof(Cliente) * (numClientes + 1));
            isPrimeiro = FALSE;
        }else if (isPrimeiro == FALSE) clientes = realloc(clientes, (numClientes + 1) * sizeof(Cliente));
        if (equalsString(filtrarSoATag(Linha), "<registro>") != TRUE && equalsString(filtrarSoATag(Linha), "</registro>") != TRUE){
            if(equalsString(filtrarSoATag(Linha), "</tabela>") == TRUE) break;
            switch (i) {
                case 0:
                    clientes[numClientes].codigo = atoi(removeTags(Linha));
                i++;
                break;
                case 1:
                    strcpy(clientes[numClientes].nome, removeTags(Linha));
                i++;
                break;
                case 2:
                    strcpy(clientes[numClientes].cpf_cnpj, removeTags(Linha));
                i++;
                break;
                case 3:
                    strcpy(clientes[numClientes].endereco, removeTags(Linha));
                i++;
                break;
                case 4:
                    strcpy(clientes[numClientes].telefone, removeTags(Linha));
                i++;
                break;
                case 5:
                    strcpy(clientes[numClientes].email, removeTags(Linha));
                i++;
                break;
                case 6:
                    clientes[numClientes].ativo = atoi(removeTags(Linha));
                i = 0; // Reinicia para ler o próximo cliente
                numClientes++;
                clientes = realloc(clientes, (numClientes + 1) * sizeof(Cliente));
                break;
            }
            }
    }
    qtdClientes = numClientes;
    return clientes;
}

void escrever_arquivo_txt_cliente(FILE *buffer, Cliente *clientes) {
    if (getTamanhoClientes() == 0) setTamanhoClientes(); // se acaso não tiver nenhum cliente cadastrado

    for (int i = 0; i < getTamanhoClientes(); i++) {
        int escrevendo = fprintf(buffer,
                                 "<registro>\n"
                                 "<codigo>%d</codigo>\n"
                                 "<nome>%s</nome>\n"
                                 "<cpf_cnpj>%s</cpf_cnpj>\n"
                                 "<endereco>%s</endereco>\n"
                                 "<telefone>%s</telefone>\n"
                                 "<email>%s</email>\n"
                                 "<ativo>%d</ativo>\n"
                                 "</registro>\n",
                                 clientes[i].codigo,
                                 clientes[i].nome,
                                 clientes[i].cpf_cnpj,
                                 clientes[i].endereco,
                                 clientes[i].telefone,
                                 clientes[i].email,
                                 clientes[i].ativo
        );
        if (escrevendo < 0) {
            return;
        }
    }
}
Cliente * ler_arquivo_bin_cliente( FILE *buffer){
     qtdClientes = 0;
    fseek(buffer,0,SEEK_END);// move o ponteiro até fim do arquivo
    qtdClientes = (int) ftell(buffer)/sizeof(Cliente); //pega a quantidade de clientes
    fseek(buffer,0,SEEK_SET); //volta o ponteiro para o inicio da arquivo
    Cliente * clientes = malloc(sizeof(Cliente) * qtdClientes); //aloca a qtd exata de clientes

    if (clientes == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória para os clientes.\n");
        return NULL;
    }

    fread(clientes,sizeof(Cliente),qtdClientes,buffer); // le todos os clientes do arquivo e insere no vetor de clientes
    return clientes;
}
/*Cliente *ler_arquivo_bin_cliente(FILE *buffer) {
    qtdClientes = 0;
    Cliente *clientes = malloc(sizeof(Cliente) * (getTamanhoClientes() + 1));
    int i = 0;
    while (fread(&clientes[i], sizeof(Cliente), 1, buffer)) {
        i++;
        setTamanhoClientes();
        clientes = realloc(clientes, (getTamanhoClientes() + 1) * sizeof(Cliente));
    }
    return clientes;
}*/

void *escrever_arquivo_bin_cliente(FILE *buffer, Cliente *clientes) {
    for (int i = 0; i < getTamanhoClientes(); i++) {
        if (fwrite(&clientes[i], sizeof(Cliente), 1, buffer)) {
        }
    }
}
