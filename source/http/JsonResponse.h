// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_HTTPRESPONSE_H
#define QUICKSAVE_HTTPRESPONSE_H

void GenericJsonResponse(proxygen::ResponseHandler* responseHandler, int statusCode, std::unique_ptr<folly::IOBuf> response);

template<int status_code>
void JsonResponse(proxygen::ResponseHandler* responseHandler);

/*
class Success(QsJsonResponse):
status_text = 'success'
status_code = 200


class Failed(QsJsonResponse):
status_text = 'failed'
status_code = 200


class BadRequest(QsJsonResponse):
status_text = 'bad request'
status_code = 400


class NotAuthenticated(QsJsonResponse):
status_text = 'not authenticated'
status_code = 401


class NotAuthorized(QsJsonResponse):
status_text = 'not authorized'
status_code = 403


class NotFound(QsJsonResponse):
status_text = 'not found'
status_code = 404


class UnsupportedMediaType(QsJsonResponse):
status_text = 'unsupported media type'
status_code = 415


class InternalServerError(QsJsonResponse):
status_text = 'internal server error'
status_code = 500


class NotImplementedResponse(QsJsonResponse):
status_text = 'not implemented'
status_code = 501
*/

#endif
