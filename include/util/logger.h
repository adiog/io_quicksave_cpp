//
// Created by adiog on 06.03.17.
//

#ifndef QUICKSAVE_LOGGER_H_H
#define QUICKSAVE_LOGGER_H_H

#include <cstdio>
#include <ctime>
#include <string>
#include <iostream>
#include <util/format.h>
class Logger
{
public:
    template<typename... Args>
    static void log(std::string fmt, Args... args)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [80];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        std::strftime(buffer,80,"[%d/%m/%Y %H:%M:%S] ",timeinfo);

        std::cout << buffer << Format::format(fmt, args...) << std::endl;
    }
};


#endif //QUICKSAVE_LOGGER_H_H
