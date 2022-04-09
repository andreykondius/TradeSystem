#pragma once

#include <QMap>
#include <fstream>
#include <memory>

class Data;
class DataBase
{
    std::ofstream *filePush;
    std::fstream *filePull;
    std::string fileName;
    std::string nameActive;
    void setPath(const bool fWrite);
public:
    explicit DataBase(const std::string &nameActive);
    DataBase(const std::string &nameActive, const std::string &fileName);
    ~DataBase();
    void closeFile();
    void push(const char *val);
    bool pull(char *bufData);
    void setNameActive(const std::string &val);
};

