// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <env.h>

#include <zconf.h>
#include <qs/util/hash.h>
#include <thread>
#include <qs/mq/queue.h>
#include <qsgen/bean/DatabaseTaskBean.h>

#include <qs/database/Action.h>
#include <qs/database/ProviderFactory.h>
#include <qsgen/databaseBean/DatabaseBeans.h>


template <typename DB, typename Bean>
void insert(DB *db, Bean bean)
{
    database::Action::insert(db, bean);
}

template <typename DB, typename Bean>
void update(DB *db, Bean bean)
{
    database::Action::update(db, bean);
}

template <typename DB, typename Bean>
void operation(std::string op, DB *db, Bean bean)
{
    if (op == "insert")
    {
        insert(db, bean);
    }
    else if (op == "update")
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
        auto databaseConnection = database::ProviderFactory::create(databaseTaskBean.databaseConnectionString);
        auto databaseTransaction = databaseConnection->getTransaction();

        if (databaseTaskBean.beanname == "Meta")
        {
            MetaBean meta(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseTransaction.get(), meta);
        }
        else if (databaseTaskBean.beanname == "File")
        {
            FileBean file(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseTransaction.get(), file);
        }
        else if (databaseTaskBean.beanname == "Tag")
        {
            TagBean tag(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseTransaction.get(), tag);
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

    std::cout << FLAGS_post_threads << std::endl;

    int threads = FLAGS_post_threads;
    if (threads <= 0)
    {
        threads = static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
        CHECK(threads > 0);
    }

    std::vector<std::thread> workers(threads);

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
