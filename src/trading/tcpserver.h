#pragma once

#include <memory>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <thread>
#include <QDir>

enum Command
{
    eNONE=100,
    eGETDATA
};

class Perceptron;
class TcpSocket;
class TcpServer : public QObject
{
    Q_OBJECT

    const int port = 2048;
    double expected = 0.15;    // for TEST
    bool fSaveData = false;
    bool fStop = false;
    std::string newNameActive;
    std::unique_ptr<QTcpServer> server;
    std::unordered_map< std::string, TcpSocket* > clients;       // список подключённых клиентов
    std::unique_ptr<QTcpSocket> sockConnect;
    std::unique_ptr<Perceptron> nperceptron_TEST;

    void clearClients();

public:
    explicit TcpServer(const int port, const bool fSave, QObject *parent = 0);
    ~TcpServer();

    void setSaveData(const bool val);
    bool isSaveData() const;

    void trainingNetFromData(const QDir &dir, const int indexEpochTest);
    bool setConnect();
    void sendCommand(const char &val);

private slots:
    void onConnected();
    void onNewClient();

public slots:
    void onStopTest(const bool flag);
signals:
    void sigSaveData(const bool flag);
    void toRes(double err, double res, int epoch, bool valid);
    void toProfit(double newProf, const char* date);
};

class ThrPushToFile;
class TcpSocket : public QObject
{
    Q_OBJECT

    enum TypeDeal{
        eBUY,
        eSELL,
        eNONE
    };

    TypeDeal type;
    bool fSaveData = false;
    double sizeTick;
    std::string nameActive;
    std::unique_ptr<QTcpSocket> sock;
    QThread *thrPush;
    ThrPushToFile* thrPushToFile;

public:
    TcpSocket(std::unique_ptr<QTcpSocket> val, const std::string &nameActive, QObject *parent = 0);
    ~TcpSocket();

    void readyRead();
    QTcpSocket *getSock();

    void setNameActive(const std::string &val);

    void setSizeTick(const double val);
    double getSizeTick() const;

    ThrPushToFile *getThrPushToFile();

public slots:
    void onSaveData(const bool flag);
};

