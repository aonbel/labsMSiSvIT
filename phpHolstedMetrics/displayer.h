#ifndef DISPLAYER_H
#define DISPLAYER_H
#include "./ui_mainwindow.h"
#include <map>
#include <QTextBrowser>
#include <math.h>

class Displayer
{
private:
    Ui::MainWindow* ui;
    std::map<std::string, size_t> operatorsMap = std::map<std::string, size_t>();
    std::map<std::string, size_t> operandsMap = std::map<std::string, size_t>();

public:
    Displayer();
    Displayer(Ui::MainWindow* p);
    Displayer(Ui::MainWindow* p, std::map<std::string, size_t> operatorsMap, std::map<std::string, size_t> operandsMap);

    void setOperatorsMap(const std::map<std::string, size_t>& operatorsMap);
    void setOperandsMap(const std::map<std::string, size_t>& operandsMap);

    void displayOperators();
    void displayOperands();
    void displayHolstedMetrics(size_t uniqueOperatorsNumber, size_t totalOperatorsNumber, size_t uniqueOperandsNumber, size_t totalOperandsNumber);
};

#endif // DISPLAYER_H
