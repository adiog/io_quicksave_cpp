// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
#include <stdint.h>

#include <assert.h>
#include <functional>
#include <iostream>

#include "rabbitmq-c-utils.h"


class Queue
{
public:
    template <typename Bean>
    static void consume(std::function<void(Bean)> callback)
    {
        char const *hostname;
        int port, status;
        char const *queuename;

        amqp_socket_t *socket = NULL;
        amqp_connection_state_t conn;

        hostname = FLAGS_IO_QUICKSAVE_MQ_HOST.c_str();
        port = FLAGS_IO_QUICKSAVE_MQ_PORT;
        queuename = "response";

        conn = amqp_new_connection();

        socket = amqp_tcp_socket_new(conn);
        if (!socket)
        {
            die("creating TCP socket");
        }

        status = amqp_socket_open(socket, hostname, port);
        if (status)
        {
            die("opening TCP socket");
        }

        die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"),
                          "Logging in");
        amqp_channel_open(conn, 1);
        die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

        amqp_basic_consume(conn, 1, amqp_cstring_bytes(queuename), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
        die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

        {
            for (;;)
            {
                amqp_rpc_reply_t res;
                amqp_envelope_t envelope;

                amqp_maybe_release_buffers(conn);

                res = amqp_consume_message(conn, &envelope, NULL, 0);

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
                amqp_basic_ack(conn, 1, envelope.delivery_tag, 0);

                amqp_destroy_envelope(&envelope);
            }
        }

        die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
        die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
        die_on_error(amqp_destroy_connection(conn), "Ending connection");
    }
};
