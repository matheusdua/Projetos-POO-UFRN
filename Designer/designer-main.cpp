#include <iostream>
#include <string>
#include "designer.h"

using namespace std;


int main(int argc, char* argv[])
{
  /// Variavel que contem o desenho a ser exibido
  Designer V;

  // Leitura do nome do arquivo com o desenho
  string nome;
  cout << "Nome do arquivo com as formas a serem incluidas no desenho (sem espacos): ";
  cin >> nome;

  // Leh o arquivo.
  // Gera excecao em caso de erro.
  try
  {
    V.read(nome);
  }
  catch(exception& E)
  {
    cerr << "Erro na leitura do arquivo " << nome << ": " << E.what() << endl;
  }

  // Leitura do nome do arquivo SVG
  cout << "Nome do arquivo SVG a ser gerado (sem espacos): ";
  cin >> nome;

  // Salva o arquivo.
  // Gera excecao em caso de erro.
  try
  {
    V.save(nome);
  }
  catch(exception& E)
  {
    cerr << "Erro na escrita do arquivo " << nome << ": " << E.what() << endl;
  }

  return 0;
}

