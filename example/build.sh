#!/usr/bin/env bash

em++ app.cpp ../src/creact.cpp \
    -o main.js \
    -s WASM=1 \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -O3
