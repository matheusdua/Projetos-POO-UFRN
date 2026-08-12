# 🎨 Designer - Motor Vetorial 2D (Exportação SVG)

## 📌 Sobre o Projeto

Este projeto consiste na implementação de um motor de renderização bidimensional vetorial. A aplicação permite a criação de desenhos complexos através da composição de formas geométricas básicas (Linhas, Círculos, Retângulos, Losangos, Triângulos e Estrelas).

O motor oferece duas vias de entrada: a interpretação de roteiros descritos em arquivos de texto plano (`.txt`) ou a construção programática direta via código C++. O resultado de ambas as vias é unificado em um gerador estruturado que compila o canvas e exporta as imagens no padrão web XML **SVG (Scalable Vector Graphics)**. Como prova de conceito da robustez matemática do motor, o repositório inclui os mapeamentos exatos para renderizar as bandeiras do Brasil, Cuba, Estados Unidos e Grécia.

## 🛠️ Destaques Técnicos e Arquitetura Orientada a Objetos

O cerne deste projeto é a aplicação prática e rigorosa do **Polimorfismo** em C++. O código foi blindado contra o "code smell" de condicionais excessivas (`switch`/`if` de tipos) ao delegar o comportamento para a resolução em tempo de execução (_Dynamic Dispatch_).

- **Hierarquia de Classes e Métodos Virtuais:** Todo o sistema orbita uma classe base abstrata `Shape` e sua sub-base `FilledShape`. Formas geométricas concretas herdam essas bases e sobrescrevem funções puramente virtuais essenciais como `read()` (para _parsing_ de arquivos), `save()` (para injeção SVG) e `clone()` (para clonagem no _heap_).

- **Gerenciamento Dinâmico de Memória:** O motor gráfico (`Designer`) gerencia o canvas através de um contêiner de ponteiros polimórficos (`std::vector<Shape*>`). A alocação (`new`) e desalocação (`delete`) são estritamente controladas para evitar vazamentos de memória (Memory Leaks).

- **Deep Copy e a Regra dos Cinco:** A classe `Designer` possui implementação completa para Cópia e Movimento (Construtores e Operadores de Atribuição), permitindo a transferência segura e clonagem profunda de todo o vetor de ponteiros polimórficos na memória sem corromper as instâncias originais.

- **Geometria Computacional:** Algoritmos trigonométricos embutidos no polimorfismo das classes, como a classe `Star` que projeta pontos nos eixos cartesianos dinamicamente calculando ângulos (`sin`/`cos`) e raios alternados.

## ⚙️ Funcionalidades

- **Leitura de Projetos (`.txt`):** O interpretador processa comandos identificados por caracteres singulares (`R`, `C`, `S`, `L`, `T`, `-`), extraindo cor (RGB), espessura e limites geométricos de forma robusta e livre de falhas estruturais por separadores (espaços/TAB).

- **Exportação Vetorial (`.svg`):** Geração sem perdas visuais. O código traduz instâncias geométricas perfeitamente em tags XML padronizadas pelo W3C (`<rect>`, `<circle>`, `<polygon>`, `<line>`).

## 🚀 Como Compilar e Rodar

O projeto possui flexibilidade total na compilação, podendo utilizar os binários em qualquer ambiente que possua o compilador C++ (G++ nativo ou IDEs).

### Opção 1: Geração via Arquivos de Texto (Terminal)

Compile as classes de base com o main do interpretador de texto:

```bash
g++ -std=c++11 designer.cpp shape.cpp designer-main.cpp -o designer

```

Execute a aplicação. Ela pedirá o `.txt` de entrada e o `.svg` de saída:

```bash
./designer
# Input sugerido: cuba.txt
# Output sugerido: cuba.svg

```

### Opção 2: Geração Programática (C++)

Para testar a geração construída puramente por instâncias de objetos C++ em memória (exemplo da bandeira dos Estados Unidos ou Grécia), compile o arquivo correspondente substituindo o main genérico:

```bash
g++ -std=c++11 designer.cpp shape.cpp eua.cpp -o eua_bandeira
./eua_bandeira

```

_(O script rodará silenciosamente no terminal e o arquivo `eua.svg` será criado na raiz do diretório)._

### Opção 3: Code::Blocks

Abra o projeto importando o arquivo `Designer.cbp` na sua IDE Code::Blocks. O arquivo de gerenciamento já possui o link adequado de todas as bibliotecas padrão necessárias para executar e _buildar_ o programa com (F9).
