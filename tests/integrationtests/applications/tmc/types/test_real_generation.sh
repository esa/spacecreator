#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=n7s-spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_real_generation
RESOURCE_DIR=resources/real-generation
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

echo "Running TMC test: ${0##*/}"

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -delta 1.4 \
    -enable-reals

cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -o system.out pan.c \
    && ./system.out -m1000000 -a -n > system.output \
    && grep -q "errors: 0" system.output \
    && grep -q "select (MyReal_tmp : 0 .. 14);" env_inlines.pml \
    && grep -q "MyReal_gv = (-10 + (MyReal_tmp * 1.4));" env_inlines.pml \
    && cd .. \
    && rm -rf $TEST_OUTPUT_DIR

mkdir $TEST_OUTPUT_DIR
