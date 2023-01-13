#!/bin/bash
mkdir -p opcua/open62541/build
pushd opcua/open62541/build
cmake .. -G "Unix Makefiles" \
    -DUA_ARCHITECTURE_POSIX=1 \
    -DUA_ENABLE_AMALGAMATION=ON \
    -DUA_ENABLE_SUBSCRIPTIONS=ON \
    -DUA_ENABLE_NODEMANAGEMENT=ON \
    -DUA_ENABLE_IMMUTABLE_NODES=ON \
    -DUA_ENABLE_MULTITHREADING=ON \
    -DUA_ENABLE_DISCOVERY=ON
make -j
cp "open62541.c" "../.."
cp "open62541.h" "../.."
popd
