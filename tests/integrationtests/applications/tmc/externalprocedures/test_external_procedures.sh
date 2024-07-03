#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc
ASN1SCC=asn1scc

TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties

echo "Running TMC test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -scl $PROPERTIES_DIR/StopConditions/CoolCondition.scl

$ASN1SCC -uPER -typePrefix asn1Scc -renamePolicy 3 -c -o "${TEST_OUTPUT_DIR}" "${RESOURCE_DIR}/work/dataview/dataview-uniq.asn"

cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -o system.out *.c \
    && ./system.out -m1000000 -a -n > system.output \
    && grep -q "assertion violated  !((global_state.ponger.argoutval==3))" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
