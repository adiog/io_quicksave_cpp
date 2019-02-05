#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

docker rm storage_quicksave_docker
docker run \
    --privileged \
    --name storage_quicksave_docker \
    --net network_quicksave_docker \
    --expose 22 \
    -p 2222:22 \
    -it quicksave/test-storage
