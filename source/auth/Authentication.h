// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_AUTHENTICATION_H
#define QUICKSAVE_AUTHENTICATION_H

#include <CppBeans.h>
#include <iostream>
#include <proxygen/lib/http/HTTPMessage.h>
#include <proxygen/lib/utils/Base64.h>

struct Authentication
{
    static std::pair<std::string, std::string> dispatchPost(const proxygen::HTTPMessage* headers)
    {
        std::cout << headers->getQueryParam("name") << " " << headers->getQueryParam("pass") << std::endl;
        return {headers->getQueryParam("name"), headers->getQueryParam("pass")};
    }

    static std::pair<std::string, std::string> dispatchBasicAuth(const proxygen::HTTPMessage* headers)
    {
        std::string auth_with_prefix_based = headers->getHeaders().rawGet("Authorization");

        int trail = 0;
        while (
            (auth_with_prefix_based[auth_with_prefix_based.size() - 1 - trail] == '=')
            && (static_cast<int>(auth_with_prefix_based.size()) > trail - 1))
        {
            trail++;
        }

        if (static_cast<int>(auth_with_prefix_based.size()) - 6 - trail > 1)
        {
            std::string auth_based = {&auth_with_prefix_based[6], auth_with_prefix_based.size() - 6 - trail};
            std::string auth = proxygen::Base64::urlDecode(auth_based);

            std::string login;
            std::string pass;

            long unsigned int p = 0;
            for (auto c : auth)
            {
                if (c == ':')
                {
                    login = {&auth[0], p};
                    pass = {&auth[p + 1], auth.size() - p - 1};
                    return {login, pass};

                }
                ++p;
            }
        }
        else
        {
            std::cout << "No basic auth string detected" << std::endl;
        }

        return {"", ""};
    }
};

#endif
