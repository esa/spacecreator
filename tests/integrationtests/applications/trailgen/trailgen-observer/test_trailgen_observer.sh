#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
TRAILGEN=$SPACECREATOR_BUILD_DIR/bin/trailgen
SPIN=spin
CC=gcc
ASN1SCC=asn1scc

DIFF="diff -w -B"
TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

$TMC -iv $RESOURCE_DIR/interfaceview.xml \
	 -o $TEST_OUTPUT_DIR \
	 -os $PROPERTIES_DIR/observer/observer.pr

$ASN1SCC -uPER -typePrefix asn1Scc -renamePolicy 3 -c -o "${TEST_OUTPUT_DIR}" "${RESOURCE_DIR}/work/dataview/dataview-uniq.asn"

cd $TEST_OUTPUT_DIR \
	&& $SPIN -a system.pml \
	&& $CC -o system.out *.c \
	&& ./system.out > system.output \
	&& grep -q "errors: 1" system.output \
	&& ./system.out -S -n -r system.pml.trail > scenario.spt \
	&& cd .. \
	&& $TRAILGEN -iv $RESOURCE_DIR/interfaceview.xml -os $PROPERTIES_DIR/observer/observer.pr -o $TEST_OUTPUT_DIR -ot $TEST_OUTPUT_DIR/scenario.sim $TEST_OUTPUT_DIR/scenario.spt \
	&& grep -v  'unhandled-input' $TEST_OUTPUT_DIR/scenario.sim | $DIFF - $RESOURCE_DIR/scenario.sim \
	&& rm -r $TEST_OUTPUT_DIR
