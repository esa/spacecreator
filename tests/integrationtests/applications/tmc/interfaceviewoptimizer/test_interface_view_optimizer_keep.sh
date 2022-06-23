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
# gdb --args \
$OPTIMIZER -i $RESOURCE_DIR/pizza.xml \
           -o $TEST_OUTPUT_DIR/optimized.xml \
           -k Customer \
           -k Clerk

# cd $TEST_OUTPUT_DIR \
#     && cd .. \
#     && rm -r $TEST_OUTPUT_DIR
