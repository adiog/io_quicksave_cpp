// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <env.h>

#include <reference_cast>

#include <zconf.h>
#include <thread>

#include <qs/database/ProviderFactory.h>
#include <qs/mq/queue.h>
#include <qs/server/Config.h>
#include <qs/util/hash.h>

#include <qsgen/bean/DatabaseTaskBean.h>
#include <qsgen/bean/FileBean.h>
#include <qsgen/bean/MetaBean.h>
#include <qsgen/bean/TagBean.h>

#include <qsgen/orm/sqlppWrappers.h>


template <typename DB, typename Bean>
void insert(DB &db, Bean& bean)
{
    qsgen::orm::ORM<Bean>::insert(db, bean);
}

template <typename DB, typename Bean>
void update(DB &db, Bean& bean)
{
    qsgen::orm::ORM<Bean>::update(db, bean);
}

template <typename DB, typename Bean>
void operation(const std::string& operationName, DB &db, Bean bean)
{
    if (operationName == "insert")
    {
        insert(db, bean);
    }
    else if (operationName == "update")
    {
        update(db, bean);
    }
    else
    {
        throw std::runtime_error("");
    }
};


void consumeBean(DatabaseTaskBean databaseTaskBean)
{
    try
    {
        std::cout << databaseTaskBean.to_string() << std::endl;
        auto databaseConnectionOwner = qs::database::ProviderFactory::create(databaseTaskBean.databaseConnectionString);
        auto& databaseConnection = reference_cast(databaseConnectionOwner);

        if (databaseTaskBean.beanname == "Meta")
        {
            MetaBean meta(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseConnection, meta);
        }
        else if (databaseTaskBean.beanname == "File")
        {
            FileBean file(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseConnection, file);
        }
        else if (databaseTaskBean.beanname == "Tag")
        {
            TagBean tag(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseConnection, tag);
        }
        else
        {
            std::cout << "ERROR: beanname not supported " << databaseTaskBean.beanname << std::endl;
        }
    }
    catch (...)
    {
    }
}

void engine_thread()
{
    Queue().consume<DatabaseTaskBean>(consumeBean);
}

int main(int argc, char *argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    auto postThreads = qs::server::Config::getThreads(FLAGS_post_threads);

    std::vector<std::thread> workers(postThreads);

    for (auto &thread : workers)
    {
        thread = std::thread(engine_thread);
    }

    for (auto &thread : workers)
    {
        thread.join();
    }

    return 0;
}
