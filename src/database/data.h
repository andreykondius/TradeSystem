#pragma once

#include <QByteArray>
using llint = long long int;

class Data  // size of Data = 60 bytes
{
    enum TypeFlag{
        BID,        // 2
        ASK,        // 4
        LAST,       // 8
        VOLUME,     // 16
        BUY,        // 32
        SELL        // 64
    } typeFlag;
    uint  flags;        // 4 bytes Флаги тиков
    std::string datetime;  // ~20 bytes
    llint time_msc;     // 8 bytes
    llint volume;       // 8 Объем для текущей цены Last
    double bid;         // 8 текущая цена bid
    double ask;         // 8 текущая цена ack
    double last;        // 8 Текущая цена последней сделки (Last)
    double volume_real; // 8 Объем для текущей цены Last c повышенной точностью

    char *data;

public:
    explicit Data(const char *vol);
    ~Data();
    uint getFlags() const;
    char *getDateTime() const;
    llint getTime() const;
    llint getVolume() const;
    double getBid() const;
    double getAsk() const;
    double getLast() const;
    double getVolume_Real() const;

    char *getData() const;
};
