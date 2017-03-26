#!/bin/bash

cd $(dirname $0)

function run_stress_tests()
{
    locust -f api_stress.py --host=https://${IO_QUICKSAVE_API} --port ${IO_QUICKSAVE_LOCUST_API_PORT}
}

run_stress_tests
