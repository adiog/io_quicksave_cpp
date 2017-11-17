// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/str_join.h>

#include <folly/io/IOBuf.h>

#include <qs/auth/Authorization.h>
#include <qs/server/Exception.h>
#include <qs/server/RequestContext.h>
#include <qs/storage/StorageFactory.h>

#include <qsgen/ORM.h>
#include <qsgen/abi/FileUpdateRequestBean.h>
#include <qsgen/abi/MessageBean.h>


namespace qs {

class FileUpdateRequest : public FileUpdateRequestBean
{
public:
    using FileUpdateRequestBean::FileUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        MessageBean messageBean;

        auto updated_file = ORM<FileBean>::get(ctx.databaseTransaction, *file.file_hash);

        if (updated_file)
        {
            if (!isAuthorized(file.meta_hash, ctx) || !isAuthorized(updated_file->meta_hash, ctx))
            {
                throw ::qs::server::NotAuthorized("");
            }
            if (updated_file->filename != file.filename)
            {
                auto storage = ::qs::storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
                storage->move(*updated_file, file);
            }
            updated_file->update(file);
            ORM<FileBean>::update(ctx.databaseTransaction, *updated_file);
            messageBean.message = "OK";
        }
        else
        {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};
}