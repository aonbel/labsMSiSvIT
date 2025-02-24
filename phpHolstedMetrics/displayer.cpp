#include "displayer.h"
#include <QDebug>


Displayer::Displayer() {}

Displayer::Displayer(Ui::MainWindow *ui) : ui(ui) {}

Displayer::Displayer(Ui::MainWindow *ui, std::map<std::string, size_t> operatorsMap, std::map<std::string, size_t> operandsMap) : ui(ui), operatorsMap(operatorsMap), operandsMap(operandsMap) {}


void Displayer::setOperatorsMap(const std::map<std::string, size_t> &operatorsMap)
{
    this->operatorsMap = operatorsMap;
}


void Displayer::setOperandsMap(const std::map<std::string, size_t> &operandsMap)
{
    this->operandsMap = operandsMap;
}


void Displayer::displayOperators()
{
    ui->operatorsTable->horizontalHeader()->setStretchLastSection(true);
    ui->operatorsTable->setRowCount(operatorsMap.size());
    ui->operatorsTable->setColumnCount(2);
    ui->operatorsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int i = 0;
    std::map<std::string, size_t>::iterator ii;
    for (ii = operatorsMap.begin(), i = 0; ii != operatorsMap.end(); ii++, i++)
    {
        ui->operatorsTable->setItem(i, 0, new QTableWidgetItem());
        ui->operatorsTable->setItem(i, 1, new QTableWidgetItem());
        ui->operatorsTable->item(i, 0)->setText(QString::fromStdString(ii->first));
        ui->operatorsTable->item(i, 1)->setText(QString::number(ii->second));
    }
}


void Displayer::displayOperands()
{
    ui->operandsTable->horizontalHeader()->setStretchLastSection(true);
    ui->operandsTable->setRowCount(operandsMap.size());
    ui->operandsTable->setColumnCount(2);
    ui->operandsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int i = 0;
    std::map<std::string, size_t>::iterator ii;
    for (ii = operandsMap.begin(), i = 0; ii != operandsMap.end(); ii++, i++)
    {
        ui->operandsTable->setItem(i, 0, new QTableWidgetItem());
        ui->operandsTable->setItem(i, 1, new QTableWidgetItem());
        ui->operandsTable->item(i, 0)->setText(QString::fromStdString(ii->first));
        ui->operandsTable->item(i, 1)->setText(QString::number(ii->second));
    }
}


void Displayer::displayHolstedMetrics()
{
    size_t uniqueOperatorsNumber = 0;
    size_t totalOperatorsNumber = 0;
    size_t uniqueOperandsNumber = 0;
    size_t totalOperandsNumber = 0;

    for (const auto& ii : operatorsMap)
    {
        uniqueOperatorsNumber += 1;
        totalOperatorsNumber += ii.second;
    }

    for (const auto& ii : operandsMap)
    {
        uniqueOperandsNumber += 1;
        totalOperandsNumber += ii.second;
    }

    QString holstedMetrixText = "";
    holstedMetrixText += "Количество уникальных операторов n1 = " + QString::number(uniqueOperatorsNumber) + "\n";
    holstedMetrixText += "Количество уникальных операндов n2 = " + QString::number(uniqueOperandsNumber) + "\n";
    holstedMetrixText += "Количество всех операторов N1 = " + QString::number(totalOperatorsNumber) + "\n";
    holstedMetrixText += "Количество всех операндов N2 = " + QString::number(totalOperandsNumber) + "\n";
    holstedMetrixText += "Словарь программы n = " + QString::number(uniqueOperatorsNumber) + " + " + QString::number(uniqueOperandsNumber) + " = " + QString::number(uniqueOperatorsNumber + uniqueOperandsNumber) + "\n";
    holstedMetrixText += "Длина программы N = " + QString::number(totalOperatorsNumber) + " + " + QString::number(totalOperandsNumber) + " = " + QString::number(totalOperatorsNumber + totalOperandsNumber) + "\n";
    holstedMetrixText += "Объём программы V = " + QString::number(totalOperatorsNumber + totalOperandsNumber) + "log2(" + QString::number(uniqueOperatorsNumber + uniqueOperandsNumber) + ") = " + QString::number((totalOperatorsNumber + totalOperandsNumber) * log2(uniqueOperatorsNumber + uniqueOperandsNumber));

    ui->programTextBrowser->setText(holstedMetrixText);
}
