#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\bibliotecas/utils.h"
#include "../bibliotecas/importacaoExportacao.h"


void menuImportacaoExportacao(void **cadastrosIE) {
    int opcao = lerInt("O que você deseja fazer?\n"
                        "1 - Importar dados\n"
                        "2 - Exportar dados\n"
                        "0 - Voltar\n"
                        "=>");
    if(opcao == 1) importacaoExportacao(cadastrosIE,1);
    else if(opcao == 2) importacaoExportacao(cadastrosIE,2);
}
void importacaoExportacao(void ** cadastrosIE, int tipo) {
    int vet[] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
    printf("O que você deseja exportar?\n");
    int opcao = lerInt("Cliente? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[0] = TRUE;
    opcao = lerInt("Veiculo? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[1] = TRUE;
    opcao = lerInt("Peça? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[2] = TRUE;
    opcao = lerInt("Fornecedor? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[3] = TRUE;
    opcao = lerInt("Serviço? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[4] = TRUE;
    opcao = lerInt("Funcionário? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[5] = TRUE;
    opcao = lerInt("Oficina? \n 1 - Sim \n 2 - Não \n =>");
    if(opcao == 1) vet[6] = TRUE;
    if(tipo ==1) {
        char * path = lerString("O caminho COMPLETO do arquivo que deseja importar: ");
        if(importDados(cadastrosIE, vet,path) == TRUE) printf("Dados importados com sucesso!\n");
        else printf("Erro ao exportar dados!\n");
    }else {
        if(exportDados(cadastrosIE, vet) == TRUE) printf("Dados exportados com sucesso!\n");
        else printf("Erro ao exportar dados!\n");
    }

}