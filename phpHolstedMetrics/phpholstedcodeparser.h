#ifndef PHPHOLSTEDCODEPARSER_H
#define PHPHOLSTEDCODEPARSER_H

#include <string>
#include <map>
#include <regex>

#include "phpholstedoperatorparser.h"

class PhpHolstedCodeParser
{
private:
    std::string code;
    std::map<std::string, size_t> operandsData;
    std::map<std::string, size_t> operatorsData;
    void AddOperand(std::string name);
    void AddOperator(std::string name);
public:
    PhpHolstedCodeParser();
    void SetCode(std::string code);
    void ParseCode();
    std::map<std::string, size_t> GetOperatorsData();
    std::map<std::string, size_t> GetOperandsData();
};

#endif // PHPHOLSTEDCODEPARSER_H
