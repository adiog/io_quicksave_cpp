#!/usr/bin/env bash

cat Dockerfile.in | envsubst > Dockerfile
sudo docker build -t quicksave/test-storage .
rm Dockerfile

