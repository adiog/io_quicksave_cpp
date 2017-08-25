// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_UPLOADREQUEST_H
#define QUICKSAVE_UPLOADREQUEST_H

#include <bean/UploadRequestBean.h>
#include <bean/MessageBean.h>
#include <folly/io/IOBuf.h>
#include <storage/StorageFactory.h>
#include <databaseBean/DatabaseBeans.h>

class UploadRequest : public UploadRequestBean
{
public:
    using UploadRequestBean::UploadRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);

        const std::string filebody = qs::Base64::decode(filebase);

        storage->save(meta_hash, filename, filebody);

        FileBean file;
        file.meta_hash = meta_hash;
        file.filename = filename;
        file.mimetype = mimetype;
        file.filesize = filebody.size();

        file.file_hash = database::Action::insert<FileBean>(ctx.databaseTransaction, file);

        MessageWithHashBean messageBean;

        messageBean.message = "OK";
        messageBean.hash = *file.file_hash;
        return messageBean;
    }
};

#endif
