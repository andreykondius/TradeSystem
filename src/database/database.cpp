#include "database.h"
#include "data.h"

#include <QDir>
#include <iostream>

DataBase::DataBase(const std::string &nameActive)
    : filePush(nullptr), filePull(nullptr),nameActive(nameActive)
{
}

DataBase::DataBase(const std::string &nameActive, const std::string &fileName)
    : filePush(nullptr), filePull(nullptr), fileName(fileName), nameActive(nameActive)
{
}

DataBase::~DataBase()
{
    delete filePush;
    delete filePull;
}

void DataBase::closeFile()
{
}

void DataBase::setPath(const bool fWrite)
{
    if((!filePush && fWrite) || (!filePull && !fWrite))
    {
        auto getDataFromFormat = [](const std::string &val)
        {
            time_t sec = time(nullptr);
            tm* timeinfo = localtime(&sec);
            char* format = const_cast<char*>(val.c_str());
            char curTime[80];
            strftime(curTime, 80, format, timeinfo);
            return QString(curTime);
        };

        if(fWrite)
        {
            QString subPath = getDataFromFormat("%d.%m.%Y")+"\\";
            QString name = QString(nameActive.c_str())+getDataFromFormat("__%d_%m_%Y.bin");
            fileName = "FILES\\HISTORY\\"+subPath.toStdString() + name.toStdString();

            QDir dir;
            dir.mkpath( "FILES\\HISTORY\\"+subPath );
            filePush = new std::ofstream();
            filePush->open(fileName, std::ios_base::app | std::ios_base::binary);
        }
        else
        {
            filePull = new std::fstream();
            filePull->open(fileName, std::ios_base::in | std::ios_base::binary);
        }
    }
}

void DataBase::push(const char *val)
{
    setPath(true);
    std::string temp(val);
    int size = temp.size()-1;
    filePush->write(std::to_string(size).c_str(), 4);
    filePush->write(val, size);
}

bool DataBase::pull(char *bufData)
{
    setPath(false);
    if(filePull->eof())
        return false;
    int sz = 0;
    char cSz[4];
    if(filePull->read(cSz, 4))
    {
        if(filePull->eof())
            return false;
    }
    else
        return false;
    sz = std::stoi(cSz);
    if(filePull->read(bufData, sz))
    {
        if(filePull->eof())
            return false;
    }
    else
        return false;
    return true;
}

void DataBase::setNameActive(const std::string &val)
{
    nameActive = val;
}
