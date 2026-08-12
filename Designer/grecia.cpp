#include <iostream>
#include <cmath> // Para round
#include <string>
#include "designer.h"

using namespace std;

/// Programa para desenhar e exportar a bandeira da Grecia, usando a classe Designer

int main(void)
{
    // A constante ALTURA_LINHA significa quantos pixels de altura vai ter
    // cada uma das 9 linhas da bandeira.
    // As demais dimensoes s�o calculadas para manter as proporcoes corretas.
    const int ALTURA_LINHA = 60;
    const int altura_bandeira = 9 * ALTURA_LINHA;
    const int largura_bandeira = 810;

    // Variaveis auxiliares
    int r, g, b; // Cor
    int lin1, lin2, col1, col2;
    int i;

    Designer X(largura_bandeira, altura_bandeira);

    // Um contorno preto
    r = g = b = 0;
    lin1 = 0;
    lin2 = altura_bandeira;
    col1 = 0;
    col2 = largura_bandeira;
    X.include(Rectangle(r, g, b, 1, false, col1, col2, lin1, lin2));

    // Desenha as 9 linhas
    for (i = 0; i < 9; ++i)
    {
        if (i % 2 == 0) // azul
        {
            r = 13;
            g = 94;
            b = 175;
        }
        else // BRANCO
        {
            r = 255;
            g = 255;
            b = 255;
        }
        lin1 = i * ALTURA_LINHA + 1;
        lin2 = (i + 1) * ALTURA_LINHA;
        col1 = 1;
        col2 = largura_bandeira - 1;
        X.include(Rectangle(r, g, b, 1, true, col1, col2, lin1, lin2));
        /*
        // PARA DEBUG
        cout << "Linha " << i << ':';
        cout << " xmin=" << col1 << " xmax=" << col2
             << " ymin=" << lin1 << " ymax=" << lin2 << endl;
        */
    }

    // Desenha o campo azul p/ cruz
    r = 13;
    g = 94;
    b = 175;
    lin1 = 1;
    lin2 = 5 * ALTURA_LINHA;
    col1 = 1;
    col2 = 5 * ALTURA_LINHA;
    X.include(Rectangle(r, g, b, 1, true, col1, col2, lin1, lin2));
    /*
    // PARA DEBUG
    cout << "Campo azul:";
    cout << " xmin=" << col1 << " xmax=" << col2
         << " ymin=" << lin1 << " ymax=" << lin2 << endl;
    */

    // Desenha a cruz branco
    r = g = b = 255;
    // branco horizontal cruz
    lin1 = 2 * ALTURA_LINHA + 1;
    lin2 = 3 * ALTURA_LINHA;
    col1 = 1;
    col2 = 5 * ALTURA_LINHA;
    X.include(Rectangle(r, g, b, 1, true, col1, col2, lin1, lin2));

    // branco vertical cruz
    lin1 = 1;
    lin2 = 5 * ALTURA_LINHA;
    col1 = 2 * ALTURA_LINHA + 1;
    col2 = 3 * ALTURA_LINHA;
    X.include(Rectangle(r, g, b, 1, true, col1, col2, lin1, lin2));

    const string arquivo = "grecia.svg";

    X.save(arquivo);
    cout << "Arquivo " << arquivo << " exportado!\n";

    return 0;
}
