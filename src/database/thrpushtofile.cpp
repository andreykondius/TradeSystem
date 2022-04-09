#include "thrpushtofile.h"
#include "database/data.h"
#include "database/database.h"

ThrPushToFile::ThrPushToFile(const std::string &nameActive)
    : nameActive(nameActive)
{
}

void ThrPushToFile::setExit(const bool val)
{
    fExit = val;
}

void ThrPushToFile::toClosefile()
{
    fClosefile = true;
}

void ThrPushToFile::push(const char *val)
{
    std::lock_guard m(mut);
    data = const_cast<char*>(val);
    fNotify = true;
}

void ThrPushToFile::run()
{
    while (!fExit)
    {
        if(fNotify)
        {
            fNotify = false;
            if(!dataBase)
            {
                dataBase = std::make_shared<DataBase>( nameActive );
                fClosefile = false;
            }
            if(fClosefile)
            {
                dataBase->closeFile();
                fClosefile = false;
            }
            dataBase->push( data );
        }
    }
}
