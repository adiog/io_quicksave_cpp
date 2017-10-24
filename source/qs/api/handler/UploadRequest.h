// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/storage/StorageFactory.h>
#include <qs/util/base64.h>
#include <qsgen/abi/MessageBean.h>
#include <qsgen/abi/UploadRequestBean.h>


namespace qs {

class UploadRequest : public UploadRequestBean
{
public:
    using UploadRequestBean::UploadRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx,
                                                                                    ctx.userBean.storageConnectionString);

        const std::string filebody = qs::util::Base64::decode(filebase);

        storage->save(meta_hash, filename, filebody);

        FileBean file;
        file.meta_hash = meta_hash;
        file.filename = filename;
        file.mimetype = mimetype;
        file.filesize = filebody.size();

        file.file_hash = ORM<FileBean>::insert(ctx.databaseTransaction, file);

        MessageWithHashBean messageBean;

        messageBean.message = "OK";
        messageBean.hash = *file.file_hash;
        return messageBean;
    }
};
}
