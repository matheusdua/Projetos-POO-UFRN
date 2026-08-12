# 🧮 BigInt - Calculadora de Inteiros de Precisão Infinita

## 📌 Sobre o Projeto

Este projeto consiste na implementação de uma classe C++ customizada chamada `BigInt`, projetada para representar e operar sobre números inteiros com uma quantidade **ilimitada de dígitos**.

Diferente dos tipos primitivos nativos (`int`, `long long int`), que possuem limites estritos de memória estourando em _overflows_, a classe `BigInt` gerencia dinamicamente a alocação de memória para cada dígito. O projeto culmina em uma calculadora interativa via terminal capaz de realizar operações matemáticas complexas com números colossalmente grandes.

Desenvolvido originalmente como parte dos estudos de Programação Avançada na Universidade Federal do Rio Grande do Norte (UFRN).

## 🛠️ Destaques Técnicos e Arquitetura

Para demonstrar domínio dos fundamentos do C++, este projeto foi construído **sem** a utilização de contêineres ou algoritmos da STL (como `std::vector` ou `std::list`). Tudo foi feito _from scratch_ (do zero).

- **Gerenciamento Manual de Memória:** Uso extensivo de ponteiros brutos (`int8_t*`) e primitivas `new`/`delete` para alocação exata da memória necessária para os dígitos.

- **A "Regra dos Cinco" (Rule of Five):** Implementação completa de destrutores, construtores de cópia, construtores por movimento e operadores de atribuição para garantir ausência de _memory leaks_ e eficiência extrema.

- **Sobrecarga de Operadores (Operator Overloading):** Implementação de dezenas de operadores (aritméticos, lógicos, relacionais e de fluxo `<<` `>>`) para que objetos `BigInt` se comportem sintaticamente como tipos primitivos da linguagem.

- **Algoritmos Matemáticos:** Implementação de lógicas manuais para soma com "vai-um" (carry), subtração com empréstimo (borrow), multiplicação aninhada e divisão longa, além de fatoriais iterativos e rotacionamento de bits (`<<`, `>>`).

## ⚙️ Funcionalidades da Calculadora

Ao rodar a aplicação principal (`bigint-main.cpp`), você terá acesso a um menu interativo que suporta:

- Entrada de números gigantes através do console.

- Operações básicas: Soma (`+`), Subtração (`-`), Multiplicação (`*`), Divisão Quociente (`/`) e Resto (`%`).

- Operações avançadas: Fatorial (`!`), Valor Absoluto (`abs`) e Inversão de Sinal (`inv`).

- Rotacionamento de dígitos à esquerda (`<<`) e à direita (`>>`).

## 🚀 Como Compilar e Rodar

Você pode compilar o projeto facilmente via terminal, o que é altamente recomendado em ambientes Linux (como Debian/Ubuntu), ou utilizar o Code::Blocks.

### Opção 1: Via Terminal (GCC/G++)

Abra o terminal na pasta do projeto e compile a interface da calculadora junto com a classe:

```bash
g++ -std=c++11 bigint.cpp bigint-main.cpp -o calculadora_bigint

```

Para executar:

```bash
./calculadora_bigint

```

(Nota: Você também pode trocar o `bigint-main.cpp` por qualquer arquivo de teste de 1 a 8, como `teste1.cpp`, para compilar e rodar as baterias de validação unitária).

### Opção 2: Via Code::Blocks

Basta abrir o arquivo do projeto (`Projeto_BigInt.cbp`) na IDE Code::Blocks, clicar em **Build and Run** (F9) e interagir com o terminal gerado.
