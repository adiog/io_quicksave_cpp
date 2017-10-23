// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <env.h>

#include <cstring>
#include <unordered_map>

#include <curl/curl.h>


namespace qs {
namespace audit {

namespace {
size_t read_function(char* bufptr, size_t size, size_t nitems, void* userp)
{
    auto * message = static_cast<std::string*>(userp);
    size_t length = message->size();
    if (length > size * nitems)
    {
        length = size * nitems;
    }
    std::memcpy(bufptr, &((*message)[0]), length);
    return length;
}
}

class Audit
{
public:
    void operator<<(std::string message)
    {
        CURL* curlHandle = curl_easy_init();
        if (curlHandle)
        {
            CURLcode res;
            curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curlHandle, CURLOPT_READFUNCTION, read_function);
            curl_easy_setopt(curlHandle, CURLOPT_READDATA, &message);
            curl_easy_setopt(curlHandle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)message.size());
            curl_easy_setopt(curlHandle, CURLOPT_UPLOAD, 1L);
            res = curl_easy_perform(curlHandle);
            curl_easy_cleanup(curlHandle);
        }
    }

    Audit(const std::string& severity)
            : severity(severity)
    {
        url = FLAGS_IO_QUICKSAVE_AUDIT + "/log/" + severity + "/";
    }

private:
    std::string severity;
    std::string url;
};
}
}

#define AUDIT_FATAL qs::audit::Audit("fatal")