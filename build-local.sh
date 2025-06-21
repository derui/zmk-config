#!/usr/bin/env bash

BASE_DIR=/tmp/zmk-config

if [ -d "$BASE_DIR" ]; then
    echo "Removing existing directory: $BASE_DIR"
    #rm -rf $BASE_DIR
else
    echo "Creating new directory: $BASE_DIR"
fi

mkdir -p $BASE_DIR/config

cp -R config/* $BASE_DIR/config/

cd $BASE_DIR
if [ ! -d "$BASE_DIR/.west" ]; then
    west init -l $BASE_DIR/config
    west update
fi

west zephyr-export

west build -s zmk/app -p -d build -b nice_nano_v2 -- \
     -DSHIELD=orbit_one \
     -DZMK_EXTRA_MODULES='/workspaces/zmk-config' \
     -DZMK_CONFIG=$BASE_DIR/config

