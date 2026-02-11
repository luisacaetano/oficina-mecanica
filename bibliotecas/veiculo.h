#ifndef VEICULO_H
#define VEICULO_H

typedef struct {
    int codigo;            // Código identificador do veículo
    char modelo[20];       // Nome do modelo do veículo
    char marca[20];        // Nome da marca do veículo
    int anofabricacao;     // Ano de fabricação do veículo
    char chassi[17];       // Código do chassi do veículo
    char placa[8];         // Placa do veículo (formato padrão de 7 caracteres)
    int ativo;             // Indicador de ativo (1 para ativo, -1 para inativo)
} Veiculo;

// Model Veículos
int getTamanhoVeiculos();                         // Retorna a quantidade de veículos cadastrados
void setTamanhoVeiculos();                        // Incrementa a quantidade de veículos cadastrados
void editTamanhoVeiculos(int tamanho);            // Atualiza a quantidade de veículos cadastrados
Veiculo* migraDadosVeiculo();                     // Migra os dados dos veículos entre diferentes formatos de arquivo
void setVeiculos(Veiculo *veiculo);               // Atualiza a lista de veículos cadastrados
Veiculo* getVeiculos();                           // Retorna todos os veículos cadastrados
Veiculo* ler_arquivo_txt_veiculo(FILE *buffer);   // Lê os dados dos veículos de um arquivo de texto
void escrever_arquivo_txt_veiculo(FILE *buffer, Veiculo *veiculos);  // Escreve os dados dos veículos em um arquivo de texto
Veiculo* ler_arquivo_bin_veiculo(FILE *buffer);   // Lê os dados dos veículos de um arquivo binário
void* escrever_arquivo_bin_veiculo(FILE *buffer, Veiculo *veiculos); // Escreve os dados dos veículos em um arquivo binário

// Controller Veículos
Veiculo* readVeiculos();                          // Retorna a lista de veículos cadastrados
int createVeiculo(Veiculo **veiculos, Veiculo *veiculo);  // Cria um novo veículo
int showVeiculo(Veiculo *veiculos, int id);       // Exibe as informações de um veículo
int updateVeiculo(Veiculo *veiculos, Veiculo *veiculo);   // Atualiza os dados de um veículo
int deleteVeiculo(Veiculo *veiculos, int id);     // Remove um veículo pelo ID
int saveVeiculoCSV(Veiculo * veiculos, int tamanho);  // Salva os dados dos veículos em um arquivo CSV
Veiculo * filterVeiculoModelo(Veiculo *veiculos, char * modelo, int *tamanho);  // Filtra os veículos por modelo

// View Veículos

void menuVeiculo(Veiculo **veiculos);             // Exibe o menu para manipulação dos veículos
void novoVeiculo(Veiculo **veiculos);             // Interage com o usuário para cadastrar um novo veículo
void mostrarVeiculo(Veiculo *veiculos);           // Mostra as informações de um veículo específico
void apagarVeiculo(Veiculo *veiculos);            // Remove um veículo
void editarVeiculo(Veiculo *veiculos);            // Permite editar as informações de um veículo
int buscaNovoIDVeiculo(Veiculo *veiculos);        // Busca um novo ID válido para cadastro de um novo veículo
void mostrarTodosVeiculos(Veiculo *veiculos);     // Exibe todos os veículos cadastrados
void filtrarVeiculosIDModelo(Veiculo *veiculos);  // Filtra os veículos por ID ou modelo
#endif //VEICULO_H
