# 🗺️ Planejador de Caminhos - Roteamento com Algoritmo A\*

## 📌 Sobre o Projeto

Este projeto implementa um sistema de planejamento de rotas altamente eficiente projetado para encontrar o caminho mais curto entre dois pontos geográficos em um mapa (baseado em cidades e entroncamentos do estado do Rio Grande do Norte).

O motor principal da aplicação é a implementação manual do **Algoritmo A\*** (A-Star), um dos mais poderosos algoritmos de busca em grafos. O sistema realiza a leitura dinâmica de arquivos de texto contendo vértices (cidades/coordenadas) e arestas (rotas rodoviárias), validando meticulosamente os dados de entrada contra dezenas de cenários de corrupção de arquivo.

## 🛠️ Destaques Técnicos e Arquitetura

Ao contrário de abordagens de força bruta, este projeto destaca o uso idiomático e avançado do C++ moderno:

- **Algoritmo A\* e Heurística Espacial:** A busca pelo menor caminho prioriza nós baseando-se na função de custo `f(n) = g(n) + h(n)`. A heurística `h(n)` foi implementada utilizando a **Fórmula de Haversine**, que calcula a distância euclidiana real entre duas coordenadas globais (Latitude/Longitude) considerando a curvatura da Terra.

- **Domínio da STL (Standard Template Library):** Substituição de laços tradicionais pesados por algoritmos otimizados da biblioteca `<algorithm>` (como `std::find`, `std::find_if` com _lambdas_ e `std::upper_bound`) operando sobre estruturas de dados adequadas (`std::vector`, `std::list` e `std::stack`) para gerenciar as listas _Aberto_ e _Fechado_.

- **File I/O e Parsing Rigoroso:** Leitura segura de fluxos de dados (`ifstream`) extraindo atributos separados por delimitadores (`pontos.txt` e `rotas.txt`), com rotinas nativas para limpeza de espaços em branco (trimming).

- **Tratamento de Exceções (Exception Handling):** Arquitetura blindada com blocos `try-catch` profundos. O sistema foi validado contra 48 cenários intencionais de arquivos corrompidos (erros de sintaxe, identificadores ausentes, coordenadas impossíveis) sem apresentar falhas de execução (_crashes_) ou perda de estado.

- **Profiling de Desempenho:** Uso da biblioteca `<chrono>` para aferir o tempo de execução do motor de roteamento em milissegundos.

## ⚙️ Funcionalidades

A aplicação fornece um menu iterativo (`planejador-main.cpp`) que permite:

1. Imprimir o mapa de pontos registrados no sistema.
2. Listar todas as rotas e conexões disponíveis.
3. Calcular e imprimir o caminho ótimo entre uma origem e um destino informando o tempo de busca, a quantidade de nós avaliados e o traçado detalhado (passo a passo) da rota.

## 🚀 Como Compilar e Rodar

O projeto foi construído garantindo compatibilidade multiplataforma. É recomendado o uso do terminal (ambiente Linux/Debian) para compilação nativa.

### Opção 1: Via Terminal (GCC/G++)

Abra o terminal na raiz do projeto e compile os arquivos fonte:

```bash
g++ -std=c++11 planejador.cpp planejador-main.cpp -o planejador_rotas

```

Para iniciar a aplicação interativa:

```bash
./planejador_rotas

```

**Executando a Bateria de Testes:**
Para rodar a suíte rigorosa de testes de estresse e I/O (que valida o comportamento do sistema contra os arquivos corrompidos na pasta `arq_teste/`):

```bash
g++ -std=c++11 planejador.cpp teste.cpp -o testes_planejador
./testes_planejador

```

### Opção 2: Via Code::Blocks

Abra o projeto importando o arquivo `planejador.cbp` na sua IDE Code::Blocks e pressione **Build and Run** (F9). Certifique-se de que os arquivos `pontos.txt` e `rotas.txt` estejam no mesmo diretório do executável gerado.
