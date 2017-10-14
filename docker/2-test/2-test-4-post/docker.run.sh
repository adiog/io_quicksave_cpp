#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`/sbin/ip route|awk '/default/ { print  $3}'`

sudo docker rm post_quicksave_docker
sudo docker run \
    --privileged \
        --name post_quicksave_docker \
        --net network_quicksave_docker \
        -it quicksave/test-post
