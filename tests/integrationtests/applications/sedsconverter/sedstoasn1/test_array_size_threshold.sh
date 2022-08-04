#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_array_size_threshold.xml \
    --asn1-sequence-size-threshold 2 --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Compare and clean-up on success
$DIFF $TEST_OUTPUT_DIR/COM-N7SPACE-ARRAYSIZETHRESHOLD.asn resources/test_array_size_threshold_asn.output && \
    $DIFF $TEST_OUTPUT_DIR/COM-N7SPACE-ARRAYSIZETHRESHOLD.acn resources/test_array_size_threshold_acn.output && \
    asn1scc -c -o $TEST_OUTPUT_DIR $TEST_OUTPUT_DIR/COM-N7SPACE-ARRAYSIZETHRESHOLD.asn $TEST_OUTPUT_DIR/COM-N7SPACE-ARRAYSIZETHRESHOLD.acn && \
    rm -r -f $TEST_OUTPUT_DIR
