// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_UPLOADREQUEST_H
#define QUICKSAVE_UPLOADREQUEST_H

#include <bean/UploadRequestBean.h>
#include <bean/MessageBean.h>
#include <folly/io/IOBuf.h>
#include <util/base64.h>

class UploadRequest : public UploadRequestBean
{
public:
    using UploadRequestBean::UploadRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        /*
        std::cout << filename << std::endl;
        std::cout << filebase << std::endl;
        std::cout << qs::Base64::decode(filebase) << std::endl;
        */

        std::ofstream filestream{filename};
        filestream << qs::Base64::decode(filebase);
        filestream.close();

        MessageBean messageBean;

        messageBean.message = "OK";
        return messageBean;
    }
};

#endif
