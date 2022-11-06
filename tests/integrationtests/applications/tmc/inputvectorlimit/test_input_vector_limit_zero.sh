#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc

TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources

echo "Running TMC test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -ivl 0 \
    -ivl rightinterface:1

cd $TEST_OUTPUT_DIR \
    && grep -qv "Gui_0_RI_0_LeftInterface" system.pml \
    && grep -q "Gui_0_RI_0_RightInterface" system.pml \
    && $SPIN -a system.pml \
    && $CC -o system.out pan.c \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
