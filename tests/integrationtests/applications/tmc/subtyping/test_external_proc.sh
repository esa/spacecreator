#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output
RESOURCES_DIR=resources/external_proc
PROPERTIES_DIR=$RESOURCES_DIR/work/modelchecking/properties
SUBTYPES_DIR=$RESOURCES_DIR/work/modelchecking/subtypes

echo "Running TMC test: ${0##*/}"

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCES_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -sub $SUBTYPES_DIR/subtyping.asn \
    -scl $PROPERTIES_DIR/properties/properties.scl

cd $TEST_OUTPUT_DIR \
    && $SPIN -a -run -bfspar -n system.pml > system.output \
    && grep -q "errors: 0" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR


