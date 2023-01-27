#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

TEST_OUTPUT_DIR=output
RESOURCES_DIR=resources/cleandata
PROPERTIES_DIR=$RESOURCES_DIR/work/modelchecking/properties

echo "Running TMC test: ${0##*/}"

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCES_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR

cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -DVECTORSZ=2048 -o system.out pan.c \
    && ! grep -q "typedef System_State" dataview.pml \
    && ! grep -q "nothing_init" dataview.pml \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
    
