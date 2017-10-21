// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/str_split.h>

#include <qs/storage/LocalStorage.h>
#include <qs/storage/StorageProvider.h>


namespace storage {

class Sshfs : public storage::LocalStorage
{
public:
    Sshfs(RequestContext &ctx, const std::string& connectionString)
    {
        auto splitConnectionString = absl::StrSplit(connectionString, ' ');

        std::string host = FLAGS_STORAGE_DEFAULT_HOST;
        std::string port = FLAGS_STORAGE_DEFAULT_PORT;
        std::string keyName = FLAGS_STORAGE_DEFAULT_KEY;
        std::string user = "";
        std::string path = "/dev/null";

        for (auto arg : splitConnectionString)
        {
            std::vector<std::string> splitArg = absl::StrSplit(arg, '=');

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

        std::string keyPath = folly::format("/keys/{}_id_rsa", ctx.userBean.user_hash->c_str()).str();
        std::string sshPath = folly::format("/sshfs/{}", ctx.userBean.user_hash->c_str()).str();

        auto keys = qsgen::orm::ORM<KeyBean>::getBy(ctx.databaseTransaction, qsgen::orm::Key{}.userHash, *ctx.userBean.user_hash);
        for (auto &key : keys)
        {
            if (key.name == keyName)
            {
                std::ofstream fileStream(keyPath, std::ios::binary);
                fileStream << key.value << '\n';
                fileStream.close();

                std::string chmod = folly::format(std::string("chmod 600 {}"), keyPath.c_str()).str();
                auto chmodReturnCode = std::system(chmod.c_str());

                if (chmodReturnCode != 0) {
                    throw(std::runtime_error("chmod failed"));
                }

                break;
            }
        }

        try
        {
            std::string check_exist = folly::format(std::string("test -d {}"), sshPath.c_str()).str();
            auto check = std::system(check_exist.c_str());
            if (!check)
            {
                std::string mkdir = folly::format(std::string("mkdir -p {}"), sshPath.c_str()).str();
                auto mkdirReturnCode = std::system(mkdir.c_str());

                if (mkdirReturnCode != 0) {
                    throw(std::runtime_error("mkdir failed"));
                }
            }
        }
        catch (...) //TODO
        {
        }

        try
        {
            std::string check_mount = folly::format(std::string("mountpoint -q {}"), sshPath.c_str()).str();
            auto check = std::system(check_mount.c_str());
            if (!check)
            {
                std::string command = folly::format(
                    std::string("sshfs -o IdentityFile={} -o idmap=user -p {} {}@{}:{} {}"), keyPath.c_str(), port.c_str(), user.c_str(), host.c_str(), path.c_str(), sshPath.c_str()).str();
                std::cout << command << std::endl;
                auto sshfsReturnCode = std::system(command.c_str());

                if (sshfsReturnCode != 0) {
                    throw(std::runtime_error("sshfs failed"));
                }
            }
        }
        catch (...)
        {
        }

        this->path = sshPath;
    }
};
}
