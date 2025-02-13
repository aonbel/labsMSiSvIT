#ifndef PHPHOLSTEDOPERATORPARSER_H
#define PHPHOLSTEDOPERATORPARSER_H

#include <regex>
#include <QDebug>
#include <string>
#include <iostream>

class PhpHolstedOperatorParser
{
    std::string code;
    std::map<std::string, size_t> operandsData;
    std::map<std::string, size_t> operatorsData;
    void AddOperand(std::string name);
    void AddOperator(std::string name);
    bool checkIfOperand();
public:
    PhpHolstedOperatorParser();
    explicit PhpHolstedOperatorParser(std::string code);
    void SetCode(std::string code);
    void ParseCode()
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
    std::map<std::string, size_t> GetOperatorsData();
    std::map<std::string, size_t> GetOperandsData();
    void AddDataFrom(const PhpHolstedOperatorParser& other);
};

#endif // PHPHOLSTEDOPERATORPARSER_H
