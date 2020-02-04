#!/usr/bin/env bash

branch_name=${1:-"master"}

git checkout ${branch_name}
git pull
git submodule update --recursive
if [ ! -d build ]; then mkdir build; fi
cd build

rm -rf *
cmake ../
make all
