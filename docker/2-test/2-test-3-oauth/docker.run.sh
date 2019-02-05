#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`ip route get 8.8.8.8 | awk '{print $7; exit}'`

docker rm oauth_quicksave_docker
docker run \
    --privileged \
        --name oauth_quicksave_docker \
        --net network_quicksave_docker \
        --add-host audit.quicksave.io:${HOSTIP} \
        -p 13000:13000 \
        -it quicksave/test-oauth
