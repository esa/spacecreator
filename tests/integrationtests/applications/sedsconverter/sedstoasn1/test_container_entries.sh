#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_container_entries

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_container_entries.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Compare and clean-up on success
$DIFF $TEST_OUTPUT_DIR/COM-N7SPACE-CONTAINERENTRIES.asn resources/test_container_entries_asn.output && \
    $DIFF $TEST_OUTPUT_DIR/COM-N7SPACE-CONTAINERENTRIES.acn resources/test_container_entries_acn.output && \
    asn1scc -c -o $TEST_OUTPUT_DIR $TEST_OUTPUT_DIR/COM-N7SPACE-CONTAINERENTRIES.asn $TEST_OUTPUT_DIR/COM-N7SPACE-CONTAINERENTRIES.acn && \
    rm -r -f $TEST_OUTPUT_DIR
