#ifndef SERVICO_H
#define SERVICO_H
typedef struct {
    int codigo;
    char descricao[30];
    float preco;
    float comicao;
    int ativo;
}Servico;
//modelServicos

int getTamanhoServicos(); //retorna a quantidade de serviços cadastrado
void setTamanhoServicos(); // aumenta sempre 1 unidade na quantidade de serviços
void editTamanhoServicos(int tamanho); // atualiza a quantidade de serviços
Servico *ler_arquivo_txt_servico(FILE *buffer); // função que le do arquivo txt e retorna um ponteiro do tipo serviço
void escrever_arquivo_txt_servico(FILE *buffer, Servico *servicos); // escreve no arquivo txt de serviço
Servico *ler_arquivo_bin_servico(FILE *buffer); // função que le do arquivo binario e retorna um ponteiro do tipo serviço
void escrever_arquivo_bin_servico(FILE *buffer, Servico *servicos); // escreve no arquivo binario de serviço
Servico *getServicos(); // função que deve ser usada para receber os dados lidos do arquivo
void setServicos(Servico *servicos); // função que deve ser usada para atualizar as informações do serviço
Servico *migraDadosServicos(); // migra os dados entre os diferentes tipos de arquivos (txt e bin) e para a memoria

//servicoController

Servico * readServicos(); //função que retorna os serviços já cadastrados
int createServico(Servico **servicos, Servico *servico); // função que cria um novo serviço
int showServico(Servico *servicos, int codigo); // função que procura o serviço desejado
int updateServico(Servico *servicos, Servico *servico);// função que atualiza um serviço
int deleteServico(Servico *servicos, int codigo);// função que apaga um serviço
int buscaNovoIDServico(Servico *servicos);// função que encontra um id valido para um novo serviço
int saveServicoCSV(Servico *servicos, int tamanho); // função que salva os serviços em um arquivo CSV
Servico *filterServicoDescricao(Servico *servicos, char *descricao, int *tamanho); // função que filtra os serviços por descrição

// servicoView
void menuServicos(Servico **servicos); //menu que inclue todas as operações disponiveis para manipular os serviços
void novoServico(Servico **servicos); // função que interage com o usuario para cadastrar um novo serviço
void mostrarServico(Servico *servicos);// função que interage com o usuario para mostrar os dados de um serviço
void editarServico(Servico *servicos);// função que interage com o usuario para editar um serviço
void apagarServico(Servico *servicos); // função que interage com o usuario para apagar um serviço
void mostrarTodosServicos(Servico *servicos); // função que mostra o ID e a descrição de todos os serviços
void filtrarServicosIDDescricao(Servico *servicos); // função que filtra os serviços por ID ou descrição

#endif //SERVICO_H
