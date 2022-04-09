#include "logrec.h"
#include <chrono>
#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif
#include <QApplication>
#include <QString>
#ifdef __cplusplus
#include "putlog.h"
#endif
Logrec::Logrec()
{
    fullPath = nameLogDir;
    fileName = fullPath + getLogNameAsString();
    start();
}

Logrec::Logrec(const char *path)
{

}
Logrec::~Logrec()
{
    close();
}
std::string Logrec::getCurTime()
{
    return getDataFromFormat("%H:%M:%S ");
}
std::string Logrec::getDataFromFormat(const std::string &val)
{
    time_t sec = time(nullptr);
    tm* timeinfo = localtime(&sec);
    char* format = const_cast<char*>(val.c_str());
    char curTime[80];
    strftime(curTime, 80, format, timeinfo);
    return curTime;
}
std::string Logrec::getShortFileName() const
{
    return shortFileName;
}
void Logrec::setFullPath(const std::string &val)
{
    fullPath = val;
    if (fullPath[fullPath.length()-1] != '/')
    {
        fullPath += "/";
    }
}
std::string Logrec::getLogNameAsString()
{
    return getDataFromFormat(LogFormat)+".txt";
}

std::string Logrec::getFullPath() const
{
    return fullPath;
}
std::ostream &Logrec::getFileLog() const
{
    return FileLog;
}
std::string Logrec::getName()
{
    std::string name = QApplication::applicationName().toStdString();
    if (name.size()>0)
    {
        name +=":";
    }
    return name;
}
std::string Logrec::getVersion()
{
    return PROJECT_VERSION;
}
void Logrec::start()
{
    open();
    FileLog << getCurTime() << "[" << getName().c_str()  << getVersion().c_str() << "]" << " LOG_INFO: Начало лога." << std::endl;
    FileLog.close();
}
void Logrec::open()
{
    FileLog.open(fileName.c_str(), std::ios::app);
}
void Logrec::close()
{
    open();
    FileLog << getCurTime() << "[" << getName().c_str() << getVersion().c_str() << "]" << " LOG_INFO: Конец лога." << std::endl;
    FileLog.close();
}


