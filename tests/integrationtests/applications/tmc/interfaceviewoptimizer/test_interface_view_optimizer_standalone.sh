#!/bin/bash

set -euo pipefail

OPTIMIZER=$SPACECREATOR_BUILD_DIR/bin/tmcivoptimizer
SPIN=spin
CC=gcc

TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources

echo "Running TMC test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Optimize
$OPTIMIZER -i $RESOURCE_DIR/interfaceview.xml -o $TEST_OUTPUT_DIR/optimized.xml -e Pinger \

cd $TEST_OUTPUT_DIR \
    && grep -q "^<Function name=\"Pinger\".*language=\"GUI\" default_implementation=\"environment\"" optimized.xml \
    && grep -q "^<Function name=\"Ponger\".*language=\"SDL\" default_implementation=\"default\"" optimized.xml \
    && grep -qv "pingerTimer" optimized.xml \
    && grep -qv "pingerParam" optimized.xml \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
