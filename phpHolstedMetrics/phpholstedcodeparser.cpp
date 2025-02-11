#include "phpholstedcodeparser.h"

PhpHolstedCodeParser::PhpHolstedCodeParser() = default;

void PhpHolstedCodeParser::AddOperand(std::string name)
{
    operandsData[std::regex_replace(name, std::regex(R"(^\s+|\s+$)"), "")]++;
}

void PhpHolstedCodeParser::AddOperator(std::string name)
{
    operatorsData[std::regex_replace(name, std::regex(R"(^\s+|\s+$)"), "")]++;
}

void PhpHolstedCodeParser::SetCode(std::string code)
{
    this->code = code;
}

void PhpHolstedCodeParser::ParseCode()
{
    PhpHolstedOperatorParser* operatorParser = new PhpHolstedOperatorParser(this, code);
    operatorParser->ParseCode();
}

std::map<std::string, size_t> PhpHolstedCodeParser::GetOperatorsData()
{
    return operatorsData;
}

std::map<std::string, size_t> PhpHolstedCodeParser::GetOperandsData()
{
    return operandsData;
}
