#include <iostream>
#include <cmath> // Para round
#include <string>
#include "designer.h"

using namespace std;

/// Programa para desenhar e exportar a bandeira dos EUA, usando a classe Designer

int main(void)
{
  // A constante ALTURA_LINHA significa quantos pixels de altura vai ter
  // cada uma das 13 linhas da bandeira.
  // As demais dimensoes são calculadas para manter as proporcoes corretas.
  const int ALTURA_LINHA = 40;
  const int altura_bandeira = 13*ALTURA_LINHA;
  const int largura_bandeira = round(24.7*ALTURA_LINHA);
  const int raio_estrela = round(0.25*ALTURA_LINHA);

  // Variaveis auxiliares
  int r,g,b; // Cor
  int lin1,lin2,col1,col2;
  int i,j;

  Designer X(largura_bandeira, altura_bandeira);

  // Um contorno preto
  r = g = b = 0;
  lin1 = 0;
  lin2 = altura_bandeira;
  col1 = 0;
  col2 = largura_bandeira;
  X.include(Rectangle(r,g,b, 1, false, col1,col2, lin1,lin2));

  // Desenha as 13 linhas
  for (i=0; i<13; ++i)
  {
    if (i%2 == 0)  // VERMELHO
    {
      r = 191;
      g = 10;
      b = 48;
    }
    else // BRANCO
    {
      r = 255;
      g = 255;
      b = 255;
    }
    lin1 = i*ALTURA_LINHA + 1;
    lin2 = (i+1)*ALTURA_LINHA;
    col1 = 1;
    col2 = largura_bandeira-1;
    X.include(Rectangle(r,g,b, 1, true, col1,col2, lin1,lin2));
    /*
    // PARA DEBUG
    cout << "Linha " << i << ':';
    cout << " xmin=" << col1 << " xmax=" << col2
         << " ymin=" << lin1 << " ymax=" << lin2 << endl;
    */
  }

  // Desenha o campo azul para as estrelas
  r = 0;
  g = 32;
  b = 91;
  lin1 = 1;
  lin2 = 7*ALTURA_LINHA;
  col1 = 1;
  col2 = 10*ALTURA_LINHA;
  X.include(Rectangle(r,g,b, 1, true, col1,col2, lin1,lin2));
  /*
  // PARA DEBUG
  cout << "Campo de estrelas:";
  cout << " xmin=" << col1 << " xmax=" << col2
       << " ymin=" << lin1 << " ymax=" << lin2 << endl;
  */

  // Desenha as estrelas brancas
  r = g = b = 255;
  // Desenha as 5 linhas com 6 estrelas em cada
  for (i=0; i<5; ++i)
  {
    lin1 = round((7.0/10.0)*ALTURA_LINHA*(1+2*i));
    for (j=0; j<6; ++j)
    {
      col1 = round((10.0/12.0)*ALTURA_LINHA*(1+2*j));
      X.include(Star(r,g,b, 1, true, col1,lin1,raio_estrela));
      /*
      // PARA DEBUG
      cout << "Estrela " << j << " da linha " << i << " de 6 estrelas:";
      cout << " xc=" << col1 << " yc=" << lin1 << " rc=" << raio_estrela << endl;
      */
    }
  }
  // Desenha as 4 linhas com 5 estrelas em cada
  for (i=0; i<4; ++i)
  {
    lin1 = round((7.0/10.0)*ALTURA_LINHA*(2+2*i));
    for (j=0; j<5; ++j)
    {
      col1 = round((10.0/12.0)*ALTURA_LINHA*(2+2*j));
      X.include(Star(r,g,b, 1, true, col1,lin1,raio_estrela));
      /*
      // PARA DEBUG
      cout << "Estrela " << j << " da linha " << i << " de 5 estrelas:";
      cout << " xc=" << col1 << " yc=" << lin1 << " rc=" << raio_estrela << endl;
      */
    }
  }

  const string arquivo="eua.svg";

  X.save(arquivo);
  cout << "Arquivo " << arquivo << " exportado!\n";

  return 0;
}
