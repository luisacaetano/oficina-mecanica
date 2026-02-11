#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../bibliotecas/utils.h"
//função para facilitar a leitura de int no terminal
int lerInt(char* msg) {
    printf("%s", msg);
    int valor;
    setbuf(stdin, NULL);
    scanf("%d", &valor);
    return valor;
}
//função para facilitar a leitura de float no terminal
float lerFloat(char* msg) {
    printf("%s", msg);
    float valor;
    setbuf(stdin, NULL);
    scanf("%f", &valor);
    return valor;
}
//função para facilitar a leitura de string no terminal
char* lerString(char* msg) {
    printf("%s", msg);
    char* valor = malloc(sizeof(char) * 100);
    setbuf(stdin, NULL);
    gets(valor);
    //fgets(valor, sizeof(valor), stdin);
    setbuf(stdin, NULL);
    return valor;
}
// remove os tokens do xml
char* removeTags(char* dado) {
    char *stringResult = malloc(1); // Aloca um espaço inicial de 1 byte (para o terminador nulo)
    if (stringResult == NULL) {
        return NULL; // Retorna NULL se a alocação falhar
    }
    stringResult[0] = '\0'; // Inicializa a string vazia

    int i = 0;
    // Ignora o conteúdo antes do primeiro '>'
    while (dado[i] != '>' && dado[i] != '\0') {
        i++;
    }
    i++; // Avança para o caractere após '>'

    // Adiciona os caracteres entre as tags '<' e '>'
    while (dado[i] != '<' && dado[i] != '\0') {
        int tamanhoAtual = strlen(stringResult);
        stringResult = realloc(stringResult, tamanhoAtual + 2); // Realoca memória para adicionar um novo caractere
        if (stringResult == NULL) {
            return NULL; // Retorna NULL em caso de falha de alocação
        }
        strncat(stringResult, &dado[i], 1); // Adiciona o caractere à string resultante
        i++;
    }

    return stringResult; // Retorna o conteúdo extraído entre as tags
}

/*char* removeTags(char* dado) {
    //retorna o onde está o token >
    char* inicio = strchr(dado, '>');
    inicio++;
    char* fim = strchr(inicio, '<');
    int tamanho = fim - inicio;
    char *conteudo = malloc(sizeof (char));
    if (conteudo == NULL) {
        return NULL;
    }
    strncpy(conteudo, inicio, tamanho);
    inicio = NULL;
    fim = NULL;
    return conteudo; // Retorna o conteúdo extraído
}*/

//funçãoq eu compara se duas strings são iguais
int equals(char* s1, char* s2) {
    //if (strlen(s1) != strlen(s2)) return FALSE;
    for (int i = 0; i < strlen(s1); i++) {
        if (s1[i] == '\n' && s2[i] == '\n') return TRUE; // esse if serva para ler no txt sem isso, não funciona
        if (s1[i] != s2[i]) return FALSE;
    }
    return FALSE;
}

int tipoArquivo = 0;// variavel global que diz o tipo de arquivo que o usuario deseja salvar os dados

