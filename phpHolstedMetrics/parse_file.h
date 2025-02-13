#ifndef PARSE_FILE_H
#define PARSE_FILE_H

#include <QFile>
#include <QString>
#include <QDebug>
#include <filesystem>
#include <regex>
#include <string>
#include <iostream>

const QString DEFAULT_FILE = QString::fromStdString(std::filesystem::current_path().u8string())  + QString("/") + QString("code.php");

class Parse_file
{
private:
    QFile file = QFile(DEFAULT_FILE);

public:
    Parse_file();
    explicit Parse_file(QString fileName);
    std::string Parse();
};

#endif // PARSE_FILE_H
