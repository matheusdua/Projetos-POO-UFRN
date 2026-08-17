#include "mainplotador.h"
#include "ui_mainplotador.h"

#include <QMessageBox>

#include <QLabel>
#include <QPixmap>

MainPlotador::MainPlotador(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainPlotador)
    , lehFuncao(new LehFuncao(this))
    , grafico(new Grafico(this))
{
    ui->setupUi(this);

    connect(lehFuncao, &LehFuncao::signIncluirFuncao, this, &MainPlotador::slotIncluirFuncao);

    ui->tableFuncoes->setStyleSheet("QHeaderView::section {background-color:lightgray}");

    ui->tableFuncoes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableFuncoes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->tableFuncoes->setHorizontalHeaderLabels(QStringList() << "COR" << "FUNCAO");

    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    ui->horizontalLayout->insertWidget(0, grafico);

    connect(grafico, &Grafico::signGraficoClicked, this, &MainPlotador::slotGraficoClicked);

    desenharGrafico();
}

MainPlotador::~MainPlotador()
{
    delete ui;
}

void MainPlotador::on_actionFun_o_triggered()
{
    lehFuncao->clear();
    lehFuncao->show();
}


void MainPlotador::on_actionApagar_triggered()
{

    on_pushApagar_clicked();

}


void MainPlotador::on_actionSair_triggered()
{
    QCoreApplication::quit();
}


void MainPlotador::on_spinMinX_valueChanged(int arg1)
{
    ui->spinMaxX->setMinimum(arg1 + 1);

    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    grafico->desenharGrafico();
}


void MainPlotador::on_spinMaxX_valueChanged(int arg1)
{

    ui->spinMinX->setMaximum(arg1 - 1);

    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    grafico->desenharGrafico();

}


void MainPlotador::on_spinExpX_valueChanged(int arg1)
{

    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    grafico->desenharGrafico(); //
}


void MainPlotador::on_spinMinY_valueChanged(int arg1)
{
    ui->spinMaxY->setMinimum(arg1 + 1);

    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    grafico->desenharGrafico();
}


void MainPlotador::on_spinMaxY_valueChanged(int arg1)
{

    ui->spinMinY->setMaximum(arg1 - 1);

    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    grafico->desenharGrafico();
}


void MainPlotador::on_spinExpY_valueChanged(int arg1)
{
    grafico->setLimites(ui->spinMinX->value(), ui->spinMaxX->value(), ui->spinExpX->value(),
                        ui->spinMinY->value(), ui->spinMaxY->value(), ui->spinExpY->value());

    grafico->desenharGrafico();
}


void MainPlotador::on_pushApagar_clicked()
{
    grafico->clearFuncoes();

    exibirFuncoes();

    grafico->desenharGrafico();
}


void MainPlotador::slotIncluirFuncao(QString Funcao, QColor Cor)
{

    try
    {
        grafico->pushFuncao(Funcao, Cor);

        exibirFuncoes();
        grafico->desenharGrafico();
    }
    catch (const std::invalid_argument &E)
    {
        QMessageBox::critical(this, "Funcao invalida", "Erro na funcao: " + QString(E.what()));
    }

}

void MainPlotador::exibirFuncoes()
{


    ui->tableFuncoes->clearContents();

    ui->tableFuncoes->setRowCount(grafico->size());

    QLabel* prov;
    QPixmap img(20, 20);

    for (int k = 0; k < static_cast<int>(grafico->size()); ++k)
    {
        img.fill(grafico->getCor(k));
        prov = new QLabel;

        prov->setAlignment(Qt::AlignCenter);

        prov->setPixmap(img);

        ui->tableFuncoes->setCellWidget(k, 0, prov);

        prov = new QLabel;

        prov->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        prov->setWordWrap(true);

        prov->setText(QString::fromStdString(grafico->getEval(k).getText()));

        ui->tableFuncoes->setCellWidget(k, 1, prov);

    }



}

void MainPlotador::desenharGrafico()
{
    grafico->desenharGrafico();
}

void MainPlotador::slotGraficoClicked(double X, double Y)
{
    QString mensagem = QString("X=%1 Y=%2").arg(X).arg(Y);
    statusBar()->showMessage(mensagem, 2000);
}
