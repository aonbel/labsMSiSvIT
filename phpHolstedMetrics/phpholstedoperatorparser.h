#ifndef PHPHOLSTEDOPERATORPARSER_H
#define PHPHOLSTEDOPERATORPARSER_H

#include <QDebug>
#include <string>
#include <iostream>
#include <regex>
#include <vector>

class PhpHolstedOperatorParser
{
    std::string code;
    std::map<std::string, size_t> operandsData;
    std::map<std::string, size_t> operatorsData;
    void AddOperand(std::string name);
    void AddOperator(std::string name);
    bool TryFindAndParseBinaryOperator(std::string name);
    bool TryFindAndParseOperatorWithStructure(std::string structure, std::vector<std::string>& parts);
    bool checkIfOperand();
public:
    PhpHolstedOperatorParser();
    explicit PhpHolstedOperatorParser(std::string code);
    void SetCode(std::string code);
    void ParseCode();
    std::map<std::string, size_t> GetOperatorsData();
    std::map<std::string, size_t> GetOperandsData();
    void AddDataFrom(const PhpHolstedOperatorParser& other);
};

#endif // PHPHOLSTEDOPERATORPARSER_H
