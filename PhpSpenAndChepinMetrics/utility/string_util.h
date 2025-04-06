#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <string>
#include <regex>

inline bool checkIfValuable(std::string text)
{
    return text != "false" && text != "true" && std::regex_match(text, std::regex(R"(^(([a-zA-Z]+)|(\$[a-zA-Z]+))$)"));
}

inline bool checkIfCharacterIsNaming(char character)
{
    return ((std::isalpha(character)) || character == '_');
}

#endif // STRING_UTIL_H
