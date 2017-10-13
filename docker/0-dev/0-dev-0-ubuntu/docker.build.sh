#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

cat Dockerfile.in | envsubst > Dockerfile
sudo docker build -t quicksave/dev-ubuntu .
rm Dockerfile

