#!/usr/bin/env bash

em++ app.cpp ../src/creact.cpp ../src/creact_callback.cpp \
    -std=c++20 \
    -o main.js \
    -s EXPORTED_FUNCTIONS='["_main", "_callRegisteredCallback"]' \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    --bind \
    -O3
