#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <regex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    Parse_file* fileParser = new Parse_file();
    PhpHolstedOperatorParser* codeParser = new PhpHolstedOperatorParser();

    codeParser->SetCode(fileParser->Parse());
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

