#ifndef PARSE_FILE_H
#define PARSE_FILE_H

#include <QFile>
#include <QString>
#include <QDebug>
#include <filesystem>
#include <regex>
#include <string>
#include <iostream>

const QString FILE_WITH_CODE = QString::fromStdString(std::filesystem::current_path().u8string())  + QString("/") + QString("code.php");

class Parse_file
{
private:
    QFile file = QFile(FILE_WITH_CODE);


public:
    Parse_file();
    std::string parse();
};

#endif // PARSE_FILE_H
