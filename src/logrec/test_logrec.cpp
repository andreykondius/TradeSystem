#include "putlog.hpp"
#include <chrono>
#include <thread>

void fo(char);
void bar(char);

int main(){
    int count = 30;
    std::thread thr1 ([](char a){fo(a);},count);
    std::thread thr2 ([](char a){bar(a);},count);
    std::thread thr3 ([](char a){fo(a);},count+5);
    std::thread thr4 ([](char a){bar(a);},count+10);
    int i = 1;
    while(count > i-1)
    {
    std::string message = "mesage_from_main_" + std::to_string(i) ;
    Putlog(LOG_MESSAGE,"thread_main ", message);
    Putlog(LOG_WARN,"thread_main ","warning1","warning2", "warning3", "warning4");
    Putlog(LOG_ERROR,"thread_main ","error1", "error2");
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    ++i;
    }
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
}

void fo(char count)
{
    int i = 1;
    while(count > i-1)
    {
    std::string message = "mesage_from_fo_" + std::to_string(i) ;
    Putlog(LOG_INFO,"thread_slave ", "messageInfo1","messageInfo2","messageInfo3","messageInfo4");
    Putlog(LOG_MESSAGE,"thread_slave ",  message);
    std::this_thread::sleep_for(std::chrono::milliseconds(8));
    ++i;
    }
}

void bar(char count)
{
    int i = 1;
    while(count > i-1)
    {
    std::string message = "mesage_from_bar_" + std::to_string(i) ;
    Putlog(LOG_INFO,"thread_slave ", "errorInfo1","errorInfo2");
    Putlog(LOG_MESSAGE,"thread_slave ", message);
    std::this_thread::sleep_for(std::chrono::milliseconds(8));
    ++i;
    }
}
