#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
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
    && ./system.out -E -n -a -e -c1 -m1000000 > system.output \
    && grep -q "errors: 0" system.output \
    && ! grep -q "Function_1_Event_msg_in_pi_2_p1" dataview.pml \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
    
