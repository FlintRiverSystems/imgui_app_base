#!/bin/bash

if [ -e build.wasm ]; then
	rm -rf build.wasm
fi

mkdir build.wasm

cp -f index.html build.wasm

cd build.wasm

export EMSDK=$FRSTPSRC/emsdk

cmake \
	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
	-DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
	-DWASM=TRUE \
	..

make

