#ifndef PHPCHEPINOPERATORPARSER_H
#define PHPCHEPINOPERATORPARSER_H

#include <QDebug>
#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include "../utility/vector_util.h"
#include "../utility/string_util.h"

class PhpChepinOperatorParser
{
    const std::vector<std::string> BINARY_OPERATORS_ORDER = {
        ",", ".", "->", "=", "+=", "-=", "*=", "||", "&&",
        "&", "|", "^", "==", "!=", "===", "!==", "<>", "<=>",
        "<", "<=", ">", ">=", "++", "--", "+", "-", "*", "/", "%", "new", "clone"
    };

    void AddOperand(std::string name, std::vector<std::string> path);

    bool TryFindBinaryOperator(std::string name, std::vector<std::string>& parts);
    bool TryFindOperatorWithStructure(std::string structure, std::vector<std::string>& parts);

    void ParsePartsOfConstruction(const std::vector<std::string> &parts, std::string nameOfConstruction, std::string nspace);
public:

    PhpChepinOperatorParser();
    explicit PhpChepinOperatorParser(std::string code);

    void SetCode(std::string code);
    void SetPath(std::vector<std::string> path);
    void SetNamespaces(std::string classNspace, std::string funcNspace);
    void AddDataFrom(const PhpChepinOperatorParser& other);

    std::map<std::string, std::vector<std::vector<std::string>>> GetValuablesData();
    std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> GetMetrics();

    void ParseCode();

private:

    std::string code_;
    std::map<std::string, std::vector<std::vector<std::string>>> valuablesData_;
    std::map<std::string, std::string> valuablesType_;
    std::vector<std::string> path_;
    std::string classNspace_;
    std::string funcNspace_;
};

#endif // PHPCHEPINOPERATORPARSER_H
