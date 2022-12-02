#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_msc_when_then_sequence_failure
RESOURCE_DIR=resources/MscWhenThenSequenceFailure
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

echo "Running TMC test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -mos $PROPERTIES_DIR/MscObserver/MscObserver.msc \
    -ivl 1

cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -o system.out pan.c \
    && ./system.out -m1000000 -a -n -E > system.output \
    && grep -q "errors: 1" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
