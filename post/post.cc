// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <env.h>

#include <bean/DatabaseTaskBean.h>
#include <mq/queue.h>
#include <thread>
#include <hash>
#include <zconf.h>

#include <database/Action.h>
#include <databaseBean/DatabaseBeans.h>
#include <database/ProviderFactory.h>


template<typename DB, typename Bean>
void insert(DB *db, Bean bean)
{
    database::Action::insert(db, bean);
}

template<typename DB, typename Bean>
void update(DB *db, Bean bean)
{
    database::Action::insert(db, bean);
}

template<typename DB, typename Bean>
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
    try {
        std::cout << databaseTaskBean.to_string() << std::endl;
        auto databaseConnection = database::ProviderFactory::create(databaseTaskBean.databaseConnectionString);
        auto databaseTransaction = databaseConnection->getTransaction();

        if (databaseTaskBean.beanname == "File") {
            FileBean file(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseTransaction.get(), file);
        } else if (databaseTaskBean.beanname == "Tag") {
            TagBean tag(databaseTaskBean.beanjson.c_str());
            operation(databaseTaskBean.type, databaseTransaction.get(), tag);
        }
    }
    catch(...)
    {}
}

void engine_thread()
{
    Queue::consume<DatabaseTaskBean>(consumeBean);
}

int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << FLAGS_post_threads << std::endl;

    long int threads = FLAGS_post_threads;
    if (threads <= 0)
    {
        threads = sysconf(_SC_NPROCESSORS_ONLN);
        CHECK(threads > 0);
    }

    std::vector<std::thread> workers(threads);

    for(auto &thread : workers){
        thread = std::thread(engine_thread);
    }

    for(auto &thread : workers) {
        thread.join();
    }

    return 0;
}
