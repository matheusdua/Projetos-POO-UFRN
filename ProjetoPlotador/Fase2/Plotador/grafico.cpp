#include "grafico.h"
#include <QPainter>
#include <QPen>
#include <QLineF>
#include <cmath>


Grafico::Grafico(QWidget *parent)
    : QLabel(parent)
    , eval(), cor()
    , largura(0) ,altura(0)
    , minX(0.0), maxX(0.0), minY(0.0) , maxY(0.0)
    , nMarcX(0), nMarcY(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setMinimumSize(540, 540);

    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Plain);

    setAlignment(Qt::AlignCenter);

}

bool Grafico::empty() const {   return eval.empty();    }

size_t Grafico::size() const{   return eval.size();    }


const Evaluator& Grafico::getEval(int i) const {    return eval.at(i);  }

const QColor& Grafico::getCor(int i) const {    return cor.at(i);   }


void Grafico::pushFuncao(QString Funcao, QColor Cor)
{
    Evaluator new_eval;

    new_eval.set(Funcao.toStdString());
    eval.push_back(new_eval);
    cor.push_back(Cor);
}

void Grafico::clearFuncoes()
{
    eval.clear();
    cor.clear();
}

void Grafico::setLimites(int MinX, int MaxX, int ExpX, int MinY, int MaxY, int ExpY)
{
    minX = MinX * std::pow(10.0, ExpX);
    maxX = MaxX * std::pow(10.0, ExpX);
    minY = MinY * std::pow(10.0, ExpY);
    maxY = MaxY * std::pow(10.0, ExpY);

    nMarcX = 1 + MaxX - MinX;
    nMarcY = 1 + MaxY - MinY;
}



double Grafico::convXtoJ(double X) const
{
    return (largura - 1.0) * (X - minX) / (maxX - minX);
}

double Grafico::convYtoI(double Y) const
{
    return (altura - 1.0) * (maxY - Y) / (maxY - minY);
}

double Grafico::convJtoX(double J) const
{
    return minX + (maxX - minX) * J / (largura - 1.0);
}

double Grafico::convItoY(double I) const
{
    return maxY - (maxY - minY) * I / (altura - 1.0);
}

void Grafico::desenharGrafico()
{
    QPen pen;
    QPainter painter;

    largura = width();
    altura = height();

    QPixmap img(largura, altura);
    img.fill(Qt::white);

    painter.begin(&img);

    pen.setColor(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    double Izero = convYtoI(0.0);

    if (Izero >= 0.0 && Izero <= (altura - 1.0))
    {
        QLineF linhaEixoX(0.0, Izero, largura - 1.0, Izero);
        painter.drawLine(linhaEixoX);

        for (int i = 0; i < nMarcX; ++i)
        {
            double Jmarc = convXtoJ(minX + (maxX - minX) * i / (nMarcX - 1.0));
            QLineF marcadorX(Jmarc, Izero - 3.0, Jmarc, Izero + 3.0);
            painter.drawLine(marcadorX);
        }
    }

    double Jzero = convXtoJ(0.0);

    if (Jzero >= 0.0 && Jzero <= (largura - 1.0))
    {
        QLineF linhaEixoY(Jzero, 0.0, Jzero, altura - 1.0);
        painter.drawLine(linhaEixoY);

        for (int i = 0; i < nMarcY; ++i)
        {
            double Imarc = convYtoI(minY + (maxY - minY) * i / (nMarcY - 1.0));
            QLineF marcadorY(Jzero - 3.0, Imarc, Jzero + 3.0, Imarc);
            painter.drawLine(marcadorY);
        }
    }

    if (eval.empty())
    {
        painter.end();
        setPixmap(img);
        return;
    }

    double X, Y;
    double I, Iant;

    pen.setWidth(1);

    for (int k = 0; k < static_cast<int>(eval.size()); ++k)
    {
        pen.setColor(cor.at(k));
        painter.setPen(pen);

        Iant = -1.0;

        for (int J = 0; J < largura; ++J)
        {
            X = convJtoX(J);
            Y = eval.at(k)(X);
            I = convYtoI(Y);

            if (I >= 0.0 && I <= (altura - 1.0) && Iant >= 0.0 && Iant <= (altura - 1.0))
            {
                QLineF segmentoCurva(J - 1, Iant, J, I);
                painter.drawLine(segmentoCurva);
            }

            Iant = I;
        }
    }

    painter.end();
    setPixmap(img);
}


void Grafico::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);

    if (event->oldSize() != event->size()) desenharGrafico();

}


void Grafico::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);

    double J = event->position().x();
    double I = event->position().y();

    double X = convJtoX(J);
    double Y = convItoY(I);

    emit signGraficoClicked(X, Y);
}
