// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <style>
#include <qs/storage/Storage.h>
#include <qs/server/RequestContext.h>


namespace storage {

class StorageProvider
{
public:
    virtual bool validate(const std::string& connectionString) const abstract;
    virtual std::unique_ptr<storage::Storage> accept(RequestContext &ctx, const std::string& connectionString) const abstract;
};
}