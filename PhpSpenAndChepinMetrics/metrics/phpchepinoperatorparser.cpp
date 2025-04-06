#include "phpchepinoperatorparser.h"

PhpChepinOperatorParser::PhpChepinOperatorParser() = default;

PhpChepinOperatorParser::PhpChepinOperatorParser(std::string code)
    : code_(std::move(code))
{}

void PhpChepinOperatorParser::AddOperand(std::string name, std::vector<std::string> path)
{
    valuablesData_[name].push_back(path);
}

bool PhpChepinOperatorParser::TryFindBinaryOperator(std::string name,
                                                    std::vector<std::string> &parts)
{
    parts.clear();

    int rbal = 0, fbal = 0, sbal = 0;

    for (int i = 0; i + name.size() <= code_.size(); i++) {
        if (code_.substr(i, name.size()) == name && rbal == 0 && fbal == 0 && sbal == 0) {
            bool canBeOther = false;

            for (const auto &other : BINARY_OPERATORS_ORDER) {
                if (other.size() > name.size() && i + other.size() <= code_.size()
                    && code_.substr(i, other.size()) == other) {
                    canBeOther = true;
                    break;
                }
            }

            if (canBeOther) {
                continue;
            }

            PhpChepinOperatorParser parser;

            parts.push_back(code_.substr(0, i));
            parts.push_back(code_.substr(i + name.size()));

            return true;
        }

        if (code_[i] == '(') {
            rbal++;
        }

        if (code_[i] == ')') {
            rbal--;
        }

        if (code_[i] == '{') {
            fbal++;
        }

        if (code_[i] == '}') {
            fbal--;
        }

        if (code_[i] == '[') {
            sbal++;
        }

        if (code_[i] == ']') {
            sbal--;
        }
    }

    return false;
}

// \f - any symbols, stop when next character is found, will contribute to parts
// \r - any symbols, stop when next character is found, will not contribute to parts
// \a - only letters, nonempty, stop when next character is found, will contribute to parts
// (,[,{ - bracket sequence, will contribute to parts
// any other symbol will count as letter in code in will be just skipped

bool PhpChepinOperatorParser::TryFindOperatorWithStructure(std::string structure,
                                                           std::vector<std::string> &parts)
{
    parts.clear();

    size_t balance = 0;
    size_t structurePtr = 0;
    std::string currPart = "";

    for (int i = 0; i < code_.size(); i++) {
        if (structurePtr == structure.size()) {
            return false;
        }

        if (code_[i] == ' ' || code_[i] == '\t' || code_[i] == '\n') {
            currPart += code_[i];
            continue;
        }

        if (structure[structurePtr] == '\f') {
            if (structurePtr != structure.size() && code_[i] == structure[structurePtr + 1]) {
                parts.push_back(currPart);
                currPart = "";
                structurePtr++;
            } else {
                currPart += code_[i];
                continue;
            }
        }

        if (structure[structurePtr] == '\r') {
            if (structurePtr != structure.size() && code_[i] == structure[structurePtr + 1]) {
                currPart = "";
                structurePtr++;
            } else {
                continue;
            }
        }

        if (structure[structurePtr] == '\a') {
            if ((structurePtr != structure.size() && code_[i] == structure[structurePtr + 1])
                || !checkIfCharacterIsNaming(code_[i])) {
                if (i == 0 || !checkIfCharacterIsNaming(code_[i - 1])) {
                    return false;
                }

                parts.push_back(currPart);
                currPart = "";
                structurePtr++;
            } else {
                currPart += code_[i];
                continue;
            }
        }

        if (balance) {
            currPart += code_[i];

            if ((code_[i] == ')' && structure[structurePtr] == '(')
                || (code_[i] == '}' && structure[structurePtr] == '{')
                || (code_[i] == ']' && structure[structurePtr] == '[')) {
                balance--;
            }

            if ((code_[i] == '(' && structure[structurePtr] == '(')
                || (code_[i] == '{' && structure[structurePtr] == '{')
                || (code_[i] == '[' && structure[structurePtr] == '[')) {
                balance++;
            }

            if (balance == 0) {
                structurePtr++;

                parts.push_back(currPart);

                currPart = "";
            }
        } else if ((structure[structurePtr] == '(' || structure[structurePtr] == '{'
                    || structure[structurePtr] == '[')
                   && structure[structurePtr] == code_[i]) {
            currPart = code_[i];
            balance++;
        } else if (structure[structurePtr] == code_[i]) {
            structurePtr++;
        } else {
            return false;
        }
    }

    parts.push_back(currPart);

    while (structurePtr < structure.size()
           && (structure[structurePtr] == '\r' || structure[structurePtr] == '\f')) {
        structurePtr++;
    }

    if (structurePtr != structure.size()) {
        return false;
    }

    return true;
}

