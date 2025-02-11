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

void PhpHolstedOperatorParser::ParseCode()
{
    operatorsData.clear();
    operandsData.clear();

    code = std::regex_replace(code, std::regex(R"(^\s+|\s+$)"), "");

    if (code == "")
    {
        return;
    }

    if (checkIfOperand())
    {
        AddOperand(this->code);
        return;
    }

    std::smatch match;

    PhpHolstedOperatorParser next;

    qDebug() << QString::fromStdString(code) << '\n';

    if (std::regex_match(code, match, std::regex(R"(^\{(.*)\}$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator("{...}");
    }
    else if (std::regex_match(code, match, std::regex(R"(^\((.*)\)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator("(...)");
    }
    else if (std::regex_match(code, match, std::regex(R"(^(.*)(;)(.*)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        next.SetCode(match[3]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[2]);
    }
    else if (std::regex_match(code, match, std::regex(R"(^if\s*\((.*?)\)\s*(\{.*?\})\s*(elseif\s*\((.*?)\)\s*(\{.*?\})\s*)*(else\s*(\{.*?\}))?$)")))
    {
        size_t currPtr = 0;

        this->AddOperator("if..elseif..else");

        while (currPtr != match.size())
        {
            next.SetCode(match[currPtr + 1]);
            next.ParseCode();
            this->AddDataFrom(next);

            if (currPtr + 3 <= match.size())
            {
                next.SetCode(match[currPtr + 2]);
                next.ParseCode();
                this->AddDataFrom(next);

                currPtr += 3;
            }
            else
            {
                currPtr += 2;
            }
        }
    }
    else if (regex_match(code, match, std::regex(R"(^(.*)(=|\+=|-=|\*=)(.*)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        next.SetCode(match[3]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[2]);
    }
    else if (regex_match(code, match, std::regex(R"(^(.*)(==|!=|===|!==|<>|<=>)(.*)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        next.SetCode(match[3]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[2]);
    }
    else if (regex_match(code, match, std::regex(R"(^(.*)(\+|-)(.*)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        next.SetCode(match[3]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[2]);
    }
    else if (regex_match(code, match, std::regex(R"(^(.*)(\*|/|%)(.*)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        next.SetCode(match[3]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[2]);
    }
    else if (regex_match(code, match, std::regex(R"(^(.*)(--|\+\+)$)")))
    {
        next.SetCode(match[1]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[2]);
    }
    else if (regex_match(code, match, std::regex(R"(^(\x+|--)(.*)$)")))
    {
        next.SetCode(match[2]);
        next.ParseCode();
        this->AddDataFrom(next);
        AddOperator(match[1]);
    }
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
