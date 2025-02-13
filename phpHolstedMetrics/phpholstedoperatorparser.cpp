#include "phpholstedoperatorparser.h"

PhpHolstedOperatorParser::PhpHolstedOperatorParser() = default;

PhpHolstedOperatorParser::PhpHolstedOperatorParser(std::string code) : code(std::move(code)) { }

void PhpHolstedOperatorParser::AddOperand(std::string name)
{
    operandsData[name]++;
}

void PhpHolstedOperatorParser::AddOperator(std::string name)
{
    operatorsData[name]++;
}

void PhpHolstedOperatorParser::SetCode(std::string code)
{
    this->code = code;
}



std::map<std::string, size_t> PhpHolstedOperatorParser::GetOperatorsData()
{
    return operatorsData;
}

std::map<std::string, size_t> PhpHolstedOperatorParser::GetOperandsData()
{
    return operandsData;
}

void PhpHolstedOperatorParser::AddDataFrom(const PhpHolstedOperatorParser &other)
{
    for (const auto& instance: other.operatorsData)
    {
        operatorsData[instance.first] += instance.second;
    }

    for (const auto& instance: other.operandsData)
    {
        operandsData[instance.first] += instance.second;
    }
}

bool PhpHolstedOperatorParser::checkIfOperand()
{
    return std::regex_match(code, std::regex(R"(^(\$\w+|\d+(\.\d*)?(e\d+)?|'.*'|".*")$)"));
}
