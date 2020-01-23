#!/usr/bin/env bash

branch_name=${1:-"master"}

if ! python -c "import pystospa" &> /dev/null; then
    if [[ ! -d StoSpa2 ]]; then
        git clone --recursive https://github.com/BartoszBartmanski/StoSpa2.git --branch ${branch_name}
    fi
    cd StoSpa2
    git checkout ${branch_name}
    git pull
    pip install dev-requirements.txt
    python setup.py install
fi
