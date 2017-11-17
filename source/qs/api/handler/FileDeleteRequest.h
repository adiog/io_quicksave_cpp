// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/auth/Authorization.h>

#include <qs/storage/StorageFactory.h>
#include <qsgen/abi/FileDeleteRequestBean.h>
#include <qsgen/abi/MessageBean.h>
#include <qs/server/Exception.h>


namespace qs {

class FileDeleteRequest : public FileDeleteRequestBean
{
public:
    using FileDeleteRequestBean::FileDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        MessageBean messageBean;

        auto file = ORM<FileBean>::get(ctx.databaseTransaction, file_hash);
        if (file)
        {
            if (isAuthorized(file->meta_hash, ctx))
            {
                throw ::qs::server::NotAuthorized("");
            }

            auto storage = ::qs::storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
            storage->remove(*file);
            ORM<FileBean>::remove(ctx.databaseTransaction, file_hash);
        }

        messageBean.message = "OK";

        return messageBean;
    }
};
}