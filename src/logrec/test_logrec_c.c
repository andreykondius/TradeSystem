#include "putlog.hpp"

void callFromMain();
void fo_c();
void bar_c();
void* fo_c_thread(void *args);
void* bar_c_thread(void *args);

typedef struct
{
    int count;
} args;

int main()
{
    args argsToThread;
    argsToThread.count = 3;
    pthread_mutex_init(&mutLogrec_c,NULL);
    printf("argsToThread.count: %d \n",argsToThread.count);
    Putlog(LOG_INFO,"begin logging");
    callFromMain();

    #define NUM_THREADS 5
    pthread_t treads_fo[NUM_THREADS];
    pthread_t treads_bar[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if(pthread_create(&treads_fo[i], NULL, fo_c_thread, &argsToThread) == 0)
        {
             printf("thread fo_%d create. \n", i+1);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if(pthread_create(&treads_bar[i], NULL, bar_c_thread, &argsToThread) == 0)
        {
             printf("thread bar_%d create. \n", i+1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
       if(pthread_join(*&treads_fo[i], NULL) == 0)
       {
           printf("thread fo_%d joined. \n", i+1);
       }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
       if(pthread_join(*&treads_bar[i], NULL) == 0)
       {
           printf("thread bar_%d joined. \n", i+1);
       }
    }
    pthread_mutex_destroy(&mutLogrec_c);
    Putlog(LOG_INFO,"end logging");
}

void* fo_c_thread(void *argsToThread)
{
    args *arg = (args*) argsToThread;
    for(int i=0; i < *&(arg->count); ++i)
    {
        Putlog(LOG_MESSAGE,"thread_slave_fo","mesage_from_fo1", "mesage_from_fo2");
        Putlog(LOG_INFO,"thread_slave_fo","info_from_fo");
        printf("count_in_thread_fo=%d \n",i);
    }
    return arg;
}

void* bar_c_thread(void *argsToThread)
{
    args *arg = (args*) argsToThread;
    for(int i=0; i < *&(arg->count); ++i)
    {
        Putlog(LOG_MESSAGE,"thread_slave_bar","mesage_from_bar");
        Putlog(LOG_ERROR,"thread_slave_bar","error_from_bar1", "error_from_bar2", "error_from_bar3");
        printf("count_in_thread_bar=%d \n",i);
    }
    return arg;
}

void fo_c()
{
    Putlog(LOG_MESSAGE,"thread_main","mesage_from_fo1", "mesage_from_fo2");
}

void bar_c()
{
    Putlog(LOG_INFO,"thread_main","info_from_bar1", "info_from_bar2", "info_from_bar3");
}

void callFromMain()
{
    fo_c();
    Putlog(LOG_INFO,"thread_main","info_message1", "info_message2", "info_message3", "info_message4");
    Putlog(LOG_MESSAGE,"thread_main","message5", "message6");
    Putlog(LOG_ERROR,"thread_main","error1");
    Putlog(LOG_WARN,"thread_main","warning_massage1","warning_massage2");
    bar_c();
}
