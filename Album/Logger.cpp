//
// Created by yasalzzt on 2025/12/17.
//
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>

Logger& Logger::getInstance() {
    static Logger log;
    return log;
}

Logger::Logger() {
    logFile_.open("logFile.log",std::ios::app);
    if (!logFile_.is_open()) {
        throw std::runtime_error("Logger could not open logFile.txt");
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(const std::string &msg, level lev)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::string level_str;
    switch (lev) {
    case DEBUG:
        level_str = "[DEBUG] ";
        break;
    case INFO:
        level_str = "[INFO] ";
        break;
    case WARNING:
        level_str = "[WARNING] ";
        break;
    case ERROR:
        level_str = "[ERROR] ";
        break;
    }
    if (logFile_.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&time));

        logFile_ << level_str << buffer << msg << std::endl;
    }
}






