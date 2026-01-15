//
// Created by yasalzzt on 2025/12/17.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

enum level {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();
    ~Logger();
    Logger(const Logger& ) = delete;
    Logger& operator=(const Logger& ) = delete;
    void log(const std::string& msg,level lev = DEBUG);

private:
    Logger();
    std::ofstream logFile_;
    std::mutex mutex_;
};



#endif //LOGGER_H
