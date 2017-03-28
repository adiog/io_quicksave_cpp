// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <filesystem>
#include <provider/Provider.h>

class FileSystem : public Provider
{
public:
    FileSystem(std::string path) : path(path) {}

    int accept(std::string& meta_hash, std::string &file_hash, std::string & filename, std::string file) const override
    {
        std::string basedir = path + "/" + meta_hash + "/" + file_hash + "/";
        std::filesystem::create_directories(basedir);

        auto filestream = std::ofstream(basedir + filename, std::ios::binary);
        filestream << file;
        filestream.close();

        return file.size();
    }

private:
    std::string path;
};
