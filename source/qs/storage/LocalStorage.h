// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <filesystem>
#include <fstream>

#include <qs/server/RequestContext.h>
#include <qs/storage/Storage.h>


namespace qs {
namespace storage {

class LocalStorage : public storage::Storage
{
public:
    LocalStorage() = default;

    LocalStorage(RequestContext& ctx, std::string path)
            : path(std::move(path))
    {
    }

    std::string getMetaPath(const std::string &metaHash) const
    {
        return path + "/" + std::string(&metaHash[0], 2) + "/" + std::string(&metaHash[2], 2) + "/" + std::string(&metaHash[4], metaHash.size() - 4) + "/";
    }

    std::string getMetaPrefixPath(const std::string &metaHash) const
    {
        return path + "/" + std::string(&metaHash[0], 2) + "/";
    }

    std::string getFilePath(const std::string &metaHash, const std::string &filename) const
    {
        return getMetaPath(metaHash) + filename;
    }

    void init(const std::string &meta) override
    {
        std::string metaPath = getMetaPath(meta);
        std::filesystem::create_directories(metaPath);
    }

    void save(const std::string &metaHash, const std::string &filename, const std::string &filebody) override
    {
        const std::string filePath = getFilePath(metaHash, filename);

        std::ofstream fileStream(filePath, std::ios::binary);
        fileStream << filebody;
        fileStream.close();
    }

    std::string read(const FileBean &fileBean) const override
    {
        const std::string filePath = getFilePath(fileBean.meta_hash, fileBean.filename);

        std::ifstream fileStream(filePath, std::ios::binary);
        std::ostringstream stringStream;
        stringStream << fileStream.rdbuf();
        fileStream.close();
        std::string test = stringStream.str();
        return test;
    }

    void remove(const std::string &metaHash) override
    {
        std::filesystem::remove_all(getMetaPrefixPath(metaHash));
    }

    void remove(const FileBean &fileBean) override
    {
        std::filesystem::remove_all(getFilePath(fileBean.meta_hash, fileBean.filename));
    }

protected:
    std::string path;
};
}
}
