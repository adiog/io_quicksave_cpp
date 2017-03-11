// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <proxygen/httpserver/ResponseHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

void GenericJsonResponse(proxygen::ResponseHandler* responseHandler, int statusCode, std::unique_ptr<folly::IOBuf> response)
{
    proxygen::ResponseBuilder(responseHandler)
            .status(statusCode, "OK")
            .header("Content-Type: application/json",
                    folly::to<std::string>(""))
            .body(std::move(response))
            .sendWithEOM();
}

template<int status_code>
void JsonResponse(proxygen::ResponseHandler* responseHandler)
{

}

template<>
void JsonResponse<200>(proxygen::ResponseHandler* responseHandler)
{

}

template<>
void JsonResponse<400>(proxygen::ResponseHandler* responseHandler)
{
    auto response = folly::IOBuf::copyBuffer("{\"message\": \"bad request\"}");
    GenericJsonResponse(responseHandler, 400, std::move(response));
}

template<>
void JsonResponse<401>(proxygen::ResponseHandler* responseHandler)
{
    auto response = folly::IOBuf::copyBuffer("{\"message\": \"not authenticated\"}");
    GenericJsonResponse(responseHandler, 401, std::move(response));
}

template<>
void JsonResponse<404>(proxygen::ResponseHandler* responseHandler)
{
    auto response = folly::IOBuf::copyBuffer("{\"message\": \"not found\"}");
    GenericJsonResponse(responseHandler, 404, std::move(response));
}

