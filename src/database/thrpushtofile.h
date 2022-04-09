#pragma once

// Класс, который в потоке добавляет данные в историю
#include <QObject>
#include <memory>
#include <mutex>

class Data;
class DataBase;
class ThrPushToFile : public QObject
{
    Q_OBJECT

    std::atomic_bool fExit = false;
    std::atomic_bool fClosefile = false;
    std::atomic_bool fNotify = false;
    std::string nameActive;
    char *data = nullptr;
    std::shared_ptr<DataBase> dataBase;
    std::recursive_mutex mut;
public:
    explicit ThrPushToFile(const std::string &nameActive);
    void setExit(const bool val);
    void toClosefile();
    void push(const char *val);
signals:
    void toFinished();
public slots:
    void run();
};

