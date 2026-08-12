#ifndef GRAFICO_H
#define GRAFICO_H

#include <QLabel>

#include <QColor>
#include <QString>
#include <vector>
#include "evaluator.h"

#include <QResizeEvent>

class Grafico : public QLabel
{
    Q_OBJECT

public:
    Grafico(QWidget *parent = nullptr);

    bool empty() const;
    size_t size() const;

    const Evaluator& getEval(int i) const;
    const QColor& getCor(int i) const;

    void pushFuncao(QString Funcao, QColor Cor);

    void clearFuncoes();

    void setLimites(int MinX, int MaxX, int ExpX, int MinY, int MaxY, int ExpY);

    void desenharGrafico();


private:
    double convXtoJ(double X) const;
    double convYtoI(double Y) const;
    double convJtoX(double J) const;
    double convItoY(double I) const;

    std::vector<Evaluator> eval;
    std::vector<QColor> cor;

    int largura, altura;

    double minX, maxX, minY, maxY;

    int nMarcX, nMarcY;


    void resizeEvent(QResizeEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;


signals:
    void signGraficoClicked(double X, double Y);

};

#endif // GRAFICO_H
