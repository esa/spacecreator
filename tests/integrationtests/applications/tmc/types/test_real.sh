#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=/home/taste/Spin/Src/spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources/real
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

echo "Running TMC test: ${0##*/}"

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -scl $PROPERTIES_DIR/StopConditions/Finish.scl \
    -o $TEST_OUTPUT_DIR \
    -enable-reals

cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -o system.out pan.c \
    && ./system.out -m1000000 -a -n > system.output \
    && grep -q "errors: 0" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -scl $PROPERTIES_DIR/StopConditions/Failure.scl \
    -o $TEST_OUTPUT_DIR \
    -enable-reals

cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -o system.out pan.c \
    && ./system.out -m1000000 -a -n > system.output \
    && grep -q "errors: 1" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
