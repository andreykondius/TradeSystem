// Автор: Кондиус А.В.
// Класс-синглтон предназначенный для управления логированием
#pragma once
#include <assert.h>

#include "logrec.h"
#include <utility>
#include <mutex>

class Logrec;

class SingltoneCollection
{
private:
    SingltoneCollection();
    ~SingltoneCollection();

    static SingltoneCollection inst;
    Logrec* logfile = nullptr;
    static std::mutex mut;
public:
    SingltoneCollection(const SingltoneCollection &val) = delete;
    SingltoneCollection &operator=(const SingltoneCollection &val) = delete;

    static SingltoneCollection& instance();
    Logrec &getLogFile();
//    void createLogFile(const std::string &val);
    void createLogFile();
    void closeLogFile();
    std::string truncateString(std::string string);
};

extern std::mutex mut;
