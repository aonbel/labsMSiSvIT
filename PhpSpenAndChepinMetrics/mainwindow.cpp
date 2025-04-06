#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "metrics/phpchepinoperatorparser.h"
#include "parse_file.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PhpChepinOperatorParser phpChepinOperatorParser;
    Parse_file fileParser;

    phpChepinOperatorParser.SetCode(fileParser.Parse());
    phpChepinOperatorParser.ParseCode();

    auto varData = phpChepinOperatorParser.GetValuablesData();

    ui->SpenTable->horizontalHeader()->setStretchLastSection(true);
    ui->SpenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->SpenTable->setRowCount(varData.size());
    ui->SpenTable->setColumnCount(2);

    auto iter = varData.begin();

    QString spenResult = "Calculating sum of spens ";

    size_t spen = 0;

    for (size_t it = 0; it < varData.size(); ++it) {
        ui->SpenTable->setItem(it, 0, new QTableWidgetItem(QString::fromStdString(iter->first)));
        ui->SpenTable->setItem(it,
                               1,
                               new QTableWidgetItem(QString::number(iter->second.size() - 1)));

        spenResult += "+" + QString::number(iter->second.size() - 1);
        spen += iter->second.size() - 1;
        ++iter;
    }

    spenResult += "=" + QString::number(spen);

    ui->SpenResult->setText(spenResult);

    std::cout << std::endl;

    auto chepinsInfo = phpChepinOperatorParser.GetMetrics();

    double p = std::get<0>(chepinsInfo).size();
    double m = std::get<1>(chepinsInfo).size();
    double c = std::get<2>(chepinsInfo).size();
    double t = std::get<3>(chepinsInfo).size();


    {
        ui->PTable->horizontalHeader()->setStretchLastSection(true);
        ui->PTable->setRowCount(p);
        ui->PTable->setColumnCount(1);
        ui->PTable->setHorizontalHeaderLabels(QStringList("P"));

        size_t cnt = 0;
        for (const auto &item : std::get<0>(chepinsInfo)) {
            ui->PTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->PTotal->setText("Total = " + QString::number(p));
    }

    {
        ui->MTable->horizontalHeader()->setStretchLastSection(true);
        ui->MTable->setRowCount(m);
        ui->MTable->setColumnCount(1);
        ui->MTable->setHorizontalHeaderLabels(QStringList("M"));

        size_t cnt = 0;
        for (const auto &item : std::get<1>(chepinsInfo)) {
            ui->MTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->MTotal->setText("Total = " + QString::number(m));
    }

    {
        ui->CTable->horizontalHeader()->setStretchLastSection(true);
        ui->CTable->setRowCount(c);
        ui->CTable->setColumnCount(1);
        ui->CTable->setHorizontalHeaderLabels(QStringList("C"));

        size_t cnt = 0;
        for (const auto &item : std::get<2>(chepinsInfo)) {
            ui->CTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->CTotal->setText("Total = " + QString::number(c));
    }

    {
        ui->TTable->horizontalHeader()->setStretchLastSection(true);
        ui->TTable->setRowCount(t);
        ui->TTable->setColumnCount(1);
        ui->TTable->setHorizontalHeaderLabels(QStringList("T"));

        size_t cnt = 0;
        for (const auto &item : std::get<3>(chepinsInfo)) {
            ui->TTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->TTotal->setText("Total = " + QString::number(t));
    }

    QString chepinResult = "Calculating Сhepin's metric as " + QString::number(p) + " * 1 + "
                           + QString::number(m) + " * 2 + " + QString::number(c) + " * 3 + "
                           + QString::number(t)
                           + " * 0.5 = " + QString::number(p + 2 * m + 3 * c + 0.5 * t);

    ui->ResultChepin->setText(chepinResult);

    auto chepinsIOInfo = phpChepinOperatorParser.GetInputOutputMetrics();

    p = std::get<0>(chepinsIOInfo).size();
    m = std::get<1>(chepinsIOInfo).size();
    c = std::get<2>(chepinsIOInfo).size();
    t = std::get<3>(chepinsIOInfo).size();

    {
        ui->PIOTable->horizontalHeader()->setStretchLastSection(true);
        ui->PIOTable->setRowCount(p);
        ui->PIOTable->setColumnCount(1);
        ui->PIOTable->setHorizontalHeaderLabels(QStringList("P"));

        size_t cnt = 0;
        for (const auto &item : std::get<0>(chepinsInfo)) {
            ui->PIOTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->PIOTotal->setText("Total = " + QString::number(p));
    }

    {
        ui->MIOTable->horizontalHeader()->setStretchLastSection(true);
        ui->MIOTable->setRowCount(m);
        ui->MIOTable->setColumnCount(1);
        ui->MIOTable->setHorizontalHeaderLabels(QStringList("M"));

        size_t cnt = 0;
        for (const auto &item : std::get<1>(chepinsInfo)) {
            ui->MIOTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->MIOTotal->setText("Total = " + QString::number(m));
    }

    {
        ui->CIOTable->horizontalHeader()->setStretchLastSection(true);
        ui->CIOTable->setRowCount(c);
        ui->CIOTable->setColumnCount(1);
        ui->CIOTable->setHorizontalHeaderLabels(QStringList("C"));

        size_t cnt = 0;
        for (const auto &item : std::get<2>(chepinsInfo)) {
            ui->CIOTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->CIOTotal->setText("Total = " + QString::number(c));
    }

    {
        ui->TIOTable->horizontalHeader()->setStretchLastSection(true);
        ui->TIOTable->setRowCount(t);
        ui->TIOTable->setColumnCount(1);
        ui->TIOTable->setHorizontalHeaderLabels(QStringList("T"));

        size_t cnt = 0;
        for (const auto &item : std::get<3>(chepinsInfo)) {
            ui->TIOTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
        }
        ui->TIOTotal->setText("Total = " + QString::number(t));
    }

    QString chepinIOResult = "Calculating Сhepin's IO metric as " + QString::number(p) + " * 1 + "
                           + QString::number(m) + " * 2 + " + QString::number(c) + " * 3 + "
                           + QString::number(t)
                           + " * 0.5 = " + QString::number(p + 2 * m + 3 * c + 0.5 * t);

    ui->ResultChepinIO->setText(chepinIOResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}
