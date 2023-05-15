#! /usr/bin/bash

set -e

mkdir -p bld
pushd bld
# generate make files
cmake -G"MSYS Makefiles" ..
# run make files to compile programs
make
# test programs
./main
popd

