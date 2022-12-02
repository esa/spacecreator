#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
XMLDIFF="xmldiff"
TEST_OUTPUT_DIR=output_test_multiple_asn_models

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from ASN.1 --to SEDS --multiple-asn-models -i resources/test_multiple_models.asn,resources/test_multiple_models.acn \
    --out $TEST_OUTPUT_DIR/package.xml

# Compare and clean-up on success
$XMLDIFF $TEST_OUTPUT_DIR/package.xml resources/test_multiple_models.output && \
    rm -r -f $TEST_OUTPUT_DIR
