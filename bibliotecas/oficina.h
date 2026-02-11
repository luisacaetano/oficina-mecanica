//
// Created by costa on 06/11/2024.
//


#ifndef OFICINA_H
#define OFICINA_H

// Estrutura que armazena os dados de uma oficina
typedef struct{
    char nome[100];           // Nome da oficina
    char cpf_cnpj[20];        // CPF ou CNPJ da oficina
    char endereco[150];       // Endereço da oficina
    char telefone[15];        // Telefone da oficina
    char email[100];          // E-mail da oficina
    float porcentagem_lucro;  // Porcentagem de lucro sobre as vendas de peças
} Oficina;

// Model Oficina

int hasOficina();                          // Função que verifica se a oficina está cadastrada
void addOficina();                         // Função que adiciona uma nova oficina
int removeOficina();                       // Função que remove a oficina cadastrada
Oficina *migraDadosOficina();              // Função que migra dados da oficina entre diferentes tipos de arquivo
Oficina *getOficina();                     // Função que retorna a oficina cadastrada
void setOficina(Oficina *oficina);         // Função que atualiza a oficina cadastrada
Oficina *ler_arquivo_txt_oficina(FILE *buffer);  // Função que lê os dados de uma oficina de um arquivo de texto
Oficina *ler_arquivo_bin_oficina(FILE *buffer);  // Função que lê os dados de uma oficina de um arquivo binário
void escrever_arquivo_txt_oficina(FILE *buffer, Oficina *oficina);  // Função que escreve os dados de uma oficina no arquivo de texto
void escrever_arquivo_bin_oficina(FILE *buffer, Oficina *oficina);  // Função que escreve os dados de uma oficina no arquivo binário
void delete_arq_oficina();                 // Função que deleta o arquivo da oficina

// Oficina Controller

Oficina *readOficina();                    // Função que retorna a oficina cadastrada
int createOficina(Oficina **oficina, Oficina *oficina_view);  // Função que cria uma nova oficina
int updateOficina(Oficina **oficina, Oficina *oficina_view);  // Função que atualiza os dados de uma oficina
Oficina *deleteOficina();                  // Função que deleta a oficina cadastrada e retorna NULL

// Oficina View

void menuOficina(Oficina **oficina);       // Função que exibe o menu de opções para interagir com as oficinas
void novaOficina(Oficina *oficina);       // Função que interage com o usuário para cadastrar uma nova oficina
void mostrarOficina(Oficina *oficina);    // Função que exibe os dados de uma oficina
void editarOficina(Oficina *oficina);     // Função que permite editar os dados de uma oficina
void apagarOficina(Oficina **oficina);    // Função que apaga os dados da oficina

#endif //OFICINA_H
