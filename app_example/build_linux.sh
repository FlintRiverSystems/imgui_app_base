#!/bin/bash

if [ -e build.linux ]; then
	rm -rf build.linux
fi

mkdir build.linux

cd build.linux

cmake  ..

make

