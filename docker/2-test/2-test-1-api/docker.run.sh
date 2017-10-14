#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`/sbin/ip route|awk '/default/ { print  $3}'`

sudo docker rm api_quicksave_docker
sudo docker run \
    --privileged \
    --add-host mem.quicksave.io:${HOSTIP} \
    --add-host mq.quicksave.io:${HOSTIP} \
        --name api_quicksave_docker \
        --net network_quicksave_docker \
        -p 11000:11000 \
        -it quicksave/test-api
