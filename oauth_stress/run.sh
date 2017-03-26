#!/bin/bash

cd $(dirname $0)

function run_stress_tests()
{
    locust -f oauth_stress.py --host=https://${IO_QUICKSAVE_OAUTH} --port ${IO_QUICKSAVE_LOCUST_OAUTH_PORT}
}

run_stress_tests
