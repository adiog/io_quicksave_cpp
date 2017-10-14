#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`/sbin/ip route|awk '/default/ { print  $3}'`

sudo docker rm cdn_quicksave_docker
sudo docker run \
    --privileged \
    --add-host mem.quicksave.io:${HOSTIP} \
    --add-host mq.quicksave.io:${HOSTIP} \
        --name cdn_quicksave_docker \
        --net network_quicksave_docker \
        -p 12000:12000 \
        -it quicksave/test-cdn
