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

    ui->PTable->horizontalHeader()->setStretchLastSection(true);
    ui->PTable->setRowCount(p);
    ui->PTable->setColumnCount(1);
    size_t cnt = 0;
    for (const auto &item : std::get<0>(chepinsInfo)) {
        ui->PTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
    }

    ui->MTable->horizontalHeader()->setStretchLastSection(true);
    ui->MTable->setRowCount(m);
    ui->MTable->setColumnCount(1);
    cnt = 0;
    for (const auto &item : std::get<1>(chepinsInfo)) {
        ui->MTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
    }

    ui->CTable->horizontalHeader()->setStretchLastSection(true);
    ui->CTable->setRowCount(c);
    ui->CTable->setColumnCount(1);
    cnt = 0;
    for (const auto &item : std::get<2>(chepinsInfo)) {
        ui->CTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
    }

    ui->TTable->horizontalHeader()->setStretchLastSection(true);
    ui->TTable->setRowCount(t);
    ui->TTable->setColumnCount(1);
    cnt = 0;
    for (const auto &item : std::get<3>(chepinsInfo)) {
        ui->TTable->setItem(cnt++, 0, new QTableWidgetItem(QString::fromStdString(item)));
    }

    QString chepinResult = "Calculating chepin's metric as " + QString::number(p) + " * 1 + "
                           + QString::number(m) + " * 2 + " + QString::number(c) + " * 3 + "
                           + QString::number(t)
                           + " * 0.5 = " + QString::number(p + 2 * m + 3 * c + 0.5 * t);

    ui->ResultChepin->setText(chepinResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}