void PhpChepinOperatorParser::ParsePartsOfConstruction(const std::vector<std::string> &parts,
                                                       std::string nameOfConstruction = "",
                                                       std::string Nspace = "")
{
    std::vector<std::string> newPath = path_;
    std::string newClassNspace = classNspace_;
    std::string newFuncNspace = funcNspace_;

    PhpChepinOperatorParser parser;

    for (size_t partNum = 0; partNum < parts.size(); partNum++) {
        if (nameOfConstruction != "") {
            newPath.push_back(std::to_string(partNum) + nameOfConstruction);
        }

        if (nameOfConstruction == "class") {
            newClassNspace += Nspace + "::";
        } else if (nameOfConstruction == "function") {
            newFuncNspace += Nspace + "::";
        } else if (nameOfConstruction == "->") {
            if (partNum == 0) {
                continue;
            }

            if (parts[0] == "$this") {
                newFuncNspace = "";
            } else {
                newFuncNspace += parts[0] + "::";
            }
        }

        // dont look

        parser.SetCode(parts[partNum]);
        parser.SetPath(newPath);
        parser.SetNamespaces(newClassNspace, newFuncNspace);
        parser.ParseCode();
        AddDataFrom(parser);

        if (nameOfConstruction != "") {
            newPath.pop_back();
        }

        if (nameOfConstruction == "class") {
            newClassNspace = classNspace_;
        } else if (nameOfConstruction == "function") {
            newFuncNspace = funcNspace_;
        } else if (nameOfConstruction == "->") {
            newClassNspace = classNspace_;
            newFuncNspace = funcNspace_;
        }
    }
}

void PhpChepinOperatorParser::SetCode(std::string code)
{
    code_ = code;
}

void PhpChepinOperatorParser::SetPath(std::vector<std::string> path)
{
    path_ = path;
}

void PhpChepinOperatorParser::SetNamespaces(std::string classNspace, std::string funcNspace)
{
    classNspace_ = classNspace;
    funcNspace_ = funcNspace;
}

