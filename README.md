# 🚀 Projetos de Programação Avançada em C++ e Qt

Este repositório consolida a coleção de projetos desenvolvidos para a disciplina de **Programação Avançada** do curso de Engenharia de Computação da Universidade Federal do Rio Grande do Norte (UFRN), ministrada pelo Prof. Adelardo Adelino Dantas de Medeiros.

Os projetos focam na aplicação rigorosa de conceitos fundamentais e avançados de Engenharia de Software, abrangendo desde o gerenciamento manual de memória dinâmica e Estruturas de Dados (STL), até o desenvolvimento de Interfaces Gráficas (GUI) e comunicação em rede nativa utilizando arquiteturas multithread. O código foi desenvolvido em C++ moderno (C++17) e projetado para compilação multiplataforma.

---

## 📂 Visão Geral dos Projetos

### 1. ♾️ Projeto BigInt (Matemática de Precisão Arbitrária)

Uma biblioteca criada do zero para modelar números inteiros infinitos, contornando as limitações físicas dos tipos nativos (`long long int`) do C++.

- **Gerenciamento Dinâmico de Memória:** Controle e alocação estrita de memória na heap via `new` e `delete` para lidar com arrays dinâmicos na representação estrutural da classe.

- **Rule of Five Semantics:** Implementação manual de construtores de cópia, movimento (_Move Semantics_) e operadores de atribuição para lidar de forma segura com o ciclo de vida dos ponteiros internos.

- **Sobrecarga de Operadores:** Mapeamento polimórfico de toda a álgebra fundamental e booleana (`+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `!`, `++`, `<`, `==`) operando dígito a dígito de trás para a frente no armazenamento em memória.

### 2. 🗺️ Planejador de Caminhos (Busca com Algoritmo A\*)

Um motor de roteamento inteligente capaz de determinar o caminho ótimo entre coordenadas (cidades) do estado do Rio Grande do Norte.

- **Inteligência Artificial (Busca em Grafos):** Implementação nativa do algoritmo **A\*** (A-Star) para calcular o caminho de menor custo.

- **Heurística de Haversine:** Otimização do cálculo de distância euclidiana espacial lidando com cálculos trigonométricos envolvendo as latitudes e longitudes lidas.

- **STL Robusta:** Utilização ampla da Standard Template Library do C++ (`std::vector`, `std::list`, `std::stack`) e tratamento estrito de exceções (try/catch) durante a validação e processamento de erros (parsing) de dezenas de arquivos de texto.

### 3. 📈 Plotador de Funções Matemáticas

Uma aplicação gráfica visual interativa em Qt que plota séries matemáticas customizadas em um plano cartesiano com marcações calculadas dinamicamente.

- **Avaliador de Expressões (Parser):** Integração de um Avaliador Léxico/Sintático (_Abstract Syntax Tree_ derivado da `evaluator.cpp`) para digerir a string da função fornecida pelo usuário e retornar vetores matemáticos reais.

- **Eventos Interativos em Qt:** Uso avançado de _Event Handlers_ nativos (`QResizeEvent` para redimensionamento limpo da interface e `QMouseEvent` para emitir sinais com a coordenada geográfica ao se clicar no gráfico).

- **Renderização Customizada:** Desenho nativo usando `QPainter` e `QPixmap` mapeando coordenadas cartesianas virtuais (X, Y) para a matriz dimensional gráfica nativa (I, J).

### 4. 📐 Designer de Formas Geométricas

Uma _engine_ vetorial de desenho bidimensional voltada à renderização paramétrica e exportação nativa no padrão vetorizado da Web (`.svg`).

- **Polimorfismo Puro e Herança:** O núcleo do projeto aplica conceitos profundos de herança em C++, utilizando funções virtuais puras (Classes Abstratas como `Shape` e `FilledShape`) e rotinas focadas no encadeamento polimórfico.

- **Tratamento Geométrico Otimizado:** Cálculos trigonométricos avançados na definição e projeção de retas, círculos, triângulos, losangos e estrelas pentagramas parametrizadas por eixos focais.

### 5. 🏭 SupTanques (Sistema SCADA Cliente-Servidor)

Um sistema supervisório completo (SCADA) desenvolvido para monitorar e controlar remotamente uma planta industrial simulada composta por tanques, válvulas e bombas.

- **Arquitetura Cliente-Servidor:** Implementa uma biblioteca customizada nativa para Sockets TCP/IP (`MySocket`), garantindo a comunicação multiplexada e não-bloqueante entre o servidor da planta e múltiplos clientes conectados.

- **Multithreading:** O servidor utiliza threads dedicadas para integrar a simulação matemática e física dos fluidos da planta de forma contínua, isolando a lógica de processamento de rede.

- **Interfaces Gráfica e CLI:** Possui um front-end gráfico construído com **Qt** para renderização animada dos componentes (modo nível e modo gráfico) e uma versão _headless_ focada no terminal.

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C++ (Padrão ISO C++17)
- **Framework de GUI:** Qt 5 / Qt 6 (`QMainWindow`, `QPainter`, `QPixmap`, Signals & Slots)
- **Bibliotecas Nativas (Redes e Concorrência):** `WinSock2` / `sys/socket.h`, `<thread>`, `<mutex>`
- **Bibliotecas (Dados e Core):** Standard Template Library (STL), File I/O (`<fstream>`)
- **Ferramentas e Ambientes:** Compatível com os compiladores e integrações em VS Code, Code::Blocks e Qt Creator.

---

## 🚀 Como Compilar e Executar

A arquitetura dos projetos é modular e as dependências focam unicamente no pacote Qt e no suporte nativo ao protocolo TCP/IP no SO.

### Projetos com Interface Gráfica Qt (SupTanques Cliente, Plotador)

1. Certifique-se de possuir o **Qt Creator** devidamente configurado em sua máquina.
2. Abra o arquivo `.pro` do respectivo projeto (ex: `Plotador.pro`).
3. Confirme que o seu kit de compilação reflete os módulos GUI e a diretiva obrigatória `CONFIG += c++17`.
4. Compile e execute pelo IDE.

### Projetos Focados em Console (Planejador, BigInt, Designer, Servidor SCADA)

1. Navegue até o diretório do respectivo projeto. Os projetos fornecem arquivos `.cbp` otimizados para o **Code::Blocks**.
2. **Importante (Redes no Windows):** Para iniciar os sockets TCP em distribuições não POSIX (Windows), é mandatório checar se a diretiva estática do compilador incluiu a biblioteca `-lWs2_32`.
3. Alternativamente, no terminal de um ambiente Linux/MinGW:

```bash
g++ -std=c++17 -Wall -O3 <arquivos_fontes_principais.cpp> -o app_executavel
./app_executavel

```

---

> **Nota Acadêmica:** Os esqueletos básicos iniciais de declarações dos cabeçalhos, arquivos do Qt Designer e os protocolos especificados compõem o modelo de arquitetura idealizado e fornecido pelo Prof. Adelardo. Todas as regras de negócio, modelagem inteligente de memória em ponteiros C++, _parsing_ e algoritmos de grafos (A*), *event handlers\* personalizados no Qt e a base estrutural de comunicação via threads foram integralmente codificadas para atender às demandas de aprovação das avaliações do curso.
