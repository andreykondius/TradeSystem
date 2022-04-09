#include "tcpserver.h"
#include "database/data.h"
#include "database/database.h"
#include "database/thrpushtofile.h"
#include "neurocpu/neuron.h"
#include "neurocpu/perceptron.h"
#include "trademanager.h"

#include <iostream>
#include <iterator>
#include <QDir>
#include <QStringList>
#include <QApplication>

void TcpServer::clearClients()
{
    while(clients.begin() != clients.end())
    {
        auto it = clients.begin();
        it->second->getThrPushToFile()->setExit(true);
        emit it->second->getThrPushToFile()->toFinished();
        delete it->second;
        clients.erase(it);
    }
}

void TcpServer::sendCommand(const char &val)
{
    for(auto &client: clients)
    {
        char* dat = new char(val+'\0');
        client.second->getSock()->write( dat );
    }
}

TcpServer::TcpServer(const int port, const bool fSave, QObject *parent) : QObject(parent), port(port), fSaveData(fSave)
{
}

TcpServer::~TcpServer()
{
    clearClients();
    std::cout << "~TcpServer" << std::endl;
}

void TcpServer::setSaveData(const bool val)
{
    fSaveData = val;
    emit sigSaveData( fSaveData );
}

bool TcpServer::isSaveData() const
{
    return fSaveData;
}

void TcpServer::trainingNetFromData(const QDir &dir, const int indexEpochTest)
{    
    if(fStop)
        return;
    QStringList listFiles = dir.entryList(QStringList("*.bin"), QDir::Files);
    foreach (QString file, listFiles)
    {
        QStringList lTemp = file.split("__");
        std::string nameActive = lTemp[0].toStdString();
        DataBase database ( nameActive, dir.absoluteFilePath(file).toStdString() );
        TradeManager tradeManager("RTS-6.20", 10.0, indexEpochTest, true);

        while(!fStop)
        {
            char c[1000] = {0};
            if(!database.pull(c))
                break;
            tradeManager.calcData(c);
            QApplication::processEvents();
        }
        tradeManager.saveToWeights(true);        
        emit toProfit( tradeManager.getTestProfit(), tradeManager.getCurDate().c_str() );
        return;
    }

    QStringList listDir = dir.entryList(QDir::Dirs);
    foreach (QString subdir, listDir) {
        if (subdir == "." || subdir == "..") {
            continue;
        }
        trainingNetFromData(QDir(dir.absoluteFilePath(subdir)), indexEpochTest);
    }
}

bool TcpServer::setConnect()
{
    clearClients();
    if(server != nullptr)
    {
        server->close();
        server.reset();
    }
    server = std::make_unique<QTcpServer>();
    connect(server.get(), &QTcpServer::newConnection, this, &TcpServer::onConnected);
    if(!server->listen(QHostAddress::Any, port))
    {
        std::cout << "Sock can't to listen the port: " << port << std::endl;
        return false;
    }
    std::cout << "The socket is listens the port: " << port << std::endl;

    return true;
}

void TcpServer::onNewClient()
{
    disconnect(sockConnect.get(), &QTcpSocket::readyRead, this, &TcpServer::onNewClient);
    while(sockConnect->bytesAvailable() > 0)
    {
        newNameActive = sockConnect->readAll().data();
    }

    clients[ newNameActive ] = new TcpSocket( std::move(sockConnect), newNameActive, this );
    connect(clients[newNameActive]->getSock(), &QTcpSocket::readyRead, clients[newNameActive], &TcpSocket::readyRead);
    connect(this, &TcpServer::sigSaveData, clients[newNameActive], &TcpSocket::onSaveData);
    emit sigSaveData( fSaveData );
}

void TcpServer::onStopTest(const bool flag)
{
    fStop = flag;
}

void TcpServer::onConnected()
{
    sockConnect = std::make_unique<QTcpSocket>();
    sockConnect = std::unique_ptr<QTcpSocket>(server->nextPendingConnection());
    connect(sockConnect.get(), &QTcpSocket::readyRead, this, &TcpServer::onNewClient);
}
//------------------ TcpSocket ------------------------------------------
TcpSocket::TcpSocket(std::unique_ptr<QTcpSocket> val, const std::string &nameActive, QObject *parent)
    : QObject(parent), nameActive(nameActive), sock(std::move(val))
{
    std::cout << "Sock is connected: " <<  sock->peerAddress().toString().toStdString()
              << ":" << sock->peerPort() <<std::endl;
    thrPush = new QThread(this);
    thrPushToFile = new ThrPushToFile(nameActive);
    thrPushToFile->moveToThread( thrPush );
    connect(thrPush, SIGNAL(started()), thrPushToFile, SLOT(run()));
    connect(thrPushToFile, SIGNAL(toFinished()), thrPush, SLOT(quit()));
    connect(thrPushToFile, SIGNAL(toFinished()), thrPush, SLOT(deleteLater()));
    thrPush->start();
}

TcpSocket::~TcpSocket()
{
    std::cout << "~TcpSocket" << std::endl;
}

void TcpSocket::readyRead()
{
    while(sock->bytesAvailable() > 0)
    {
        if(fSaveData)
            thrPushToFile->push( sock.get()->readAll().data() );
    }
}

QTcpSocket *TcpSocket::getSock()
{
    return sock.get();
}

void TcpSocket::setNameActive(const std::string &val)
{
    nameActive = val;
}

void TcpSocket::onSaveData(const bool flag)
{
    fSaveData = flag;
    if(!fSaveData)
        thrPushToFile->toClosefile();
}

void TcpSocket::setSizeTick(const double val)
{
    sizeTick = val;
}

double TcpSocket::getSizeTick() const
{
    return sizeTick;
}

ThrPushToFile *TcpSocket::getThrPushToFile()
{
    return thrPushToFile;
}
