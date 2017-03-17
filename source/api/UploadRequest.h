// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_UPLOADREQUEST_H
#define QUICKSAVE_UPLOADREQUEST_H

#include <bean/UploadRequestBean.h>
#include <bean/MessageBean.h>
#include <folly/io/IOBuf.h>
#include <provider/FileSystem.h>

class UploadRequest : public UploadRequestBean
{
public:
    using UploadRequestBean::UploadRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        std::unique_ptr<Provider> provider = std::make_unique<FileSystem>("/io.quicksave.cdn/"); // TODO

                provider->accept_base(filename, filebase);

        /*
        std::cout << filename << std::endl;
        std::cout << filebase << std::endl;
        std::cout << qs::Base64::decode(filebase) << std::endl;
        */


        MessageBean messageBean;

        messageBean.message = "OK";
        return messageBean;
    }
};

#endif
