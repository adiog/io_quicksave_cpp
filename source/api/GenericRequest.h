// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_GENERICREQUEST_H
#define QUICKSAVE_GENERICREQUEST_H

template <typename Request>
class GenericRequest
{
public:
    template<typename RequestContext>
    static std::unique_ptr<folly::IOBuf> handle(RequestContext *ctx, rapidjson::Document &document)
    {
        Request request = Request(document);
        return request.handle(ctx);
    }
};

#endif
