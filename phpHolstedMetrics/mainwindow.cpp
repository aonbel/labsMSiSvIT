#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <regex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PhpHolstedOperatorParser* codeParser = new PhpHolstedOperatorParser();

    codeParser->SetCode("$a = 2;$b = 10;if ($a < 4) {    $b = \"Penis\";    echo $b;}elseif ($a < 6) {    $b = \"Kkkk\";    echo $b;}elseif($a < 10) {    $b = (1+2)*3;    echo $b;}else {    $b = \"sDRFYJmrd\";    echo $b;}");
    codeParser->ParseCode();
    std::map<std::string, size_t> data = codeParser->GetOperatorsData();

    for (const auto& y : data)
    {
        qDebug() << QString::fromStdString(y.first) << ' ' << y.second << '\n';
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

