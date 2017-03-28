// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_UPLOADREQUEST_H
#define QUICKSAVE_UPLOADREQUEST_H

#include <bean/UploadRequestBean.h>
#include <bean/MessageBean.h>
#include <folly/io/IOBuf.h>
#include <provider/FileSystem.h>
#include <databaseBean/DatabaseBeanFile.h>

class UploadRequest : public UploadRequestBean
{
public:
    using UploadRequestBean::UploadRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        std::unique_ptr<Provider> provider = std::make_unique<FileSystem>(ctx->userBean.filesystemConnectionString);

        FileBean file;
        file.meta_hash = meta_hash;
        file.filename = filename;
        file.mimetype = mimetype;
        file.file_hash = DatabaseBean<FileBean>::insert(ctx->db.get(), file);

        int filesize = provider->accept_base(meta_hash, *file.file_hash, filename, filebase);

        file.filesize = filesize;
        DatabaseBean<FileBean>::update(ctx->db.get(), file);


        /*
        std::cout << filename << std::endl;
        std::cout << filebase << std::endl;
        std::cout << qs::Base64::decode(filebase) << std::endl;
        */


        MessageWithHashBean messageBean;

        messageBean.message = "OK";
        messageBean.hash = *file.file_hash;
        return messageBean;
    }
};

#endif
