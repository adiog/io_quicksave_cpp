// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/auth/Authorization.h>
#include <qs/server/RequestContext.h>
#include <qs/useCase/RetrieveUseCase.h>

#include <qsgen/abi/FileRetrieveRequestBean.h>
#include <qsgen/abi/FileRetrieveResponseBean.h>

#include <qsgen/ORM.h>


namespace qs {

class FileRetrieveRequest : public FileRetrieveRequestBean
{
public:
    using FileRetrieveRequestBean::FileRetrieveRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        FileRetrieveResponseBean responseBean;
        auto files = ORM<FileBean>::getBy(ctx.databaseTransaction, orm::File{}.metaHash, meta_hash);

        if (filename)
        {
            auto filePosition = std::find_if(files.begin(), files.end(), [&](FileBean& f) { return f.filename == filename; });
            if (filePosition != files.end())
            {
                responseBean.files.push_back(std::move(*filePosition));
            }
        }
        else
        {
            responseBean.files = std::move(files);
        }

        if (!files.empty() && isAuthorized(files[0].meta_hash, ctx))
        {
            throw ::qs::server::NotAuthorized("");
        }

        return responseBean;
    }
};
}