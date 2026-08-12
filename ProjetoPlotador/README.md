# 📈 Plotador de Funções Matemáticas (Qt/C++)

## 📌 Sobre o Projeto

Este projeto é uma aplicação de interface gráfica (GUI) para desktop desenvolvida em C++ moderno em conjunto com o framework **Qt**. O software funciona como um plotador interativo, capaz de renderizar gráficos de múltiplas funções matemáticas bidimensionais simultaneamente em um plano cartesiano customizável.

O projeto foi construído sob uma arquitetura limpa, separando o motor de avaliação matemática (_Evaluator_) da lógica de renderização gráfica visual, garantindo alta performance na avaliação de coordenadas.

## 🛠️ Destaques Técnicos e Arquitetura

- **Parser Matemático Customizado (Evaluator):** Em vez de utilizar bibliotecas externas prontas, a aplicação conta com um avaliador léxico e sintático próprio. Ele utiliza estruturas de pilhas (`std::stack`) para analisar strings matemáticas e interpretar funções trigonométricas (`sin`, `cos`, `tan`), exponenciais (`exp`), logarítmicas (`log`) e raízes quadradas (`sqrt`) em tempo de execução.

- **Mapeamento de Coordenadas Geométricas:** Implementação de algoritmos de conversão bidirecional (Matemática ↔ Tela). As funções `convXtoJ`, `convYtoI`, `convJtoX` e `convItoY` mapeiam os valores flutuantes contínuos dos eixos reais (X, Y) para os pixels discretos da tela (matriz J, I) de forma dinâmica.

- **Renderização Customizada com QPainter:** A área do gráfico não é um componente pronto. As curvas, eixos e marcadores são traçados manualmente pixel a pixel sobre um `QPixmap` usando `QPainter` e `QLineF`.

- **Promoção de Widgets e Overrides de Eventos:** O projeto eleva o uso do Qt promovendo um simples `QLabel` para uma classe customizada `Grafico`. Essa classe estende funcionalidades vitais do sistema:

- **`resizeEvent`:** Sobrescrito para recalcular a geometria e redesenhar instantaneamente o gráfico sempre que a janela for redimensionada pelo usuário.

- **`mouseReleaseEvent`:** Sobrescrito para capturar cliques na tela, converter as coordenadas de pixel para as coordenadas matemáticas do gráfico e exibi-las diretamente na _StatusBar_.

## ⚙️ Funcionalidades

- **Inclusão de Múltiplas Curvas:** Interface de diálogo para inserir novas funções matemáticas e atribuir a elas cores distintas (Azul, Vermelho, Verde ou Preto) para fácil identificação visual.

- **Controle de Eixos em Tempo Real:** Painel de controle completo com `QSpinBox` permitindo ajustar dinamicamente os limites Mínimo e Máximo dos eixos X e Y, além de aplicar multiplicadores de ordem de grandeza (Expoentes).

- **Tabela de Gerenciamento:** Tabela lateral (`QTableWidget`) que lista as funções ativas em tempo real junto a uma pré-visualização de suas cores.

- **Interação via Clique:** Clique em qualquer ponto da malha gráfica para inspecionar os valores exatos de X e Y no plano cartesiano.

## 🚀 Como Compilar e Rodar

Este projeto utiliza o sistema de compilação qmake padronizado pelo Qt. O ambiente recomendado é o **Qt Creator**.

1. Abra o **Qt Creator**.
2. Selecione `Abrir Projeto` (Open Project) e escolha o arquivo `Plotador.pro` localizado na pasta principal do projeto.

3. Configure o kit de compilação para o seu sistema (Garante-se o uso da flag `CONFIG += c++17`).
4. Pressione **Build and Run** (ou o atalho `Ctrl+R`) para compilar as dependências de interface (`.ui`) e executar a aplicação.

_(As bibliotecas base do Qt Core e Qt Gui Widgets devem estar devidamente instaladas no ambiente de desenvolvimento)._
