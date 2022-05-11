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
# gdb --args \
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_interface_inheritance_types.xml \
    --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

# Compare and clean-up on success
cd $TEST_OUTPUT_DIR

$DIFF interfaceview.xml ../resources/test_interface_inheritance_types.output \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