void PhpChepinOperatorParser::ParseCode()
{
    valuablesData_.clear();

    code_ = std::regex_replace(code_, std::regex(R"(^\s+|\s+$)"), "");

    if (code_ == "") {
        return;
    }

    std::smatch match;

    std::cout << code_ << std::endl;

    // special constructions

    std::vector<std::string> findInfo;
    if (TryFindBinaryOperator(";", findInfo)) {
        ParsePartsOfConstruction(findInfo);
        return;
    }
    if (std::regex_match(code_, match, std::regex(R"(^\((.*)\)$)"))) {
        ParsePartsOfConstruction({match[1]});
        return;
    } else if (std::regex_match(code_, match, std::regex(R"(^\{(.*)\}$)"))) {
        ParsePartsOfConstruction({match[1]});
        return;
    } else if (std::regex_match(code_, match, std::regex(R"(^\[(.*)\]$)"))) {
        ParsePartsOfConstruction({match[1]});
        return;
    }

    if (TryFindOperatorWithStructure("while({\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "while");
        return;
    } else if (TryFindOperatorWithStructure("for({\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "for");
        return;
    } else if (TryFindOperatorWithStructure("if({\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "if");
        return;
    } else if (TryFindOperatorWithStructure("elseif({\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "elseif");
        return;
    } else if (TryFindOperatorWithStructure("else{\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "else");
        return;
    } else if (TryFindOperatorWithStructure("function\a({\f", findInfo)) {
        auto cutName = vector_cut(findInfo, 1);
        auto cutFunction = vector_cut(cutName.second, 2);
        ParsePartsOfConstruction(cutFunction.first, "function", cutName.first[0]);
        ParsePartsOfConstruction(cutFunction.second);
        return;
    } else if (TryFindOperatorWithStructure("class\a{\f", findInfo)) {
        auto cutName = vector_cut(findInfo, 1);
        auto cutClass = vector_cut(cutName.second, 1);
        ParsePartsOfConstruction(cutClass.first, "class", cutName.first[0]);
        ParsePartsOfConstruction(cutClass.second);
        return;
    } else if (TryFindOperatorWithStructure("return\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "return");
        return;
    } else if (TryFindOperatorWithStructure("break\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "break");
        return;
    } else if (TryFindOperatorWithStructure("public\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "public");
        return;
    } else if (TryFindOperatorWithStructure("echo\f", findInfo)) {
        ParsePartsOfConstruction(findInfo, "echo");
        return;
    } else if (TryFindOperatorWithStructure("\a(\f", findInfo)) {
        auto cut = vector_cut(findInfo, 1);
        ParsePartsOfConstruction(cut.second, "cfun" + findInfo[0]);
        return;
    }
    // arithmetic and logical operators

    for (const std::string &operatorName : BINARY_OPERATORS_ORDER) {
        if (TryFindBinaryOperator(operatorName, findInfo)) {
            ParsePartsOfConstruction(findInfo, operatorName);
            return;
        }
    }

    //indexator(fix)

    if (TryFindOperatorWithStructure("$\a[", findInfo)) {
        ParsePartsOfConstruction(findInfo, "[]");
        return;
    }

    // operands

    if (checkIfValuable(code_)) {
        if (code_[0] != '$') {
            code_ = '$' + code_;
        }

        AddOperand(classNspace_ + funcNspace_ + code_, path_);
    }
}

std::map<std::string, std::vector<std::vector<std::string>>>
PhpChepinOperatorParser::GetValuablesData()
{
    return valuablesData_;
}

// dont look here

std::tuple<std::vector<std::string>,
           std::vector<std::string>,
           std::vector<std::string>,
           std::vector<std::string>>
PhpChepinOperatorParser::GetMetrics()
{
    std::vector<std::string> p;
    std::vector<std::string> m;
    std::vector<std::string> c;
    std::vector<std::string> t;

    for (const auto &variable : valuablesData_) {
        size_t cntWhenModifying = 0;

        if (variable.second.size() == 1) {
            t.push_back(variable.first);
            goto next;
        }

        for (const auto &usage : variable.second) {
            for (const auto &instance : usage) {
                if (instance == "0if" || instance == "0for" || instance == "0while") {
                    c.push_back(variable.first);
                    goto next;
                }

                if (instance == "0readline" || instance == "0function") {
                    p.push_back(variable.first);
                    goto next;
                }
            }
        }

        m.push_back(variable.first);

    next:
        (void) 0;
    }

    return {p, m, c, t};
}

std::tuple<std::vector<std::string>,
           std::vector<std::string>,
           std::vector<std::string>,
           std::vector<std::string>>
PhpChepinOperatorParser::GetInputOutputMetrics()
{
    std::vector<std::string> p;
    std::vector<std::string> m;
    std::vector<std::string> c;
    std::vector<std::string> t;

    for (const auto &variable : valuablesData_) {
        size_t cntWhenModifying = 0;

        bool canBeUsed = false;

        for (const auto &usage : variable.second) {
            for (const auto &instance : usage) {
                if (instance == "0cfunreadline" || instance == "0echo") {
                    canBeUsed = true;
                }
            }
        }

        if (!canBeUsed) {
            goto next;
        }

        if (variable.second.size() == 1) {
            t.push_back(variable.first);
            goto next;
        }

        for (const auto &usage : variable.second) {
            for (const auto &instance : usage) {
                if (instance == "0if" || instance == "0for" || instance == "0while") {
                    c.push_back(variable.first);
                    goto next;
                }

                if (instance == "0cfunreadline" || instance == "0function") {
                    p.push_back(variable.first);
                    goto next;
                }
            }
        }

        m.push_back(variable.first);

    next:
        (void) 0;
    }

    return {p, m, c, t};
}

void PhpChepinOperatorParser::AddDataFrom(const PhpChepinOperatorParser &other)
{
    for (const auto &instance : other.valuablesData_) {
        for (const auto &path : instance.second) {
            valuablesData_[instance.first].push_back(path);
        }
    }

    for (const auto &valuable : other.valuablesType_) {
        valuablesType_[valuable.first] = valuable.second;
    }
}
