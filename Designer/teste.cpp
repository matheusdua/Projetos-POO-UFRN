#include <iostream>
#include "designer.h"

using namespace std;

int main(void)
{
  // Testando o construtor default
  Designer D1;
  cout << "I01: ";
  if (D1.getWidth()==0 &&
      D1.getHeight()==0 &&
      D1.getNumShapes()==0) cout << "OK";
  else cout << "ERRO";
  cout << endl;

  // Testando o construtor especifico
  Designer D2(100,100);
  cout << "I02: ";
  if (D2.getWidth()==100 &&
      D2.getHeight()==100 &&
      D2.getNumShapes()==0) cout << "OK";
  else cout << "ERRO";
  cout << endl;

  // Testando a funcao include
  D2.include(Line(255,255,255, 1, 0,0,99,99));
  D2.include(Line(255,255,255, 1, 10,10,89,89));
  cout << "I03: ";
  if (D2.getWidth()==100 &&
      D2.getHeight()==100 &&
      D2.getNumShapes()==2 &&
      D2.at(0)!=nullptr &&
      D2.at(1)!=nullptr &&
      D2.at(0)!=D2.at(1)) cout << "OK";
  else cout << "ERRO";
  cout << endl;

  // Copia e apagamento
  Designer D4(D2);
  D2.clear();

  // Testando o construtor por copia
  cout << "I04: ";
  if (D4.getWidth()==100 &&
      D4.getHeight()==100 &&
      D4.getNumShapes()==2 &&
      D4.at(0)!=nullptr &&
      D4.at(1)!=nullptr &&
      D4.at(0)!=D4.at(1)) cout << "OK";
  else cout << "ERRO";
  cout << endl;

  // Testando a funcao clear
  cout << "I05: ";
  if (D2.getWidth()==0 &&
      D2.getHeight()==0 &&
      D2.getNumShapes()==0) cout << "OK";
  else cout << "ERRO";
  cout << endl;

  // Movimento
  Designer D6(move(D4));

  // Testando o construtor por movimento
  cout << "I06: ";
  if (D6.getWidth()==100 &&
      D6.getHeight()==100 &&
      D6.getNumShapes()==2 &&
      D6.at(0)!=nullptr &&
      D6.at(1)!=nullptr &&
      D6.at(0)!=D6.at(1) &&
      D4.getWidth()==0 &&
      D4.getHeight()==0 &&
      D4.getNumShapes()==0) cout << "OK";
  else cout << "ERRO";
  cout << endl;

  return 0;
}

