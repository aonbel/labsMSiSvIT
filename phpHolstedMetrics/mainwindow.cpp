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

    codeParser->SetCode(fileParser->Parse());
    codeParser->ParseCode();

    std::map<std::string, size_t> data = codeParser->GetOperatorsData();

    displayer->setOperatorsMap(codeParser->GetOperatorsData());
    displayer->setOperandsMap(codeParser->GetOperandsData());

    displayer->displayOperators();
    displayer->displayOperands();
    displayer->displayHolstedMetrics();
}

MainWindow::~MainWindow()
{
    delete ui;
}
