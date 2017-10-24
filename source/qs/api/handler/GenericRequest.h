// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>


namespace qs {

template <typename Request>
class GenericRequest
{
public:
    static std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx, rapidjson::Document& document)
    {
        Request request = Request(document);
        return request.handle(ctx);
    }
};
}
