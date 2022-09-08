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

# Translate
$OPTIMIZER -i $RESOURCE_DIR/ergo.xml \
           -o $TEST_OUTPUT_DIR/optimized.xml \
           -k Battery1

cd $TEST_OUTPUT_DIR \
    && grep -q "^<Function name=\"Battery1\".*language=\"SDL\" default_implementation=\"default\"" optimized.xml \
    && grep -q "^<Function name=\"FDIR\".*language=\"GUI\" default_implementation=\"environment\"" optimized.xml \
    && grep -q "<Source func_name=\"Battery1\".*ri_name=\"POWER_LEVEL_FDIR\"" optimized.xml \
    && grep -q "<Target func_name=\"FDIR\".*pi_name=\"POWER_LEVEL_FDIR\"" optimized.xml \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
