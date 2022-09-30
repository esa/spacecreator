#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources/subtyping
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

echo "Running TMC test: ${0##*/}"

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -sub $RESOURCE_DIR/subtypes1.asn \
    -sub $RESOURCE_DIR/subtypes2.asn

cd $TEST_OUTPUT_DIR \
    && $SPIN -a -run -bfspar -n system.pml > system.output \
    && grep -q "errors: 0" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
