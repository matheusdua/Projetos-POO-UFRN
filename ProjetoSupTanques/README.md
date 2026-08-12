# 🏭 SupTanques - Sistema Supervisório Cliente-Servidor (SCADA)

## 📌 Sobre o Projeto

Este projeto implementa um sistema supervisório (SCADA) completo em C++ projetado para monitorar e controlar remotamente uma planta industrial simulada. A planta é composta por dois tanques interligados, uma bomba de entrada, válvulas solenoide e sensores de nível e vazão.

A aplicação adota uma arquitetura estrita de **Cliente-Servidor**, comunicando-se através de uma rede TCP/IP na porta local `23456`. O projeto abrange desde a simulação da física dos fluidos no _back-end_ (servidor) até a renderização gráfica em tempo real no _front-end_ (cliente Qt) e acesso via terminais CLI.

## 🛠️ Destaques Técnicos e Arquitetura

- **Comunicação TCP/IP Customizada (`MySocket`):** Em vez de depender de bibliotecas de alto nível para rede, o projeto implementa um _wrapper_ multiplataforma (compatível com WinSock2 no Windows e `sys/socket.h` no Linux) para gerenciar _sockets stream_. O código inclui o gerenciamento de filas de conexão (`mysocket_queue`), utilizando a chamada de sistema `select` para I/O multiplexado e não-bloqueante.

- **Multithreading e Concorrência (`std::thread` / `std::mutex`):**
- O **Servidor** instencia uma thread dedicada unicamente para a simulação matemática contínua da física da planta e outra para aceitar conexões e despachar requisições de múltiplos clientes de forma simultânea.

- O **Cliente** lança uma thread de _polling_ para solicitar pacotes de dados atualizados (`CMD_GET_DATA`) periodicamente, enquanto a thread principal mantém a interface GUI responsiva. O acesso à escrita no _socket_ e à memória compartilhada é rigorosamente protegido por `std::mutex` para evitar condições de corrida (_race conditions_).

- **Simulação Física de Fluidos Avançada:** O motor matemático do servidor (`Tanks::simulate`) integra numericamente a variação de volume nos tanques. Ele respeita parâmetros rigorosos: tanques com 28cm de altura máxima, orifício de transição a 6.5cm e limite de transbordamento aos 25cm. A simulação emula a imperfeição de sensores reais aplicando ruído dinâmico gaussiano calculado via Transformada de Box-Muller.

- **Controle de Acesso Baseado em Perfis (RBAC):** O servidor autentica clientes exigindo _Login_ e _Senha_ (cadastros de 6 a 12 caracteres). A arquitetura define os perfis de **Administrador** (autorizado a atuar nos comandos `CMD_SET_V1`, `CMD_SET_V2` e `CMD_SET_PUMP`) e **Visualizador** (somente leitura), rejeitando tentativas de atuação não autorizadas com pacotes `CMD_ERROR`.

## ⚙️ Componentes do Sistema

### 1. O Servidor (`SupServidor`)

- Responsável por manter o modelo matemático da planta em processamento contínuo independente da quantidade de clientes conectados.

- Gerencia ativamente a autenticação e registro de usuários iterando sobre uma `std::list<User>`.

- Empacota e transmite o estado da planta via estrutura binária compacta (`SupState`), informando níveis, vazões e alertas de transbordamento.

### 2. O Cliente Visual (`SupClienteQt`)

- Interface gráfica construída nativamente com o framework **Qt**.

- Processa as respostas binárias do servidor e as renderiza diretamente manipulando pixels com o `QPainter` sobre a classe `SupImg`.

- **Modo Level:** Exibe graficamente o preenchimento da água, o escoamento entre orifícios, o status de abertura das válvulas e pisca um ícone de alerta de `O-FLO` caso os limites do tanque sejam violados.

- **Modo Graph:** Plota uma série temporal bidimensional mapeando o histórico das variáveis físicas `h1` e `h2` ao longo do tempo da sessão.

### 3. O Cliente Terminal (`SupClienteTerm`)

- Uma versão CLI (Interface de Linha de Comando) robusta e leve, utilizando exatamente a mesma infraestrutura de rede da GUI, ideal para depuração do protocolo ou operação _headless_.

## 🚀 Como Compilar e Rodar

O projeto está modularizado em diretórios independentes. É necessário compilar e iniciar o **Servidor** antes de estabelecer as conexões com os **Clientes**.

### Compilando o Servidor (Code::Blocks / Terminal)

No Windows, é mandatório garantir a linkagem da API do WinSock (`Ws2_32`).

1. Abra o arquivo `SupServidor.cbp` na sua IDE Code::Blocks.

2. Garanta que a diretiva `-lWs2_32` esteja presente nas opções de Linker.

3. Faça o build do executável (F9).
4. No menu em terminal do Servidor, cadastre um usuário administrador (Opção 22) e inicie as instâncias da planta (Opção 0).

_Compilação manual via terminal (exemplo GCC/MinGW):_

```bash
g++ -std=c++17 -Wall -I../MySocket ../MySocket/mysocket.cpp supservidor_main.cpp supservidor.cpp supdados.cpp tanques.cpp -o servidor -lWs2_32

```

### Compilando o Cliente Gráfico (Qt Creator)

1. Abra o arquivo de projeto do Qt `SupCliente.pro` no **Qt Creator**.

2. Configure o projeto associando-o ao seu kit de compilação desktop (O arquivo instrui a configuração `CONFIG += c++17`).

3. Realize a compilação e execute a interface.
4. Na barra superior, clique em "Connection -> Login...", insira IP `127.0.0.1` e informe os dados previamente cadastrados no terminal do servidor.

### Compilando o Cliente Terminal

Caso deseje testar a concorrência do servidor através de uma interface de linha de comando:

1. Compile via Code::Blocks com o projeto `SupCliente.cbp` com o alvo de terminal.

2. _Ou de forma direta:_

```bash
g++ -std=c++17 -Wall -I../MySocket ../MySocket/mysocket.cpp supcliente_main_term.cpp supcliente_term.cpp supcliente.cpp supdados.cpp -o cliente_cli -lWs2_32

```
