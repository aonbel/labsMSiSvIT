#include "parse_file.h"
#include <QDebug>

Parse_file::Parse_file() {}

Parse_file::Parse_file(QString fileName) : file(QFile(fileName)) {}

std::string Parse_file::Parse()
{
    QString code_from_file = "";

    std::smatch match;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) qDebug() << "OK";
    else qDebug() << "FILE ERROR";

    file.seek(0);
    while(!file.atEnd()) code_from_file += file.readLine();

    std::string code = code_from_file.toStdString();

    code = std::regex_replace(code, std::regex(R"(\n)"), "");                       // delete '\n'

    if (std::regex_match(code, match, std::regex(R"(^(<\?php)(.*)(\?)>$)")))        // delete '<?php...?>'
    {
        code = match[2];
    }

    return code;
}
