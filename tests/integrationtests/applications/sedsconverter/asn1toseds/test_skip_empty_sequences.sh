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
$SEDS_CONVERTER --from ASN.1 --to SEDS -i resources/test_skip_empty_sequences.asn,resources/test_skip_empty_sequences.acn \
    --out $TEST_OUTPUT_DIR/package.xml --skip-empty-sequences

# Compare and clean-up on success
$DIFF $TEST_OUTPUT_DIR/package.xml resources/test_skip_empty_sequences.output && \
    rm -r -f $TEST_OUTPUT_DIR
