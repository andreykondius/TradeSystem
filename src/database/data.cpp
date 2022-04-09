#include "data.h"
#include <iostream>
#include <QString>
#include <QStringList>
#include <string.h>
#include "logrec/putlog.hpp"

Data::Data(const char *vol)
{
    QString readS( vol );
    int sizeData = readS.size()-1;
    data = new char[sizeData];
    memcpy(data, vol, sizeData);

//    Putlog("data = ", data);
    QStringList arr = readS.split("\n");
    if(arr.size() >= 7)
    {
//        Putlog(" 0 = ", arr.at(0).toStdString());
        flags       = arr.at(0).toInt();
//        Putlog(" 1 = ", arr.at(1).toStdString());
        datetime    = arr.at(1).toStdString();
//        Putlog(" 2 = ", arr.at(2).toStdString());
        time_msc    = arr.at(2).toLongLong();
//        Putlog(" 3 = ", arr.at(3).toStdString());
        bid         = arr.at(3).toDouble();
//        Putlog(" 4 = ", arr.at(4).toStdString());
        ask         = arr.at(4).toDouble();
//        Putlog(" 5 = ", arr.at(5).toStdString());
        last        = arr.at(5).toDouble();
//        Putlog(" 6 = ", arr.at(6).toStdString());
        volume_real = arr.at(6).toDouble();
    }

    for(auto i=0; i < 7; ++i)
    {
        int index = readS.indexOf("\n")+1;
        if(index == 0)
        {
            readS.clear();
            sizeData = -1;
            break;
        }
        sizeData -= index;
        readS.remove(0, index);
    }
    if(sizeData > 0)
    {
        memcpy(data, readS.toStdString().c_str(), sizeData+2);
        data[sizeData] = '\n';
        data[sizeData+1] = '\0';
    }
    else
    {
        delete []data;
        data = nullptr;
    }
}

Data::~Data()
{
    delete []data;
    data = nullptr;
}

uint Data::getFlags() const
{
    return flags;
}

char *Data::getDateTime() const
{
    return const_cast<char*>(datetime.c_str());
}

llint Data::getTime() const
{
    return time_msc;
}

llint Data::getVolume() const
{
    return volume;
}

double Data::getBid() const
{
    return bid;
}

double Data::getAsk() const
{
    return ask;
}

double Data::getLast() const
{
    return last;
}

double Data::getVolume_Real() const
{
    return volume_real;
}

char *Data::getData() const
{
    return data;
}
