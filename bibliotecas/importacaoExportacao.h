#ifndef IMPORTACAOEXPORTACAO_H
#define IMPORTACAOEXPORTACAO_H
int exportDados(void ** cadastros, int * vet);

void menuImportacaoExportacao(void **cadastrosIE);
void importacaoExportacao(void ** cadastrosIE, int tipo);
int importDados(void **cadastros, int *vet, char *path);
int exportDados(void **cadastros, int *vet);
#endif //IMPORTACAOEXPORTACAO_H
