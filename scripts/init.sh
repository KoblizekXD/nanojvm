#!/bin/bash
set -e
git submodule update --init

if [ -d "./lib/" ]; then
    cd ./lib/classparse
    make dev
else
    echo "Please run this script on the project root!"
    exit 1
fi
