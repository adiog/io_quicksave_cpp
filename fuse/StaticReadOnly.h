// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <iostream>
#include <reference_cast>

#include <regex>

#include <FS.h>
#include <qs/oauth/OAuthAPI.h>
#include <qs/oauth/OAuthMasterDatabase.h>
#include <qs/server/RequestContext.h>
#include <qs/storage/Storage.h>
#include <qs/storage/StorageFactory.h>
#include <qs/useCase/RetrieveUseCase.h>
#include <qsgen/abi/RetrieveResponseBean.h>
#include <qsgen/abi/UserBean.h>


class StaticReadOnly
{
public:
    static void build(FS &fs, const std::string &username, const std::string &password, const std::string &query)
    {
        std::pair<std::string, std::string> credentials{username, password};
        absl::optional<qs::UserBean> anOptionalUserBean = qs::OAuthMasterDatabase::authenticateWithPassword(credentials);
        if (!anOptionalUserBean)
        {
            throw std::runtime_error("");
        }

        auto databaseConnectionOwner = qs::database::ProviderFactory::create(anOptionalUserBean->databaseConnectionString);
        auto &databaseConnection = reference_cast(databaseConnectionOwner);

        qs::RequestContext mockRequestContext{
            *anOptionalUserBean,
            "",
            databaseConnection};
        std::unique_ptr<qs::storage::Storage> storage = qs::storage::StorageFactory::create(mockRequestContext, anOptionalUserBean->storageConnectionString);


        qs::RetrieveResponseBean retrieveResponseBean = qs::useCase::Retrieve::getBean(databaseConnection, query, *anOptionalUserBean->user_hash, 1000, 0);

        std::cout << ::serialize(retrieveResponseBean) << std::endl;

        std::multiset<std::string> names;
        std::set<std::string> tags;

        const std::regex allowed_chars("[^a-zA-Z0-9\\.]");
        for (const auto &item : retrieveResponseBean.items)
        {
            auto sanitized_name = std::regex_replace(*item.meta.name, allowed_chars, "_");
            if (sanitized_name == "")
            {
                sanitized_name = "noname";
            }
            size_t count = names.count(sanitized_name);
            names.insert(sanitized_name);
            if (count != 0)
            {
                sanitized_name += std::string(" (") + std::to_string(count) + std::string(")");
                names.insert(sanitized_name);
            }
            std::string item_path = std::string("/") + sanitized_name;
            std::cout << item_path << std::endl;
            fs.make_dir(item_path);
            fs.make_file(item_path, "item.json", ::prettySerialize(item));
            if (!item.files.empty())
            {
                std::string files_path = std::string("/") + sanitized_name + std::string("/files");
                fs.make_dir(files_path);
                for (auto &file : item.files)
                {
                    std::string filebody = storage->read(file);
                    fs.make_file(files_path, file.filename, filebody);
                }
            }
            for (auto &tag : item.tags)
            {
                auto sanitized_tag = std::regex_replace(*tag.name, allowed_chars, "_");
                size_t count_tags = tags.count(sanitized_tag);
                std::string tag_dir = std::string("/BY_TAG/") + sanitized_tag;
                if (count_tags == 0)
                {
                    fs.make_dir(tag_dir);
                    tags.insert(sanitized_tag);
                }
                fs.make_link(tag_dir, sanitized_name, std::string("../..") + item_path);
            }
        }
    }
};
