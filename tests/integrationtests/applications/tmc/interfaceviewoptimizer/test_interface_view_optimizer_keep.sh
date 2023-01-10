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
$OPTIMIZER -i $RESOURCE_DIR/pizza.xml \
           -o $TEST_OUTPUT_DIR/optimized.xml \
           -k Vendor

cd $TEST_OUTPUT_DIR \
    && grep -q "^<Function.*name=\"Clerk\".*language=\"SDL\" default_implementation=\"default\"" optimized.xml \
    && grep -q "^<Function.*name=\"Chef\".*language=\"SDL\" default_implementation=\"default\"" optimized.xml \
    && grep -q "^<Function.*name=\"Customer\".*language=\"GUI\" default_implementation=\"environment\"" optimized.xml \
    && grep -q "^<Function.*name=\"Delivery_Boy\".*language=\"SDL\" default_implementation=\"default\"" optimized.xml \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
