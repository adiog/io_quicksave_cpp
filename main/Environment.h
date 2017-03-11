// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_ENVIRONMENT_H
#define QUICKSAVE_ENVIRONMENT_H

#include <Config.h>

class Environment
{
    static const char * get(const char * var, const char * defaultValue)
    {
        const char * value = getenv(var);
        if (value == nullptr){
            return defaultValue;
        } else
        {
            return value;
        }
    }

    static const char * getDatabase()
    {
        return get("IO_QUICKSAVE_DB", IO_QUICKSAVE_DB);
    }
};

#endif //QUICKSAVE_ENVIRONMENT_H
