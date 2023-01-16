#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output
RESOURCES_DIR=resources/choice_present
PROPERTIES_DIR=$RESOURCES_DIR/work/modelchecking/properties
SUBTYPES_DIR=$RESOURCES_DIR/work/modelchecking/subtypes

echo "Running TMC test: ${0##*/}"

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCES_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -sub $SUBTYPES_DIR/new-subtypes.asn \
    -scl $PROPERTIES_DIR/scl/scl.scl

cd $TEST_OUTPUT_DIR \
    && $SPIN -a -n system.pml \
    && $CC -DVECTORSZ=65536 -o system.out pan.c \
    && ./system.out -E -n -a -e -c1 -m1000000 > system.output \
    && grep -q "errors: 0" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
