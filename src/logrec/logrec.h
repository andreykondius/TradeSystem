// Автор: Кондиус А.В.
// Класс, предназначенный для ведения логов.

#pragma once

#include <fstream>
#include <string>
#ifndef PROJECT_VERSION
    #define PROJECT_VERSION "111"
#endif


class Logrec
{
    std::string fileName;
    std::string fullPath;
    mutable std::string shortFileName;
    mutable std::ofstream FileLog;
public:
	Logrec();
	Logrec(const char *path);
	~Logrec();
    int Loglevel{3};
    std::ostream &getFileLog() const;
    void start();
    void open();
    void close();
    std::string getCurTime();
    std::string getDataFromFormat(const std::string &val);
    std::string getCurTimeMs();
    std::string getShortFileName() const;
    void setFullPath(const std::string &val);
    std::string getLogNameAsString();
    std::string getFullPath() const;
    std::string getName();
    std::string getVersion();
};

