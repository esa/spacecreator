#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
TRAILGEN=$SPACECREATOR_BUILD_DIR/bin/trailgen
SPIN=spin
CC=gcc

DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_trailgen_observer
RESOURCE_DIR=resources
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

$TMC -iv $RESOURCE_DIR/interfaceview.xml \
	 -o $TEST_OUTPUT_DIR \
	 -os $PROPERTIES_DIR/observer/observer.pr

cd $TEST_OUTPUT_DIR \
	&& $SPIN -a system.pml \
	&& $CC -o system.out pan.c \
	&& ./system.out > system.output \
	&& grep -q "errors: 1" system.output \
	&& spin -t -r -s system.pml > scenario.spt \
	&& cd .. \
	&& $TRAILGEN -iv $RESOURCE_DIR/interfaceview.xml -os $PROPERTIES_DIR/observer/observer.pr -o $TEST_OUTPUT_DIR -ot $TEST_OUTPUT_DIR/scenario.sim $TEST_OUTPUT_DIR/scenario.spt \
	&& $DIFF $TEST_OUTPUT_DIR/scenario.sim $RESOURCE_DIR/scenario.sim \
	&& rm -r $TEST_OUTPUT_DIR
