#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`/sbin/ip route|awk '/default/ { print  $3}'`

sudo docker rm oauth_quicksave_docker
sudo docker run \
    --privileged \
    --add-host mem.quicksave.io:${HOSTIP} \
    --add-host mq.quicksave.io:${HOSTIP} \
        --name oauth_quicksave_docker \
        --net network_quicksave_docker \
        -p 13000:13000 \
        -it quicksave/test-oauth
