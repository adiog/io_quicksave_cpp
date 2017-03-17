//
// Created by adiog on 16.03.17.
//

#ifndef QUICKSAVE_PROVIDER_H
#define QUICKSAVE_PROVIDER_H

#include <string>
#include <util/base64.h>

class Provider
{
public:
    void accept_base(std::string& filename, std::string& filebase)
    {
        this->accept(filename, qs::Base64::decode(filebase));
    }

    virtual void accept(std::string& filename, std::string file) const = 0;
};

#endif  //QUICKSAVE_PROVIDER_H
