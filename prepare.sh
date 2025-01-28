#!/bin/bash

today=$(date  +"%Y%m%d")

source config
new_build=$((BUILD+1))
sed -i "s/^BUILD=.*/BUILD=$new_build/g" config

sed -i "s/define VERSION .*/define VERSION \"$today.$new_build\"/g" config.h
sed -i "s/define BOARD_ID .*/define BOARD_ID \"$BOARD_ID\"/g" config.h
