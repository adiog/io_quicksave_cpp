// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <env.h>

#include <bean/DoneTaskBean.h>
#include <mq/queue.h>
#include <thread>

void consumeBean(DoneTaskBean bean)
{
    std::cout << "Got callback from " << bean.name << std::endl;
}

void engine_thread()
{
    Queue::consume<DoneTaskBean>(consumeBean);
}

int main()
{
    long int threads = FLAGS_api_threads;
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