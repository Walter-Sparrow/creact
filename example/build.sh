#!/usr/bin/env bash

emcc app.c ../src/creact.c \
    -o main.js \
    -s WASM=1 \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -O3
