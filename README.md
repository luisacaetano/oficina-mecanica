# Sistema de Oficina Mecânica

Sistema de gerenciamento para oficina mecânica desenvolvido em C como projeto acadêmico.

## Funcionalidades

- Cadastro de clientes e veículos
- Gerenciamento de ordens de serviço
- Controle de peças e estoque
- Geração de relatórios
- Importação e exportação de dados

## Tecnologias

- Linguagem C
- Arquitetura MVC

## Estrutura

- `model/` - Modelos de dados
- `view/` - Interface com usuário
- `controller/` - Lógica de negócio
- `bd/` - Banco de dados
- `relatorio/` - Geração de relatórios
- `importacaoExportacao/` - I/O de dados

## Como Compilar

```bash
gcc -o oficina main.c
./oficina
```
