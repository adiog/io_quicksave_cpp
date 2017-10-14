#!/usr/bin/env bash

cat Dockerfile.in | envsubst > Dockerfile
sudo docker build --no-cache -t quicksave/test-storage .
rm Dockerfile

