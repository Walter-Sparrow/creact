#!/usr/bin/env bash

em++ app.cpp ../src/creact.cpp \
    -o main.js \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    --bind \
    -O3
