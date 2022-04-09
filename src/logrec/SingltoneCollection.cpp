#include "SingltoneCollection.h"
#include <string>
#define WIN32

SingltoneCollection SingltoneCollection::inst;
std::mutex SingltoneCollection::mut;
std::mutex mut;

SingltoneCollection::SingltoneCollection()
{
}

SingltoneCollection::~SingltoneCollection()
{
    closeLogFile();
}

Logrec &SingltoneCollection::getLogFile()
{
    std::lock_guard<std::mutex> lock(mut);
    if (logfile == nullptr)
#ifdef WIN32
    logfile = new Logrec();
#else
        logfile = new Logrec("/var/opt/SPO/");
#endif
    return *logfile;
}

void SingltoneCollection::createLogFile()
{
    if (logfile == nullptr)
    logfile = new Logrec();
}

SingltoneCollection &SingltoneCollection::instance()
{
    std::lock_guard<std::mutex> lock(mut);
    return inst;
}

void SingltoneCollection::closeLogFile()
{
    std::lock_guard<std::mutex> lock(mut);
    delete inst.logfile;
    inst.logfile = nullptr;
}

std::string SingltoneCollection::truncateString(std::string str)
{
    auto numLastSlash = str.rfind('\\');
    return str.substr(numLastSlash + 1);
}
