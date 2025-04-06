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

bool PhpHolstedOperatorParser::TryFindAndParseOperatorWithStructure(std::string structure, std::vector<std::string>& parts)
{
    parts.clear();

    size_t balance = 0;
    size_t structurePtr = 0;
    std::string currPart = "";
    std::vector<std::string> autoParse;

    for (int i = 0;i<code.size();i++)
    {
        if (structurePtr == structure.size())
        {
            return false;
        }

        // \f - any symbols, stop when next character is found, will contribute to parts
        // \r - any symbols, stop when next character is found, will not contribute to parts
        // \a - only letters, nonempty, stop when next character is found, will contribute to parts
        // (,[,{ - bracket sequence, will contribute to parts

        if (structure[structurePtr] == '\f')
        {
            if (structurePtr != structure.size() && code[i] == structure[structurePtr + 1])
            {
                parts.push_back(currPart);

                autoParse.push_back(currPart);
                currPart = "";
                structurePtr++;
            }
            else
            {
                currPart += code[i];
                continue;
            }
        }

        if (structure[structurePtr] == '\r')
        {
            if (structurePtr != structure.size() && code[i] == structure[structurePtr + 1])
            {
                currPart = "";
                structurePtr++;
            }
            else
            {
                currPart += code[i];
                continue;
            }
        }

        if (structure[structurePtr] == '\a')
        {
            if ((structurePtr != structure.size() && code[i] == structure[structurePtr + 1]) || !checkIfCharacterIsNaming(code[i]))
            {
                if (i == 0 || !checkIfCharacterIsNaming(code[i-1]))
                {
                    return false;
                }

                parts.push_back(currPart);
                currPart = "";
                structurePtr++;
            }
            else
            {
                currPart += code[i];
                continue;
            }
        }

        if (balance)
        {
            currPart += code[i];

            if ((code[i] == ')' && structure[structurePtr] == '(') ||
                (code[i] == '}' && structure[structurePtr] == '{') ||
                (code[i] == ']' && structure[structurePtr] == '['))
            {
                balance--;
            }

            if ((code[i] == '(' && structure[structurePtr] == '(') ||
                (code[i] == '{' && structure[structurePtr] == '{') ||
                (code[i] == '[' && structure[structurePtr] == '['))
            {
                balance++;
            }

            if (balance == 0)
            {
                structurePtr++;

                currPart.erase(0, 1);
                currPart.erase(currPart.size() - 1, 1);

                parts.push_back(currPart);
                autoParse.push_back(currPart);

                currPart = "";
            }
        }
        else if ((structure[structurePtr] == '(' || structure[structurePtr] == '{' || structure[structurePtr] == '[') && structure[structurePtr] == code[i])
        {
            currPart = code[i];
            balance++;
        }
        else if (code[i] == ' ' || code[i] == '\t' || code[i] == '\n')
        {
            continue;
        }
        else if (structure[structurePtr] == code[i])
        {
            structurePtr++;
        }
        else
        {
            return false;
        }
    }

    autoParse.push_back(currPart);

    while (structurePtr < structure.size() && (structure[structurePtr] == '\r' || structure[structurePtr] == '\f'))
    {
        structurePtr++;
    }

    if (structurePtr != structure.size())
    {
        return false;
    }

    PhpHolstedOperatorParser parser;

    for (const std::string& codePart : autoParse)
    {
        parser.SetCode(codePart);
        parser.ParseCode();
        AddDataFrom(parser);
    }

    return true;
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

    std::smatch match;

    qDebug() << QString::fromStdString(code) << '\n';

    // special constructions

    PhpHolstedOperatorParser parser;

    std::vector<std::string> findInfo;
    if (TryFindAndParseBinaryOperator(";"))
    {
        return;
    }
    if (std::regex_match(code, match, std::regex(R"(^\((.*)\)$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        AddDataFrom(parser);
        AddOperator("()");
        return;
    }
    else if (std::regex_match(code, match, std::regex(R"(^\{(.*)\}$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        AddDataFrom(parser);
        AddOperator("{}");
        return;
    }
    else if (std::regex_match(code, match, std::regex(R"(^\[(.*)\]$)")))
    {
        parser.SetCode(match[1]);
        parser.ParseCode();
        AddDataFrom(parser);
        AddOperator("[]");
        return;
    }

    if (TryFindAndParseOperatorWithStructure("while({\f", findInfo))
    {
        AddOperator("while");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("for({\f", findInfo))
    {
        AddOperator("for");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("if({\f", findInfo))
    {
        AddOperator("if..elseif..else");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("elseif({\f", findInfo))
    {
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("else{\f", findInfo))
    {
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("function\r{\f", findInfo))
    {
        AddOperator("function");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("class\r{\f", findInfo))
    {
        AddOperator("class");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("return\f", findInfo))
    {
        AddOperator("return");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("break\f", findInfo))
    {
        AddOperator("break");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("public\f", findInfo))
    {
        AddOperator("public");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("echo\f", findInfo))
    {
        AddOperator("echo");
        return;
    }
    else if (TryFindAndParseOperatorWithStructure("\a(\f", findInfo))
    {
        qDebug() << QString::fromStdString(findInfo[0]);
        AddOperator(findInfo[0] + "()");
        return;
    }

    // arithmetic and logical operators

    std::vector<std::string> binaryOperatorsInOrder = {",",".","->","=","+=","-=","*=","||","&&","&","|","^","==","!=","===","!==","<>","<=>","<","<=",">",">=","++","--","+","-","*","/","%","new ","clone "};

    std::stable_sort(binaryOperatorsInOrder.begin(), binaryOperatorsInOrder.end(), [] (std::string a, std::string b) {return a.size() > b.size();});

    for (const std::string& operatorName : binaryOperatorsInOrder)
    {
        if (TryFindAndParseBinaryOperator(operatorName))
        {
            return;
        }
    }

    // operands

    if (checkIfOperand())
    {
        AddOperand(this->code);
        return;
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
    return std::regex_match(code, std::regex(R"(^((\w+)|(\$\w+)|(\d+(\.\d*)?(e\d+)?)|('[^']*'|"[^"]*"))$)"));
}

bool PhpHolstedOperatorParser::checkIfCharacterIsNaming(char character)
{
    return ((std::isalpha(character)) || character == '_');
}
