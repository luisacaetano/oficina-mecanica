#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/cliente.h"
#include "../bibliotecas/utils.h"
#include "../bibliotecas/veiculo.h"
#include "../bibliotecas/peca.h"
#include "../bibliotecas/fornecedor.h"
#include "../bibliotecas/servico.h"
#include "../bibliotecas/funcionario.h"
#include "../bibliotecas/oficina.h"
#include "../bibliotecas/importacaoExportacao.h"

int exportDados(void **cadastros, int *vet) {
    FILE *buffer = fopen("../importacaoExportacao/dadosOficina.txt", "w");
    if (buffer == NULL) {
        printf("Erro ao escrever no arquivo de Exportação\n");
        return FALSE;
    }
    fprintf(buffer, "<dados>\n");

    if (vet[0] != FALSE) {
        fprintf(buffer, "<tabela=clientes>\n");
        Cliente *clientes = (Cliente *) cadastros[0];
        escrever_arquivo_txt_cliente(buffer, clientes);
        fprintf(buffer, "</tabela>\n");
        free(clientes);
    }
    if (vet[1] != FALSE) {
        fprintf(buffer, "<tabela=veiculos>\n");
        Veiculo *veiculos = (Veiculo *) cadastros[1];
        escrever_arquivo_txt_veiculo(buffer, veiculos);
        fprintf(buffer, "</tabela>\n");
        free(veiculos);
    }
    if (vet[2] != FALSE) {
        fprintf(buffer, "<tabela=pecas>\n");
        Peca *pecas = (Peca *) cadastros[2];
        escrever_arquivo_txt_peca(buffer, pecas);
        fprintf(buffer, "</tabela>\n");
        free(pecas);
    }
    if (vet[3] != FALSE) {
        fprintf(buffer, "<tabela=fornecedores>\n");
        Fornecedor *fornecedores = (Fornecedor *) cadastros[3];
        escrever_arquivo_txt_fornecedor(buffer, fornecedores);
        fprintf(buffer, "</tabela>\n");
        free(fornecedores);
    }
    if (vet[4] != FALSE) {
        fprintf(buffer, "<tabela=servicos>\n");
        Servico *servicos = (Servico *) cadastros[4];
        escrever_arquivo_txt_servico(buffer, servicos);
        fprintf(buffer, "</tabela>\n");
        free(servicos);
    }
    if (vet[5] != FALSE) {
        fprintf(buffer, "<tabela=funcionarios>\n");
        Funcionario *funcionarios = (Funcionario *) cadastros[5];
        escrever_arquivo_txt_funcionario(buffer, funcionarios);
        fprintf(buffer, "</tabela>\n");
        free(funcionarios);
    }
    if (vet[6] != FALSE) {
        fprintf(buffer, "<tabela=oficina>\n");
        Oficina *oficina = (Oficina *) cadastros[6];
        escrever_arquivo_txt_oficina(buffer, oficina);
        fprintf(buffer, "</tabela>\n");
        free(oficina);
    }
    fprintf(buffer, "</dados>\n");
    fclose(buffer);

    return TRUE;
}

int importDados(void **cadastros, int *vet, char *path) {
    FILE *buffer = fopen(path, "r");
    if (buffer == NULL) {
        printf("Erro ao ler no arquivo de Importação ou Arquivo não encontrado\n");
        return FALSE;
    }
    Cliente *clientes = NULL;
    Veiculo *veiculos = NULL;
    Peca *pecas = NULL;
    Fornecedor *fornecedores = NULL;
    Servico *servicos = NULL;
    Funcionario *funcionarios = NULL;
    Oficina *oficina = NULL;

    char linha[100];
    fgets(linha, sizeof(linha), buffer); // ignora a primeira linha <dados>
    while (equalsString(filtrarSoATag(linha), "</dados>") != TRUE) {
        fgets(linha, sizeof(linha), buffer);
        if (equalsString(filtrarSoATag(linha), "<tabela=clientes>") == TRUE) {
            clientes = ler_arquivo_txt_cliente(buffer);
        } else if (equalsString(filtrarSoATag(linha), "<tabela=veiculos>") == TRUE) {
            veiculos = ler_arquivo_txt_veiculo(buffer);
        } else if (equalsString(filtrarSoATag(linha), "<tabela=pecas>") == TRUE) {
            pecas = ler_arquivo_txt_peca(buffer);
        } else if (equalsString(filtrarSoATag(linha), "<tabela=fornecedores>") == TRUE) {
            fornecedores = ler_arquivo_txt_fornecedor(buffer);
        } else if (equalsString(filtrarSoATag(linha), "<tabela=servicos>") == TRUE) {
            servicos = ler_arquivo_txt_servico(buffer);
        } else if (equalsString(filtrarSoATag(linha), "<tabela=funcionarios>") == TRUE) {
            funcionarios = ler_arquivo_txt_funcionario(buffer);
        } else if (equalsString(filtrarSoATag(linha), "<tabela=oficina>") == TRUE) {
            oficina = ler_arquivo_txt_oficina(buffer);
        }
    }

    for (int i = 0; i < 7; i++) {
        if (vet[i] == TRUE) {
            switch (i) {
                case 0:
                    cadastros[i] = clientes;
                    if (getTipoArquivo() != MEM) setClientes(clientes);
                    break;
                case 1:
                    cadastros[i] = veiculos;
                    if (getTipoArquivo() != MEM) setVeiculos(veiculos);
                    break;
                case 2:
                    cadastros[i] = pecas;
                    if (getTipoArquivo() != MEM) setPecas(pecas);
                    break;
                case 3:
                    cadastros[i] = fornecedores;
                    if (getTipoArquivo() != MEM) setFornecedores(fornecedores);
                    break;
                case 4:
                    cadastros[i] = servicos;
                    if (getTipoArquivo() != MEM) setServicos(servicos);
                    break;
                case 5:
                    cadastros[i] = funcionarios;
                    if (getTipoArquivo() != MEM) setFuncionarios(funcionarios);
                    break;
                case 6:
                    cadastros[i] = oficina;
                    if (getTipoArquivo() != MEM) setOficina(oficina);
                    break;
            }
        }
    }
    /*for(int i = 0; i < 7; i++) {
        if(vet[i] == FALSE) {
           cadastros[i] = NULL;
            switch (i) {
                case 0:
                    editTamanhoClientes(0);
                break;
                case 1:
                    editTamanhoVeiculos(0);
                break;
                case 2:
                    editTamanhoPecas(0);
                break;
                case 3:
                    editTamanhoFornecedores(0);
                break;
                case 4:
                    editTamanhoServicos(0);
                break;
                case 5:
                    editTamanhoFuncionarios(0);
                break;
                case 6:
                    removeOficina();
                break;
            }
        }
    }*/
    return TRUE;
}