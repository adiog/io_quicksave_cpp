// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <storage/StorageProvider.h>
#include <storage/LocalStorage.h>
#include <databaseBean/DatabaseBeanKey.h>
#include <util/split>


namespace storage {

class Sshfs : public storage::LocalStorage
{
public:
    Sshfs(RequestContext &ctx, std::string connectionString) {

        auto splitConnectionString = split(connectionString, ' ');

        std::string host = FLAGS_STORAGE_DEFAULT_HOST;
        std::string port = FLAGS_STORAGE_DEFAULT_PORT;
        std::string keyName = FLAGS_STORAGE_DEFAULT_KEY;
        std::string user = "";
        std::string path = "/dev/null";

        for(auto arg : splitConnectionString)
        {
            auto splitArg = split(arg, '=');

            if (splitArg[0] == "host")
            {
                host = splitArg[1];
            }

            if (splitArg[0] == "port")
            {
                port = splitArg[1];
            }

            if (splitArg[0] == "key")
            {
                keyName = splitArg[1];
            }

            if (splitArg[0] == "user")
            {
                user = splitArg[1];
            }

            if (splitArg[0] == "path")
            {
                path = splitArg[1];
            }
        }

        std::string keyPath = Format::format("/keys/%s_id_rsa", ctx.userBean.user_hash->c_str());
        std::string sshPath = Format::format("/sshfs/%s", ctx.userBean.user_hash->c_str());

        auto keys = DatabaseBean<KeyBean>::get_by(ctx.databaseTransaction, "user_hash", *ctx.userBean.user_hash);
        for(auto &key : keys)
        {
            if (key.name == keyName)
            {
                std::ofstream fileStream(keyPath, std::ios::binary);
                fileStream << key.value << '\n';
                fileStream.close();

                std::string chmod = Format::format(std::string("chmod 600 %s"), keyPath.c_str());
                std::system(chmod.c_str());

                break;
            }
        }

        try {
            std::string check_exist = Format::format(std::string("test -d %s"), sshPath.c_str());
            auto check = std::system(check_exist.c_str());
            if (!check) {
                std::string mkdir = Format::format(std::string("mkdir -p %s"), sshPath.c_str());
                std::system(mkdir.c_str());
            }
        } catch(...)
        {}

        try {
            std::string check_mount = Format::format(std::string("mountpoint -q %s"), sshPath.c_str());
            auto check = std::system(check_mount.c_str());
            if (!check) {
                std::string command = Format::format(
                        std::string("sshfs -o IdentityFile=%s -o idmap=user -p %s %s@%s:%s %s"), keyPath.c_str(),
                        port.c_str(), user.c_str(), host.c_str(), path.c_str(), sshPath.c_str());
                std::cout << command << std::endl;
                std::system(command.c_str());
            }
        } catch (...)
        {}

        this->path = sshPath;
    }
};
}
