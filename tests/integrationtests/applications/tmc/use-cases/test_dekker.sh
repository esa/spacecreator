#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc
ASN1SCC=asn1scc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_dekker
RESOURCE_DIR=resources/dekker1
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
    -scl ${PROPERTIES_DIR}/sc.scl \
    -os ${PROPERTIES_DIR}/InputObserver/InputObserver.pr \
    -os ${PROPERTIES_DIR}/OutputObserver/OutputObserver.pr \
    -o $TEST_OUTPUT_DIR

$ASN1SCC -uPER -typePrefix asn1Scc -renamePolicy 3 -c -o "${TEST_OUTPUT_DIR}" "${RESOURCE_DIR}/work/dataview/dataview-uniq.asn"

# Compile the actual Spin model checker. This tests
# whether all the features are supported, and that
# one feature does not interfere with another.
# The model is not checked, as it would take a huge amounth of time,
# potentially infinite given no signal generation optimizations.
# Checking could be added in the future, once the optimizations
# are implemented.
cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -DVECTORSZ=65536 -o system.out *.c \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
