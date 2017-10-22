#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`/sbin/ip route|awk '/default/ { print  $3}'`

sudo docker rm oauth_quicksave_docker
sudo docker run \
    --privileged \
        --name oauth_quicksave_docker \
        --net network_quicksave_docker \
        --add-host audit.quicksave.io:192.168.0.1 \
        -p 13000:13000 \
        -it quicksave/test-oauth
