#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.

wget -O qs-audit https://raw.githubusercontent.com/adiog/io_quicksave_audit-server/master/cli/qs-audit

cat Dockerfile.in | envsubst > Dockerfile
sudo docker build -t quicksave/dev-curl .
rm Dockerfile
rm qs-audit
