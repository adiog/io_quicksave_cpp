//
// Created by adiog on 06.03.17.
//

#ifndef QUICKSAVE_LOGGER_H_H
#define QUICKSAVE_LOGGER_H_H

#include <cstdio>
#include <ctime>
#include <string>
#include <iostream>

class Logger
{
public:
   template<typename... Args>
static std::string format(std::string fmt, Args... args)
{
    std::vector<char>::size_type size = 256;
    std::vector<char> buf;
    do {
        // use snprintf instead of sprintf (to avoid buffer overflows)
        // snprintf returns the required size (without terminating null)
        // if buffer is too small initially: loop should run at most twice
        buf.resize(size+1);
        size = std::snprintf(
                &buf[0], buf.size(),
                fmt.c_str(), args...);
    } while (size+1 > buf.size());
    // assign to std::string
    std::string str = &buf[0];
    return str;
}

    static void log(const std::string &message)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [80];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        std::strftime(buffer,80,"[%d/%m/%Y %H:%M:%S] ",timeinfo);

        std::cout << buffer << message << std::endl;
    }
};

#endif //QUICKSAVE_LOGGER_H_H
