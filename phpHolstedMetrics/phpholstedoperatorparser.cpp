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

bool PhpHolstedOperatorParser::TryFindAndParseBinaryOperator(std::string name)
{
    int rbal = 0, fbal = 0;

    for (int i = 0;i + name.size() <= code.size();i++)
    {
        if (code.substr(i, name.size()) == name && rbal == 0 && fbal == 0)
        {
            this->AddOperator(name);

            PhpHolstedOperatorParser parser;

            parser.SetCode(code.substr(0, i));
            parser.ParseCode();
            this->AddDataFrom(parser);

            parser.SetCode(code.substr(i + name.size()));
            parser.ParseCode();
            this->AddDataFrom(parser);

            return true;
        }

        if (code[i] == '(')
        {
            rbal++;
        }

        if (code[i] == ')')
        {
            rbal--;
        }

        if (code[i] == '{')
        {
            fbal++;
        }

        if (code[i] == '}')
        {
            fbal--;
        }
    }

    return false;
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

    qDebug() << QString::fromStdString(code) << '\n';

    // special constructions

    if (TryFindAndParseBinaryOperator(";"))
    {
        return;
    }

    PhpHolstedOperatorParser parser;

    if (regex_match(code, match, std::regex(R"(^\{(.*)\}$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        AddOperator("{...}");
    }
    else if (regex_match(code, match, std::regex(R"(^\((.*)\)$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        AddOperator("(...)");
    }
    else if (regex_match(code, match, std::regex(R"(^echo\s*(.*)$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator("echo");
    }
    else if (regex_match(code, match, std::regex(R"(^while\s*\((.*?)\)\s*(\{.*?\})(.*))")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        parser.SetCode(match[3]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator("while");
    }
    else if (regex_match(code, match, std::regex(R"(^for\s*\((.*?)\)\s*(\{.*?\})(.*))")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        parser.SetCode(match[3]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator("for");
    }
    else if (regex_match(code, match, std::regex(R"(^if\s*\((.*?)\)\s*(\{.*?\})\s*(.*))")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        parser.SetCode(match[3]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator("if");
    }
    else if (regex_match(code, match, std::regex(R"(^elseif\s*\((.*?)\)\s*(\{.*?\})\s*(.*))")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        parser.SetCode(match[3]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator("elseif");
    }
    else if (regex_match(code, match, std::regex(R"(^else\s*(\{.*?\})(.*))")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator("else");
    }
    else if (regex_match(code, match, std::regex(R"((\w*)\((.*?)\))")))
    {
        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        this->AddOperator(match[1].str() + "(..)");
    }

    // binary operators

    std::vector<std::string> binaryOperatorsInOrder = {";","=","+=","-=","*=","==","!=","===","!==","<>","<=>","<","<=",">",">=","+","-","*","/","%"};

    for (const std::string& operatorName : binaryOperatorsInOrder)
    {
        if (TryFindAndParseBinaryOperator(operatorName))
        {
            return;
        }
    }

    // unary operators

    if (regex_match(code, match, std::regex(R"(^(.*)(--|\+\+)$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        AddOperator(match[2]);
    }
    else if (regex_match(code, match, std::regex(R"(^(-|\+|--)(.*)$)")))
    {
        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        AddOperator(match[1]);
    }
    else if (regex_match(code, match, std::regex(R"(^(.*)\[(.*)\]$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        this->AddDataFrom(parser);
        parser.SetCode(match[2]);
        parser.ParseCode();
        this->AddDataFrom(parser);

        AddOperator("[...]");
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
