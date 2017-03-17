//
// Created by adiog on 16.03.17.
//

#ifndef QUICKSAVE_PROVIDERFILESYSTEM_H
#define QUICKSAVE_PROVIDERFILESYSTEM_H

#include <provider/Provider.h>

class FileSystem : public Provider
{
public:
    FileSystem(std::string path) : path(path) {}


    void accept(std::string & filename, std::string file) const override
    {
        std::ofstream filestream{path + "/" + filename};
        filestream << file;
        filestream.close();
    }

private:
    std::string path;
};
#endif //QUICKSAVE_PROVIDERFILESYSTEM_H
