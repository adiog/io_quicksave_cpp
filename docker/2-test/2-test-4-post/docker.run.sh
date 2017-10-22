#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

HOSTIP=`ip route get 8.8.8.8 | awk '{print $NF; exit}'`

sudo docker rm post_quicksave_docker
sudo docker run \
    --privileged \
        --name post_quicksave_docker \
        --net network_quicksave_docker \
        --add-host audit.quicksave.io:${HOSTIP} \
        -it quicksave/test-post
