#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_uart_oo
RESOURCE_DIR=resources/uart_protocol_dlc
SUBTYPES_DIR=${RESOURCE_DIR}/work/modelchecking/subtypes
OBSERVERS_DIR=${RESOURCE_DIR}/work/modelchecking/properties/observers
PROPERTIES_DIR=${RESOURCE_DIR}/work/modelchecking/properties

echo "Running TMC test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -ivl 1 \
    -sub ${SUBTYPES_DIR}/subtypes.asn \
    -os ${PROPERTIES_DIR}/OutputObserver/OutputObserver.pr \
    -o $TEST_OUTPUT_DIR

# Compile the actual Spin model checker.
# Run the model and expect an error.
# This way the working of a condition is confirmed in
# the fastest possible way.
# State number comparison is discarded, as state numbering is not stable
cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -DVECTORSZ=16384 -DBITSTATE -o system.out pan.c \
    && ./system.out > system.log \
    && grep -q "errors: 1" system.log \
    && grep -q "assertion violated  !((global_state.outputobserver.state==" system.log \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
