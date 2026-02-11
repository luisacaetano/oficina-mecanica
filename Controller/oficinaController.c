#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/oficina.h"
#include "../bibliotecas/utils.h"

// Função para ler a oficina, retornando um ponteiro para a oficina atual
Oficina *readOficina() {
    return getOficina();  // Retorna a oficina obtida pela função getOficina
}

// Função para criar uma nova oficina, associando um novo objeto de oficina ao ponteiro
int createOficina(Oficina **oficina, Oficina *oficina_view) {
    *oficina = oficina_view;  // Atribui o valor de oficina_view ao ponteiro oficina

    // Se o tipo de arquivo não for de memória (MEM), salva a oficina no arquivo
    if (getTipoArquivo() != MEM)
        setOficina(*oficina);  // Salva a oficina no arquivo
    return TRUE;  // Retorna verdadeiro, indicando sucesso
}

// Função para atualizar a oficina, associando um novo objeto de oficina ao ponteiro
int updateOficina(Oficina **oficina, Oficina *oficina_view) {
    *oficina = oficina_view;  // Atribui o valor de oficina_view ao ponteiro oficina

    // Se o tipo de arquivo não for de memória (MEM), atualiza a oficina no arquivo
    if (getTipoArquivo() != MEM)
        setOficina(*oficina);  // Atualiza a oficina no arquivo
    return TRUE;  // Retorna verdadeiro, indicando sucesso
}

// Função para excluir a oficina, removendo-a do sistema e retornando NULL
Oficina *deleteOficina() {
    removeOficina();  // Remove a oficina atual
    delete_arq_oficina();  // Deleta os dados da oficina no arquivo
    return NULL;  // Retorna NULL, indicando que a oficina foi excluída
}
