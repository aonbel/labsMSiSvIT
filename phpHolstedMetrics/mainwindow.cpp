#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Displayer* displayer = new Displayer(ui);
    Parse_file* fileParser = new Parse_file();
    PhpHolstedOperatorParser* codeParser = new PhpHolstedOperatorParser();

    codeParser->SetCode(fileParser->parse());
    //codeParser->ParseCode();
    std::map<std::string, size_t> data = codeParser->GetOperatorsData();

    for (const auto& y : data)
    {
        qDebug() << QString::fromStdString(y.first) << ' ' << y.second << '\n';
    }

    displayer->displayOperators();
    displayer->displayOperands();
    displayer->displayHolstedMetrics(2,3,4,5);
}

MainWindow::~MainWindow()
{
    delete ui;
}
