#!/bin/bash

set -euo pipefail

TMC=$SPACECREATOR_BUILD_DIR/bin/tmc
SPIN=spin
CC=gcc
ASN1SCC=asn1scc

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output
RESOURCE_DIR=resources/name_matching
PROPERTIES_DIR=$RESOURCE_DIR/work/modelchecking/properties
SUBTYPES_DIR=$RESOURCE_DIR/work/modelchecking/subtypes

rm -rf $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$TMC -iv $RESOURCE_DIR/interfaceview.xml \
    -o $TEST_OUTPUT_DIR \
    -scl $PROPERTIES_DIR/validation/validation.scl \
    -sub $SUBTYPES_DIR/test-subtype.asn

$ASN1SCC -uPER -typePrefix asn1Scc -renamePolicy 3 -c -o "${TEST_OUTPUT_DIR}" "${RESOURCE_DIR}/work/dataview/dataview-uniq.asn"

# If subtypes are working correctly, the verifier shouldn't find any errors.
# Otherwise, the verifier shall find that stop condition is not true.
cd $TEST_OUTPUT_DIR \
    && $SPIN -a system.pml \
    && $CC -o system.out *.c \
    && ./system.out -m1000000 -n > system.output \
    && grep -q "errors: 0" system.output \
    && cd .. \
    && rm -r $TEST_OUTPUT_DIR
