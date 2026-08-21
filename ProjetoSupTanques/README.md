# 🏭 SupTanques - Sistema Supervisório Cliente-Servidor (SCADA)

## 📌 Sobre o Projeto

Este projeto implementa um sistema supervisório (SCADA) completo em C++ projetado para monitorar e controlar remotamente uma planta industrial simulada. A planta é composta por dois tanques interligados, uma bomba de entrada, válvulas solenoide e sensores de nível e vazão.

A aplicação adota uma arquitetura estrita de **Cliente-Servidor**, comunicando-se através de uma rede TCP/IP na porta local `23456`. O projeto abrange desde a simulação da física dos fluidos no _back-end_ (servidor) até a renderização gráfica em tempo real no _front-end_ (cliente Qt) e acesso via terminais CLI.

## 🛠️ Destaques Técnicos e Arquitetura

- **Comunicação TCP/IP Customizada e Multiplataforma (`MySocket`):** Em vez de depender de bibliotecas de alto nível para rede, o projeto implementa um _wrapper_ nativo para gerenciar _sockets stream_. O código possui identificação automática de sistema operacional via diretivas de pré-processador (`#ifdef`), compilando de forma nativa e transparente tanto no Windows (WinSock2) quanto no Linux/macOS (`sys/socket.h`). A biblioteca inclui o gerenciamento de filas de conexão (`mysocket_queue`), utilizando a chamada de sistema `select` para I/O multiplexado e não-bloqueante.
- **Multithreading e Concorrência (`std::thread` / `std::mutex`):**
- O **Servidor** instancia uma thread dedicada unicamente para a simulação matemática contínua da física da planta e outra para aceitar conexões e despachar requisições de múltiplos clientes de forma simultânea.
- O **Cliente** lança uma thread de _polling_ para solicitar pacotes de dados atualizados (`CMD_GET_DATA`) periodicamente, enquanto a thread principal mantém a interface GUI responsiva. O acesso à escrita no _socket_ e à memória compartilhada é rigorosamente protegido por `std::mutex` para evitar condições de corrida (_race conditions_).

- **Simulação Física de Fluidos Avançada:** O motor matemático do servidor (`Tanks::simulate`) integra numericamente a variação de volume nos tanques. Ele respeita parâmetros rigorosos: tanques com 28cm de altura máxima, orifício de transição a 6.5cm e limite de transbordamento aos 25cm. A simulação emula a imperfeição de sensores reais aplicando ruído dinâmico gaussiano calculado via Transformada de Box-Muller.
- **Controle de Acesso Baseado em Perfis (RBAC):** O servidor autentica clientes exigindo _Login_ e _Senha_ (cadastros de 6 a 12 caracteres). A arquitetura define os perfis de **Administrador** (autorizado a atuar nos comandos `CMD_SET_V1`, `CMD_SET_V2` e `CMD_SET_PUMP`) e **Visualizador** (somente leitura), rejeitando tentativas de atuação não autorizadas com pacotes `CMD_ERROR`.

## ⚙️ Componentes do Sistema

### 0. A Biblioteca Base (`MySocket`)

- Módulo independente responsável por abstrair a complexidade dos sockets do sistema operacional. Possui rotinas próprias de teste para garantir a integridade da comunicação TCP antes de rodar a aplicação principal.

### 1. O Servidor (`SupServidor`)

- Responsável por manter o modelo matemático da planta em processamento contínuo independente da quantidade de clientes conectados.
- Gerencia ativamente a autenticação e registro de usuários.
- Empacota e transmite o estado da planta via estrutura binária compacta (`SupState`).

### 2. O Cliente Visual (`SupClienteQt`)

- Interface gráfica construída nativamente com o framework **Qt**.
- Processa as respostas binárias do servidor e as renderiza diretamente manipulando pixels com o `QPainter` sobre a classe `SupImg`.

### 3. O Cliente Terminal (`SupClienteTerm`)

- Uma versão CLI (Interface de Linha de Comando) robusta e leve, utilizando exatamente a mesma infraestrutura de rede da GUI, ideal para depuração do protocolo ou operação _headless_.

---

## 🚀 Como Compilar e Rodar

Por ser um sistema distribuído, a execução ocorre em etapas: você deve **primeiro iniciar o Servidor** e, somente depois, abrir os **Clientes** para se conectarem a ele.

Todos os projetos podem ser compilados via **Code::Blocks** (utilizando os arquivos `.cbp` já configurados com as flags automáticas para Linux/Windows) ou diretamente pelo **Terminal**.

### Passo 1: Testando a Biblioteca `MySocket` (Opcional)

Antes de rodar a planta, você pode verificar se a sua infraestrutura de rede local está compilando os sockets corretamente.

- **Via Code::Blocks:** Abra `MySocket/mysocket.cbp` e clique em _Build and Run_.
- **Via Terminal (Linux):** Navegue até a pasta `MySocket` e rode:

```bash
g++ -std=c++17 -Wall mysocket.cpp mysocket-teste.cpp -o mysocket_teste -pthread
./mysocket_teste

```

- **Via Terminal (Windows):**

```bash
g++ -std=c++17 -Wall mysocket.cpp mysocket-teste.cpp -o mysocket_teste.exe -lws2_32
mysocket_teste.exe

```

_A saída esperada é: `Criei objetos MySocket!_`

### Passo 2: Inicializando o Servidor

O servidor precisa estar rodando para gerenciar a física e aceitar os logins.

- **Via Code::Blocks:** Abra `SupTanques/SupServidor.cbp` e clique em _Build and Run_.
- **Via Terminal (Linux):** Navegue até a pasta `SupTanques` e rode:

```bash
g++ -std=c++17 -Wall -I../MySocket ../MySocket/mysocket.cpp supservidor_main.cpp supservidor.cpp supdados.cpp tanques.cpp -o servidor -pthread
./servidor

```

- **Via Terminal (Windows):** Troque a flag `-pthread` no comando acima por `-lws2_32` e o `-o servidor` por `-o servidor.exe`.

> ⚠️ **Configuração Inicial do Servidor:**
> Ao abrir o terminal do servidor, escolha a **Opção 22** para criar um usuário (ex: Login: `admin123`, Senha: `admin123`, Administrador: `S`). Depois, escolha a **Opção 0** para ligar a planta.

### Passo 3: Conectando os Clientes

Com o servidor rodando e o usuário criado, você pode abrir quantas instâncias de cliente desejar simultaneamente.

**A. Cliente Gráfico (GUI - Requer Qt)**

1. Abra o arquivo `SupCliente.pro` na IDE **Qt Creator** (O projeto já exige `CONFIG += c++17`).
2. Faça o _Build and Run_ (`Ctrl+R`).
3. Na barra superior da interface, vá em `Connection -> Login...`
4. Use o IP `127.0.0.1` (localhost) e as credenciais que você criou no Passo 2.

**B. Cliente Terminal (CLI)**

1. **Via Code::Blocks:** Abra `SupTanques/SupCliente.cbp` e clique em _Build and Run_.
2. **Via Terminal (Linux):** Dentro da pasta `SupTanques`, rode:

```bash
g++ -std=c++17 -Wall -I../MySocket ../MySocket/mysocket.cpp supcliente_main_term.cpp supcliente_term.cpp supcliente.cpp supdados.cpp -o cliente_cli -pthread
./cliente_cli

```

3. Escolha a **Opção 1** para conectar, digite IP `127.0.0.1` e informe suas credenciais. O terminal passará a exibir o estado em tempo real e habilitará os comandos de controle das válvulas e bomba.