void setTipoArquivo(int Arquivo){//muda o tipo de arquivo que vai ser trabalhado durante a execução do programa
    tipoArquivo = Arquivo;
}
int getTipoArquivo() {// retorna o tipo de arquivo que vai ser trabalhado durante a execução do programa
    return tipoArquivo;
}
// retorna a partir da data atual, mais 30 dias
time_t addtrintaDias(time_t dataAtual) {
    return dataAtual + (30 * 24 * 3600);
}
//mostra a data já formatada
void printData(time_t data) {
    struct tm *info;
    info = localtime(&data);
    printf("%d/%d/%d\n", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900);
}
void printDataHora(time_t dataHora) {
    struct tm *info;
    info = localtime(&dataHora);
    printf("%d/%d/%d %d:%d\n", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900,info->tm_hour,info->tm_min);
}
//converte a data para o formato time_t
time_t converteData(int dia, int mes, int ano) {
    struct tm data;
    data.tm_mday = dia;
    data.tm_mon = mes - 1;
    data.tm_year = ano - 1900;
    data.tm_hour = 0;
    data.tm_min = 0;
    data.tm_sec = 0;
    return mktime(&data);
}
time_t converteDataHora(int dia, int mes, int ano, int hora) {
    struct tm data;
    data.tm_mday = dia;
    data.tm_mon = mes - 1;
    data.tm_year = ano - 1900;
    data.tm_hour = hora;
    data.tm_min = 0;
    data.tm_sec = 0;
    return mktime(&data);
}
//trasforma qualquer tipo de dado em string Obs: DEVE-SE PASSAR NO TIPO O SEGUINTE: i ID, f FLOAT, d DATA
char * transformaString(void *dado, char tipo) {
    if (dado == NULL) {
        return NULL;  // Retorna NULL se o ponteiro for inválido
    }

    char *string = malloc(50); // Aloca espaço suficiente
    if (string == NULL) {
        return NULL; // Falha na alocação
    }

    // Tenta identificar o tipo pelo tamanho do dado
    if (tipo == 'i') {
        sprintf(string, "%d", *((int *)dado));
        return string;

    }
    if (tipo == 'f') {
        float dadoF = *((float *)dado);  // Correção: mantém como float
        sprintf(string, "%.2f", dadoF);
    }
    if (tipo == 'd'){
        sprintf(string, "%ld", *((long *)dado));
    }

    return string;
}
//concatena duas strings com ponto e virgula
char* concatenarStringPontoEVirgula(const char* str1, const char* str2) {
    // Calcula o tamanho total necessário para a string resultante
    size_t tamanhoTotal = strlen(str1) + strlen(str2) + 2;  // +2 para o ';' e o terminador nulo '\0'

    // Aloca memória para a string resultante
    char* resultado = malloc(tamanhoTotal);

    if (resultado == NULL) {
        return NULL;  // Retorna NULL em caso de falha na alocação
    }

    // Copia a primeira string para o resultado
    strcpy(resultado, str1);

    // Adiciona o ponto e vírgula
    strcat(resultado, ";");

    // Concatena a segunda string
    strcat(resultado, str2);

    return resultado;
}


void escreverCSV(char ** dados, char * nomeArquivo,int tamanho) {
        char nome[50] ="../relatorio/";
    strcat(nome,nomeArquivo);
    strcat(nome,".txt");
    FILE *arquivo = fopen(nome, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    for(int i = 0; i < tamanho; i++){
        fprintf(arquivo, "%s\n", dados[i]);
    }
    fclose(arquivo);

}

int equalsString(char* s1, char* s2) {
    if (strlen(s1) != strlen(s2)) return FALSE;
    for (int i = 0; i < strlen(s1); i++) {
        if (s1[i] != s2[i]) return FALSE;
    }
    return TRUE;
}

int comparelimitesDatas(time_t dataI, time_t dataF, time_t dataComparacao) {
    return (dataComparacao >= dataI && dataComparacao <= dataF) ? TRUE: FALSE;
}
//função responsavel por filtar as tags de um xml
char *filtrarSoATag(char *dado) {
    char *stringResult = malloc(1); // Aloca um espaço inicial para o terminador nulo
    if (stringResult == NULL) {
        return NULL;
    }
    stringResult[0] = '\0'; // Inicializa como string vazia

    int i = 0;
    while (dado[i] != '\0') {
        if (dado[i] == '<') {
            // Encontra o início da tag
            int j = i;
            while (dado[j] != '>' && dado[j] != '\0') {
                j++;
            }
            if (dado[j] == '>') {
                // Confirma que há um fechamento de tag
                int tamanhoAtual = strlen(stringResult);
                int tamanhoTag = j - i + 1;
                stringResult = realloc(stringResult, tamanhoAtual + tamanhoTag + 1);
                if (stringResult == NULL) {
                    return NULL;
                }
                strncat(stringResult, &dado[i], tamanhoTag);
                i = j; // Avança para o próximo caractere após '>'
                return stringResult;
            }
        }
        i++;
    }
    return stringResult;
}

int *separaVetor(char *str, int tam) {
    int *result = malloc(sizeof(int) * tam);
    if (result == NULL) {
        return NULL; // Evita problemas caso malloc falhe
    }

    int i = 0;
    int tamanhoStr = strlen(str); // Armazena o tamanho para evitar chamadas repetidas

    for (int j = 0; j < tamanhoStr; j++) {
        if (i >= tam) break; // Evita estouro de memória

        if (str[j] != ',') {
            result[i] = str[j] - '0'; // Converte char para int
            i++; // Incrementa corretamente
        }
    }

    return result;
}





