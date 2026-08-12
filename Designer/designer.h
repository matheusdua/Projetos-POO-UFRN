#ifndef _DESIGNER_H_
#define _DESIGNER_H_

#include <string>
#include <vector>

#include "shape.h"

/// ESTE ARQUIVO NAO PODE MODIFICADO.

class Designer
{
public:
  // Construtores e destrutor
  Designer(): width(0.0), height(0.0), VS() {}
  Designer(double Width, double Height);
  Designer(const Designer& D);
  Designer(Designer&& D) noexcept;
  ~Designer() {clear();}

  // Atribuicao
  Designer& operator=(const Designer& D);
  Designer& operator=(Designer&& D) noexcept;

  // Limpa o desenho
  void clear() noexcept;

  // Funcoes de consulta
  size_t getNumShapes() const {return VS.size();}
  double getWidth() const {return width;}
  double getHeight() const {return height;}
  pShape at(int i) const {return VS.at(i);} // Gera excecao se indice fora de faixa

  // Inclusao de nova forma no desenho
  void include(const Shape& S);

  // Leitura de arquivo com as formas
  void read(const std::string& fileName);

  // Salvamento do desenho em formato SVG
  void save(const std::string& fileName) const;

private:
  // Limites do desenho
  double width, height;
  // Todas as formas do desenho
  std::vector<pShape> VS;
};

#endif // _DESIGNER_H_
