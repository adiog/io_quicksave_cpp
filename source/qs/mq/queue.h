// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>

#include <functional>
#include <iostream>

#include "rabbitmq-c-utils.h"

namespace qs {

class Queue
{
public:
    amqp_socket_t *socket;
    amqp_connection_state_t connectionState;

    Queue()
    {
        char const *hostname = FLAGS_IO_QUICKSAVE_MQ_HOST.c_str();
        int port = FLAGS_IO_QUICKSAVE_MQ_PORT;
        int status;
        char const *queueName = "response";

        socket = nullptr;
        connectionState = amqp_new_connection();

        socket = amqp_tcp_socket_new(connectionState);
        if (!socket)
        {
            die("creating TCP socket");
        }

        status = amqp_socket_open(socket, hostname, port);
        if (status)
        {
            die("opening TCP socket");
        }

        die_on_amqp_error(amqp_login(connectionState, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"),
                          "Logging in");
        amqp_channel_open(connectionState, 1);
        die_on_amqp_error(amqp_get_rpc_reply(connectionState), "Opening channel");

        amqp_basic_consume(connectionState, 1, amqp_cstring_bytes(queueName), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
        die_on_amqp_error(amqp_get_rpc_reply(connectionState), "Consuming");
    }

    ~Queue()
    {
        die_on_amqp_error(amqp_channel_close(connectionState, 1, AMQP_REPLY_SUCCESS), "Closing channel");
        die_on_amqp_error(amqp_connection_close(connectionState, AMQP_REPLY_SUCCESS), "Closing connection");
        die_on_error(amqp_destroy_connection(connectionState), "Ending connection");
    }

    template <typename Bean>
    void consume(std::function<void(Bean)> callback)
    {
        {
            for (;;)
            {
                amqp_rpc_reply_t res;
                amqp_envelope_t envelope;

                amqp_maybe_release_buffers(connectionState);

                res = amqp_consume_message(connectionState, &envelope, nullptr, 0);

                if (AMQP_RESPONSE_NORMAL != res.reply_type)
                {
                    break;
                }

                printf("Delivery %u, exchange %.*s routingkey %.*s\n",
                       (unsigned)envelope.delivery_tag,
                       (int)envelope.exchange.len,
                       (char *)envelope.exchange.bytes,
                       (int)envelope.routing_key.len,
                       (char *)envelope.routing_key.bytes);

                if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG)
                {
                    printf("Content-type: %.*s\n",
                           (int)envelope.message.properties.content_type.len,
                           (char *)envelope.message.properties.content_type.bytes);
                }
                printf("----\n");

                amqp_dump(envelope.message.body.bytes, envelope.message.body.len);


                std::string beanString = std::string{(char *)envelope.message.body.bytes,
                                                     envelope.message.body.len};
                Bean bean;
                try
                {
                    bean = Bean(beanString.c_str());
                }
                catch (...)
                {
                    std::cout << "ACHTUNG" << std::endl;
                }
                callback(bean);
                amqp_basic_ack(connectionState, 1, envelope.delivery_tag, 0);

                amqp_destroy_envelope(&envelope);
            }
        }
    }
};
}