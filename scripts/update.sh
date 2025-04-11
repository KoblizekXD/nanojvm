#!/bin/bash

git pull --recurse-submodules
git submodule update --recursive --remote

cd ./lib/classparse
make debug

if ! make install; then
    echo "make install failed, please run it manually, as you might need to use sudo!"
fi

