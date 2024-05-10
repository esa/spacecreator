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
$OPTIMIZER -i $RESOURCE_DIR/pus_model.xml \
           -o $TEST_OUTPUT_DIR/optimized.xml \
		   -k alpha_pus17_perform_an_are_you_alive_connection_test

cd $TEST_OUTPUT_DIR \
    && grep -q "^<Function.*name=\"alpha_pus17_perform_an_are_you_alive_connection_test\".*language=\"SDL\" default_implementation=\"default\"" optimized.xml \
    && grep -q "^<Function.*name=\"alpha_telemetry_packer\".*language=\"GUI\" default_implementation=\"environment\"" optimized.xml \
    && grep -q "^<Function.*name=\"alpha_pus1_execution_reporting\".*language=\"GUI\" default_implementation=\"environment\"" optimized.xml \
	&& grep -q "^<Function.*name=\"alpha_capability_router\".*language=\"GUI\" default_implementation=\"environment\"" optimized.xml \
	&& grep -q "^<Provided_Interface.*name=\"perform_an_are_you_alive_connection_test\"" optimized.xml \
	&& grep -q "^<Required_Interface.*name=\"successful_start_of_execution\"" optimized.xml \
	&& grep -q "^<Required_Interface.*name=\"successful_completion_of_execution\"" optimized.xml \
	&& grep -q "^<Required_Interface.*name=\"send_report\"" optimized.xml \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
